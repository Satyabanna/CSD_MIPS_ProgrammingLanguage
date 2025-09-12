# Grammar Rules and Syntactic Structure of KIK Language

KIK is a procedural programming language, with syntax for control structures (using colons after conditions), data types, constants, and input/output operations. Programs begin with a main function `int kik() { ... }`, supporting declarations, statements, and additional functions. Imports (e.g., `import "io.kik";`) enable custom I/O functionality. The grammar is defined in Extended Backus-Naur Form (EBNF), where terminals are quoted (e.g., `"int"`), non-terminals are in angle brackets (e.g., `<identifier>`), `{ ... }*` denotes zero or more repetitions, `{ ... }*` denotes one or more, `|` separates alternatives, and `[ ... ]` indicates optional elements.

Here, we go with all grammars for KIK language . . . .

## 1. Program Structure

A KIK program consists of optional imports, a mandatory main function, and optional additional functions or declarations.

```ebnf
<program> ::= { <import> }* <main-function> { <function> | <declaration> }*
<import> ::= "import" <string-literal> ";"
<main-function> ::= "int" "kik" "(" ")" "{" <statement>* "return" <expression> ";" "}"
<function> ::= <type> <identifier> "(" [ <parameter-list> ] ")" "{" <statement>* "}"
<parameter-list> ::= <parameter> { "," <parameter> }*
<parameter> ::= <type> <identifier>
```

Notes: The main function returns an integer (e.g., `return 0;`). Additional functions follow similar syntax.

## 2. Data Types

KIK supports primitive types (`int`, `float`, `char`, `bool`) and strings (via `str` or `char` arrays). Booleans use `true` or `false`.

```ebnf
<type> ::= "int" | "float" | "char" | "bool" | "str" | "void"
<array-type> ::= "char" <identifier> "[" <integer-literal> "]"
<boolean-literal> ::= "true" | "false"
```

Examples:

```kik
int age = 25;
float height = 5.9;
char grade = 'A';
bool isStudent = true;
str firstName;
char name[100];
```

## 3. Declarations

Variables can be declared with or without initialization. Multiple variables in a single declaration initialize only the last variable unless explicitly assigned. Constants use the `constant` keyword.

```ebnf
<declaration> ::= [ "constant" ] <type> <var-list> ";"
<var-list> ::= <var-init> { "," <var-init> }*
<var-init> ::= <identifier> [ "=" <expression> ]
```

Multiple Variables:

Syntax: `<type> var1, var2, var3 = value;` (only `var3` is initialized; `var1`, `var2` are uninitialized).
Explicit initialization: `a = b = c = 10;`.

Constants:

Syntax: `constant [type] constantName = value;`
Examples: `constant price_apple = 1.50;` (type inferred), `constant float price_banana = 0.75;`.

## 4. Expressions

Expressions include literals, variables, operators, function calls, string concatenation, and array access. Operator precedence mirrors major programming language (e.g., `*`, `/` before `+`, `-`).

```ebnf
<expression> ::= <assignment-expr> | <logical-expr> | <comparison-expr> | <arithmetic-expr> | <unary-expr> | <primary-expr>
<primary-expr> ::= <identifier> | <literal> | "(" <expression> ")" | <function-call> | <string-concat> | <array-access>
<literal> ::= <integer-literal> | <float-literal> | <char-literal> | <string-literal> | <boolean-literal>
<integer-literal> ::= [ "-" ] <digit>+
<float-literal> ::= [ "-" ] <digit>* "." <digit>+
<char-literal> ::= "'" <character> "'"
<string-literal> ::= '"' { <character> }* '"'
<arithmetic-expr> ::= <expression> ("+" | "-" | "*" | "/" | "%") <expression>
<comparison-expr> ::= <expression> ("==" | "!=" | ">" | "<" | ">=" | "<=") <expression>
<logical-expr> ::= <expression> ("&&" | "||") <expression> | "!" <expression>
<assignment-expr> ::= <identifier> ("=" | "+=" | "-=" | "*=" | "/=" | "%=") <expression>
<function-call> ::= <identifier> "(" [ <arg-list> ] ")"
<arg-list> ::= <expression> { "," <expression> }*
<string-concat> ::= <expression> "+" <expression>
<array-access> ::= <identifier> "[" <expression> "]"
<array-assign> ::= <identifier> "[" <expression> "]" "=" <expression>
```

Operators:

Arithmetic: `+`, `-`, `*`, `/`, `%`.
Assignment: `=`, `+=`, `-=`, `*=`, `/=`, `%=`.
Comparison: `==`, `!=`, `>`, `<`, `>=`, `<=` (return `bool`).
Logical: `&&` (AND), `||` (OR), `!` (NOT).

String Operations:

Declaration: `str name;`
Concatenation: `str result = string1 + string2;`
Access/Modify: `char c = str[0];`, `str[0] = 'X';`.

## 5. Statements

Statements include declarations, assignments, I/O operations, control structures, and control flow keywords.

```ebnf
<statement> ::= <declaration> | <expression> ";" | <io-statement> | <if-statement> | <switch-statement> | <loop-statement> | "return" <expression> ";" | "break;" | "continue;"
```

Input/Output:

Uses major programming language-style `cout << expr << endl;` and `cin >> var;`, or custom `io.kik` functions: `input()` (returns string), `output(message)`.

```ebnf
<io-statement> ::= "cout" "<<" <expression> { "<<" <expression> }* [ "<<" "endl" ] ";"
| "cin" ">>" <identifier> { ">>" <identifier> }* ";"
| "output" "(" <expression> ")" ";"
| <identifier> "=" "input" "(" ")" ";"
```

## 6. Control Structures

Control structures use a colon (`:`) after conditions or expressions, distinguishing KIK from major programming language.

If-Else:

```ebnf
<if-statement> ::= "if" <expression> ":" "{" <statement>* "}"
{ "else if" <expression> ":" "{" <statement>* "}" }*
[ "else" ":" "{" <statement>* "}" ]
```

Example: `if score >= 90: { cout << "Grade: A"; } else if score >= 80: { ... } else: { ... }`

Switch-Case:

```ebnf
<switch-statement> ::= "switch" <expression> ":" "{"
{ "case" <literal> ":" <statement>* "break;" }*
[ "default" ":" <statement>* ] "}"
```

Example: `switch day: { case 1: dayName = "Monday"; break; ... default: ... }`

Loops:

While:

```ebnf
<while-loop> ::= "while" <expression> ":" "{" <statement>* "}"
```

Example: `while count <= 5: { cout << count; count++; }`

Do-While:

```ebnf
<do-while-loop> ::= "do" "{" <statement>* "}" "while" <expression> ":" "{" "}"
```

Example: `do { cout << count; count++; } while count <= 5: { }` (empty block after colon).

For:

```ebnf
<for-loop> ::= "for" <initialization> ";" <expression> ";" ":" <increment> ":" "{" <statement>* "}"
<initialization> ::= [ <type> ] <assignment-expr>
<increment> ::= <expression>
```

Example: `for int count = 1; count <= 5;: count : { cout << count; }`

Nested Loops: For loops can be nested, e.g., `for int i = 1; i <= 5; i++ : { for int j = 1; j <= 5; j++ : { ... } }`

Break and Continue:

`break;` (exits loop or switch).
`continue;` (skips to next iteration).

## 7. Identifiers and Literals

```ebnf
<identifier> ::= <letter> { <letter> | <digit> | "_" }*
<digit> ::= "0".."9"
<letter> ::= "a".."z" | "A".."Z"
<character> ::= any printable character except "'" or '"'
```

Semantics:

KIK is statically typed. Uninitialized variables have undefined values.
Division by zero or invalid operations cause runtime errors.
Strings are mutable via index access.


## 8. Functions
 
 ### Function Declaration
```cpp=
## Basic function syntax
returnType functionName(parameterType parameterName): {
    ## Function body
    return value;
}
```
Example: Simple Function
```cpp=
## Function to add two numbers
int add(int a, int b): {
    return a + b;
}

int kik() {
    ## Function call
    int sum = add(5, 7);
    cout << "The sum is: " << sum << endl;  ## Output: 12
    return 0;
}
```

### Multiple Parameters
```cpp=
## Function with multiple parameters
int calculateArea(int length, int width): {
    return length * width;
}

int kik() {
    int area = calculateArea(5, 10);
    cout << "The area of the rectangle is: " << area << endl;  ## Output: 50
    return 0;
}
```
### Return Keyword
```cpp=
## Function with return value
int square(int number): {
    return number * number;
}

int kik() {
    int result = square(4);
    cout << "The square of 4 is: " << result << endl;  ## Output: 16
    return 0;
}
```
### Pass By Reference
```cpp=
## Function with pass by reference
void increment(int& value): {
    value++;  ## Modifies the original variable
}

int kik() {
    int number = 10;
    cout << "Original number: " << number << endl;  ## Output: 10
    
    increment(number);  ## Pass by reference
    
    cout << "Modified number after increment: " << number << endl;  ## Output: 11
    return 0;
}
```
### Variable Scope
#### Local Scope
```code=
int kik() {
    int localVar = 10;  ## Local variable
    cout << "Local variable: " << localVar << endl;  ## Output: 10
    return 0;
}
```

#### Global Variable

```cpp=
int globalVar = 20;  ## Global variable

int kik() {
    cout << "Global variable: " << globalVar << endl;  ## Output: 20
    return 0;
}
```

#### Parameter Scope
```cpp=
void display(int param): {
    cout << "Parameter value: " << param << endl;  ## Output: 30
}

int kik() {
    display(30);
    return 0;
}
```

##### Complete Code example for all concepts
```cpp=
import "io.kik";

## Global variable
int globalCounter = 0;

## Function with multiple parameters and return value
int calculate(int a, int b, int& resultRef): {
    int sum = a + b;  ## Local variable
    resultRef = sum;  ## Modify reference parameter
    
    globalCounter++;  ## Modify global variable
    
    return sum * 2;  ## Return value
}

int kik() {
    int x = 5, y = 3;
    int result;
    
    ## Function call
    int doubleSum = calculate(x, y, result);
    
    cout << "Sum: " << result << endl;  ## Output: 8
    cout << "Double sum: " << doubleSum << endl;  ## Output: 16
    cout << "Function called " << globalCounter << " time(s)" << endl;  ## Output: 1
    
    return 0;
}
```

## 9. Recursion

Sample code for factorial calculation using recursion
```cpp=
import "io.kik";

## Recursive factorial function
int factorial(int n) {
    if n <= 1: {
        return 1;  ## Base case
    } else: {
        return n * factorial(n - 1);  ## Recursive case
    }
}

int kik() {
    output("Enter a number to calculate its factorial: ");
    str inputStr = input();
    
    ## Convert string to integer
    int num = 0;
    for int i = 0; i < inputStr.length(); i++: {
        num = num * 10 + (inputStr[i] - '0');
    }
    
    if num < 0: {
        output("Factorial is not defined for negative numbers.");
        return 1;
    }
    
    int result = factorial(num);
    output("Factorial of " + num + " is " + result);
    
    return 0;
}
```


## 10.  Type Casting

Type casting in KIK allows conversion between compatible data types (e.g., int, float, char, bool, str). KIK supports both implicit and explicit type casting, enabling flexible data manipulation while maintaining type safety.


### Implicit Type Casting 

*  Syntax: dataType newVariable = existingVariable;
* Description: Automatically converts a variable to the target type when the conversion is safe (widening), such as from int to float. No explicit notation is required, and the conversion preserves the value with necessary adjustments (e.g., adding a decimal point for float).
* Use Case: Useful when assigning a smaller data type to a larger one, like int to float, where data loss is not a concern.
* Semantics: The compiler handles the conversion, ensuring compatibility. For example, assigning an int value of 5 to a float results in 5.0.

 
```kik=
## implicit_casting.kik - Simple Implicit Type Casting
int kik() {
    int intValue = 5;       ## Integer value
    float floatValue;       ## Float variable
    floatValue = intValue;  ## Implicitly converts int to float
    cout << "Integer: " << intValue << ", Float: " << floatValue << endl;  ## Outputs: Integer: 5, Float: 5.0
    return 0;               ## Indicate successful execution
}
```

### Explicit Type Casting

* Syntax: dataType newVariable = (dataType) existingVariable;
* Description: Forces conversion to the target type, even if it may result in data loss (narrowing), such as from float to int. The target type is specified in parentheses before the variable.
* Use Case: Required for conversions that may lose precision, like float to int, where the decimal part is truncated.
* Semantics: The programmer explicitly indicates the desired type. For example, casting a float value of 7.8 to int yields 7.

```kik=
## explicit_casting.kik - Simple Explicit Type Casting
int kik() {
    float floatValue = 7.8;  ## Float value
    int intValue;            ## Integer variable
    intValue = (int) floatValue;  ## Explicitly converts float to int
    cout << "Float: " << floatValue << ", Integer: " << intValue << endl;  ## Outputs: Float: 7.8, Integer: 7
    return 0;                ## Indicate successful execution
}
```

## 11. Exception Handling
Exception handling in KIK provides a mechanism to manage runtime errors gracefully using try, catch, and throw constructs, ensuring robust program execution.

* Syntax:

```ebnf=
<try-catch-statement> ::= "try" ":" <statement>* ":" <catch-block>+
<catch-block> ::= "catch" "(" <type> <identifier> ")" ":" <statement>* ":"
<throw-statement> ::= "throw" <expression> ";"
```
* Description:
     * try:Encloses a block of code that may throw an exception. The block is delimited by colons (:).
    * catch: Handles exceptions thrown in the try block, specifying the exception type (e.g., int, str) and an identifier to access the thrown value. Multiple catch blocks can handle different exception types.
    * throw: Raises an exception with a specified value (e.g., throw 0; or throw "Error message";).

* Use Case: Used to handle errors like division by zero, invalid input, or file access failures, preventing program crashes.
* Semantics: When an exception is thrown, control transfers to the matching catch block. If no matching catch is found, the program terminates with a runtime error. The catch block processes the exception and continues execution.

 ```kik=
 ## division.kik - Simple Exception Handling
import "io.kik";

int kik() {
    float number = 10.0;
    float divisor;
    float result;
    output("Enter a divisor: ");
    divisor = input();  ## Assume input is converted to float
    try:
        if divisor == 0:
            throw "Zero divisor";
        result = number / divisor;
        cout << "Result: " << result << endl;  ## Outputs result if no exception
    :
    catch (str error):
        cout << "Error: " << error << endl;  ## Outputs: Error: Zero divisor
    :
    return 0;  ## Indicate successful execution
}
```
### 12. Dynamic Memory Allocation 

Dynamic memory allocation in KIK allows runtime allocation and deallocation of memory using new and delete, supporting both single variables and arrays.

* Syntax:

```ebnf=
<dynamic-memory-statement> ::= <type> "$" <identifier> ":" "new" <type> [ "[" <expression> "]" ] ";" 
                             | "delete" <identifier> ";" 
                             | "delete[]" <identifier> ";"
```
* Description:
  * Declaration: Pointers are declared with a $ symbol (e.g., int$ ptr;).
  * Allocation:
    * new dataType; allocates memory for a single variable (e.g., int$ ptr: new int;).
    * new dataType[size]; allocates memory for an array (e.g., int$ arr: new int[5];).
  * Deallocation:
     * delete ptr; frees memory for a single variable.
    * delete[] arr; frees memory for an array
 * Dereference: Use *ptr to access or modify the value at the pointer's address (e.g., *ptr = 42; or value = *ptr;).
* Use Case: Enables flexible memory management for variables and arrays whose size is determined at runtime, such as dynamic arrays or objects.

* Semantics: Memory allocated with new resides on the heap and must be explicitly deallocated with delete or delete[] to prevent memory leaks. Accessing a pointer after deletion or dereferencing a null pointer causes runtime errors.


```kik=
## dynamic_memory.kik - Simple Dynamic Memory Allocation
int kik() {
    int$ ptr: new int;     ## Allocate memory for a single integer
    *ptr = 15;             ## Assign value
    cout << "Value: " << *ptr << endl;  ## Outputs: Value: 15
    delete ptr;            ## Deallocate memory
    
    int$ array: new int[3];  ## Allocate memory for an array
    array[0] = 1; array[1] = 2; array[2] = 3;  ## Assign values
    cout << "Array: " << array[0] << " " << array[1] << " " << array[2] << endl;  ## Outputs: Array: 1 2 3
    delete[] array;        ## Deallocate array
    return 0;              ## Indicate successful execution
}
```

## 13. Files

File pointer 
```cpp=
 filePointer file = fopen("example.txt", "w");
```
File read 
```cpp=
fread(dataPointer, size, count, filePointer); ## Read data from the file
```

File write 
```cpp=
fwrite(filePointer, dataPointer, size, count);
## Write data to the file
```

File close 
```cpp=
   fclose(file);
```


#### Sample code for file operations 
```kik
import "file.kik";  // Assuming file operations are in this module

int kik() {
    // Data to write to the file
    str data = "Hello, Kik! This is a file operation example.";
    int dataSize = data.length();  // Get string length

    // Open the file in write mode
    filePointer file = fopen("example.txt", "w");
    if file == NULL: {
        cout << "Error opening file for writing." << endl;
        return 1;  // Indicate failure
    }

    // Write data to the file
    fwrite(file, data, 1, dataSize);  // Write characters one by one
    fclose(file);  // Close the file

    // Open the file in read mode
    file = fopen("example.txt", "r");
    if file == NULL: {
        cout << "Error opening file for reading." << endl;
        return 1;  // Indicate failure
    }

    // Buffer to read data from the file
    char buffer[256];  // Buffer to hold read data
    fread(buffer, 1, dataSize, file);  // Read dataSize characters
    buffer[dataSize] = '\0';  // Null-terminate the string
    fclose(file);  // Close the file

    // Display the read data
    cout << "Read from file: " << buffer << endl;
    return 0;  // Indicate successful execution
}

```


---

## 14. Structures (struct)
This section provides a complete specification for user-defined structures in the KIK language. Structures are a powerful feature for grouping related data into a single, custom data type.

### 14.1. Design and Purpose
A *structure* is a composite data type that allows you to bundle together variables of different types under a single name. This is useful for creating logical records, such as representing a student with an ID, name, and GPA, or a point with x and y coordinates.

The design of KIK's struct is aligned with its C++ heritage and is stylistically consistent with the class definition, using KIK's signature colon (:) to introduce the body. By default, all members of a struct are *public*.

### 14.2. Feature Specification

#### Syntax for Definition
A struct is defined using the struct keyword, a unique name for the new type, a colon, a body of member variables enclosed in curly braces ({}), and a terminating semicolon (;).

```kik
// Defines a new custom type named 'Student'
struct Student: {
    int student_id;
    str name;
    float gpa;
};

// Defines a new custom type named 'Point'
struct Point: {
    int x;
    int y;
};

## Sample code --> factorial.kik - KIK Program to Calculate Factorial of a Number

```kik
import "io.kik";

int kik() {
int factorial = 1;
output("Enter a positive integer: ");
number = input();
## Convert string input to integer (assuming input is valid)
int num = 0;
for int i = 0; i < number.length(); i++: {
num = num * 10 + (number[i] - '0');
}
if num < 0: {
cout << "Error: Please enter a non-negative number!" << endl;
return 1;
}
## Calculate factorial using a loop
for int i = 1; i <= num; i++: {
factorial *= i;
}
cout << "Factorial of " << num << " is " << factorial << endl;
return 0;
}
```

### Variable Declaration
Once a struct type has been defined, you can declare variables of that type just like any primitive type.

// Declares a variable 's1' of type 'Student'
``` 
Student s1;
```
// Declares and initializes a variable 'origin' of type 'Point'
```
Point origin;
```

### Member Access
The members of a struct variable are accessed using the dot (.) operator. If you have a pointer to a struct (a feature from the Pointers & References module), you would use the arrow (->) operator.

```
// Create a Student variable
Student s1;

// Assign values to its members using the dot operator
s1.student_id = 101;
s1.name = "Ojas Ghambeera";
s1.gpa = 3.8;

// Read a value from a member
output("Student Name: ");
output(s1.name);

// Conceptual example of pointer access
// Student* s_ptr = &s1;
// s_ptr->gpa = 4.0;

```

## 14.3. Grammar Rules (EBNF)
The following grammar rules are added to the KIK language specification to support structures

```// 1. Modify the <declaration> rule to include struct definitions.
// This allows a struct to be defined anywhere a variable or class can be.
<declaration> ::= <var_declaration> 
                | <const_declaration> 
                | <class_definition> 
                | <struct_definition> ;

// 2. Add the new rule for a struct definition.
// It consists of the 'struct' keyword, a name, a colon, and a list of member variables.
<struct_definition> ::= "struct" <identifier> ":" "{" { <member_variable> }* "}" ";" ;

// 3. Define the rule for a member variable inside a struct.
// This reuses the existing rule for variable declarations.
<member_variable> ::= <type> <var-list> ";" ;

// 4. Modify the expression grammar to include member access.
// This adds the dot and arrow operators.
<primary-expr> ::= <identifier> 
                 | <literal> 
                 | "(" <expression> ")" 
                 | <function-call>
                 | <array-access>
                 | <member-access> ;

// 5. Add the new rule for member access expressions.
<member-access> ::= <primary-expr> "." <identifier>
                  | <primary-expr> "->" <identifier> ;

```

## 14.4. Complete Test Case
This sample program demonstrates the definition, declaration, initialization, and use of a struct in KIK. It shows how struct variables can be used and passed to functions.

```
 * KIK Language - Structure Test Case
 * This program defines a 'Rectangle' struct, creates an instance,
 * calculates its area, and prints the details.
 */

// 1. Define a 'Rectangle' struct with width and height members.
struct Rectangle: {
    int width;
    int height;
};

// 2. A helper function that takes a Rectangle struct as a parameter.
void printRectangleInfo(Rectangle rect) {
    output("--- Rectangle Info ---");
    output("Width: ");
    output(rect.width);
    output("Height: ");
    output(rect.height);

    // Calculate the area inside the function
    int area = rect.width * rect.height;
    output("Area: ");
    output(area);
}

// Main program entry point
int kik() {
    // 3. Declare a variable 'rect1' of our new 'Rectangle' type.
    Rectangle rect1;

    output("Initializing rectangle...");

    // 4. Assign values to the members of the 'rect1' variable.
    rect1.width = 20;
    rect1.height = 30;

    // 5. Pass the struct variable to our helper function.
    printRectangleInfo(rect1);

    return 0;
}
```
# 15. Object-Oriented Programming (OOP) Features

This section details the Object-Oriented Programming (OOP) features for the KIK language. The design aligns with the existing C++-inspired procedural nature of KIK, extending its syntax to support modern OOP paradigms. The goal is to provide a powerful, intuitive, and consistent set of features for creating complex, modular programs.

This module covers the design and implementation of:

- Classes and Objects
- Access Modifiers (public, private)
- Inheritance
- Polymorphism (virtual, override)

## 15.1. Design Philosophy

The OOP syntax is designed to be a natural extension of KIK’s core style. It maintains a strong C++ influence for familiarity while integrating KIK's unique use of the colon (:) to introduce code blocks. This creates a consistent feel across procedural and object-oriented code within the language.

## 15.2. Feature Specification

### 15.2.1. Classes and Objects

A class is a blueprint for creating objects. The class definition begins with the `class` keyword, followed by the class name, KIK's signature colon (`:`), and a body enclosed in curly braces (`{}`). A semicolon is required after the closing brace.

Syntax:

```kik
class MyClass: {
    // member declarations
};
```

### 15.2.2. Access Modifiers

KIK supports encapsulation through access modifiers. Members are grouped into `public` and `private` sections, which are introduced by the keyword followed by a colon.

- `public::`: Members are accessible from anywhere.
- `private::`: Members are only accessible from within the class.

Syntax:

```kik
class Player: {
private:
    // This member can only be accessed by Player's functions
    int health;
public:
    // These members can be accessed from anywhere
    str name;
    void takeDamage(int amount);
};
```

### 15.2.3. Inheritance

Inheritance allows a new class (derived class) to inherit properties and methods from an existing class (base class). KIK uses the standard C++ syntax for public inheritance.

Syntax:

```kik
class Animal: {
public:
    int age;
};

// The Dog class inherits all public members from Animal
class Dog : public Animal: {
public:
    str breed;
};
```

### 15.2.4. Polymorphism

Polymorphism allows objects of different classes to be treated as objects of a common base class. This is achieved through virtual functions.

- `virtual`: This keyword is used on a function in the base class to indicate that it can be overridden by derived classes.
- `override`: This keyword is used on a function in the derived class to state that it is intentionally replacing a virtual function from a base class. This acts as a safety check to prevent bugs from typos.

Syntax:

```kik
class Shape: {
public:
    // This function can be overridden
    virtual void draw();
};

class Circle : public Shape: {
public:
    // This function redefines the draw() function from Shape
    void draw() override;
};
```

## 15.3. Grammar Rule Additions (EBNF)

The following rules should be added to the main KIK grammar to support OOP features.

```ebnf
// A declaration can now also be a class definition.
<declaration> ::= <var_declaration> | <const_declaration> | <class_definition> ;

// The formal rule for a class definition
<class_definition> ::= "class" <identifier> [ <inheritance_specifier> ] ":" "{" { <class_member> }* "}" ";" ;

// The rule for specifying a base class for inheritance
<inheritance_specifier> ::= ":" "public" <identifier> ;

// A class member can be an access specifier or a member declaration
<class_member> ::= <access_specifier> | <member_declaration> ;
<access_specifier> ::= "public" ":" | "private" ":" ;

// Member declarations are standard function/variable declarations
// with optional 'virtual' and 'override' keywords for functions.
<member_declaration> ::= [ "virtual" ] <type> <identifier> "(" [ <parameter-list> ] ")" [ "override" ] ";"
                       | <type> <var-list> ";" ;
```

## 15.4. Complete Example

This program demonstrates all the newly designed OOP features working together.

```cpp=
// Base class with a virtual function
class Character: {
public:
    // This function can be overridden by derived classes
    virtual void introduce();
};

// Definition for the Character's function
// Note: The '::' scope resolution operator will be defined in a later module.
// For now, we assume a naming convention like ClassName_FunctionName.
void Character_introduce() {
    output("I am a generic character.");
}

// Derived class that inherits from Character
class Wizard : public Character: {
public:
    // Override the base class function for a specific implementation
    void introduce() override;
};

// Definition for the Wizard's function
void Wizard_introduce() {
    output("I am a powerful Wizard!");
}

// Main program entry point
int kik() {
    output("--- KIK OOP Demo ---");

    // The design for object creation ('new' keyword) and pointers
    // will be added in a future module. The following code is a
    // conceptual demonstration of how polymorphism would work.
    
    // Character* player1 = new Character();
    // Character* player2 = new Wizard();
    
    // player1->introduce(); // Expected Output: I am a generic character.
    // player2->introduce(); // Expected Output: I am a powerful Wizard!
    
    // delete player1;
    // delete player2;

    output("\nKIK program finished successfully.");
    return 0;
}
```




## Test Cases for KIK Language Compiler
### Test Case 1: Basic Syntax and Control Structures (basic_syntax.kik)

```cpp=
import "io.kik";

int kik() {
    // Variable declarations and basic arithmetic
    int a = 10;
    float b = 5.5;
    int result = a + (int)b;
    
    // Conditional statements
    if result > 10: {
        cout << "Result is greater than 10: " << result << endl;
    } else: {
        cout << "Result is 10 or less: " << result << endl;
    }
    
    // Loop structures
    for int i = 0; i < 5; i++: {
        cout << "Loop iteration: " << i << endl;
    }
    
    int count = 0;
    while count < 3: {
        cout << "While loop count: " << count << endl;
        count++;
    }
    
    return 0;
}
```
### Test Case 2: Functions and Recursion (functions.kik)

```cpp=
import "io.kik";

// Global variable
int globalCounter = 0;

// Function with parameters and return value
int multiply(int x, int y): {
    globalCounter++;
    return x * y;
}

// Recursive function
int factorial(int n): {
    if n <= 1: {
        return 1;
    } else: {
        return n * factorial(n - 1);
    }
}

// Function with pass by reference
void increment(int& value): {
    value++;
}

int kik() {
    int num1 = 5;
    int num2 = 7;
    
    // Function call
    int product = multiply(num1, num2);
    cout << "Product: " << product << endl;
    
    // Recursive function call
    int fact = factorial(5);
    cout << "Factorial of 5: " << fact << endl;
    
    // Pass by reference
    int value = 10;
    cout << "Original value: " << value << endl;
    increment(value);
    cout << "After increment: " << value << endl;
    
    cout << "Function called " << globalCounter << " times" << endl;
    
    return 0;
}
```

### Test Case 3: File Operations and Exception Handling (file_ops.kik)

```cpp=
import "io.kik";
import "file.kik";

int kik() {
    // File writing
    str data = "Hello, KIK Language!";
    filePointer file = fopen("test.txt", "w");
    
    if file == NULL: {
        cout << "Error opening file for writing." << endl;
        return 1;
    }
    
    fwrite(file, data, 1, data.length());
    fclose(file);
    
    // File reading with exception handling
    try:
        file = fopen("test.txt", "r");
        if file == NULL: {
            throw "File not found";
        }
        
        char buffer[100];
        fread(buffer, 1, data.length(), file);
        buffer[data.length()] = '\0';
        fclose(file);
        
        cout << "File content: " << buffer << endl;
    :
    catch (str error):
        cout << "Error: " << error << endl;
    :
    
    // Division with exception handling
    try:
        float numerator = 10.0;
        float denominator = 0.0;
        
        if denominator == 0.0: {
            throw "Division by zero";
        }
        
        float result = numerator / denominator;
        cout << "Division result: " << result << endl;
    :
    catch (str error):
        cout << "Math error: " << error << endl;
    :
    
    return 0;
}
```

### Test Case 4: Type Casting and Dynamic Memory (memory_ops.kik)
```cpp=
import "io.kik";

int kik() {
    // Implicit type casting
    int intValue = 5;
    float floatValue = intValue;
    cout << "Implicit cast: " << intValue << " -> " << floatValue << endl;
    
    // Explicit type casting
    floatValue = 7.8;
    intValue = (int)floatValue;
    cout << "Explicit cast: " << floatValue << " -> " << intValue << endl;
    
    // Dynamic memory allocation
    int$ ptr: new int;
    *ptr = 42;
    cout << "Dynamic value: " << *ptr << endl;
    delete ptr;
    
    // Dynamic array
    int$ arr: new int[3];
    for int i = 0; i < 3; i++: {
        arr[i] = i * 10;
    }
    
    cout << "Dynamic array: ";
    for int i = 0; i < 3; i++: {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    delete[] arr;
    
    return 0;
}
```

### Test Case 5: Object-Oriented Programming (oop.kik)
```cpp=
import "io.kik";

// Base class
class Shape: {
public:
    virtual float area();
};

// Derived class
class Rectangle : public Shape: {
private:
    float width;
    float height;
public:
    Rectangle(float w, float h): {
        width = w;
        height = h;
    }
    
    float area() override: {
        return width * height;
    }
};

// Another derived class
class Circle : public Shape: {
private:
    float radius;
public:
    Circle(float r): {
        radius = r;
    }
    
    float area() override: {
        return 3.14159 * radius * radius;
    }
};

int kik() {
    // Create objects
    Rectangle rect(5.0, 3.0);
    Circle circle(2.5);
    
    // Calculate and display areas
    cout << "Rectangle area: " << rect.area() << endl;
    cout << "Circle area: " << circle.area() << endl;
    
    return 0;
}
```

## Library Creation
We are learning it 

##  Plan for Editor Development
we are planning for it 

## Work Division & Schedule (3 Weeks)

| Date        | Member      | Module / Task                                | Details |
|-------------|-------------|----------------------------------------------|---------|
| Aug 18 - Aug 24 | Satya       | Module 1: Basic Syntax & Data Types          | Define basic syntax and data types,  keywords (int, float, char, bool, str, void, constant,return). Implement parser support. |
| Aug 18 - Aug 24 | Guru Rohith | Module 1: Iterative & Control Structures     | Implement loops, conditionals, and flow control keywords.(if, else, switch, case, default,while, do, for,break, continue) |
| Aug 18 - Aug 24 | Aman        | Module 1: Function Basics and expression, statement and identifiers and dynamic allocation. | Define function declaration/return rules, syntax rules for simple function,and dynamic memory allocation using new/delete. |
| Aug 25 - Aug 31 | Satya       | Module 2: Pointers                           | Design pointer syntax, references. |
| Aug 25 - Aug 31 | Guru Rohith | Module 2: OOP (Classes & Objects)            | Implement classes, access modifiers, inheritance, and polymorphism. |
| Aug 25 - Aug 31 | Aman        | Module 2: Extended Functions & Scope Rules & pass by reference, and handled exception. | Extend functions with overloading , multiple parameters, reference passing, scope, and namespaces. |
| Sep 1 - Sep 7   | Satya       | Module 3: File Operations                    | Implement fopen, fclose, fread, fwrite. |
| Sep 1 - Sep 7   | Guru Rohith | Module 3: Arrays & Structures                | Design syntax for single and multi-dimensional arrays, structures. |
| Sep 1 - Sep 7   | Aman        | Module 3: Implemented Recursion and type casting . Exception handling. | Give sample recursion code and implicit as well as explicit type casting syntax, description. |






