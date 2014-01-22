/** 
* @file XabslEngine.cpp
*
* Implementation of class Engine
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
* @author <a href="http://www.informatik.hu-berlin.de/~juengel">Matthias JŸngel</a>
*/

#include "XabslEngine.h"

namespace xabsl 
{
  
Engine::Engine(ErrorHandler& e,unsigned long (*pTimeFunction)())
: Symbols(e), selectedAgent(0), rootAction(0),
errorHandler(e), initialized(false), pTimeFunction(pTimeFunction)
{
}

Engine::~Engine()
{
  int i;
  for (i=0; i< options.getSize(); i++)
    if (options[i]!=0) 
      delete options[i];
  for (i=0; i< agents.getSize(); i++)
    if (agents[i]!=0) 
      delete agents[i];
  if (rootAction!=0)
    delete rootAction;
}

void Engine::execute()
{
  if (!initialized)
  {
    errorHandler.error("execute(): Call createOptionGraph before first execute.");
    return;
  }

  resetOutputSymbols();

  rootAction->execute();

  for (int i=0; i < options.getSize(); i++)
  {
    options[i]->wasActive = options[i]->isActive;
    options[i]->isActive = false;
  }

  for (int i=0;i<basicBehaviors.getSize();i++)
  {
    basicBehaviors[i]->wasActive = basicBehaviors[i]->isActive;
    basicBehaviors[i]->isActive = false;
  }
}

void Engine::createOptionGraph(InputSource& input)
{
  int i;
  char buf1[100],buf2[100];

  if (initialized)
  {
    errorHandler.error("createOptionGraph(): Don't call this function twice");
    return;
  }
  if (!input.open()) 
  {
    errorHandler.error("createOptionGraph(): Can't open input source");
    return;
  }

  // the total number of options in the intermediate code
  int numberOfOptions = (int)input.readValue(); 
  
  // create empty options
  for (i=0; i< numberOfOptions; i++)
  {
    input.readString(buf1,99);
    options.append(buf1,new Option(buf1,input,errorHandler,*this,pTimeFunction));
    if (errorHandler.errorsOccurred)
    {
      errorHandler.error("XabslEngine::createOptionGraph(): could not create option \"%s\"",options[i]->n);
      return;
    }
  }
  XABSL_DEBUG_INIT(errorHandler.message("registered %i options",numberOfOptions));

  // create internal enumerations
  int numberOfInternalEnumerations = (int)input.readValue();
  for (i=0; i < numberOfInternalEnumerations; i++)
  {
    input.readString(buf1, 99);
    int numberOfElements = (int)input.readValue();
    for (int j=0; j < numberOfElements; j++)
    {
      input.readString(buf2, 99);
      registerEnumElement(buf1, buf2, j);
      if (errorHandler.errorsOccurred)
      {
        errorHandler.error("XabslEngine::createOptionGraph(): could not register internal enumeration \"%s\"",buf1);
        return;
      }
    }
  }

  // create internal symbols
  int numberOfInternalSymbols = (int)input.readValue();
  for (i=0; i < numberOfInternalSymbols; i++)
  {
    char c[2];
    input.readString(c,1);
    switch (c[0])
    {
      case 'd':
        input.readString(buf1,99);
        internalDecimalSymbols.append(buf1, 0);
        registerDecimalOutputSymbol(buf1, &(internalDecimalSymbols.getElement(internalDecimalSymbols.getSize() - 1)));
        if (errorHandler.errorsOccurred)
        {
          errorHandler.error("XabslEngine::createOptionGraph(): could not register internal decimal symbol \"%s\"",buf1);
          return;
        }
        break;
      case 'b':
        input.readString(buf1,99);
        internalBooleanSymbols.append(buf1, false);
        registerBooleanOutputSymbol(buf1, &(internalBooleanSymbols.getElement(internalBooleanSymbols.getSize() - 1)));
        if (errorHandler.errorsOccurred)
        {
          errorHandler.error("XabslEngine::createOptionGraph(): could not register internal boolean symbol \"%s\"",buf1);
          return;
        }
        break;
      case 'e':
        input.readString(buf1,99);
        if (!enumerations.exists(buf1))
        {
          errorHandler.error("XabslEngine::createOptionGraph(): enumeration \"%s\" was not registered",buf1);
          return;
        }
        const Enumeration* enumeration = enumerations[buf1];
        if (enumeration->enumElements.getSize() == 0)
        {
          errorHandler.error("XabslEngine::createOptionGraph(): no enumeration elements for \"%s\" were registered",buf1);
          return;
        }
        input.readString(buf2,99);
        internalEnumeratedSymbols.append(buf2, enumeration->enumElements[0]->v);
        registerEnumeratedOutputSymbol(buf2, buf1, &(internalEnumeratedSymbols.getElement(internalEnumeratedSymbols.getSize() - 1)));
        if (errorHandler.errorsOccurred)
        {
          errorHandler.error("XabslEngine::createOptionGraph(): could not register internal enumerated symbol \"%s\"",buf2);
          return;
        }
        break;
    }
  }

  // create the options and its states
  for (i=0; i< numberOfOptions; i++)
  {
    XABSL_DEBUG_INIT(errorHandler.message("creating option \"%s\"",options[i]->n));
    options[i]->create(input,options,basicBehaviors,*this);
    if (errorHandler.errorsOccurred)
    {
      errorHandler.error("XabslEngine::createOptionGraph(): could not create option \"%s\"",options[i]->n);
      return;
    }
  }

  // create the agents
  int numberOfAgents = (int)input.readValue();
  for (i=0; i< numberOfAgents; i++)
  {
    input.readString(buf1,99);
    input.readString(buf2,99);
    agents.append(buf1,new Agent(buf1,options[buf2],errorHandler));
  }

  // check for loops in the option graph
  for (i=0;i<agents.getSize();i++)
  {
    if (Option* option = dynamic_cast<Option*>(agents[i]->getRootOption()))
    {
      Option* currentOptionPath[1000];
      
      currentOptionPath[0] = option;

      // call recursively the checkForLoops function
      if (checkForLoops(currentOptionPath,0)) 
      {
        errorHandler.error("createOptionGraph(): The created option graph contains loops");
      };
    }
  }


  selectedAgent = agents[0];
  setRootAction();

  XABSL_DEBUG_INIT(errorHandler.message("selected agent: \"%s\"",selectedAgent->n));
  input.close();
  initialized = true;
}


bool Engine::checkForLoops(Option* currentOptionPath[],int currentDepth)
{
  int i,j,k;
  Option* currentOption = currentOptionPath[currentDepth];

  for (i=0; i<currentOption->states.getSize(); i++)
  {
    for (j=0; j<currentOption->states[i]->actions.getSize(); j++)
    {
      if (ActionOption* nextAction = dynamic_cast<ActionOption*>(currentOption->states[i]->actions[j])) 
      {
        for(k=0; k<=currentDepth; k++)
        {
          // check for the subsequent option of each state whether the referenced 
          // option is contained in the current option path
          if (nextAction->option == currentOptionPath[k])
          {
            errorHandler.error("checkForLoops() : state \"%s\" in option \"%s\" references subsequent option \"%s\". But option \"%s\" is also directly or indirectly referenced by option \"%s\".",
              currentOption->states[i]->n, 
              currentOption->n,
              nextAction->option->n,
              currentOption->n,
              nextAction->option->n);

            return true;
          }
        }
        
        // recursion
        currentOptionPath[currentDepth+1] = nextAction->option;
        if (checkForLoops(currentOptionPath,currentDepth+1))
        {
          return true;
        }
      }
    }
  }

  return false;
}

void Engine::registerBasicBehavior(BasicBehavior& basicBehavior)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering basic behavior \"%s\"",basicBehavior.n));

  if (basicBehaviors.exists(basicBehavior.n))
  {
    errorHandler.error("registerBasicBehavior(): basic behavior \"%s\" was already registered",basicBehavior.n);
    return;
  }

  basicBehavior.parameters->registerEnumerations(enumerations);
  basicBehavior.registerParameters();

  basicBehaviors.append(basicBehavior.n,&basicBehavior);
}

bool Engine::setRootAction(const char* name, bool isOption)
{
  if (isOption)
  {
    // check if the option exists
    if (!options.exists(name)) return false;

    // set the current root option to the requested option
    setRootAction(options[name]);
  }
  else
  {
    // check if the basic behavior exists
    if (!basicBehaviors.exists(name)) return false;

    // set the current root option to the requested option
    setRootAction(basicBehaviors[name]);
  }

  return true;
}

void Engine::setRootAction()
{
  setRootAction(selectedAgent->getRootOption());
}

void Engine::setRootAction(Behavior* behavior)
{
  if (rootAction != 0) delete rootAction;
  rootAction = Action::create(behavior, errorHandler, pTimeFunction);
}

const Action* Engine::getRootAction() const
{
  return rootAction;
}

Action* Engine::getRootAction()
{
  return rootAction;
}

const Option* Engine::getRootOption() const
{
  return rootAction->getOption();
}

const char* Engine::getSelectedAgentName() const
{
  return selectedAgent->n;
}

bool Engine::setSelectedAgent(const char* name)
{
  if (!agents.exists(name)) 
  {
    return false;
  }

  Agent* newAgent = agents[name];

  if (selectedAgent != newAgent)
  {
    selectedAgent = newAgent;
    setRootAction();
  }

  return true;
}

void Engine::reset()
{
  int i;
  for (i=0; i< options.getSize(); i++)
  {
    options[i]->wasActive = false;
  }
  for (i=0; i< basicBehaviors.getSize(); i++)
  {
    basicBehaviors[i]->wasActive = false;
  }
}

} // namespace

