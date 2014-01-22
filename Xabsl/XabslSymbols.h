/**
* @file XabslSymbols.h
*
* Definition of class Symbols and helper classes
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*/

#ifndef __XabslSymbols_h_
#define __XabslSymbols_h_

#include "XabslTools.h"
#include "XabslParameters.h"

namespace xabsl 
{

/**
* @class FunctionProvider
*
* Base class for all those classes that want to register functions for symbols
* at a Engine
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class FunctionProvider {
  //virtual void dummy(){}
};

/**
* @class EnumElement
* Represents an enum element that is part of an enumerated input or output symbol.
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class EnumElement : public NamedItem
{
public:
/** 
* Constructor 
* @param name The name of the enum element as specified in the XML formalization
* @param value The value for the element from the software environment
  */
  EnumElement(const char* name, int value)
    : NamedItem(name), v(value) {};
  
  /** The enum value from a function or variable in the software environment */
  int v;
};
/**
* @class Enumeration
*
* Represents a list of enum elements
*/
class Enumeration : public NamedItem
{
public:
  /** 
  * Constructor 
  * @param name The name of the enumeration as specified in the XML formalization
  */
  Enumeration(const char* name) : NamedItem(name) {};

  /** Destructor. Deletes the enum elements */
  ~Enumeration();

  /** 
  * Assigns an enum value from a function or variable in the software environment 
  * to the enum-element string in the XML formalization.
  */
  Array<EnumElement*> enumElements;
};

/** 
* A Template for the input symbol classes
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
template<class T> class InputSymbol : public NamedItem
{
public:
/** 
* Constructor 
* @param name The name of the symbol, for debugging purposes
* @param pVariable A pointer to the variable that the symbol stands for
  */
  InputSymbol(const char* name, const T* pVariable, ErrorHandler& errorHandler)
    : NamedItem(name), pV(pVariable), pI(0), pF(0), parameters(errorHandler)
  {};
  
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pFunction A pointer to a boolean returning function in the software environment 
  * @param pInstance A pointer to the instance that contains the function
  */
  InputSymbol(const char* name,
    FunctionProvider* pInstance,
    T (FunctionProvider::*pFunction)(),
    ErrorHandler& errorHandler)
    : NamedItem(name), pV(0), pI(pInstance), pF(pFunction), parameters(errorHandler) {};
  
  /** returns the value of the symbol */
  T getValue() const
  { if (pF!=0) return (pI->*pF)(); else return *pV; }
  
  /** The parameters of the input symbol*/
  Parameters parameters;
private:
  /** A pointer to a variable in the software environment */
  const T* pV; 
  
  /** A pointer to the instance object that contains the function */
  FunctionProvider* pI;
  
  /** A pointer to a T returning function in the software environment */
  T (FunctionProvider::*pF)(); 
};

/** 
* @class DecimalInputSymbol
*
* Represents a decimal input symbol of the Engine 
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class DecimalInputSymbol : public InputSymbol<double>
{
public:
/** 
* Constructor 
* @param name The name of the symbol, for debugging purposes
* @param pVariable A pointer to the variable that the symbol stands for
  */
  DecimalInputSymbol(const char* name, const double* pVariable, ErrorHandler& errorHandler)
    : InputSymbol<double>(name, pVariable, errorHandler) 
  {};
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pFunction A pointer to a double returning function in the software environment 
  * @param pInstance A pointer to the instance that contains the function
  */
  DecimalInputSymbol(const char* name,
    FunctionProvider* pInstance,
    double (FunctionProvider::*pFunction)(),
    ErrorHandler& errorHandler)
    : InputSymbol<double>(name, pInstance, pFunction, errorHandler) {};
};

/** 
* @class BooleanInputSymbol
*
* Represents a boolean input symbol of the Engine 
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class BooleanInputSymbol : public InputSymbol<bool>
{
public:
/** 
* Constructor 
* @param name The name of the symbol, for debugging purposes
* @param pVariable A pointer to the variable that the symbol stands for
  */
  BooleanInputSymbol(const char* name, const bool* pVariable, ErrorHandler& errorHandler)
    : InputSymbol<bool>(name, pVariable, errorHandler) 
  {};
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pFunction A pointer to a boolean returning function in the software environment 
  * @param pInstance A pointer to the instance that contains the function
  */
  BooleanInputSymbol(const char* name, FunctionProvider* pInstance,
    bool (FunctionProvider::*pFunction)(),
    ErrorHandler& errorHandler)
    : InputSymbol<bool>(name, pInstance, pFunction, errorHandler) {};
};

/** 
* @class EnumeratedInputSymbol
*
* Represents a enumerated input symbol of the Engine 
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class EnumeratedInputSymbol : public InputSymbol<int>
{
public:
/** 
* Constructor 
* @param name The name of the symbol, for debugging purposes
* @param pVariable A pointer to the variable that the symbol stands for
  */
  EnumeratedInputSymbol(const char* name, Enumeration* enumeration, const int* pVariable, 
    ErrorHandler& errorHandler)
    : enumeration(enumeration), InputSymbol<int>(name, pVariable, errorHandler) 
  {};
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param enumeration Pointer to the list of enumeration elements
  * @param pFunction A pointer to an int returning function in the software environment 
  * @param pInstance A pointer to the instance that contains the function
  */
  EnumeratedInputSymbol(const char* name, Enumeration* enumeration, FunctionProvider* pInstance,
    int (FunctionProvider::*pFunction)(),
    ErrorHandler& errorHandler)
    : enumeration(enumeration), InputSymbol<int>(name, pInstance, pFunction, errorHandler) {};

  /** Pointer to the list of enumeration elements */
  Enumeration* enumeration;
};

/** 
* @class OutputSymbol
*
* A Template for the output symbol classes
*
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*/
template<class T> class OutputSymbol : public NamedItem
{
public:
  /** 
  * Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pVariable A pointer to the variable that the symbol stands for
  * @param defaultValue The default value
  */
  OutputSymbol(const char* name, T* pVariable)
    : NamedItem(name), pV(pVariable), pI(0), pSetF(0), pGetF(0), activeValueWasSet(false)
  {};
  
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pSetFunction A pointer to a boolean accepting function in the software environment 
  * @param pGetFunction A pointer to a boolean returning function in the software environment 
  * @param pInstance A pointer to the instance that contains the function
  * @param defaultValue The default value
  */
  OutputSymbol(const char* name,
    FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(T),
    T (FunctionProvider::*pGetFunction)())
    : NamedItem(name), pV(0), pI(pInstance), pSetF(pSetFunction), pGetF(pGetFunction), activeValueWasSet(false)
  {};


  /** Set the value of the symbol. */
  void setValue(T value)
  {
    if (pSetF!=0) 
    {
      (pI->*pSetF)(value);
    }
    else 
    {
      *pV=value;
    }
    activeValueWasSet = true;
  }

  /** Returns the current value of the symbol. */
  T getValue() const
  {
    if (pGetF!=0) 
    {
      return (pI->*pGetF)();
    }
    else 
    {
      return *pV;
    }
  }

  /** If true, the value was set during the last execution of the option graph. */
  bool activeValueWasSet;

private:
  /** A pointer to a variable in the software environment */
  T* pV; 
  
  /** A pointer to the instance object that contains the function */
  FunctionProvider* pI;
  
  /** A pointer to a function that sets the value of the symbol in the software environment */
  void (FunctionProvider::*pSetF)(T); 

  /** A pointer to a function that gets the value of the symbol from the software environment */
  T (FunctionProvider::*pGetF)(); 
};

/** 
* @class DecimalOutputSymbol
*
* Represents a decimal output symbol of the Engine 
*
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*/
class DecimalOutputSymbol : public OutputSymbol<double>
{
public:
  /** 
  * Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pVariable A pointer to the variable that the symbol stands for
  */
  DecimalOutputSymbol(const char* name, double* pVariable)
    : OutputSymbol<double>(name, pVariable)
  {};
    
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pSetFunction A pointer to a function in the software environment that sets the value of the symbol
  * @param pGetFunction A pointer to a function in the software environment that returns the value of the symbol
  * @param pInstance A pointer to the instance that contains the function
  */
  DecimalOutputSymbol(const char* name, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(double),
    double (FunctionProvider::*pGetFunction)())
    : OutputSymbol<double>(name, pInstance, pSetFunction, pGetFunction)
  {};
};

/** 
* @class BooleanOutputSymbol
*
* Represents a boolean output symbol of the Engine 
*
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*
*/
class BooleanOutputSymbol : public OutputSymbol<bool>
{
public:
  /** 
  * Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pVariable A pointer to the variable that the symbol stands for
  */
  BooleanOutputSymbol(const char* name, bool* pVariable)
    : OutputSymbol<bool>(name, pVariable)
  {};
    
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param pSetFunction A pointer to a function in the software environment that sets the value of the symbol
  * @param pGetFunction A pointer to a function in the software environment that returns the value of the symbol
  * @param pInstance A pointer to the instance that contains the function
  */
  BooleanOutputSymbol(const char* name, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(bool),
    bool (FunctionProvider::*pGetFunction)())
    : OutputSymbol<bool>(name, pInstance, pSetFunction, pGetFunction)
  {};
};

/** 
* @class EnumeratedOutputSymbol
*
* Represents a enumerated output symbol of the Engine 
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
*/
class EnumeratedOutputSymbol : public OutputSymbol<int>
{
public:
  /** 
  * Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param enumeration Pointer to the list of enumeration elements
  * @param pVariable A pointer to the variable that the symbol stands for
  */
  EnumeratedOutputSymbol(const char* name, Enumeration* enumeration, int* pVariable)
    : OutputSymbol<int>(name, pVariable), enumeration(enumeration)
  {};
  
  
  /** Constructor 
  * @param name The name of the symbol, for debugging purposes
  * @param enumeration Pointer to the list of enumeration elements
  * @param pSetFunction A pointer to a function in the software environment that sets the value of the symbol
  * @param pGetFunction A pointer to a function in the software environment that returns the value of the symbol
  * @param pInstance A pointer to the instance that contains the function
  */
  EnumeratedOutputSymbol(const char* name, Enumeration* enumeration, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(int),
    int (FunctionProvider::*pGetFunction)())
    : OutputSymbol<int>(name, pInstance, pSetFunction, pGetFunction), enumeration(enumeration)
  {};
  
  /** Pointer to the list of enumeration elements */
  Enumeration* enumeration;
};

/**
* @class Symbols
*
* Handles the symbols of the Engine.
*
* @author <a href="http://www.martin-loetzsch.de">Martin Loetzsch</a>
* @author <a href="http://www.sim.informatik.tu-darmstadt.de/pers/card/risler.html">Max Risler</a>
*/
class Symbols
{
public:
/** 
* Constructor.
* @param errorHandler Is invoked when errors occur
  */
  Symbols(ErrorHandler& errorHandler)
    : errorHandler(errorHandler) {};
  
  /** Destructor */
  ~Symbols();
  
  /**
  * Registers an enum element for an enumeration.
  * @param enumName The name of the enumeration
  * @param name The name of the enum element
  * @param value The value of the element
  */
  void registerEnumElement(const char* enumName, 
    const char* name, int value);

  /** 
  * Registers the address of a variable for a decimal input symbol.
  * @param name The name of the symbol
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerDecimalInputSymbol(const char* name, const double* pVariable);
  
  /** 
  * Registers the address of a function for a decimal input symbol.
  * @param name The name of the symbol
  * @param pFunction A pointer to a function that calculates a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerDecimalInputSymbol(const char* name, FunctionProvider* pInstance,
    double (FunctionProvider::*pFunction)());
  
  /** 
  * Registers a parameter of a parameterized decimal input symbol.
  * @param symbolName The name of the symbol
  * @param name The name of the parameter
  * @param param A reference to the parameter
  */
  void registerDecimalInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param);
  void registerDecimalInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param);
  void registerDecimalInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param);

  /** 
  * Registers the address of a variable for a boolean input symbol.
  * @param name The name of the symbol
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerBooleanInputSymbol(const char* name, const bool* pVariable);
  
  /** 
  * Registers the address of a function for a boolean input symbol.
  * @param name The name of the symbol
  * @param pFunction A pointer to a function that calculates a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerBooleanInputSymbol(const char* name, FunctionProvider* pInstance,
    bool (FunctionProvider::*pFunction)());
  
  /** 
  * Registers a parameter of a parameterized boolean input symbol.
  * @param symbolName The name of the symbol
  * @param name The name of the parameter
  * @param param A reference to the parameter
  */
  void registerBooleanInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param);
  void registerBooleanInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param);
  void registerBooleanInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param);

  /** 
  * Registers the address of a variable for a enumerated input symbol.
  * @param name The name of the symbol
  * @param enumName The name of the associated enumeration
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerEnumeratedInputSymbol(const char* name, const char* enumName, const int* pVariable);
  
  /** 
  * Registers the address of a function for a enumerated input symbol.
  * @param name The name of the symbol
  * @param enumName The name of the associated enumeration
  * @param pFunction A pointer to a function that calculates a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerEnumeratedInputSymbol(const char* name, const char* enumName, FunctionProvider* pInstance,
    int (FunctionProvider::*pFunction)());
  
  /** 
  * Registers a parameter of an enumerated input symbol.
  * @param symbolName The name of the symbol
  * @param name The name of the parameter
  * @param param A reference to the parameter
  */
  void registerEnumeratedInputSymbolDecimalParameter(const char* symbolName, 
    const char* name, double& param);
  void registerEnumeratedInputSymbolBooleanParameter(const char* symbolName, 
    const char* name, bool& param);
  void registerEnumeratedInputSymbolEnumeratedParameter(const char* symbolName, 
    const char* name, const char* enumName, int& param);

  /** 
  * Registers the address of a variable for a decimal output symbol.
  * @param name The name of the symbol
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerDecimalOutputSymbol(const char* name, double* pVariable);

  /** 
  * Registers the address of a function for a decimal output symbol.
  * @param name The name of the symbol
  * @param pSetFunction A pointer to a function that sets a value for the symbol
  * @param pGetFunction A pointer to a function that returns a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerDecimalOutputSymbol(const char* name, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(double),
    double (FunctionProvider::*pGetFunction)()
  );

  /** 
  * Registers the address of a variable for a boolean output symbol.
  * @param name The name of the symbol
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerBooleanOutputSymbol(const char* name, bool* pVariable);

  /** 
  * Registers the address of a function for a boolean output symbol.
  * @param name The name of the symbol
  * @param pSetFunction A pointer to a function that sets a value for the symbol
  * @param pGetFunction A pointer to a function that returns a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerBooleanOutputSymbol(const char* name, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(bool),
    bool (FunctionProvider::*pGetFunction)()
  );

  /** 
  * Registers the address of a variable for a enumerated output symbol.
  * @param name The name of the symbol
  * @param enumName The name of the associated enumeration
  * @param pVariable A pointer to a variable in the software environment 
  */
  void registerEnumeratedOutputSymbol(const char* name, const char* enumName, int* pVariable);
  
  /** 
  * Registers the address of a function for a enumerated output symbol.
  * @param name The name of the symbol
  * @param enumName The name of the associated enumeration
  * @param pSetFunction A pointer to a function that sets a value for the symbol
  * @param pGetFunction A pointer to a function that returns a value for the symbol
  * @param pInstance A pointer to an object that provides the function
  */
  void registerEnumeratedOutputSymbol(const char* name, const char* enumName, FunctionProvider* pInstance,
    void (FunctionProvider::*pSetFunction)(int),
    int (FunctionProvider::*pGetFunction)()
  );
  
  /** Sets all output symbols to unset */
  void resetOutputSymbols();
  
  /** The enumerations */
  Array<Enumeration*> enumerations;

  /** The decimal input symbols */
  Array<DecimalInputSymbol*> decimalInputSymbols;
    
  /** The boolean input symbols */
  Array<BooleanInputSymbol*> booleanInputSymbols;
  
  /** The enumerated input symbols */
  Array<EnumeratedInputSymbol*> enumeratedInputSymbols;
  
  /** The decimal output symbols */
  Array<DecimalOutputSymbol*> decimalOutputSymbols;

  /** The boolean output symbols */
  Array<BooleanOutputSymbol*> booleanOutputSymbols;

  /** The enumerated output symbols */
  Array<EnumeratedOutputSymbol*> enumeratedOutputSymbols;

private:
  /** Is invoked when errors occur */
  ErrorHandler& errorHandler;
};

} // namespace

#endif //__XabslSymbols_h_
