CXX = g++-5
CXXFLAGS = -std=c++11 -g

TARGET = subvor
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
DEPS = $(SRCS:.cpp=.depends)

INCLUDE=-lpng -lboost_program_options -L/usr/lib -lglut -lglui -lGLU -lGL

# Build commands.
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(INCLUDE) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o $(TARGET)

# Test commands.
test:
	./vor2d ./tests/test_input &

conics:
	./vor2d conics/ellipse &
	./vor2d conics/parabola &
	./vor2d conics/hyperbola &

conics_highres:
	./vor2d conics/ellipse --geps=0.001 &
	./vor2d conics/parabola --geps=0.001 &
	./vor2d conics/hyperbola --geps=0.001 &

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
