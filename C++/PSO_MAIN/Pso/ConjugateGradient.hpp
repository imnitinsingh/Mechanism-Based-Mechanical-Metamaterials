#ifndef CONJUGATE_GRADIENT_HPP
#define CONJUGATE_GRADIENT_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <vector>

#include "DefsPso.hpp"
#include "Vector2.hpp"
#include "Connection.hpp"
#include "Tiling.hpp"
#include "Tile.hpp"
#include <iostream>


// --------------------------------------------------------------------------------------------------------------------------------


class ConjugateGradient
{
   using ConnectionType = std::vector<std::pair<int, int>>;
   using PositionType = std::vector<Tile>;
   using PosPosType = std::vector<std::vector<Tile>>;
   using Connections = std::vector<Connection>;

public:
   explicit ConjugateGradient() noexcept : tile_count_(0), connection_count_(0) {}


   // --------------------------------------------------------------------------------------------------------------------------------

   void Initialize(const Tiling& tiling)
   {
      tile_count_ = tiling.structure.size();
      connection_count_ = tiling.connections.size();
      connections_ = tiling.connections;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   // have not accepted, understood the Dutch, their ways of thinking.
   // but why. got not time to accept and look at the world outside work ??
   // All it takes is some thinking and some acceptance.
   PositionType Df(const PositionType& x) const
   {
      auto derivative = PositionType(tile_count_);
      for (size_t connection = 0; connection < connection_count_; connection++)
      {
         auto pair = connections_[connection];
         auto tile0 = x[pair.TileIndices().first];
         auto tile1 = x[pair.TileIndices().second];

         auto edge0 = tile0.Edges[pair.EdgeIndices().first];
         auto edge1 = tile1.Edges[pair.EdgeIndices().second];

         auto term1 = tile0.Centriod.X() + edge0.Length() * cos(edge0.Theta() + tile0.Alpha)
            - tile1.Centriod.X() - edge1.Length() * cos(edge1.Theta() + tile1.Alpha);
         auto term2 = tile0.Centriod.Y() + edge0.Length() * sin(edge0.Theta() + tile0.Alpha)
            - tile1.Centriod.Y() - edge1.Length() * sin(edge1.Theta() + tile1.Alpha);

         derivative[pair.TileIndices().first].Centriod -= Vector2(2.0 * term1, 2.0 * term2);
         derivative[pair.TileIndices().first].Alpha -= 2.0 * edge0.Length() * (term2 * cos(edge0.Theta() + tile0.Alpha) - term1 * sin(edge0.Theta() + tile0.Alpha));

         derivative[pair.TileIndices().second].Centriod += Vector2(2.0 * term1, 2.0 * term2);
         derivative[pair.TileIndices().second].Alpha += 2.0 * edge1.Length() * (term2 *cos(edge1.Theta() + tile1.Alpha) - term1 * sin(edge1.Theta() + tile1.Alpha));
      }

      derivative[1].Alpha = 0;
      derivative[4].Centriod = Vector2(0, 0);
      derivative[4].Alpha = 0;
      return derivative;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   double dDDfd(const PositionType& pos, const PositionType& d) const
   {
      auto dd = std::vector<std::vector<double>>(24);
      for (auto idx = 0; idx < 24; idx++) dd[idx] = std::vector<double>(24);

      for (size_t index = 0; index < connection_count_; index++)
      {
         auto pair = connections_[index];

         auto t0 = pair.TileIndices().first;
         auto t1 = pair.TileIndices().second;

         auto x0 = pos[t0];
         auto x1 = pos[t1];
         auto edge0 = x0.Edges[pair.EdgeIndices().first];
         auto edge1 = x1.Edges[pair.EdgeIndices().second];

         dd[3 * t0][3 * t0] += 2.0;
         dd[3 * t0 + 1][3 * t0 + 1] += 2.0;
         dd[3 * t1][3 * t1] += 2.0;
         dd[3 * t1 + 1][3 * t1 + 1] += 2.0;

         dd[3 * t0][3 * t1] -= 2.0;
         dd[3 * t1][3 * t0] -= 2.0;
         dd[3 * t0 + 1][3 * t1 + 1] -= 2.0;
         dd[3 * t1 + 1][3 * t0 + 1] -= 2.0;

         dd[3 * t0][3 * t0 + 2] -= 2.0* edge0.Length() * sin(x0.Alpha + edge0.Theta());
         dd[3 * t0 + 2][3 * t0] -= 2.0* edge0.Length() * sin(x0.Alpha + edge0.Theta());
         dd[3 * t0][3 * t1 + 2] += 2.0* edge1.Length() * sin(x1.Alpha + edge1.Theta());
         dd[3 * t1 + 2][3 * t0] += 2.0* edge1.Length() * sin(x1.Alpha + edge1.Theta());

         dd[3 * t0 + 1][3 * t0 + 2] += 2.0* edge0.Length() * cos(x0.Alpha + edge0.Theta());
         dd[3 * t0 + 2][3 * t0 + 1] += 2.0* edge0.Length() * cos(x0.Alpha + edge0.Theta());
         dd[3 * t0 + 1][3 * t1 + 2] -= 2.0* edge1.Length() * cos(x1.Alpha + edge1.Theta());
         dd[3 * t1 + 2][3 * t0 + 1] -= 2.0* edge1.Length() * cos(x1.Alpha + edge1.Theta());

         dd[3 * t0 + 2][3 * t0 + 2] += 2 * edge0.Length()*(edge0.Length()*pow(sin(x0.Alpha + edge0.Theta()), 2) + edge0.Length()*pow(cos(x0.Alpha + edge0.Theta()), 2) -
            (x0.Centriod.X() - x1.Centriod.X() + edge0.Length()*cos(x0.Alpha + edge0.Theta()) - edge1.Length()*cos(x1.Alpha + edge1.Theta()))*cos(x0.Alpha + edge0.Theta())
            - (x0.Centriod.Y() - x1.Centriod.Y() + edge0.Length()*sin(x0.Alpha + edge0.Theta()) - edge1.Length()*sin(x1.Alpha + edge1.Theta()))*sin(x0.Alpha + edge0.Theta()));

         dd[3 * t0 + 2][3 * t1] += 2.0* edge0.Length() * sin(x0.Alpha + edge0.Theta());
         dd[3 * t1][3 * t0 + 2] += 2.0* edge0.Length() * sin(x0.Alpha + edge0.Theta());
         dd[3 * t0 + 2][3 * t1 + 1] -= 2.0* edge0.Length() * cos(x0.Alpha + edge0.Theta());
         dd[3 * t1 + 1][3 * t0 + 2] -= 2.0* edge0.Length() * cos(x0.Alpha + edge0.Theta());

         dd[3 * t0 + 2][3 * t1 + 2] -= 2 * edge0.Length()*edge1.Length()*(sin(x0.Alpha + edge0.Theta())* sin(x1.Alpha + edge1.Theta()) + cos(x0.Alpha + edge0.Theta())* cos(x1.Alpha + edge1.Theta()));
         dd[3 * t1 + 2][3 * t0 + 2] -= 2 * edge0.Length()*edge1.Length()*(sin(x0.Alpha + edge0.Theta())* sin(x1.Alpha + edge1.Theta()) + cos(x0.Alpha + edge0.Theta())* cos(x1.Alpha + edge1.Theta()));

         dd[3 * t1][3 * t1 + 2] -= 2 * edge1.Length() * sin(x1.Alpha + edge1.Theta());
         dd[3 * t1 + 2][3 * t1] -= 2 * edge1.Length() * sin(x1.Alpha + edge1.Theta());
         dd[3 * t1 + 1][3 * t1 + 2] += 2 * edge1.Length() * cos(x1.Alpha + edge1.Theta());
         dd[3 * t1 + 2][3 * t1 + 1] += 2 * edge1.Length() * cos(x1.Alpha + edge1.Theta());

         dd[3 * t1 + 2][3 * t1 + 2] += 2 * edge1.Length()*(edge1.Length()*pow(sin(x1.Alpha + edge1.Theta()), 2) + edge1.Length()*pow(cos(x1.Alpha + edge1.Theta()), 2) + (x0.Centriod.X() - x1.Centriod.X() +
            edge0.Length()*cos(x0.Alpha + edge0.Theta()) - edge1.Length()*cos(x1.Alpha + edge1.Theta()))*cos(x1.Alpha + edge1.Theta()) + (x0.Centriod.Y()
               - x1.Centriod.Y() + edge0.Length()*sin(x0.Alpha + edge0.Theta()) - edge1.Length()*sin(x1.Alpha + edge1.Theta()))*sin(x1.Alpha + edge1.Theta()));
      }

      for (auto index = 0; index < 24; index++)
      {
         dd[5][index] = 0.0;
         dd[index][5] = 0.0;
         dd[12][index] = 0.0;
         dd[13][index] = 0.0;
         dd[14][index] = 0.0;
         dd[index][12] = 0.0;
         dd[index][13] = 0.0;
         dd[index][14] = 0.0;
      }

      auto tmp = std::vector<double>(24);
      for (auto i = 0; i < 8; i++)
      {
         tmp[0 + 3 * i] = d[i].Centriod.X();
         tmp[1 + 3 * i] = d[i].Centriod.Y();
         tmp[2 + 3 * i] = d[i].Alpha;
      }

      double value = 0.0;
      for (auto i = 0; i < 24; i++)
         for (auto j = 0; j < 24; j++) value += tmp[i] * dd[i][j] * tmp[j];

      return value;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   double Energy(const PositionType& positions) const
   {
      auto energy = 0.0;
      for (size_t connection = 0; connection < connection_count_; connection++)
      {
         auto pair = connections_[connection];
         auto tile0 = positions[pair.TileIndices().first];
         auto tile1 = positions[pair.TileIndices().second];

         auto edge0 = tile0.Edges[pair.EdgeIndices().first];
         auto edge1 = tile1.Edges[pair.EdgeIndices().second];

         energy += pow(tile0.Centriod.X() + edge0.Length() * cos(edge0.Theta() + tile0.Alpha)
            - tile1.Centriod.X() - edge1.Length() * cos(edge1.Theta() + tile1.Alpha), 2) +
            pow(tile0.Centriod.Y() + edge0.Length() * sin(edge0.Theta() + tile0.Alpha)
               - tile1.Centriod.Y() - edge1.Length() * sin(edge1.Theta() + tile1.Alpha), 2);
      }

      return energy;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static int calcPenal2(double poly1[8][2], double poly3[4][2])
   {
      int penalFlag = 0;
      int penalVal = 0;
      int nvert = 8;

      double vertx[8] = { 0,0,0,0,0,0,0,0 };
      double verty[8] = { 0,0,0,0,0,0,0,0 };

      for (int counter2 = 0; counter2 < 8; counter2++)
      {
         vertx[counter2] = poly1[counter2][0];
         verty[counter2] = poly1[counter2][1];
      }
      int c = 0;
      for (int counter1 = 0; counter1 < 4; counter1++)
      {
         double testx = poly3[counter1][0];
         double testy = poly3[counter1][1];
         int i = 0, j = 0;
         for (i = 0, j = nvert - 1; i < nvert; j = i++)
         {
            if (((verty[i] > testy) != (verty[j] > testy)) &&
               (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
               c = !c;
         }
         if (c == 1)
         {
            c = 0; // current point is inside, continue checking next point
            penalFlag = 0; // do not raise penalty flag
         }
         else
         {
            penalFlag = 1; // raise the penalty flag
            break;
         } //current point is outside, no need to check more, break and impose penalty

      }

      if (penalFlag == 0) penalVal = 0;
      else penalVal = 1;

      return penalVal;
   }

   // -----------------------------------------------------------------------------------------------------------------------------------

   static double CCW(Vector2 a, Vector2 b, Vector2 c)
   {
      auto ccw = (b.X() - a.X())*(c.Y() - a.Y()) - (b.Y() - a.Y())*(c.X() - a.X());
      //std::cout << "CCW: " << ccw << std::endl;
      return ccw;
   }

   static bool intersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
   {
      return CCW(a, b, c)*CCW(a, b, d) < 0 && CCW(c, d, b)*CCW(c, d, a) < 0;
   }

   // ------------------------------------------------------------------------------------------------------------------------------------
   static int calcPenal3(double poly1[8][2], double poly2[12][2], double poly3[4][2])
   {
      Vector2 a, b, c, d;
      int penalValInnerPoly = 0, penalValOuterPoly = 0, penalValConnectingPoly = 0;
      int breakFlag = 0;
      for (int i = 0; i < 4; i++)
      {
         for (int j = 0; j < 4; j++)
         {
            a = Vector2(poly3[i][0], poly3[i][1]);
            b = i == 3 ? Vector2(poly3[0][0], poly3[0][1]) : Vector2(poly3[i + 1][0], poly3[i + 1][1]);
			c = Vector2(poly3[j][0], poly3[j][1]);
            d = j == 3 ? Vector2(poly3[0][0], poly3[0][1]) : Vector2(poly3[j + 1][0], poly3[j + 1][1]);

            if (intersect(a, b, c, d))
            {
               penalValInnerPoly = 1;
               breakFlag = 1;
               break;
            }
         }
         if (breakFlag == 1) break;
      }

      breakFlag = 0;

      for (int i = 0; i < 8; i++)
      {
         for (int j = 0; j < 8; j++)
         {
            a = Vector2(poly1[i][0], poly1[i][1]);
            b = i == 7 ? Vector2(poly1[0][0], poly1[0][1]) : Vector2(poly1[i + 1][0], poly1[i + 1][1]);
			c = Vector2(poly1[j][0], poly1[j][1]);
            d = j == 7 ? Vector2(poly1[0][0], poly1[0][1]) : Vector2(poly1[j + 1][0], poly1[j + 1][1]);

            if (intersect(a, b, c, d))
            {
               penalValOuterPoly = 1;
               breakFlag = 1;
               break;
            }

         }
         if (breakFlag == 1)
            break;
      }

      breakFlag = 0;
      if (penalValOuterPoly == 0)
      {
         for (int i = 0; i < 12; i++)
         {
            for (int j = 0; j < 12; j++)
            {
               a = Vector2(poly2[i][0], poly2[i][1]);
               b = i == 11 ? Vector2(poly2[0][0], poly2[0][1]) : Vector2(poly2[i + 1][0], poly2[i + 1][1]);
               c = Vector2(poly2[j][0], poly2[j][1]);
               d = j == 11 ? Vector2(poly2[0][0], poly2[0][1]) : Vector2(poly2[j + 1][0], poly2[j + 1][1]);

               if (intersect(a, b, c, d))
               {
                  penalValConnectingPoly = 1;
                  breakFlag = 1;
                  break;
               }
            }
            if (breakFlag == 1) break;
         }
      }
      else
      {
         penalValConnectingPoly = 1;
      }

      //std::cout << "penalValInnerPoly: " << penalValInnerPoly << std::endl;
      //std::cout << "penalValOuterPoly: " << penalValOuterPoly << std::endl;
      //std::cout << "penalValConnectingPoly: " << penalValConnectingPoly << std::endl;

      return penalValInnerPoly + penalValOuterPoly + penalValConnectingPoly;
   }
   //------------------------------------------------------------------------------------------------------------------------------------

  
   
   
   
   static double dotProduct(const PositionType& x, const PositionType& y)
   {
      ASSERT(x.size() == y.size());
      auto product = 0.0;
      auto x_size = x.size();
      for (size_t idx = 0; idx < x_size; idx++)
         product += Vector2::dot(x[idx].Centriod, y[idx].Centriod) + x[idx].Alpha * y[idx].Alpha;
      return product;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static void add_r(PositionType& x, const PositionType& d, double alpha)
   {
      ASSERT(x.size() == d.size());
      auto x_size = x.size();
      for (size_t idx = 0; idx < x_size; idx++)
      {
         x[idx].Centriod += alpha * d[idx].Centriod;
         x[idx].Alpha += alpha * d[idx].Alpha;
      }
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   static void add_d(PositionType& d, const PositionType& r, double beta)
   {
      ASSERT(d.size() == r.size());
      auto d_size = d.size();
      for (size_t idx = 0; idx < d_size; idx++)
      {
         d[idx].Centriod = r[idx].Centriod + beta * d[idx].Centriod;
         d[idx].Alpha = r[idx].Alpha + beta * d[idx].Alpha;
      }
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   std::pair<double, double> CalculatePenalty(PositionType& x, double distance, double log_e) const
   {
      double outerPolygon[8][2] = { {0,0},{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } };
      int tileIndexOuterPolygon[8] = { 3, 1, 1, 5, 5, 7, 7, 3 };
      int cornerIndexOuterPolygon[8] = { 0, 0, 1, 0, 1, 0, 1, 1 };

      double connectingPolygon[12][2] = { { 0,0 },{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 } ,{ 0,0 },{ 0,0 } ,{ 0,0 },{ 0,0 },{ 0,0 } };
      int tileIndexConnectingPolygon[12] = { 3 , 1, 1, 1, 5, 5, 5, 7, 7, 7, 3, 3 };
      int cornerIndexConnectingPolygon[12] = { 0 , 0, 2, 1, 0, 2, 1, 0, 2, 1, 1, 2 };

      double innerPolygon[4][2] = { {0,0} ,{0,0},{0,0},{0,0} };
      int tileIndexInnerPolygon[4] = { 4, 4, 4, 4 };
      int cornerIndexInnerPolygon[4] = { 0, 1, 2, 3 };

      int penal1 = 0.0;    // penalty for not equilibrating 
      int penal2 = 0.0;         // penalty for inside polygon not inside the outer blue polygon 
      int penal3 = 0.0;         // penalty for self intersection in either of the polygon		
	  

      if (log_e > -6.0)
      {
         penal1 = log_e / 10.0 + 1.0;
      }
      else
      {
         //----------------------------------create the three polygons---------------------------------------------------------
         for (int index = 0; index < 8; index++)
         {
            auto tempTile = x[tileIndexOuterPolygon[index]];
            auto tempEdge = tempTile.Edges[cornerIndexOuterPolygon[index]];
            outerPolygon[index][0] = tempTile.Centriod.X() + tempEdge.Length() * cos(tempTile.Alpha + tempEdge.Theta());
            outerPolygon[index][1] = tempTile.Centriod.Y() + tempEdge.Length() * sin(tempTile.Alpha + tempEdge.Theta());
         }

         for (int index = 0; index < 12; index++)
         {
            auto tempTile = x[tileIndexConnectingPolygon[index]];
            auto tempEdge = tempTile.Edges[cornerIndexConnectingPolygon[index]];
            connectingPolygon[index][0] = tempTile.Centriod.X() + tempEdge.Length() * cos(tempTile.Alpha + tempEdge.Theta());
            connectingPolygon[index][1] = tempTile.Centriod.Y() + tempEdge.Length() * sin(tempTile.Alpha + tempEdge.Theta());

         }

         for (int index = 0; index < 4; index++)
         {
            auto tempTile = x[tileIndexInnerPolygon[index]];
            auto tempEdge = tempTile.Edges[cornerIndexInnerPolygon[index]];
            innerPolygon[index][0] = tempTile.Centriod.X() + tempEdge.Length() * cos(tempTile.Alpha + tempEdge.Theta());
            innerPolygon[index][1] = tempTile.Centriod.Y() + tempEdge.Length() * sin(tempTile.Alpha + tempEdge.Theta());
         }

         //call a function calcPenal2 (cordinate of 12 vertices)
         penal2 = calcPenal2(outerPolygon, innerPolygon);

         // call a function calcPenal3 (cordinate of three polygon paths)
         penal3 = calcPenal3(outerPolygon, connectingPolygon, innerPolygon);

		 
         //---------------------------------------------------------------------------------------------------------------------- 	
      }

      //std::cout << std::setprecision(20) << std::scientific;
      //std::cout << "penal1: " << penal1 << std::endl;
      //std::cout << "penal2: " << penal2 << std::endl;
      //std::cout << "penal3: " << penal3 << std::endl;

	  double totalPenal = penal1/1 + static_cast<double>(penal2) / 2 + static_cast<double>(penal3) / 6 ;
	  //double totalPenal = penal1 + static_cast<double>(penal2) / 2 + static_cast<double>(penal3) / 6;
	  return std::make_pair(distance, totalPenal);
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   std::pair<double, double> RunAlgorithm(PositionType& x, int gc_max, int n) const
   {
      auto i = 0;
      auto k = 0;
      auto beta = 0.0;
      auto delta_old = 0.0;
      auto alpha = 0.0;

      auto r = Df(x);
      auto d = r;

      auto delta_new = dotProduct(r, r);
      auto energy_old = Energy(x);
      auto energy_new = 0.0;

      while (i < gc_max)
      {
         alpha = dotProduct(Df(x), d) / dDDfd(x, d);
         add_r(x, d, alpha);

         r = Df(x);
         delta_old = delta_new;
         delta_new = dotProduct(r, r);
         beta = delta_new / delta_old;
         add_d(d, r, beta);
         k++;

         if (k == n || dotProduct(r, d) <= 0.0)
         {
            d = r; k = 0;
         }

         energy_new = Energy(x);
         double log_new = log10(energy_new);
         double log_old = log10(energy_old);

         if (log_new < -10|| log_old - log_new < 1e-5)
            break;

         i++;
         energy_old = energy_new;
      }

      auto tile0 = x[5];
      auto tile1 = x[7];
      auto edge0 = tile0.Edges[1];
      auto edge1 = tile1.Edges[0];

      auto dx = tile0.Centriod.X() + edge0.Length() * cos(edge0.Theta() + tile0.Alpha) -
         tile1.Centriod.X() - edge1.Length() * cos(edge1.Theta() + tile1.Alpha);
      auto dy = tile0.Centriod.Y() + edge0.Length() * sin(edge0.Theta() + tile0.Alpha) -
         tile1.Centriod.Y() - edge1.Length() * sin(edge1.Theta() + tile1.Alpha);

      auto distance = sqrt(dx*dx + dy*dy);
	  //std::cout << "Distance" << distance << std::endl;
	  auto log_e = log10(energy_new);
	  	
      auto penalty = CalculatePenalty(x, distance, log_e);
      return penalty;
   }

   // --------------------------------------------------------------------------------------------------------------------------------

   // Alias of one of the fundamental unsigned integer types.
   size_t tile_count_;
   size_t connection_count_;

   std::vector<Connection> connections_;
};

// --------------------------------------------------------------------------------------------------------------------------------

#endif