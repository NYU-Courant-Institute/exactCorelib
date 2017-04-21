#define mw_out std::cout
#include "complex.h"

int main(int argc, char* argv[]) {

    if (argc >1) interactive = atoi(argv[1]);//interactive or no
    if (argc >2) cfgName = argv[2];		//config file
    if (argc >3) fileName = argv[3];	//enviroment file
    if (argc >4) inputDir = argv[4];	//Path for input files
    if (argc >5) robotDir = argv[5];	//Path for robot files
    if (argc >6) robotName = argv[6];	//robot 
    if (argc >7) alpha[0] = atof(argv[7]);	// startx	
    if (argc >8) alpha[1] = atof(argv[8]);	// starty
    if (argc >9) alpha[2] = atof(argv[9]);	// startTheta
    if (argc >10) beta[0] = atof(argv[10]);	// goalx
    if (argc >11) beta[1] = atof(argv[11]);	// goaly
    if (argc >12) beta[2] = atof(argv[12]);	// goalTheta
    if (argc >13) epsilon = atof(argv[13]);	// resolution parameter
    if (argc >14) R0 	= atof(argv[14]);	// robot radius
    if (argc > 15) boxWidth = atof(argv[15]);		// boxWidth
    if (argc > 16) boxHeight = atof(argv[16]);	// boxHeight
    if (argc > 17) windowPosX = atoi(argv[17]);	// window X pos
    if (argc > 18) windowPosY = atoi(argv[18]);	// window Y pos
    if (argc > 19) QType   = atoi(argv[19]); // PriorityQ Type(random or no)
    if (argc > 20) seed   = atoi(argv[20]);	 // for random number generator
    if (argc > 21) deltaX  = atof(argv[21]); // x-translation of input file
    if (argc > 22) deltaY  = atof(argv[22]); // y-translation of input file
    if (argc > 23) scale  = atof(argv[23]);		// scaling of input file


    if (interactive > 0) {	// non-interactive
        // do something...
        mw_out << "Non Interactive Run of Complex Robot" ;
    }

    srand(seed);
    run();

    if (interactive > 0) {	// non-interactive
        if (noPath) mw_out << "No Path Found!\n";
        else        mw_out << "Path was Found!\n" ;
        return 0;
    }

    return 0;
}
