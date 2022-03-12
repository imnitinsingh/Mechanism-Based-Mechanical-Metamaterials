#ifndef EDGE_HPP
#define EDGE_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include "Vector2.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

// Edge maybe later becomes an array of structures.
struct Edge
{
   // In C++, We can have more than one constructor in a class with same name, as long as each has a different list of arguments.This concept is known as Constructor Overloading
   // A constructor is called depending upon the number and type of arguments passed 
   Edge() noexcept {}
   // Constructors provide support for multiple ways of object creation and initialization. Constructor is a special member function.
   /*Q - what does noexcept keyword does in C++ ?
   noexcept lets the compiler knows that this function never throws exception
   in some cases C++ compiler is able to generate better machine code after the addition of noexcept.
   So far from what I have read, no except is still a lot about semantics.*/

   
   Edge(const Edge& e) : length_(e.length_), theta_(e.theta_) {}

   //initializing the member variable
   // **--** This esoteric syntax is called member initialization list **--** 
   Edge(double length, double angle) noexcept : length_(length), theta_(angle) {}
   // function specifier explicit controls unwanted implicit type conversions.
   // so as not to allow conversion from anyother type to Vector2
   // if type mismatch, then throw a compile time error.
   explicit Edge(Vector2 v) noexcept : length_(v.length()),  theta_(v.atan_mod()) {}

   double Length() const { return length_; }
   // member functions
   // const tells that it is not changing any member data. Perhaps just reads it as it is and makes sure that it won't change anything. 
   // A way to deal with private data ?? Good safety practice.
   // A function becomes const when the const keyword is used in the function’s declaration. The idea of const functions is not to allow
   // them to modify the object on which they are called. It is recommended the practice to make as many functions const as possible so that accidental changes to objects are avoided.
   // Just provided with member fuctions to access length, theta
   double Theta() const { return theta_; }

private:
	// data member. Initialized to default values zero.
   double length_ = 0.0;
   double theta_ = 0.0;
};

// --------------------------------------------------------------------------------------------------------------------------------

#endif



/*
Concept List:
1. 

*/