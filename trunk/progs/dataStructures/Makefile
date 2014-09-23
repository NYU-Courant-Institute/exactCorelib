build:
	g++ -std=c++11 -g smooth_quadtree_test.cc -o quadtree
test:
	./quadtree
valgrind:
	valgrind ./quadtree --leak-check=full --track-origins=yes
clean:
	rm -f *~ *.o quadtree