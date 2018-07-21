#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
	
	double roll = atof(argv[1])*M_PI/180.0;
	double pitch = atof(argv[2])*M_PI/180.0;
	double yaw = atof(argv[3])*M_PI/180.0;

	double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    
    double qw = cy * cr * cp + sy * sr * sp;
    double qx = cy * sr * cp - sy * cr * sp;
    double qy = cy * cr * sp + sy * sr * cp;
    double qz = sy * cr * cp - cy * sr * sp;
    
	double qw2 = sqrt(1.0 - qw * qw);
    fprintf(stderr, "%f %f %f %f\n", qx/qw2, qy/qw2, qz/qw2, 2.0*acos(qw));

	return 0;
}