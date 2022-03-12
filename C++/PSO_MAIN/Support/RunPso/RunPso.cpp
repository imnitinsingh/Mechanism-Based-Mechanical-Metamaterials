#include <sstream>
#include <iostream>
#include <cstring>
#include "ParticleSwarmOptimizer.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

std::string loadfile = "default50";
ParticleSwarmOptimizer optimizer;
int generations = 100;

// --------------------------------------------------------------------------------------------------------------------------------

void print_usage()
{
   std::cout << "** Particle Swarm Optimization **" << std::endl << std::endl;
   std::cout << "   Usage ./RunPso [options] > result.out 2> error.out" << std::endl;
   std::cout << "   [options] Sets additional options:" << std::endl;
   std::cout << "     -lf ... : import file with different initial positions and structure." << std::endl;
   std::cout << "     -gn ... : change number of generations." << std::endl;
   std::cout << "     -af ... : change activity factor." << std::endl;
   std::cout << "     -wf ... : change velocity weight factor." << std::endl;
   std::cout << "     -cf ... : change cognition factor." << std::endl;
   std::cout << "     -sf ... : change social factor." << std::endl;
}

// --------------------------------------------------------------------------------------------------------------------------------

bool HandleCommandArguments(int& i, int argc, char** argv)
{
   if (std::strcmp(argv[i], "-lf") == 0 && i + 1 < argc) loadfile = argv[++i];
   else if (std::strcmp(argv[i], "-gn") == 0 && i + 1 < argc) generations = atoi(argv[++i]);
   else if (std::strcmp(argv[i], "-af") == 0 && i + 1 < argc) optimizer.activity = atof(argv[++i]);
   else if (std::strcmp(argv[i], "-wf") == 0 && i + 1 < argc) optimizer.weigth = atof(argv[++i]);
   else if (std::strcmp(argv[i], "-cf") == 0 && i + 1 < argc) optimizer.cognition = atof(argv[++i]);
   else if (std::strcmp(argv[i], "-sf") == 0 && i + 1 < argc) optimizer.social = atof(argv[++i]);
   else return true;

   return false;
}

// --------------------------------------------------------------------------------------------------------------------------------

bool SetArguments(int argc, char** argv)
{
   for (auto i = 1; i < argc; ++i)
   {
      auto arg_err = HandleCommandArguments(i, argc, argv);
      if (arg_err) { print_usage(); return false; }
   }

   return true;
}

// --------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
   optimizer = ParticleSwarmOptimizer();
   if (argc > 1 && !SetArguments(argc, argv)) return 0;

   std::string message;
   if (!optimizer.Initialize(loadfile, message))
   {
      std::cout << message;
      return -1;
   }
   std::cout << std::setprecision(15) << std::scientific;
   std::cout << "import file : " << loadfile << std::endl;
   std::cout << "generations : " << generations << std::endl;
   optimizer.SaveSettings(std::cout);
   optimizer.SaveResult(std::cout);

   while (optimizer.Generation() < generations)
   {
      optimizer.RunGeneration();
      optimizer.SaveResult(std::cout);
   }

   std::cin.get();
   return 0;
}

// --------------------------------------------------------------------------------------------------------------------------------
