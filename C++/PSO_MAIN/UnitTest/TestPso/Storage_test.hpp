#include "Storage.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

inline int test_LoadData_HasCorrectValues()
{
   std::string testfile = "default2";

   Tiling tiling;
   std::vector<Particle> particles;
   auto load_ok = Storage::LoadData(testfile, tiling, particles);
   TINYTEST_ASSERT(load_ok);

   auto actual_pos_p0 = particles[0].positions;
   auto expected_pos_p0 = std::vector<Vector2>
   {
      Vector2(-7.2389309135751140,+4.2848068100699500), Vector2(-4.4695286855240480,+6.9640487018273790),
      Vector2(+3.7851759327041410,+4.6336038766576895), Vector2(6.75814437960329100,+4.1071228689204500),
      Vector2(+5.0006220029816160,-3.5392337340448403), Vector2(+1.9146911042256645,-6.2799885287372090),
      Vector2(-3.6646259549556484,-7.3761869862620950), Vector2(-6.6677622063250315,-3.7663952569086180),
      Vector2(-1.9659108820138180,-0.4068311425746798), Vector2(-0.9414460352134987,+2.8157012437329905),
      Vector2(+2.6843084697248574,+1.3375271585349935), Vector2(+1.5173745991397396,-3.3273819536941970)
   };

   TINYTEST_EQUAL(actual_pos_p0.size(), expected_pos_p0.size());
   for (auto el = actual_pos_p0.size() - 1; el > 0; el--) TINYTEST_EQUAL(actual_pos_p0[el], expected_pos_p0[el]);

   auto actual_pos_p1 = particles[1].positions;
   auto expected_pos_p1 = std::vector<Vector2>
   {
      Vector2(-6.8950510409013440,+4.518237967071309), Vector2(-4.24133783554381400,+4.4603185129742595),
      Vector2(+2.8107616391730605,+6.924991035547615), Vector2(+5.78277532532038900,+3.7161494634611920),
      Vector2(+7.2523422458586260,-4.644791002401719), Vector2(+3.60414813467133930,-5.9402276814962280),
      Vector2(-1.3593585105810864,-4.769876404960642), Vector2(-5.97814640715878600,-1.8742086527359063),
      Vector2(-3.1310214425637057,-1.342289252789600), Vector2(-0.05716180126583359,+1.9718441377241096),
      Vector2(+1.8082098955611450,+1.312221279274607), Vector2(+0.08607032166997047,-4.06731643978896300)
   };

   TINYTEST_EQUAL(actual_pos_p1.size(), expected_pos_p0.size());
   for (auto el = actual_pos_p1.size() - 1; el > 0; el--) TINYTEST_EQUAL(actual_pos_p1[el], expected_pos_p1[el]);

   auto actual_structure = tiling.structure;
   auto expected_structure = std::vector<std::vector<int>>{ std::vector<int>{0,1}, std::vector<int>{1,2,9},
      std::vector<int>{2,3}, std::vector<int>{0,7,8}, std::vector<int>{8,9,10,11}, std::vector<int>{3,4,10},
      std::vector<int>{6,7}, std::vector<int>{5,6,11} };

   TINYTEST_EQUAL(expected_structure.size(), actual_structure.size())
      for (auto sr = actual_structure.size() - 1; sr > 0; sr--)
      {
         TINYTEST_EQUAL(actual_structure[sr].size(), actual_structure[sr].size());
         for (auto el = actual_structure[sr].size() - 1; el > 0; el--)
            TINYTEST_EQUAL(actual_structure[sr][el], actual_structure[sr][el]);
      }

   auto actual_connections = tiling.connections;
   auto expected_connections = std::vector<Connection>{ Connection(0,3,0,0), Connection(0,1,1,0),
      Connection(1,2,1,0),Connection(2,5,1,0), Connection(6,7,0,1), Connection(3,6,1,1),
      Connection(3,4,2,0), Connection(1,4,2,1), Connection(4,5,2,2), Connection(4,7,3,2) };

   TINYTEST_EQUAL(expected_connections.size(), actual_connections.size());
   for (auto cn = actual_connections.size() - 1; cn > 0; cn--)
   {
      TINYTEST_EQUAL(expected_connections[cn].EdgeIndices(), actual_connections[cn].EdgeIndices());
      TINYTEST_EQUAL(expected_connections[cn].TileIndices(), actual_connections[cn].TileIndices());
   }


   auto actual_tiles_p0 = particles[0].tiles;
   auto expected_tiles_p0 = std::vector<Vector2>
   {
      Vector2(-5.85422979954958044857e+00, +5.62442775594866439803e+00), Vector2(-5.41932929344468461430e-01, +4.80445127407268568476e+00),
      Vector2(+5.27166015615371641445e+00, +4.37036337278907005555e+00), Vector2(-5.29086800063798801119e+00, +3.71934701955508248417e-02),
      Vector2(+3.23581537909320038349e-01, +1.04753826499776825365e-01), Vector2(+4.81435828410325417082e+00, +6.35138764470201011569e-01),
      Vector2(-5.16619408064033969197e+00, -5.57129112158535644284e+00), Vector2(-7.75200838634147598727e-02, -5.66118582289783400796e+00),
   };

   auto expected_lengths_p0 = std::vector<std::vector<double>>
   {
      { 1.92665031954958942961e+00, 1.92665031954958942961e+00 }, {4.48217240564489305399e+00,4.33048033562364675930e+00,2.02848155153978693122e+00},
      {1.50961283428615899993e+00,1.50961283428615899993e+00}, {4.67302559353573698786e+00,4.04513595057180541659e+00,3.35447428033593553209e+00},
      {2.34595283871360438255e+00,2.99157661773260308280e+00,2.66322393628031406365e+00,3.63382689278528925314e+00},
      {3.97906747952512995781e+00,4.17852604746328015750e+00,2.24286907059916496010e+00},{2.34784069272159712227e+00,2.34784069272159712227e+00},
      {2.08610215634360374182e+00,3.97599767615187893455e+00,2.82671709758539480006e+00}
   };

   auto expected_theta_p0 = std::vector<std::vector<double>>
   {
      { 3.91044502933125093236e+00, 7.68852375741457705338e-01 },{ 2.63886287290239129888e+00,6.24372276828280359240e+00,4.51414112695478575432e+00 },
      { 2.96632045759171347754e+00,6.10791311118150659354e+00 }, { 2.00079985441719276906e+00,4.36506513335953183486e+00,6.15042781628126000726e+00 },
      { 3.36143041613723925209e+00,2.00739888365111385227e+00,4.81250045488951172512e-01,5.04712755990599859501e+00 },
      { 1.06042312438096164584e+00,4.75698017308947385118e+00,2.82306866679052514968e+00 },{ 5.40630539976118473788e+00,2.26471274617139162189e+00 },
      { 5.98202230421712677355e+00,3.58756859785990833700e+00,9.71306248809079453288e-01 }
   };

   for (size_t ln = 0; ln < actual_tiles_p0.size(); ln++)
   {
      TINYTEST_EQUAL(expected_lengths_p0[ln].size(), actual_tiles_p0[ln].Edges.size());
      for (size_t eg = 0; eg < actual_tiles_p0[ln].Edges.size(); eg++)
      {
         TINYTEST_EQUAL(expected_lengths_p0[ln][eg], actual_tiles_p0[ln].Edges[eg].Length());
         TINYTEST_EQUAL(expected_theta_p0[ln][eg], actual_tiles_p0[ln].Edges[eg].Theta());
      }
   }


   auto actual_tiles_p1 = particles[1].tiles;
   auto expected_tiles_p1 = std::vector<Vector2>
   {
      Vector2(-5.56819443822257920118e+00,+4.48927824002278441640e+00), Vector2(-4.95912665878862402025e-01,+4.45238456208199462338e+00),
      Vector2(+4.29676848224672447429e+00,+5.32057024950440293765e+00), Vector2(-5.33473963020794528234e+00,+4.33913353848600913398e-01),
      Vector2(-3.23475756649605927784e-01,-5.31385068894961731800e-01), Vector2(+4.94777582224671963473e+00,+1.27859913444693162887e-01),
      Vector2(-3.66875245886993628375e+00,-3.32204252884827422321e+00), Vector2(+7.76953315253407938989e-01,-4.92580684208194430340e+00)
   };

   auto expected_lengths_p1 = std::vector<std::vector<double>>
   {
      { 1.32717259987650404440e+00,1.32717259987650404440e+00 },{ 3.74543357291735912185e+00,4.12890757129712326901e+00,2.51904408815571878577e+00 },
      { 2.18686588439924101124e+00,2.18686588439924101124e+00 },{ 4.37221674262802828537e+00,2.39612175773810864854e+00,2.83041861751620604792e+00 },
      { 2.92230702937810837838e+00,2.51735567286446348945e+00,2.81832717881588523667e+00,3.55957000913583332391e+00 },
      { 3.68416151449565010267e+00,5.29992671320735997398e+00,3.35553066635898211700e+00 },{2.72571519811071372175e+00,2.72571519811071372175e+00 },
      { 3.00367777672332403327e+00,2.14199498561998247226e+00,1.10196419254528232834e+00 }
   };

   auto expected_theta_p1 = std::vector<std::vector<double>>
   {
      { 3.11977030306187685937e+00,6.26136295665166997537e+00 },{3.13947435228465865364e+00,6.42067459290556419305e-01,4.88745541306138076010e+00},
      {2.31789686325834676239e+00,5.45948951684813987839e+00},{1.93571105527661346812e+00,4.44053263488775940004e+00,5.60479494023397517566e+00},
      {3.42277075285811038796e+00,1.46480684313031406241e+00,7.13057162759150986453e-01,4.82769927712180813018e+00},
      {1.34216391366687970432e+00,5.16223831859949822132e+00,2.78086196113328254498e+00},{5.72319761200824395075e+00,2.58160495841845127885e+00},
      {5.93868514219790544217e+00,3.06873137705813769927e+00,2.24843598612148332094e+00}
   };

   for (size_t ln = 0; ln < actual_tiles_p1.size(); ln++)
   {
      TINYTEST_EQUAL(expected_lengths_p1[ln].size(), actual_tiles_p1[ln].Edges.size());
      for (size_t eg = 0; eg < actual_tiles_p1[ln].Edges.size(); eg++)
      {
         TINYTEST_EQUAL(expected_lengths_p1[ln][eg], actual_tiles_p1[ln].Edges[eg].Length());
         TINYTEST_EQUAL(expected_theta_p1[ln][eg], actual_tiles_p1[ln].Edges[eg].Theta());
      }
   }

   return 1;
}

// --------------------------------------------------------------------------------------------------------------------------------

TINYTEST_START_SUITE(Storage);
TINYTEST_ADD_TEST(test_LoadData_HasCorrectValues);
TINYTEST_END_SUITE();

// --------------------------------------------------------------------------------------------------------------------------------
