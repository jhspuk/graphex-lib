//This is a test for the first rejig of graphEx - creating an architecture
//that is mainly independent of content, for executing and analysing
//graph structures. One big problem of the prior version was being able
//to manipulate structures already brought into memory, such as post
//join() functionality. This will address this by transitioning between
//concept datastructures, and implementation data strctures. And by keeping
//agnostic the backbone of the program.

//Another large issue with the previous issue is a specific issue with
//how cpp processes templates - the types must be specified at all levels
//which makes writing code cumbersome. This version will take advantage 
//of std::variant (or similar) to provide type erasure.

#include <iostream>
#include <variant>
