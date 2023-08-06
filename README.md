# Dictionary_avl
A generic implementation of a dictionary basead on the [AVL Tree data structure](https://en.wikipedia.org/wiki/AVL_tree) in c++.

## Instalation
The [dict.hpp](dict.hpp) file can be dropped into an existing project in c++ and used.

## Usage
An example of usage can be found in [example.cpp](example.cpp).

For a user-defined data type (UDT), it is necessary to [overload](https://en.cppreference.com/w/cpp/language/operators) assign, relational and comparison operators.

## Member-functions:

### Iterator begin ()
Return an iterator to the begin of the dictionary.

### Iterator end ()
Return an iterator to the end of the dictionary.

### Iterator insert (TK c, TV v)
Insert the pair (c, v) with types key(TK) and value(TV) respectively.
Return an iterator to the inserted pair;

### Iterator search (TK c)
Return an iterator to the element whose key is equal to c.

### void remove (Iterator i)
Remove the element pointed to the i iterator.

### void remove (TK c)
Remove the element whose key is equal to c.

## Licence information
This project is licensed under the GNU General Public License v3.0.
