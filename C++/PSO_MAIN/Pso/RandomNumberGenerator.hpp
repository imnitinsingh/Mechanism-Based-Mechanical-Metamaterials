#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <random>
#include "Vector2.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

/* Represents a random number generator. */
class RandomNumberGenerator
{
public:

   /* Creates a random number generator which creates uniformly distributed values between [0, 1). */
   explicit RandomNumberGenerator(bool random = true) noexcept : is_random(random)
   {
      std::random_device random_device;
      engine = std::mt19937(random_device());
      distribution = std::uniform_real_distribution<double>(0.0, 1.0);
   }

   /* Default constructor defined as deleted. */
   RandomNumberGenerator() = delete;

   // --------------------------------------------------------------------------------------------------------------------------------

   /* Returns a random double with value [0, 1). */
   double NextScalar() { return is_random ? distribution(engine) : 0.5; }

   /* Returns a random double vector with values [0, 1). */
   Vector2 NextVector2() { return is_random ? Vector2(distribution(engine), distribution(engine)) : Vector2(0.5, 0.5); }

   // --------------------------------------------------------------------------------------------------------------------------------

private:

   /* Produces random values uniformly distributed according to the probability function. */
   std::uniform_real_distribution<double> distribution;

   /* A random number engine based on Mersenne Twister algorithm. */
   std::mt19937 engine;

   /* A flag for testing purposes. False sets all random values to a half. */
   bool is_random;

};

// --------------------------------------------------------------------------------------------------------------------------------

#endif