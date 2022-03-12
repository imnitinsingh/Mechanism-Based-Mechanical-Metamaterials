#ifndef CONJUGATE_GRADIENT_TEST_HPP
#define CONJUGATE_GRADIENT_TEST_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include "ConjugateGradient.hpp"
#include "Storage.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_energy()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);

   auto p0 = particles[0];
   auto expected_energy_p0 = 1.84180532441640139099e-29;
   auto actual_energy_p0 = finder.Energy(p0.tiles);
   TINYTEST_ALMOST_EQUALS(expected_energy_p0, actual_energy_p0);

   auto p1 = particles[1];
   auto expected_energy_p1 = 1.84180532441640139099e-29;
   auto actual_energy_p1 = finder.Energy(p1.tiles);
   TINYTEST_ALMOST_EQUALS(expected_energy_p1, actual_energy_p1);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_Df()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);
   auto actual_df_p0 = finder.Df(particles[0].tiles);
   auto actual_df_p1 = finder.Df(particles[1].tiles);

   auto expected_centroid_p0 = std::vector<Vector2>
   {
      Vector2(-2.2204460492503130808e-15, 0), Vector2(3.5527136788005009294e-15, 7.6605388699135801289e-15), Vector2(0, -8.5487172896137053613e-15),
      Vector2(0, -1.3322676295501878485e-15), Vector2(0, 0), Vector2(8.8817841970012523234e-16, 3.1086244689504383132e-15),
      Vector2(-2.2204460492503130808e-15, 3.1086244689504383132e-15), Vector2(4.4408920985006261617e-16, -1.3322676295501878485e-15),
   };

   auto expected_alpha_p0 = std::vector<double>
   {
      4.1643784514771021277e-15, 0, 4.7859541307857161533e-15, 3.3790975842356388115e-16,
      0, 3.6095082347441464516e-15, -3.6102528414275054615e-15, 1.9133292749127770734e-14
   };

   for (size_t a = 0; a < actual_df_p0.size(); a++)
   {
      TINYTEST_ALMOST_EQUAL(expected_centroid_p0[a].X(), actual_df_p0[a].Centriod.X(), 1e-14); //, stream.str().c_str());
      TINYTEST_ALMOST_EQUAL(expected_centroid_p0[a].Y(), actual_df_p0[a].Centriod.Y(), 1e-14); //, stream.str().c_str());
      TINYTEST_ALMOST_EQUAL(expected_alpha_p0[a], actual_df_p0[a].Alpha, 1e-14);
   }

   auto expected_centroid_p1 = std::vector<Vector2>
   {
      Vector2(0,-2.2377932840100811518e-15),Vector2(-2.2204460492503130808e-16,-8.7083118494035716139e-16),Vector2(8.8817841970012523234e-16,3.1086244689504383132e-15),
      Vector2(3.5527136788005009294e-15,3.9968028886505635455e-15),Vector2(0,0),Vector2(4.4408920985006261617e-16,-3.1086244689504383132e-15),
      Vector2(-8.8817841970012523234e-16,2.7755575615628913511e-15),Vector2(-2.2204460492503130808e-15,-2.7755575615628913511e-15)
   };

   auto expected_alpha_p1 = std::vector<double>
   {
      1.7447108078246367308e-15, 0, 6.5991960477236901364e-16, 9.0572866466980592992e-15,
      0, 3.2410814415045758054e-15, 1.7462553331773452325e-14, 6.6055916133185183516e-15
   };

   for (size_t a = 0; a < actual_df_p0.size(); a++)
   {
      TINYTEST_ALMOST_EQUAL(expected_centroid_p1[a].X(), actual_df_p1[a].Centriod.X(), 1e-14);
      TINYTEST_ALMOST_EQUAL(expected_centroid_p1[a].Y(), actual_df_p1[a].Centriod.Y(), 1e-14);
      TINYTEST_ALMOST_EQUAL(expected_alpha_p1[a], actual_df_p1[a].Alpha, 1e-14);
   }

   return 1;

}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_dDDfd()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);

   auto actual_ddf = finder.dDDfd(particles[0].tiles, particles[1].tiles);
   auto expected_ddf = 4.61284010942977374725e+02;
   TINYTEST_EQUAL(expected_ddf, actual_ddf);
   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_dotProduct()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);

   auto actual_product = finder.dotProduct(particles[0].tiles, particles[1].tiles);
   auto expected_product = 2.42681068086889411006e+02;
   TINYTEST_EQUAL(expected_product, actual_product);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_runAlgorithm()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);

   auto n = 24;
   auto gc_max = 5000;

   auto actual_run_p0A = finder.RunAlgorithm(particles[0].tiles, gc_max, n);
   auto expected_run_p0A = std::make_pair(4.12731224364306026331, 0.5);
   TINYTEST_EQUAL(expected_run_p0A, actual_run_p0A);

   auto actual_run_p0B = finder.RunAlgorithm(particles[0].tiles, gc_max, n);
   auto expected_run_p0B = std::make_pair(4.12731224364306203967, 0.5);
   TINYTEST_EQUAL(expected_run_p0B, actual_run_p0B);

   auto actual_run_p1A = finder.RunAlgorithm(particles[1].tiles, gc_max, n);
   auto expected_run_p1A = std::make_pair(3.87136622685648124076, 0.5);
   TINYTEST_EQUAL(expected_run_p1A, actual_run_p1A);

   auto actual_run_p1B = finder.RunAlgorithm(particles[1].tiles, gc_max, n);
   auto expected_run_p1B = std::make_pair(3.87136622685648124076, 0.5);
   TINYTEST_EQUAL(expected_run_p1B, actual_run_p1B);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_CalculatePenalty()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto finder = ConjugateGradient();
   finder.Initialize(tiling);

   auto actual_penalty_p0A = finder.CalculatePenalty(particles[0].tiles, 5, -11);
   auto expected_penalty_p0A = std::make_pair(5.0, 0.5);
   TINYTEST_ALMOST_EQUALS(expected_penalty_p0A.first, actual_penalty_p0A.first);
   TINYTEST_ALMOST_EQUALS(expected_penalty_p0A.second, actual_penalty_p0A.second);

   auto actual_penalty_p0B = finder.CalculatePenalty(particles[0].tiles, 5, -5);
   auto expected_penalty_p0B = std::make_pair(5.0, 0.5);
   TINYTEST_ALMOST_EQUALS(expected_penalty_p0B.first, actual_penalty_p0B.first);
   TINYTEST_ALMOST_EQUALS(expected_penalty_p0B.second, actual_penalty_p0B.second);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

TINYTEST_START_SUITE(ConjugateGradient);
TINYTEST_ADD_TEST(test_energy);
TINYTEST_ADD_TEST(test_Df);
TINYTEST_ADD_TEST(test_dDDfd);
TINYTEST_ADD_TEST(test_dotProduct);
TINYTEST_ADD_TEST(test_runAlgorithm);
TINYTEST_ADD_TEST(test_CalculatePenalty);
TINYTEST_END_SUITE();

// --------------------------------------------------------------------------------------------------------------------------------

#endif