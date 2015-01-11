# Makefile for weighted Voronoi cell diagrams.
CORE_PATH=../../..
include ../Make.options

CXX = g++-4.8
CXXFLAGS = -std=c++11 -g

TARGET = vor2d
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
DEPS = $(SRCS:.cpp=.depends)

# Core level.
LEV = 1

# Build commands.
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LDFLAGS) -L/usr/lib -lglui $(CORE_LIB) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CORE_INC) $(CORE_LIB) -DCORE_LEVEL=$(LEV) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o $(TARGET)

# Test commands.
test:
	./vor2d test_input &

test_conics:
	./vor2d conics/ellipse &
	./vor2d conics/parabola &
	./vor2d conics/hyperbola &

valgrind:
	valgrind ./vor2d test_input

poly_an:
	./vor2d poly_an_test_input &

an:
	./vor2d an_test_input &