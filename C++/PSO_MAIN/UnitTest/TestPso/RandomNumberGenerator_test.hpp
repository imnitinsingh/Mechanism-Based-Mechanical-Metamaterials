#include "RandomNumberGenerator.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_ConstructRandomNumber_HasNoError()
{
   auto random = std::make_unique<RandomNumberGenerator>(true);
   auto value = random->NextScalar();
   TINYTEST_ASSERT(value >= 0 && value <= 1.0);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_ConstructRandomVector2_HasNoError()
{
   auto random = std::make_unique<RandomNumberGenerator>(true);
   auto value = random->NextVector2();
   TINYTEST_ASSERT(value.X() >= 0 && value.X() <= 1.0);
   TINYTEST_ASSERT(value.Y() >= 0 && value.Y() <= 1.0);
   TINYTEST_ASSERT(value.X() != value.Y());

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_ConstructTestRandomNumber_HasNoError()
{
   auto random = std::make_unique<RandomNumberGenerator>(false);
   TINYTEST_EQUAL(random->NextScalar(), 0.5);

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_ConstructTestRandomVector2_HasNoError()
{
   auto random = std::make_unique<RandomNumberGenerator>(false);
   TINYTEST_EQUAL(random->NextVector2(), Vector2(0.5, 0.5));

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

TINYTEST_START_SUITE(RandomNumberGenerator);
TINYTEST_ADD_TEST(test_ConstructRandomNumber_HasNoError);
TINYTEST_ADD_TEST(test_ConstructRandomVector2_HasNoError);
TINYTEST_ADD_TEST(test_ConstructTestRandomNumber_HasNoError);
TINYTEST_ADD_TEST(test_ConstructTestRandomVector2_HasNoError);
TINYTEST_END_SUITE();


// --------------------------------------------------------------------------------------------------------------------------------
