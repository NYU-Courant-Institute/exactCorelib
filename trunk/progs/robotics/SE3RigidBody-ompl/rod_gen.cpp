#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

int main(int argc, char* argv[]){
	char filename[50];
	double length = atoi(argv[1]);
	sprintf(filename, "rod_%d.raw", static_cast<int>(length));
	FILE* fptr = fopen(filename, "w");

	// pillar
	std::vector<double> ax, ay, az;
	std::vector<double> bx, by, bz;
	for (unsigned i=0;i<=360;++i) {
		double angle = i;
		angle = angle/180.0*M_PI;
		double x = 1.0-cos(angle), y = sin(angle), z = 0;
		ax.push_back(x);
		ay.push_back(y);
		az.push_back(z);
		bx.push_back(x);
		by.push_back(y);
		bz.push_back(z+length);
	}
	// red color
	for(unsigned i=0;i<ax.size()-1;++i){
		fprintf(fptr, "1 0 0 %f %f %f %f %f %f %f %f %f\n", 0.0, 0.0, 0.0, ax[i], ay[i], 0.0, ax[i+1], ay[i+1], 0.0);
	}
	for(unsigned i=0;i<ax.size()-1;++i){
		fprintf(fptr, "1 0 0 %f %f %f %f %f %f %f %f %f\n", 0.0, 0.0, 0.0, bx[i], by[i], length, bx[i+1], by[i+1], length);
	}
	for(unsigned i=0;i<ax.size()-1;++i){
		fprintf(fptr, "1 0 0 %f %f %f %f %f %f %f %f %f\n", ax[i], ay[i], 0.0, ax[i+1], ay[i+1], 0.0, bx[i], by[i], length);
		fprintf(fptr, "1 0 0 %f %f %f %f %f %f %f %f %f\n", ax[i+1], ay[i+1], 0.0, bx[i], by[i], length, bx[i+1], by[i+1], length);
	}

	fclose(fptr);

	return 0;
}
