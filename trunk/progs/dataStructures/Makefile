EXE_NAME=smooth_quadtree_test

build:
	g++ -std=c++11 -g smooth_quadtree_test.cc -o $(EXE_NAME)
test:
	./$(EXE_NAME)
valgrind:
	valgrind ./$(EXE_NAME) -v --leak-check=full --track-origins=yes
clean:
	rm -f *~ *.o $(EXE_NAME)