NAME:= Pso
SOURCES := ParticleSwarmOptimizer.cpp \
	Particle.cpp
HEADERS := ConjugateGradient.hpp \
	Connection.hpp \
	Edge.hpp \
	Tile.hpp \
	Tiling.hpp \
	Storage.hpp \
	Vector2.hpp \
	DefsPso.hpp \
	$(SOURCES:.cpp=.hpp)
INCLUDE := -I/usr/include/
CXXOPTIONS ?= -O6 -Wall
CXXFLAGS := -std=c++14 -fPIC $(CXXOPTIONS)
.PHONY: pso clean

pso : lib$(NAME).so

lib$(NAME).so : $(SOURCES:.cpp=.o) 
	$(CXX) -shared -Wl,-soname,lib$(NAME).so $? -o $@

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@$(RM) $(SOURCES:.cpp=.o) lib$(NAME).so *~ 
