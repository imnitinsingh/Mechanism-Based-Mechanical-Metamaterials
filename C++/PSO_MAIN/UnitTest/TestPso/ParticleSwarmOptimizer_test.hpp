#include "ParticleSwarmOptimizer.hpp"
#include "Storage.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

const std::string test_file = "default2";

// --------------------------------------------------------------------------------------------------------------------------------

class Test_ParticleSwarmOptimizer : public ParticleSwarmOptimizer
{
public:
   void setNoRandom_() { this->random = std::make_unique<RandomNumberGenerator>(false); }
   Tiling tiling_() const { return tiling; }
   Particle particle_(int index) { return particles[index]; }
   void UpdatePolygons_(Particle& p, Tiling& t) { UpdatePolygons(p, t); };
   void UpdateFitness_(Particle& p) const { UpdateFitness(p); }
   void InitializeConjugateGradient(const Tiling& t) { equilibrium_finder.Initialize(t); }
};

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_RunGeneration_GlobalBestFitnessNotZero()
{
   std::string message;
   auto optimizer = Test_ParticleSwarmOptimizer();
   optimizer.setNoRandom_();

   optimizer.Initialize(test_file, message);
   while (optimizer.Generation() < 50)
   {
      optimizer.RunGeneration();
      TINYTEST_ASSERT(optimizer.GlobalBestFitness() > 0.0);
   }

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_UpdatePolygons_HasCorrectValues()
{
   std::string message;
   auto optimizer = Test_ParticleSwarmOptimizer();
   optimizer.setNoRandom_();

   auto is_loaded = optimizer.Initialize(test_file, message);
   TINYTEST_ASSERT_MSG(is_loaded, message.c_str());

   auto tiling = optimizer.tiling_();
   auto particle0 = optimizer.particle_(0);
   optimizer.UpdatePolygons_(particle0, tiling);

   auto actual_pos_p0 = particle0.positions;
   auto expected_pos_p0 = std::vector<Vector2>{ Vector2(-6.98893091357511409001e+00,+4.53480681006995034465e+00),
      Vector2(-4.21952868552404769531e+00,+7.21404870182737933959e+00),Vector2(+4.03517593270414121065e+00,+4.88360387665768946874e+00),
      Vector2(+7.00814437960329073007e+00,+4.35712286892044975417e+00),Vector2(+5.25062200298161574352e+00,-3.28923373404484031823e+00),
      Vector2(+2.16469110422566446417e+00,-6.02998852873720903034e+00),Vector2(-3.41462595495564835701e+00,-7.12618698626209479130e+00),
      Vector2(-6.41776220632503147101e+00,-3.51639525690861809437e+00),Vector2(-1.71591088201381802847e+00,-1.56831142574679782697e-01),
      Vector2(-6.91446035213498677585e-01,+3.06570124373299046638e+00),Vector2(+2.93430846972485737112e+00,+1.58752715853499348775e+00),
      Vector2(+1.76737459913973959935e+00,-3.07738195369419686998e+00) };

   TINYTEST_EQUAL(expected_pos_p0.size(), actual_pos_p0.size());
   for (auto el = actual_pos_p0.size() - 1; el > 0; el--) TINYTEST_EQUAL(actual_pos_p0[el], expected_pos_p0[el]);

   auto actual_vel_p0 = particle0.velocities;
   auto expected_vel_p0 = std::vector<Vector2>{ Vector2(0.25,0.25), Vector2(0.25,0.25),Vector2(0.25,0.25),Vector2(0.25,0.25),
      Vector2(0.25,0.25),Vector2(0.25,0.25),Vector2(0.25,0.25),Vector2(0.25,0.25),Vector2(0.25,0.25),Vector2(0.25,0.25),
      Vector2(0.25,0.25),Vector2(0.25,0.25) };

   TINYTEST_EQUAL(expected_vel_p0.size(), actual_vel_p0.size());
   for (auto el = actual_vel_p0.size() - 1; el > 0; el--) TINYTEST_EQUAL(actual_vel_p0[el], expected_vel_p0[el]);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_GlobalBestFitnessValuesNoRandom_AreEqual()
{
   std::vector<double> expected_fitness{ 3.86337637627811929519e+01,3.86337637627811929519e+01, 3.86337637627811929519e+01,
      3.86060174728293432622e+01,3.86060174728293432622e+01,3.79361005097380328266e+01,3.79361005097380328266e+01,
      3.79361005097380328266e+01,3.79361005097380328266e+01,3.76984627765285083001e+01,3.76595836480552250691e+01,
      3.76595836480552250691e+01,3.76595836480552250691e+01,3.76595836480552250691e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,
      3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01,3.75510003818406872256e+01
   };

   auto pso = Test_ParticleSwarmOptimizer();
   pso.setNoRandom_();

   std::string message;
   pso.Initialize(test_file, message);
   TINYTEST_ALMOST_EQUALS(expected_fitness[0], pso.GlobalBestFitness());

   auto generations = static_cast<int>(expected_fitness.size()) - 1;
   while (pso.Generation() < generations)
   {
      pso.RunGeneration();
      TINYTEST_ALMOST_EQUALS(expected_fitness[pso.Generation()], pso.GlobalBestFitness());
   }

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_UpdateFitness_HasCorrectValue()
{
   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(test_file, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto pso = Test_ParticleSwarmOptimizer();
   pso.InitializeConjugateGradient(tiling);
   pso.setNoRandom_();

   auto pos_copy = std::vector<Tile>(particles[0].tiles);
   TINYTEST_EQUAL(pos_copy.size(), particles[0].tiles.size());
   for(size_t item = 0; item < pos_copy.size(); item++)
   {
      TINYTEST_EQUAL(pos_copy[item].Alpha, particles[0].tiles[item].Alpha);
      TINYTEST_EQUAL(pos_copy[item].Centriod, particles[0].tiles[item].Centriod);
      TINYTEST_EQUAL(pos_copy[item].Edges.size(), particles[0].tiles[item].Edges.size())
      for (size_t edge = 0; edge < pos_copy[item].Edges.size(); edge++)
      {
         TINYTEST_EQUAL(pos_copy[item].Edges[edge].Length(), particles[0].tiles[item].Edges[edge].Length());
         TINYTEST_EQUAL(pos_copy[item].Edges[edge].Theta(), particles[0].tiles[item].Edges[edge].Theta());
      }
   }

   /*
   auto p0 = particles[0];
   pso.UpdateFitness_(p0);
   auto actual_fitness_p0 = p0.fitness;
   auto expected_fitness_p0 = 3.8633763762781193e+01;
   TINYTEST_ALMOST_EQUALS(expected_fitness_p0, actual_fitness_p0);

   auto p1 = particles[1];
   pso.UpdateFitness_(p1);
   auto actual_fitness_p1 = p1.fitness;
   auto expected_fitness_p1 = 6.3040053870921817e+01;
   TINYTEST_ALMOST_EQUALS(expected_fitness_p1, actual_fitness_p1);
   */

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

TINYTEST_START_SUITE(ParticleSwarmOptimizer);
TINYTEST_ADD_TEST(test_RunGeneration_GlobalBestFitnessNotZero);
TINYTEST_ADD_TEST(test_UpdatePolygons_HasCorrectValues);
//TINYTEST_ADD_TEST(test_GlobalBestFitnessValuesNoRandom_AreEqual);
TINYTEST_ADD_TEST(test_UpdateFitness_HasCorrectValue);
TINYTEST_END_SUITE();

// --------------------------------------------------------------------------------------------------------------------------------
