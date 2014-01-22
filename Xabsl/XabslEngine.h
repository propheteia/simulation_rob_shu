/** 
* @file XabslEngine.h
*
* Declaration class Engine
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
* @author <a href="http://www.informatik.hu-berlin.de/~juengel">Matthias JŸngel</a>
*/

#ifndef __XabslEngine_h_
#define __XabslEngine_h_

#include "XabslAgent.h"
#include "XabslOption.h"

namespace xabsl 
{

/**
* Executes a behavior that was specified in the Xabsl language.
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
* @author <a href="http://www.informatik.hu-berlin.de/~juengel">Matthias JŸngel</a>
*/
class Engine : public Symbols
{
public:
  /** 
  * Constructor 
  * @param e Is invoked when there are problems during initialization
  * @param pTimeFunction a pointer to a function that returns the system time in ms.
  */
  Engine(ErrorHandler& e, unsigned long (*pTimeFunction)());

  /** Destructor */
  ~Engine();

  /** 
  * Executes the engine for the selected agent starting from the root option.
  * (Including the selected basic behavior)
  */
  void execute();

  /**
  * Reads the intermediate code from an input source and creates the option graph.
  * Note that the basic behaviors and symbols have to be registered before that function is 
  * called.
  */
  void createOptionGraph(InputSource& input);

  /** 
  * Registers a basic behavior at the engine. 
  * This must be done before the intermediate code is read.
  * @param basicBehavior A reference to the basic behavior
  */
  void registerBasicBehavior(BasicBehavior& basicBehavior);

  /** 
  * Sets the selected Agent.
  * If the last selected agent was different from the new one, 
  * the root option is changed depending on the new agent.
  * @param name The name of the agent
  * @return if the requested agent exists
  */
  bool setSelectedAgent(const char* name);

  /**
  * Resets all active options.
  * Next cycle will execute initial state of currently set root option.
  */
  void reset();

private:

  /** The agents of the engine */
  Array<Agent*> agents;

  /** The selected agent */
  Agent* selectedAgent;

  /** The options of the engine */
  Array<Option*> options;

  /** The action for the execution of the start of the option graph */
  Action* rootAction;

  /** The registered basic behaviors of the engine */
  Array<BasicBehavior*> basicBehaviors;

  /** Is invoked when there are problems during initialization */
  ErrorHandler& errorHandler;

  /** 
  * A recursive function that is used to check for loops in the option graph.
  * @param currenOptionPath An array of the currently traced option path
  * @param currentDepth The depth of the current option path
  * @return If true, then a loop was detected.
  */
  bool checkForLoops(Option* currenOptionPath[], int currentDepth);

  /** If true, the engine was successfully initialized */
  bool initialized;

  /** A pointer to a function that returns the system time in ms. */
  unsigned long (*pTimeFunction)();

  /** Arrays containing internal symbol values */
  Array<double> internalDecimalSymbols;
  Array<bool> internalBooleanSymbols;
  Array<int> internalEnumeratedSymbols;

public:
  //!@name Debugging Interface 
  //!@{

  /** Returns the agents of the engine */
  const Array<Agent*>& getAgents() const {return agents;}

  /** Return the options of the engine */
  const Array<Option*>& getOptions() const {return options;}

  /** Returns the registered basic behaviors of the engine */
  const Array<BasicBehavior*>& getBasicBehaviors() const {return basicBehaviors;}

  /** 
  * Executes the option graph starting from a given option or basic behavior.
  * Can be called to test a single option or basic behavior.
  * @param name The name of the option or basic behavior
  * @param isOption True for an option, false for a basic behavior
  * @return When false, the option is not known to the engine
  */
  bool setRootAction(const char* name, bool isOption = true);

  /** 
  * Sets the root option of the selected agent
  */
  void setRootAction();

  /** 
  * Sets the root option to the specified option or basic behavior
  */
  void setRootAction(Behavior* behavior);

  /** Returns the selected root action */
  const Action* getRootAction() const;

  /** Returns the selected root action */
  Action* getRootAction();

  /** Returns the selected root option, return 0 if root action is not an option */
  const Option* getRootOption() const;

  /** Returns the name of the selected agent */
  const char* getSelectedAgentName() const;

  //!@}
};

} // namespace

#endif // __XabslEngine_h_

