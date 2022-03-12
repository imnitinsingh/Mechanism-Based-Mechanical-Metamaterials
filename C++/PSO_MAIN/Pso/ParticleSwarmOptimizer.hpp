#ifndef PARTICLE_SWARM_OPTIMIZER_HPP
#define PARTICLE_SWARM_OPTIMIZER_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <random>
#include "DefsPso.hpp"
#include "RandomNumberGenerator.hpp"
#include "Particle.hpp"
#include "ConjugateGradient.hpp"
#include "Tiling.hpp"
#include <memory>

// --------------------------------------------------------------------------------------------------------------------------------

class ParticleSwarmOptimizer
{
public:
   PSO_EXPORT explicit ParticleSwarmOptimizer() noexcept;
   PSO_EXPORT bool Initialize(const std::string file, std::string& msg);
   PSO_EXPORT bool SaveSettings(std::ostream& file_path) const;
   PSO_EXPORT bool SaveResult(std::ostream& file_path) const;
   PSO_EXPORT void RunGeneration();

   PSO_EXPORT int Generation() const;
   PSO_EXPORT Particle GlobalBestPositions() const;
   PSO_EXPORT double GlobalMean() const;
   PSO_EXPORT double GlobalBestFitness() const;
   PSO_EXPORT int GlobalBestIndex() const;

   double activity = 1.0;
   double weigth = 0.25;
   double cognition = 0.3;
   double social = 3.0;


protected:

   PSO_EXPORT void UpdateFitness(Particle& p) const;
   PSO_EXPORT void UpdatePolygons(Particle& p, Tiling& t);
   static void UpdateLocalBest(Particle& p);
   void UpdateGlobalBest(Particle& p, int index);

   // --------------------------------------------------------------------------------------------------------------------------------

   int generation = 0;
   double mean_best = 0;
   int best_index = 0;

   Tiling tiling;
   std::vector<Particle> particles;

   Particle global_best_positions;
   double global_best_fitness = std::numeric_limits<double>::max();

   // --------------------------------------------------------------------------------------------------------------------------------

   ConjugateGradient equilibrium_finder;
   std::unique_ptr<RandomNumberGenerator> random;

};

// --------------------------------------------------------------------------------------------------------------------------------

#endif
