#ifndef TILE_HPP
#define TILE_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <vector>
#include "Vector2.hpp"
#include "Edge.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

struct Tile
{
   Tile() noexcept {}
   Tile(const Tile &t) : Edges(t.Edges), Centriod(t.Centriod), Alpha(t.Alpha) {}
   explicit Tile(const std::vector<Edge> edges, Vector2 centriod) noexcept : Edges(edges), Centriod(centriod), Alpha() {}

   // not made private because they will be changing 
   std::vector<Edge> Edges;
   Vector2 Centriod;
   double Alpha = 0.0;
};

// --------------------------------------------------------------------------------------------------------------------------------

#endif