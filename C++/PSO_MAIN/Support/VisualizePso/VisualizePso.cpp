#define GLUT_DISABLE_ATEXIT_HACK

#include <math.h>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <GL/freeglut.h>
#include "ParticleSwarmOptimizer.hpp"

// --------------------------------------------------------------------------------------------------------------------------------

using PositionType = std::pair<Vector2, double>;
using EdgesType = std::vector<Edge>;

std::string loadfile = "default50";
bool autoSim = true;
bool showHelp = true;
static float aspect;

static ParticleSwarmOptimizer optimizer;
static std::string directory_path;

// --------------------------------------------------------------------------------------------------------------------------------

std::vector<std::string> help =
{
      "ESC = exit" ,
      "H = show help",
      "F = full screen",
      "S = start, stop",
      ". = single step",
      "R = reset",
};

// --------------------------------------------------------------------------------------------------------------------------------

void AddGrid(double max_value, double step_value)
{
   glColor3d(0.8, 0.8, 0.8);
   glBegin(GL_LINES);
   for (auto i = -max_value; i <= max_value; i += step_value)
   {
      glVertex3d(max_value, i, 0); glVertex3d(-max_value, i, 0);
      glVertex3d(i, max_value, 0); glVertex3d(i, -max_value, 0);
   }
   glEnd();

   glColor3d(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
   glVertex2d(0, max_value);
   glVertex2d(0, -max_value);

   glVertex2d(max_value, 0);
   glVertex2d(-max_value, 0);
   glEnd();
}

// --------------------------------------------------------------------------------------------------------------------------------

void AddPolygon(const Tile& tile)
{
   glColor3f(0.5, 0.5, 0.5);
   auto vertices = tile.Edges.size();
   glBegin(vertices == 2 ? GL_LINES : vertices == 3 ? GL_TRIANGLES : GL_QUADS);

   for (auto egde : tile.Edges)
   {
      auto x = tile.Centriod.X() + egde.Length() * cos(tile.Alpha + egde.Theta());
      auto y = tile.Centriod.Y() + egde.Length() * sin(tile.Alpha + egde.Theta());
      glVertex2d(x, y);
   }
   glEnd();
}

// --------------------------------------------------------------------------------------------------------------------------------

void display()
{
   auto max_value = 10.0;
   auto step_size = 2.0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -static_cast<GLfloat>(max_value + 1));

   AddGrid(max_value, step_size);
   auto best_positions = optimizer.GlobalBestPositions();
   for (auto tile : best_positions.tiles)
      AddPolygon(tile);

   if (showHelp)
   {
      auto line = 1;
      for (auto& txt : help)
      {
         glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
         glRasterPos2d(-max_value + 0.5, max_value - line++ * 0.5);
         glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(txt.c_str()));
      }
   }

   glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
   glRasterPos2d(-max_value + 1, -max_value + 1);
   auto val = std::to_string(optimizer.Generation());
   glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(("generation: " + val).c_str()));

   glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
   glRasterPos2d(-max_value + 1, -max_value + 1.5);
   auto fitness = std::to_string(optimizer.GlobalMean());
   glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(("mean:  " + fitness).c_str()));

   glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
   glRasterPos2d(-max_value + 1, -max_value + 2.5);
   auto best_fitness = std::to_string(optimizer.GlobalBestFitness());
   glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(("best:  " + best_fitness).c_str()));

   glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
   glRasterPos2d(-max_value + 1, -max_value + 3);
   auto best_index = std::to_string(optimizer.GlobalBestIndex());
   glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char*>(("index: " + best_index).c_str()));

   glutSwapBuffers();
}

// --------------------------------------------------------------------------------------------------------------------------------

void reshape(int width, int height)
{
   if (height == 0) height = 1;
   aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);

   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0f, aspect, 0.1f, 100.0f);
}

// --------------------------------------------------------------------------------------------------------------------------------

void idle()
{
   if (!autoSim) return;

   optimizer.RunGeneration();
   glutPostRedisplay();
}

// --------------------------------------------------------------------------------------------------------------------------------

bool LoadSimulation(std::string file)
{
   optimizer = ParticleSwarmOptimizer();

   std::string message;
   if (!optimizer.Initialize(file, message))
   {
      std::cout << message << std::endl;
      return false;
   }

   return true;
}

// --------------------------------------------------------------------------------------------------------------------------------

void keyboard(unsigned char key, int mouse_x, int mouse_y)
{
   unused(mouse_x);
   unused(mouse_y);
   try
   {
      switch (key)
      {
      case 27: glutLeaveMainLoop(); break;
      case 'f': case 'F': glutFullScreenToggle(); break;
      case 'h': case 'H': showHelp = !showHelp; glutPostRedisplay(); break;
      case 's': case 'S': autoSim = !autoSim; glutPostRedisplay(); break;
      case '.': optimizer.RunGeneration(); glutPostRedisplay(); break;
      case 'r': case 'R': if (LoadSimulation(loadfile)) { glutPostRedisplay(); } else { abort(); } break;
      default: break;
      }
   }
   catch (std::exception ex)
   {
      abort();
   }
}

// --------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
   if (!LoadSimulation(loadfile)) return -1;

   glutInit(&argc, argv);
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100);

   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
   glutCreateWindow("Tiling Viewer");

   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glShadeModel(GL_SMOOTH);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);

   glutMainLoop();
   printf("Press any key to continue...\n");
   return 0;
}


// --------------------------------------------------------------------------------------------------------------------------------
