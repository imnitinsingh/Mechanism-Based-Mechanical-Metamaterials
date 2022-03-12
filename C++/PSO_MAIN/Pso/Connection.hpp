#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// --------------------------------------------------------------------------------------------------------------------------------
// A code is always made by stitching together little segments.
// You have to appreciate the importance of these little segments 
// and how they are stiched together that make the wholel code running possible. 
// Every little segment is like a little piece having its own function.
// Without which, the code would not function.


// He has decided Connection to be made as a Struct (similar to  a class). 
struct Connection
{
   using PairIntType = std::pair<int, int>;
   /*
   This is an example of type alias declaraiton in C++. Type alias is a name that refers to a previously 
   defined type. 
   Syntax for alias declaration:
   using identifier = type-id
   identifier is the name that is introduced by this declaration, which becomes the type name
   A type alias declaration introduces a name which can be used as a synonym for the type denoted by type-id. 
   It does not introduce a new type and it cannot change the meaning of an existing type name

   std::pair is a struct template that provides a way to store two heterogeneous objects as a single unit.
   A pair is a specific case of a std::tuple with two elements.
   */
   
   // The name of the constructor must be the same as the name of the structure type.
   Connection() noexcept {}
   /*Specifies whether a function could throw exceptions.*/
   
   // overloaded constructor 
   // making a copy.
   Connection(PairIntType tile, PairIntType edge) noexcept : egde_(edge), tile_(tile) {}

   //Third overloaded cnstructur. The explicit function specifier controls unwanted implicit type conversions.
   // std::make_pair defined in STL
   explicit Connection(int from_tile, int to_tile, int from_edge, int to_edge) noexcept
   {
       //The difference is that with std::pair you need to specify the types of both elements, 
	   // whereas std::make_pair will create a pair with the type of the elements that are passed to it,
	   //without you needing to tell it.
      tile_ = std::make_pair(from_tile, to_tile);
      egde_ = std::make_pair(from_edge, to_edge);
   }
	 
   // Member functions
   // WHat is const doing here ?
   PairIntType EdgeIndices() const { return egde_; }
   PairIntType TileIndices() const { return tile_; }

private:
	// private so that the values can't be modified from outside
   PairIntType egde_;
   PairIntType tile_;
};

// --------------------------------------------------------------------------------------------------------------------------------

#endif