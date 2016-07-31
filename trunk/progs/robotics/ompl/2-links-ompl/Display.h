#ifndef __disc__Display__
#define __disc__Display__

// Custom
#include "MainWindow.h"
#include "./mathtool/Vector.h"
#include "polygon.h"
#include "main.h"

// Qt
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include "glu.h"

// Standard Library
#include <stdio.h>
#include <iostream>
#include <string>

class Display : public QOpenGLWidget
{
    Q_OBJECT
public:
    Display(QWidget* parent = 0);
    virtual ~Display();


protected:
    // Essential Functions Inherited from QOpenGLWidget
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderScene();

    /*********************************
     * Functions used to paint scene *
     *********************************/
    void drawPolygons();
    void drawRobot(CFG cfg);
    void drawPath();
};

#endif /* defined(__disc__Display__) */
