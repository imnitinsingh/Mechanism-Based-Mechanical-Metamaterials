#include "ParticleSwarmOptimizer.hpp"
#include "ConjugateGradient.hpp"
#include "Storage.hpp"
#include "DefsPso.hpp"
#include "Vector2.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT ParticleSwarmOptimizer::ParticleSwarmOptimizer() noexcept
{
   equilibrium_finder = ConjugateGradient();
   random = std::make_unique<RandomNumberGenerator>(true);
}

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT bool ParticleSwarmOptimizer::Initialize(const std::string file, std::string& msg)
{
   if (!Storage::LoadData(file, tiling, particles))
   {
      std::stringstream stream;
      stream << "Could not load file: " << file << std::endl;
      msg = stream.str();
      return false;
   }

   auto particle_count = particles.size();
   for (size_t p_index = 0; p_index < particle_count; p_index++)
   {
      auto count = particles[p_index].positions.size();
      particles[p_index].velocities = std::vector<Vector2>(count);
      for (size_t v_index = 0; v_index < count; v_index++)
         particles[p_index].velocities[v_index] = activity *random->NextVector2();
      particles[p_index].best_fitness = std::numeric_limits<double>::max();
   }

   equilibrium_finder.Initialize(tiling);
   for (size_t index = 0; index < particle_count; index++)
   {
      UpdateFitness(particles[index]);
      UpdateLocalBest(particles[index]);
      UpdateGlobalBest(particles[index], index);
   }

   mean_best = 0;
   for (auto p : particles) mean_best += p.fitness;
   mean_best /= particles.size();

   return true;
}

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT void ParticleSwarmOptimizer::RunGeneration()
{
   auto particle_count = particles.size();
   for (size_t index = 0; index < particle_count; index++)
   {
      UpdatePolygons(particles[index], tiling);
      UpdateFitness(particles[index]);
      UpdateLocalBest(particles[index]);
      UpdateGlobalBest(particles[index], index);
   }
   generation++;

   mean_best = 0;
   for (auto p : particles) mean_best += p.fitness;
   mean_best /= particles.size();
}

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT int ParticleSwarmOptimizer::Generation() const { return generation; }

PSO_EXPORT double ParticleSwarmOptimizer::GlobalMean() const { return mean_best; }

PSO_EXPORT int ParticleSwarmOptimizer::GlobalBestIndex() const { return best_index; }

PSO_EXPORT double ParticleSwarmOptimizer::GlobalBestFitness() const { return global_best_fitness; }

PSO_EXPORT Particle ParticleSwarmOptimizer::GlobalBestPositions() const { return global_best_positions; }

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT bool ParticleSwarmOptimizer::SaveSettings(std::ostream& stream) const
{
   try
   {
      stream << std::setprecision(15) << std::scientific;
      stream << "activity : " << activity << std::endl;
      stream << "weight : " << weigth << std::endl;
      stream << "cognition : " << cognition << std::endl;
      stream << "social : " << social << "\n\n" << std::endl;
      return true;
   }
   catch (std::exception ex)
   {
      return false;
   }
}

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT bool ParticleSwarmOptimizer::SaveResult(std::ostream& stream) const
{
   stream << std::setprecision(15) << std::scientific;

   try
   {
      auto best_p = global_best_positions;
      stream << "generation :" << generation << std::endl;
      stream << "mean :" << mean_best  << "\n" << std::endl;

      stream << "particle : global best" << std::endl;
      stream << "index : " << best_index << std::endl;
      stream << "fitness : " << global_best_fitness << std::endl;

      stream << "particle position" << std::endl;
      for (auto b_pos : global_best_positions.positions) stream << b_pos.X() << "\t\t" << b_pos.Y() << std::endl;
      stream << std::endl;

      stream << "particle velocity" << std::endl;
      for (auto b_vel : global_best_positions.velocities) stream << b_vel.X() << "\t\t" << b_vel.Y() << std::endl;
      stream << std::endl;

      auto count = particles.size();
      for (size_t index = 0; index < count; index++)
      {
         auto p = particles[index];
         stream << "particle : " << index << std::endl;
         stream << "fitness : " << p.fitness << std::endl;

         stream << "particle position" << std::endl;
         for (auto pos : p.positions) stream << pos.X() << "\t\t" << pos.Y() << std::endl;
         stream << std::endl;

         stream << "particle velocity" << std::endl;
         for (auto vel : p.velocities) stream << vel.X() << "\t\t" << vel.Y() << std::endl;
         stream << std::endl;
      }

      return true;
   }
   catch (std::exception ex)
   {
      return false;
   }
}


//------------------------------------------------------------------------------------------------------------------------------------
static double calcPenal4(Particle& p)
{
	double penal4 = 0.0;
	double maxPenal = 0.25;
	double minClearance = 2;
	double maxClearance = 10;

	int Connected_Points[20][2] = { { 0,1 },{ 1,2 },{ 2,3 },{ 3,4 },{ 4,5 },{ 5,6 },{ 6,7 },{ 7,0 },{ 8,9 },{ 9,10 },{ 10,11 },{ 11,8 },{ 0,8 },{ 7,8 },{ 1,9 },{ 2,9 },{ 3,10 },{ 4,10 },{ 5,11 },{ 6,11 } };

	for (int sideLengthCount = 0; sideLengthCount < 20; sideLengthCount++)
	{
		int i = sideLengthCount;
		double sideLength = (p.positions[Connected_Points[i][0]] - p.positions[Connected_Points[i][1]]).length();

		if (sideLength < minClearance)
		{
			penal4 += maxPenal*(1 - sideLength / minClearance);
		}
		if (sideLength >= minClearance && sideLength <= maxClearance)
		{
			penal4 += 0.0;
		}
		if (sideLength > maxClearance && sideLength < minClearance + maxClearance)
		{
			penal4 += (maxPenal / minClearance)*(sideLength - maxClearance);
		}
		if (sideLength >= minClearance + maxClearance)
		{
			penal4 += maxPenal;
		}
	}

	/*
	//auto positions = std::vector<Tile>(p.tiles);
	std::cout << "*************" << std::endl;
	std::cout <<  << std::endl;
	std::cout << "*************" << std::endl;
	*/
	return penal4;
}
//------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT void ParticleSwarmOptimizer::UpdateFitness(Particle& p) const
{
   const auto gc_max = 5000;
   const auto n = 24; // what is n?

   const double steps = 20;
   const auto target = 4;
   //auto target = (18 / (PI*PI))*(x*x) + 3
   const double PI = 4 * atan(1.0);
   const auto max_angle = 1.04;

   auto fitness = 0.0;
   auto positions = std::vector<Tile>(p.tiles);

   //call a function calcPenal4 (takes care of the distance among neighboring points)
   double penal4 = calcPenal4(p);
   
   for (auto step = 0; step < steps; step++)
   {
      if (step == steps / 2)  positions = std::vector<Tile>(p.tiles);
      positions[1].Alpha = 2 * max_angle * (step % int(steps / 2) / (steps - 2)) * (step > steps / 2 ? -1.0 : 1.0);

      auto pair = equilibrium_finder.RunAlgorithm(positions, gc_max, n);
	  //auto target = (18 / (PI*PI))*(positions[1].Alpha*positions[1].Alpha) + 3;
	  //fitness += (std::abs(pair.first - target));// +pair.second + static_cast<double>(penal4) / 1;
	  fitness += ((std::abs(pair.first - target))*(std::abs(pair.first - target)) / 20) + pair.second + static_cast<double>(penal4) / 1;
	  
   }
   //std::cout << "fitness" << std::endl;
   p.fitness = fitness;
}


// --------------------------------------------------------------------------------------------------------------------------------

PSO_EXPORT void ParticleSwarmOptimizer::UpdatePolygons(Particle& p, Tiling& t)
{
   auto pos_count = p.positions.size();
   for (size_t tiles = 0; tiles < pos_count; tiles++)
   {
      auto r1 = random->NextVector2();
      auto r2 = random->NextVector2();

      auto position = p.positions[tiles];
      auto local_best = p.best_positions[tiles];
      auto global_best = global_best_positions.positions[tiles];
	 
	  //if (tiles == 11)

	  //{
	  p.velocities[tiles] *= weigth;
      p.velocities[tiles] += r1 * cognition * (local_best - position);
      p.velocities[tiles] += r2 * social * (global_best - position);
	  //std::cout << tiles << std::endl;
	  
	  p.positions[tiles] += p.velocities[tiles];
	  //}

      p.tiles.clear();
      Storage::SetTiles(p, t.structure);
   }
}

// --------------------------------------------------------------------------------------------------------------------------------

void ParticleSwarmOptimizer::UpdateLocalBest(Particle& p)
{
   if (p.best_fitness < p.fitness) return;
   p.best_fitness = p.fitness;
   p.best_positions = p.positions;
}

// --------------------------------------------------------------------------------------------------------------------------------

void ParticleSwarmOptimizer::UpdateGlobalBest(Particle& p, int index)
{
   if (global_best_fitness < p.fitness) return;
   global_best_fitness = p.fitness;
   global_best_positions = p;
   best_index = index;
}

// --------------------------------------------------------------------------------------------------------------------------------
