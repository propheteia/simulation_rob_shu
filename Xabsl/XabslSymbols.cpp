/**
* @file XabslSymbols.cpp
*
* Implementation of class Symbols and helper classes
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*/

#include "XabslSymbols.h"

namespace xabsl 
{

Enumeration::~Enumeration()
{
  int i;
  for (i=0; i< enumElements.getSize(); i++)
  {
    delete enumElements[i];
  }
}

Symbols::~Symbols()
{
  int i;
  for (i=0; i< enumerations.getSize(); i++) delete enumerations[i];
  for (i=0; i< decimalInputSymbols.getSize(); i++) delete decimalInputSymbols[i];
  for (i=0; i< booleanInputSymbols.getSize(); i++) delete booleanInputSymbols[i];
  for (i=0; i< enumeratedInputSymbols.getSize(); i++) delete enumeratedInputSymbols[i];
  for (i=0; i< decimalOutputSymbols.getSize(); i++) delete decimalOutputSymbols[i];
  for (i=0; i< booleanOutputSymbols.getSize(); i++) delete booleanOutputSymbols[i];
  for (i=0; i< enumeratedOutputSymbols.getSize(); i++) delete enumeratedOutputSymbols[i];
}

void Symbols::registerEnumElement(const char* symbolName, const char* name, int value)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enum element \"%s\" for enumeration \"%s\"",name, symbolName));

  if (!enumerations.exists(symbolName))
  {
    enumerations.append(symbolName, new Enumeration(symbolName));
  }
  if (enumerations[symbolName]->enumElements.exists(name))
  {
    errorHandler.error("registerEnumElement(): enum element \"%s\" for enumeration \"%s\" was already registered.", name, symbolName);
    return;
  }
  enumerations[symbolName]->enumElements.append(name, new EnumElement(name,value));
}

void Symbols::registerDecimalInputSymbol(const char* name, const double* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal input symbol \"%s\"",name));
  
  if (decimalInputSymbols.exists(name))
  {
    errorHandler.error("registerDecimalInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  decimalInputSymbols.append(name,new DecimalInputSymbol(name, pVariable, errorHandler));
}


void Symbols::registerDecimalInputSymbol
(const char* name, FunctionProvider* pInstance,
 double (FunctionProvider::*pFunction)())
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal input symbol \"%s\"",name));
  
  if (decimalInputSymbols.exists(name))
  {
    errorHandler.error("registerDecimalInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  decimalInputSymbols.append(name,new DecimalInputSymbol(name, pInstance, pFunction, errorHandler));
}

void Symbols::registerDecimalInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal parameter \"%s\" for decimal input symbol\"%s\"",name, symbolName));

  if (!decimalInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerDecimalInputSymbolDecimalParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (decimalInputSymbols[symbolName]->parameters.decimal.exists(name))
  {
    errorHandler.error("registerDecimalInputSymbolDecimalParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  decimalInputSymbols[symbolName]->parameters.decimal.append(name,&param);
}

void Symbols::registerDecimalInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean parameter \"%s\" for decimal input symbol\"%s\"",name, symbolName));

  if (!decimalInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerDecimalInputSymbolBooleanParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (decimalInputSymbols[symbolName]->parameters.boolean.exists(name))
  {
    errorHandler.error("registerDecimalInputSymbolBooleanParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  decimalInputSymbols[symbolName]->parameters.boolean.append(name,&param);
}

void Symbols::registerDecimalInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated parameter \"%s\" for decimal input symbol\"%s\"",name, symbolName));

  if (!decimalInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerDecimalInputSymbolEnumeratedParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (decimalInputSymbols[symbolName]->parameters.enumerated.exists(name))
  {
    errorHandler.error("registerDecimalInputSymbolEnumeratedParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  decimalInputSymbols[symbolName]->parameters.enumerations.append(name,enumerations.getElement(enumName));
  decimalInputSymbols[symbolName]->parameters.enumerated.append(name,&param);
}

void Symbols::registerBooleanInputSymbol(const char* name, const bool* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean input symbol \"%s\"",name));
  
  if (booleanInputSymbols.exists(name))
  {
    errorHandler.error("registerBooleanInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  booleanInputSymbols.append(name,new BooleanInputSymbol(name, pVariable, errorHandler));
}


void Symbols::registerBooleanInputSymbol(const char* name, FunctionProvider* pInstance,
                                               bool (FunctionProvider::*pFunction)())
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean input symbol \"%s\"",name));
  
  if (booleanInputSymbols.exists(name))
  {
    errorHandler.error("registerBooleanInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  booleanInputSymbols.append(name,new BooleanInputSymbol(name, pInstance, pFunction, errorHandler));
}

void Symbols::registerBooleanInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal parameter \"%s\" for boolean input symbol\"%s\"",name, symbolName));

  if (!booleanInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerBooleanInputSymbolDecimalParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (booleanInputSymbols[symbolName]->parameters.decimal.exists(name))
  {
    errorHandler.error("registerBooleanInputSymbolDecimalParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  booleanInputSymbols[symbolName]->parameters.decimal.append(name,&param);
}

void Symbols::registerBooleanInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean parameter \"%s\" for boolean input symbol\"%s\"",name, symbolName));

  if (!booleanInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerBooleanInputSymbolBooleanParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (booleanInputSymbols[symbolName]->parameters.boolean.exists(name))
  {
    errorHandler.error("registerBooleanInputSymbolBooleanParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  booleanInputSymbols[symbolName]->parameters.boolean.append(name,&param);
}

void Symbols::registerBooleanInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated parameter \"%s\" for boolean input symbol\"%s\"",name, symbolName));

  if (!booleanInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerBooleanInputSymbolEnumeratedParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (booleanInputSymbols[symbolName]->parameters.enumerated.exists(name))
  {
    errorHandler.error("registerBooleanInputSymbolEnumeratedParameter(): parameter \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  booleanInputSymbols[symbolName]->parameters.enumerations.append(name,enumerations.getElement(enumName));
  booleanInputSymbols[symbolName]->parameters.enumerated.append(name,&param);
}

void Symbols::registerEnumeratedInputSymbol(const char* name, const char* enumName, const int* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated input symbol \"%s\"",name));
  
  if (enumeratedInputSymbols.exists(name))
  {
    errorHandler.error("registerEnumeratedInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  enumeratedInputSymbols.append(name,new EnumeratedInputSymbol(name, enumerations[enumName], pVariable, errorHandler));
}

void Symbols::registerEnumeratedInputSymbol(const char* name, const char* enumName, FunctionProvider* pInstance,
    int (FunctionProvider::*pFunction)())
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated input symbol \"%s\"",name));
  
  if (enumeratedInputSymbols.exists(name))
  {
    errorHandler.error("registerEnumeratedInputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  enumeratedInputSymbols.append(name,new EnumeratedInputSymbol(name, enumerations[enumName], pInstance, pFunction, errorHandler));
}

void Symbols::registerEnumeratedInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal parameter \"%s\" for enumerated input symbol\"%s\"",name, symbolName));

  if (!enumeratedInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerEnumeratedInputSymbolDecimalParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (enumeratedInputSymbols[symbolName]->parameters.decimal.exists(name))
  {
    errorHandler.error("registerEnumeratedInputSymbolDecimalParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  enumeratedInputSymbols[symbolName]->parameters.decimal.append(name,&param);
}

void Symbols::registerEnumeratedInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean parameter \"%s\" for enumerated input symbol\"%s\"",name, symbolName));

  if (!enumeratedInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerEnumeratedInputSymbolBooleanParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (enumeratedInputSymbols[symbolName]->parameters.boolean.exists(name))
  {
    errorHandler.error("registerEnumeratedInputSymbolBooleanParameter(): parameter \"%s\" was already registered",name);
    return;
  }

  enumeratedInputSymbols[symbolName]->parameters.boolean.append(name,&param);
}

void Symbols::registerEnumeratedInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated parameter \"%s\" for enumerated input symbol\"%s\"",name, symbolName));

  if (!enumeratedInputSymbols.exists(symbolName))
  {
    errorHandler.error("registerEnumeratedInputSymbolEnumeratedParameter(): symbol \"%s\" was not registered",symbolName);
    return;
  }
  if (enumeratedInputSymbols[symbolName]->parameters.enumerated.exists(name))
  {
    errorHandler.error("registerEnumeratedInputSymbolEnumeratedParameter(): parameter \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  enumeratedInputSymbols[symbolName]->parameters.enumerations.append(name,enumerations.getElement(enumName));
  enumeratedInputSymbols[symbolName]->parameters.enumerated.append(name,&param);
}

void Symbols::registerDecimalOutputSymbol(const char* name, double* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal output symbol \"%s\"",name));
  
  if (decimalOutputSymbols.exists(name))
  {
    errorHandler.error("registerDcimalOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  decimalOutputSymbols.append(name,new DecimalOutputSymbol(name, pVariable));
}

void Symbols::registerDecimalOutputSymbol(const char* name, FunctionProvider* pInstance,
  void (FunctionProvider::*pSetFunction)(double),
  double (FunctionProvider::*pGetFunction)()
)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering decimal output symbol \"%s\"",name));
  
  if (decimalOutputSymbols.exists(name))
  {
    errorHandler.error("registerDecimalOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  decimalOutputSymbols.append(name,new DecimalOutputSymbol(name, pInstance, pSetFunction, pGetFunction));
}

void Symbols::registerBooleanOutputSymbol(const char* name, bool* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean output symbol \"%s\"",name));
  
  if (booleanOutputSymbols.exists(name))
  {
    errorHandler.error("registerBooleanOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  booleanOutputSymbols.append(name,new BooleanOutputSymbol(name, pVariable));
}

void Symbols::registerBooleanOutputSymbol(const char* name, FunctionProvider* pInstance,
  void (FunctionProvider::*pSetFunction)(bool),
  bool (FunctionProvider::*pGetFunction)()
)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering boolean output symbol \"%s\"",name));
  
  if (booleanOutputSymbols.exists(name))
  {
    errorHandler.error("registerBooleanOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  booleanOutputSymbols.append(name,new BooleanOutputSymbol(name, pInstance, pSetFunction, pGetFunction));
}

void Symbols::registerEnumeratedOutputSymbol(const char* name, const char* enumName, int* pVariable)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated output symbol \"%s\"",name));
  
  if (enumeratedOutputSymbols.exists(name))
  {
    errorHandler.error("registerEnumeratedOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  enumeratedOutputSymbols.append(name,new EnumeratedOutputSymbol(name, enumerations[enumName], pVariable));
}

void Symbols::registerEnumeratedOutputSymbol(const char* name, const char* enumName, FunctionProvider* pInstance,
  void (FunctionProvider::*pSetFunction)(int),
  int (FunctionProvider::*pGetFunction)()
)
{
  XABSL_DEBUG_INIT(errorHandler.message("registering enumerated output symbol \"%s\"",name));
  
  if (enumeratedOutputSymbols.exists(name))
  {
    errorHandler.error("registerEnumeratedOutputSymbol(): symbol \"%s\" was already registered",name);
    return;
  }
  if (!enumerations.exists(enumName))
  {
    enumerations.append(enumName, new Enumeration(enumName));
  }
  enumeratedOutputSymbols.append(name,new EnumeratedOutputSymbol(name, enumerations[enumName], pInstance, pSetFunction, pGetFunction));
}

void Symbols::resetOutputSymbols()
{
  for (int i=0;i<decimalOutputSymbols.getSize();i++)
    decimalOutputSymbols[i]->activeValueWasSet = false;
  for (int i=0;i<booleanOutputSymbols.getSize();i++)
    booleanOutputSymbols[i]->activeValueWasSet = false;
  for (int i=0;i<enumeratedOutputSymbols.getSize();i++)
    enumeratedOutputSymbols[i]->activeValueWasSet = false;
}

} // namespace

