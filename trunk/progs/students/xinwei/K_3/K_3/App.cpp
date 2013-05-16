#include<stdio.h>
#include<fstream>
#include<iostream>
#include "Draw.cpp"

using namespace std;

class App {
	struct Coord {
		float x, y, z;
	};
public:
	Coord coord[1000];

	int counter;

	App();
};

App::App()  {

	counter = 0;
	
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;
	
	std::ifstream iFile("input.txt");
	while (iFile>>x1>>y1>>z1>>x2>>y2>>z2>>x3>>y3>>z3) {
		coord[counter * 3 + 1].x = x1;
		coord[counter * 3 + 2].x = x2;
		coord[counter * 3 + 3].x = x3;
		coord[counter * 3 + 1].y = y1;
		coord[counter * 3 + 2].y = y2;
		coord[counter * 3 + 3].y = y3;
		coord[counter * 3 + 1].z = z1;
		coord[counter * 3 + 2].z = z2;
		coord[counter * 3 + 3].z = z3;
	}
	
}

void t_main(int argc, char *argv[]) {

	App app;

}