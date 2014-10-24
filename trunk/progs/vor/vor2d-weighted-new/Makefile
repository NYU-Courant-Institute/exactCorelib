# Makefile for weighted Voronoi cell diagrams.
CXX = g++
CXXFLAGS = -std=c++11

TARGET = vor2d
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
DEPS = $(SRCS:.cpp=.depends)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o $(TARGET)

test:
	./vor2d

qt:
	$(CXX) $(CXXFLAGS)