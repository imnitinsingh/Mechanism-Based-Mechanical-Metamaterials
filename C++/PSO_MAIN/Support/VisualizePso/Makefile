NAME := VisualizePso
SOURCES := VisualizePso.cpp

INCLUDE := -I. -I../../Pso -I/usr/include/
CXXOPTIONS ?= -O6 -Wall
CXXFLAGS := -std=c++14 $(CXXOPTIONS)
LIBS := -L../../Pso -lrt -lglut -lGLU -lGL -lPso
.PHONY: visualize clean

visualize : $(NAME)

$(NAME) : $(SOURCES:.cpp=.o) 
	$(CXX) $^ $(LIBS) -o $@

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@$(RM) $(SOURCES:.cpp=.o) $(NAME) *~
