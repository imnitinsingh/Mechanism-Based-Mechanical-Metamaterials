// --------------------------------------------------------------------------------------------------------------------------------

#include "../common/tinytest.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

#include "ConjugateGradient_test.hpp"
#include "ParticleSwarmOptimizer_test.hpp"
#include "RandomNumberGenerator_test.hpp"
#include "Storage_test.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

TINYTEST_START_MAIN();

/* Algorithms */
TINYTEST_RUN_SUITE(ParticleSwarmOptimizer);
TINYTEST_RUN_SUITE(ConjugateGradient);

/* Utils */
TINYTEST_RUN_SUITE(RandomNumberGenerator);
TINYTEST_RUN_SUITE(Storage);


TINYTEST_END_MAIN();

// --------------------------------------------------------------------------------------------------------------------------------
