#ifndef STORAGE_HPP
#define STORAGE_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "Vector2.hpp"
#include "Particle.hpp"
#include "Tiling.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

class Storage
{
   using VecVec2Type = std::vector<Vector2>;
   using VecParType = std::vector<Particle>;
   using VecConType = std::vector<Connection>;
   using VecTileType = std::vector<Tile>;
   using VecPairType = std::vector<std::pair<size_t, size_t>>;
   using VecIntType = std::vector<std::vector<int>>;

   // --------------------------------------------------------------------------------------------------------------------------------

public:
   static bool LoadData(const std::string& file, Tiling& tiling, VecParType& particles)
   {
      std::ifstream infile(file);

      try
      {
         if (!ReadStructure(infile, tiling.structure))
            return false;

         auto p_count = 0, v_count = 0;
         if (!ReadCount(infile, p_count, v_count))
            return false;

         auto count = 0;
         SetConnections(tiling, v_count);
         while (count < p_count)
         {
            count++;
            auto particle = Particle();
            particle.positions = VecVec2Type(v_count);
            if (!ReadPositions(infile, particle.positions))
               return false;

            SetTiles(particle, tiling.structure);
            particles.push_back(particle);
         }
      }
      catch (std::exception oor)
      {
         infile.close();
         return false;
      }

      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static bool ReadCount(std::ifstream& infile, int& particles, int& vertices)
   {
      std::string line;
      std::getline(infile, line);
      if (line.find("particle_count") != 0)
         return false;

      auto pos = line.find_last_of("=");
      particles = std::stoi(line.substr(++pos));

      std::getline(infile, line);
      if (line.find("vertices_count") != 0)
         return false;

      pos = line.find_last_of("=");
      vertices = std::stoi(line.substr(++pos));

      std::getline(infile, line);
      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static bool SetTiles(Particle& particle, const VecIntType& structure)
   {
      auto positions = particle.positions;
      for (auto ids : structure)
      {
         auto edges = ids.size();

         Tile tile;
         switch (edges)
         {
         case 2: tile = CreateLine(positions, ids); break;
         case 3: tile = CreateTriangle(positions, ids); break;
         case 4: tile = CreateQuadrilateral(positions, ids); break;
         default: return false;
         }

         particle.tiles.push_back(tile);
      }

      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static bool LoadResult(Particle& particle, const std::string& file)
   {
      std::ifstream infile(file);

      try
      {
         VecTileType vertices;
         if (!ReadTiles(infile, vertices)) return false;

         particle.tiles = vertices;
         return true;
      }
      catch (std::exception ex)
      {
         infile.close();
         return false;
      }
   }

   // --------------------------------------------------------------------------------------------------------------------------------

private:
   static bool ReadTiles(std::ifstream& infile, VecTileType& tiles)
   {
      std::string line;
      std::getline(infile, line);
      if (line.find("position_tiles") != 0) return false;

      auto pos = line.find_last_of("=");
      auto count = std::stoi(line.substr(++pos));
      tiles = VecTileType(count);

      double x, y, a;
      for (auto idx = 0; idx < count; idx++)
      {
         std::getline(infile, line);
         std::istringstream iss(line);
         iss >> x >> y >> a;
         tiles[idx].Centriod = Vector2(x, y);
         tiles[idx].Alpha = a;

         double l, t;
         std::getline(infile, line);
         while (!line.empty())
         {
            std::istringstream iss_e(line);
            iss_e >> l >> t;
            tiles[idx].Edges.push_back(Edge(l, t));
            std::getline(infile, line);
         }
      }
      std::getline(infile, line);
      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static bool ReadPositions(std::ifstream& infile, VecVec2Type& positions)
   {
      std::string line;
      std::getline(infile, line);
      if (line.find("particle=") != 0) return false;

      double x, y;
      auto lines = positions.size();
      for (size_t idx = 0; idx < lines; idx++)
      {
         std::getline(infile, line);
         std::istringstream iss(line);
         iss >> x >> y; positions[idx] = Vector2(x, y);
      }
      std::getline(infile, line);
      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static bool ReadStructure(std::ifstream& infile, VecIntType& structure)
   {
      std::string line;
      std::getline(infile, line);
      if (line.find("connected_vertices") != 0) return false;

      auto pos = line.find_last_of("=");
      auto count = std::stoi(line.substr(++pos));
      structure = VecIntType(count);

      int vertex;
      for (auto idx = 0; idx < count; idx++)
      {
         std::getline(infile, line);
         std::istringstream iss(line);
         while (iss >> vertex) structure[idx].push_back(vertex);
      }
      std::getline(infile, line);
      return true;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static void SetConnections(Tiling& tiling, const int& vec_count)
   {
      auto no_connect = 0;
      tiling.connections = std::vector<Connection>();
      auto str_count = tiling.structure.size();

      for (auto vertex = 0; vertex < vec_count; vertex++)
      {
         VecPairType pair;
         for (size_t tile_nr = 0; tile_nr < str_count; tile_nr++)
         {
            auto ids = tiling.structure[tile_nr];
            auto itr = std::find(ids.begin(), ids.end(), vertex);
            if (itr == ids.end()) continue;

            size_t index = itr - ids.begin();
            pair.push_back(std::make_pair(tile_nr, index));
         }

         if (pair.size() != 2) { if (no_connect >= 2) break; no_connect++; continue; }
         tiling.connections.push_back(Connection(pair[0].first, pair[1].first, pair[0].second, pair[1].second));
      }
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static Tile CreateLine(const VecVec2Type& pos, const std::vector<int>& ids)
   {
      ASSERT(ids.size() == 2);
      auto p0 = pos[ids[0]];
      auto p1 = pos[ids[1]];

      auto v0 = (p0 - p1) / 2;
      auto v1 = (p1 - p0) / 2;

      return Tile({ Edge(v0), Edge(v1) }, (p0 + p1) / 2);
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static Tile CreateTriangle(const VecVec2Type& pos, const std::vector<int>& ids)
   {
      ASSERT(ids.size() == 3);
      auto p0 = pos[ids[0]];
      auto p1 = pos[ids[1]];
      auto p2 = pos[ids[2]];

      auto v0 = (2.0 * p0 - p1 - p2) / 3.0;
      auto v1 = (2.0 * p1 - p0 - p2) / 3.0;
      auto v2 = (2.0 * p2 - p0 - p1) / 3.0;

      return Tile({ Edge(v0), Edge(v1), Edge(v2) }, (p0 + p1 + p2) / 3);
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static Tile CreateQuadrilateral(const VecVec2Type& pos, const std::vector<int>& ids)
   {
      ASSERT(ids.size() == 4);
      auto p0 = pos[ids[0]];
      auto p1 = pos[ids[1]];
      auto p2 = pos[ids[2]];
      auto p3 = pos[ids[3]];

      auto v0 = (3.0 * p0 - p1 - p2 - p3) / 4.0;
      auto v1 = (3.0 * p1 - p0 - p2 - p3) / 4.0;
      auto v2 = (3.0 * p2 - p0 - p1 - p3) / 4.0;
      auto v3 = (3.0 * p3 - p0 - p1 - p2) / 4.0;

      return Tile({ Edge(v0), Edge(v1), Edge(v2), Edge(v3) }, (p0 + p1 + p2 + p3) / 4);
   }

   // --------------------------------------------------------------------------------------------------------------------------------

};

#endif
