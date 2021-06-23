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


---
ISO C++ standard defines two kinds of entities:
- Core language features such as built-in types and iteration statements.
- Standard library components such as containers (`vector`, `map`, ..) and IO operations (`<<`, `>>`, `getline()`).

## Basics of the language

### main and basic IO

```c++
#include <iostream> // >>, <<, getline() etc for io ops

int main() {

    std::cout << "Hello World/n";
    return 1; // indicates success, < 1 is usually unsuccessful.

}
```

- `std` is the standard namespace and specifies that standard output is to be found in the single standard namespace.
- `cout` is just stdout. idk why it's not just stdout/stdin tbh...? To emphasize we can only access a single namespace? 

> The `std::` could be left out once we write `using namespace std;` under all the includes.

ie.

```c++
#include <iostream>
using namespace std;

int main() {

    cout << "Hello World/n";
    return 1;

}
```

**cout/cin**

Example program with st input/output:
```c++
bool accept()
{
    cout << "Do you want to proceed (y or n)?\n"; // OUTPUT

    char answer = 0;
    cin >> answer; // TAKING INPUT

    return answer == 'y';
}
```

**>> and <<**

- `>>` is the "get from" operator.
- `<<` is the "put to" operator.

### types, variables, and arithmetic

```c++
bool // Boolean, possible values are true and false
char // character, for example, 'a', ' z', and '9'
int // integer, for example, 1, 42, and 1066
double // double-precision floating-point number, for example, 3.14 and 299793.0
```
Each fundamental type corresponds directly to hardware facilities and has a fixed size that determines the range of values that can be stored in it:
- `bool` and `char` are a byte.
- `int` is usually 4 bytes
- `double` is unsuprisingly 8 bytes.

Can check size allocated in memory using `sizeof()`.

Arithmetic/Comparison operators are all same as C, except: 
- `+x` unary plus
- `-x` unary minus????

what are these?

Since C++ is weakly typed, when defining a variable the type need not be typed explicitly when it can be deduced from the initializer using the `auto` keyword.

```c++
auto b = true; // a bool
auto ch = 'x'; // a char
auto i = 123; // an int
auto d = 1.2; // a double
auto z = sqrt(y); // z has the type of whatever sqr t(y) retur ns
```
Using auto, we avoid redundancy and writing long type names. This is especially important in generic programming where the exact type of an object can be hard for the programmer to know and the type names can be quite long.

**Constants**

In C++ there are two main notions of immutability:
- `const`
- `constexpr`


Basic conditional and iteration statements are pretty much the same as in C...

## Pointers, Arrays, etc.

An array of some type is declared just as in C:
```c++
type arr[size]; // block of memory identified by "arr".
```
Similarly a pointer:
```c++
type* p; // pointer to type in memory.
```


**Pointer recap**

- A pointer variable can hold the address of an object of the appropriate type:

ie. 

```c++
char* p = &arr[5] // p points to the address of 6th elem in arr
char p1 = *p // p1 points to the object p points to.
```

**The prefix operators**

- A prefix unary `*` means "contents of", aka the dereference operator; ie `*ptr` means the contents of the address that `ptr` points to.
- A prefix unary `&` means "address of"; ie `&myvalue` means the address of `myvalue` in memory.

> A variable that stores the address of another variable in memory is a pointer.

Due to the ability of a pointer to directly refer to the value that it points to, a pointer has different properties when it points to a char than when it points to an int or a float. Once dereferenced, the type needs to be known. And for that, the declaration of a pointer needs to include the data type the pointer is going to point to.

```c++
int foo = 64; // some value identified by "foo" in memory.
int* bar = &foo; // pointer called bar stores the addr of foo in memory.
int baz = *bar; // dereference the addr to get 64 back.

cout << foo << " " << bar << " " << baz;
```

**Suffix Operators**

 > The `*` coming after (suffix) the type declares that it is a pointer.

- A suffix unary `*` declares a pointer variable, ie `int*` creates a pointer to a location in memory, treated as an `int` value.
- A suffix unary `&` means "reference to", this is similar to a pointer but we need not use a prefix `*` to access value at reference. 


When used in declarations, operators (such as &, ∗, and []) are called declarator operators:
```c++
T a[n]; // T[n]: array of n Ts 
T∗ p; // T*: pointer to T 
T& r; // T&: reference to T
T f(A); // T(A): function taking an argument of type A returning a result of type T
```

**Pointers and arrays**

- Pointers and arrays support the same set of operations, with the same meaning for both. The main difference being that pointers can be assigned new addresses, while arrays cannot.

ie.

```c++
// more pointers
#include <iostream>
using namespace std;

int main ()
{
  int numbers[5];
  int * p;
  p = numbers;  *p = 10;
  p++;  *p = 20;
  p = &numbers[2];  *p = 30;
  p = numbers + 3;  *p = 40;
  p = numbers;  *(p+4) = 50;
  for (int n=0; n<5; n++)
    cout << numbers[n] << ", ";
  return 0;
}
```

**Pointer arithmetic**

To conduct arithmetical operations on pointers is a little different than to conduct them on regular integer types. To begin with, only addition and subtraction operations are allowed; the others make no sense in the world of pointers. But both addition and subtraction have a slightly different behavior with pointers, according to the size of the data type to which they point.

- When adding one to a pointer, the pointer is made to point to the following element of the same type, and, therefore, the size in bytes of the type it points to is added to the pointer since that will be the location to the next val of that type in the structure being traversed.

Post/Pre increment:
```c++
*p++   // same as *(p++): increment pointer, and dereference unincremented address
*++p   // same as *(++p): increment pointer, and dereference incremented address
++*p   // same as ++(*p): dereference pointer, and increment the value it points to
(*p)++ // dereference pointer, and post-increment the value it points to 
```

Pointers can be passed as `const`, hence will not be altered after call - otherwise altered.

ie.

```c++
void print_all (const int* start, const int* stop)
{
  const int * current = start;
  while (current != stop) {
    cout << *current << '\n';
    ++current;     // increment pointer
  }
}
```

**void pointers**

The void type of pointer is a special type of pointer. In C++, void represents the absence of type. Therefore, void pointers are pointers that point to a value that has no type (and thus also an undetermined length and undetermined dereferencing properties).

This gives void pointers a great flexibility, by being able to point to any data type, from an integer value or a float to a string of characters. In exchange, they have a great limitation: the data pointed to by them cannot be directly dereferenced (which is logical, since we have no type to dereference to), and for that reason, any address in a void pointer needs to be transformed into some other pointer type that points to a concrete data type before being dereferenced.


**invalid and null pointers**

In C++, pointers are allowed to take any address value, no matter whether there actually is something at that address or not. What can cause an error is to dereference such a pointer (i.e., actually accessing the value they point to). Accessing such a pointer causes undefined behavior, ranging from an error during runtime to accessing some random value.

But, sometimes, a pointer really needs to explicitly point to nowhere, and not just an invalid address. For such cases, there exists a special value that any pointer type can take: the null pointer value. This value can be expressed in C++ in two ways: either with an integer value of zero, or with the nullptr keyword:

```c++
int * p = 0;
int * p2 = nullptr; // there is also NULL from some header, may be used in older code.
```

> Since pointers could be initialized, pointing to `nullptr` it is wise to check if a pointer is null when necessary and handle that case.

**pointers to functions**

C++ allows operations with pointers to functions. The typical use of this is for passing a function as an argument to another function. Pointers to functions are declared with the same syntax as a regular function declaration, except that the name of the function is enclosed between parentheses () and an asterisk (*) is inserted before the name:

```c++
// pointer to functions
#include <iostream>
using namespace std;

int addition (int a, int b)
{ return (a+b); }

int subtraction (int a, int b)
{ return (a-b); }

int operation (int x, int y, int (*functocall)(int,int)) // uncurried function call xD
{
  int g;
  g = (*functocall)(x,y);
  return (g);
}

int main ()
{
  int m,n;
  int (*minus)(int,int) = subtraction;

  m = operation (7, 5, addition);
  n = operation (20, m, minus);
  cout <<n;
  return 0;
}
```


**Basic looping and ranges**

A simple for loop is the old boring:
```c++
for (auto i = 0; i < 10; i++ ) {
    ...
}
```
and loops 10 times.

C++ offers for each and range loops as so:
```c++
void print()
{
    int v[] = {0,1,2,3,4,5,6,7,8,9};

    for (auto x : v) // for each x in v
        cout << x << '\n'; // print x ...

    for (auto x : {10,21,32,43,54,65}) // for each elem in that list, x
        cout << x << '\n'; // print x ...
    // ...
}
```

## User Defined Types

C++’s set of built-in types and operations is rich, but deliberately low-level. They directly and efficiently reflect the capabilities of conventional computer hardware. 

We define user-defined types using the `struct` keyword.

ie.

```c++
struct Vector {
    int sz; // size of structure
    double * elem; // pointer to block of elements.
}
```

Then, when constructing a `Vector`,  these properties must be provided:

```c++
// "constructor"
void vector_init(Vector& v, int s)
{
    v.elem = new double[s]; // allocate an array of s doubles
    v.sz = s; // set sz to given size.
}
```

- The `Vector`, `v` gets `elem` pointing to an array of size `s` and sets `sz` to the given value `s`.
- The `new` operator allocates memory from an area called the free store (also known as dynamic memory and heap) - just like in Java.

Using `Vector` with the written function `vector_init` to construct it with:

```c++
double read_and_sum(int s)
// read s integers from cin and return their sum; s is assumed to be positive
{
    Vector v;
    vector_init(v,s); // allocate s elements for v
    for (int i=0; i!=s; ++i)
        cin>>v.elem[i]; // read into elements

    double sum = 0;
    for (int i=0; i!=s; ++i)
        sum+=v.elem[i]; // take the sum of the elements
    return sum;
}
```

- We use `.` to access named members of `struct` types.
- We use `->` to access `struct` members through a pointer.

ie.

```c++
void f(Vector v, Vector& rv, Vector∗ pv)
{
    int i1 = v.sz; // access through name
    int i2 = rv.sz; // access through reference
    int i4 = pv−>sz; // access through pointer
}
```

## Classes in C++

`Vector` as a class:

```c++
class Vector {
    public:
        Vector(int s) :elem{new double[s]}, sz{s} { } // constr uct a Vector
        double& operator[](int i) { return elem[i]; } // element access: subscripting
        int size() { return sz; }
    private:
        double∗ elem; // pointer to the elements
        int sz; // the number of elements
};
```

<!-- <script src="https://gist.github.com/guiqiqi/70859d2f810acd5e21b98cd23cb00b88.js"></script> -->
<script src="https://gist.github.com/Donkrzawayan/af5fda0c1427b0f5252aa090c1c97b61.js"></script>
















