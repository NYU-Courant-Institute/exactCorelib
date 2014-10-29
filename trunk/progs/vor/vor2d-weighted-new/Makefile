# Makefile for weighted Voronoi cell diagrams.
CORE_PATH=../../..
include ../Make.options

CXX = g++
CXXFLAGS = -std=c++11 -g

TARGET = vor2d
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
DEPS = $(SRCS:.cpp=.depends)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LDFLAGS) -L/usr/lib -lglui $(CORE_LIB) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CORE_INC) $(CORE_LIB) -DCORE_LEVEL=$(LEV) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o $(TARGET)

test:
	./vor2d