#ifndef PARTICLE_HPP
#define PARTICLE_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <vector>

#include "Vector2.hpp"
#include "Tile.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

class Particle
{
public:

   std::vector<Tile> tiles;

   double fitness;
   std::vector<Vector2> positions;
   std::vector<Vector2> velocities;

   double best_fitness;
   std::vector<Vector2> best_positions;
};


// --------------------------------------------------------------------------------------------------------------------------------

#endif