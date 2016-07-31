#ifndef MAIN_H_
#define MAIN_H_


#include <QDir>
#include <QApplication>
#include <QSurfaceFormat>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#include "utils/Timer.h"
#include "MainWindow.h"
#include "polygon.h"
#include "intersection.h"

struct CFG
{
    //all these values are between -1 and 1
    double x;
    double y;
    double t1;	//theta1
    double t2;	//theta2
};

#endif /* MAIN_H_ */
