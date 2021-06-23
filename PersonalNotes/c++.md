---
layout: default
title: C++ notes
---

## C++ aka C with Classes descrption and capabilities:

**Style**
C++ is a general-purpose programming language (with a bias towards systems programming) providing a direct and efficient model of hardware combined with facilities for defining lightweight abstractions.

By systems programming I mean writing code that directly uses hardware resources, has serious resource constraints, or closely interacts with code that does. In particular, the implementation of software infrastructure (e.g., device drivers, communications stacks, virtual machines, operating systems, and foundation libraries) is mostly systems programming.

C++ allows the combination of multiple programming "styles"/paradigms such as:
- Procedural programming: programming focused on processing and the design of suitable data structures.
- Data abstraction: programming focused on the design of interfaces, hiding implementation details in general and representations in particular.
- Object-oriented programming:  programming focused on the design, implementation, and use of class hierarchies.
- Generic programming: programming focused on the design, implementation, and use of general algorithms. Templates provide parametric polymorphism.

"The styles mentioned are not distinct alternatives: each contributes techniques to a more expressive and effective style of programming, and C++ provides direct language support for their use in combination."

**Type Checking**
The notion of static types and compile-time type checking is central to effective use of C++.
- Statically typed: if the type of a variable is known at compile-time instead of at run-time; once a variable has been declared with a type, it cannot ever be assigned to some other variable of different type and doing so will raise a type error at compile-time.
- Dynamically typed: type of a variable is checked during run-time; variables are bound to objects at run-time by means of assignment statements, and it is possible to bind the same variables to objects of different types during the execution of the program.
- Strongly-typed language is one in which variables are bound to specific data types, and will result in type errors if types do not match up as expected in the expression — regardless of when type checking occurs.
- Weakly-typed language on the other hand is a language in which variables are not bound to a specific data type; they still have a type, but type safety constraints are lower compared to strongly-typed languages.

C++ is statically & weakly typed language.

**C++ vs C**
The difference between C and C++ is primarily in the degree of emphasis on types and structure. C is expressive and permissive. Through extensive use of the type system, C++ is even more expressive without loss of performance.

**Language, Libraries, and Systems**
- C++ has no built-in high-level data types and no high-level primitive operations.
- Naturally, the use of C++ for larger programs leads to the use of C++ by groups of programmers. C++’s emphasis on modularity, strongly typed interfaces, and flexibility pays off here.
- C++ supports systems programming. This implies that C++ code is able to effectively interoperate with software written in other languages on a system.
- C++ was designed to operate within a single address space. The use of multiple processes and multiple address spaces relied on (extralinguistic) operating system support. A C++ programmer would have the operating systems command language available for composing processes into a system, just about any scripting language will do.

**Compilation**
- With gcc: `gcc file.cpp -lstdc++` to link properly.
- With g++: `g++ file.cpp`.
- Change name of object file: `g++ filename file.cpp`, then execute `./filename`.


---

## Basics

The ISO C++ standard defines two kinds of entities:
- Core language features such as built-in types and iteration statements.
- Standard library components such as containers (`vector`, `map`, ..) and IO operations (`<<`, `>>`, `getline()`).


**main and basic IO**

```c++
int main() {

}
```











