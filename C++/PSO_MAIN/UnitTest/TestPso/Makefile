NAME := TestPso
SOURCES := tinytest.cpp
HEADERS := ConjugateGradient_test.hpp \
ParticleSwarmOptimizer_test.hpp \
RandomNumberGenerator_test.hpp \
Storage_test.hpp 

INCLUDE := -I. -I../../Pso -I/usr/include/
CXXOPTIONS ?= -O6 -Wall
CXXFLAGS := -std=c++14 $(CXXOPTIONS)
LIBS := -L../../Pso -lPso
.PHONY: testpso clean

testpso : $(NAME)

$(NAME) : $(SOURCES:.cpp=.o) 
	$(CXX) $^ $(LIBS) -o $@

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@$(RM) $(SOURCES:.cpp=.o) $(NAME) *~
