NAME := RunPso
SOURCES := RunPso.cpp

INCLUDE := -I. -I../../Pso -I/usr/include/
CXXOPTIONS ?= -O6 -Wall
CXXFLAGS := -std=c++14 $(CXXOPTIONS)
LIBS := -L../../Pso -lPso
.PHONY: run clean

run : $(NAME)

$(NAME) : $(SOURCES:.cpp=.o) 
	$(CXX) $^ $(LIBS) -o $@

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@$(RM) $(SOURCES:.cpp=.o) $(NAME) *~
