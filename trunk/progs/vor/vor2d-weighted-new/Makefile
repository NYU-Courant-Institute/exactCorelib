# Makefile for weighted Voronoi cell diagrams.
CORE_PATH=../../..
include ../Make.options

CXX = g++-5
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
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -lpng -lboost_program_options -L/usr/lib $(CORE_LIB) -lglui -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CORE_INC) $(CORE_LIB) -DCORE_LEVEL=$(LEV) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o $(TARGET) lex.yy.c poly.tab.c

parser:
	bison -d poly.y
	flex poly.l
	g++ -std=c++11 -o poly_parser lex.yy.c poly.tab.c 

# Test commands.
test:
	./vor2d ./tests/test_input &

test_conics:
	./vor2d conics/ellipse &
	./vor2d conics/parabola &
	./vor2d conics/hyperbola &

rand_w_10:
	python gen_points.py 10 -w
	./vor2d output_w_10 &

rand_w_10_geps:
	python gen_points.py 10 -w
	./vor2d output_w_10 --geps=0.001 &

rand_w_100:
	python gen_points.py 100 -w
	./vor2d output_w_100 &

rand_w_100_geps:
	python gen_points.py 100 -w
	./vor2d output_w_100 --geps=0.001 &

valgrind:
	valgrind ./vor2d test_input

poly_an:
	./vor2d poly_an_test_input &

an:
	./vor2d an_test_input &

box_test:
	./vor2d c_ellipse3
