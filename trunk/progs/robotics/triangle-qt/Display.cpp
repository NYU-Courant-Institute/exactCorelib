//==============================================================================
// Display.cpp
// =================
// This class is the widget on ControlWindow that renders the scene.
// Display generates and paints boxes, walls, and circles using OpenGL
// to allow the user to get a full understanding of the program's output.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-08-02
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

// Header
#include "Display.h"

#include <ctime>

// Custom
#include "ShaderException.h"

// Standard Library
#include <iostream>
#include <math.h>


// Global Variables in triangle-qt.cpp
extern QuadTree* QT;
extern std::string fileName;
//extern std::string inputDir;
extern double alpha[3];
extern double beta[3];
extern double epsilon;
extern double triRobo[2];
extern double R0;
extern int QType;
extern int seed;
extern double boxWidth;
extern double boxHeight;
extern Box* boxA;
extern Box* boxB;

extern bool noPath;
extern bool hideBoxBoundary;

extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;

extern int animationSpeed;


extern vector<Box*> PATH;
extern std::vector<int> expansions;

int inc(0);

// color coding variable ========================================
extern double clr_totalFREE[3];
extern double clr_partialFREE[3];
extern double clr_MIXED[3];
extern double clr_STUCK[3];
extern double clr_EPS[3];
extern double clr_UNKNOWN[3];
extern double clr_start[3];
extern double clr_goal[3];
extern double clr_path[3];
extern double clr_robot[3];
extern double clr_obstacle[3];
extern double clr_boundary[3];

// Global Variables in MainWindow.cpp


// Member Variables
static int numQuads = 0;
static bool regenShapes = true;
// Stores Vertices of Shapes
static std::vector<double>  hollowCircles;
static std::vector<double>  filledCircles;
static std::vector<double>  lines;
static std::vector<double>  quads;
static std::vector<double>  quadOutlines;
static std::vector<double>  pathLineVertices;



int renderSteps(1);
bool step(false);



#include <unistd.h>

int pSize;
int lim;


/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent), drawable(true)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
Display::~Display() {
    makeCurrent();
    delete program;
    // Pointer to Control Window is not
    //   handled by Display
    doneCurrent();
}


/*
 * NEW SCENE
 *
 * Set 'reGenShapes' to true
 * Improves performance by preventing
 * objects in scene from being regenerated
 * every time paintGL is called
 */
void Display::genScene() {
    regenShapes = true;
}

/*
 * INITIALIZE GL
 *
 * Called once to prepare display window for
 * rendering.
 * Creates and initializes shader program
 */
void Display::initializeGL() {
   initializeOpenGLFunctions();

    // Set background color
    glClearColor(0.3, 0.3, 0.3, 1.0);

    program = new shader("simple.vert", "simple.frag");

    try {
        program->init();
    } catch (ShaderException& exc) {
        *window << "\n\n\n\n\n\n\n\n\n"
                "**************************************************\n"
                "ERROR!!\n\n" << exc.what() << "\n\n"
                "**************************************************\n"
                "\n\n\n\n";
        drawable = false;
    }
}

/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
double r0(3);
void Display::paintGL() {

    if (!drawable) return;

    if (regenShapes) {

        /*******************
         *     Cleanup     */

        glBindVertexArray(0);

        quads.clear();
        quadOutlines.clear();
        filledCircles.clear();
        hollowCircles.clear();
        lines.clear();
        pathLineVertices.clear();

        numQuads = 0;


        /*******************
         * Generate Shapes */

        // Quads and Quad Outlines
        putQuads();

        // If a Path Exists, Generate the Line Representing It
        if (!noPath) {
            //Graph graph;
            //PATH = graph.dijkstraShortestPath(boxA, boxB);
            genPath(PATH, alpha, beta, R0);
        }

        // Circle Outlines Representing Radius
        //genHollowCircle(R0*1.5, alpha[0], alpha[1], 0.3, 0.3, 0.8);     // start
        //genHollowCircle(R0*1.5, beta[0], beta[1], 0.8, 0.3, 0.3);       // goal

        // Lines Representing Obsticals
        genWalls(QT->pRoot);

        // Line From Start (alpha) to End (beta)
        //genLine(1.6, alpha[0], alpha[1], beta[0], beta[1], (noPath ? 0.0 : 1.0), 0.5, 0.75, false);

        regenShapes = false;
    }

    if (replayAnim) {
        replayAnim = false;
        inc=0;
    }

    Box* tempA = new Box(alpha[0],alpha[1],1,1);
    genRobot(tempA, alpha[0], alpha[1], alpha[2], clr_start[0], clr_start[1], clr_start[2]);
    Box* tempB = new Box(beta[0],beta[1],1,1);
    genRobot(tempB, beta[0], beta[1], beta[2], clr_goal[0], clr_goal[1], clr_goal[2]);
    if(showAnim){
        usleep((99-animationSpeed)*5000);
        if (pauseAnim) {
            genRobot(PATH.at(inc), clr_robot[0], clr_robot[1], clr_robot[2]);
            genScene();
            update();
        }
        else {
            if(inc<PATH.size()){            //each regen, draw robot at next step in path
                genRobot(PATH.at(inc++), clr_robot[0], clr_robot[1], clr_robot[2]);
                genScene();
                update();
            } else if(inc==PATH.size()){   //draw robot in final box
                    genRobot(boxB, clr_robot[0], clr_robot[1], clr_robot[2]);
                    genScene();
                    update();
                    drawCircles();
                    inc++;
            } else if(inc==PATH.size()+1){     //draw robot at end position
                update();
                showAnim=false;
                inc=0;
            }
        }
        //usleep((99-animationSpeed)*5000);
    }


    /*******************
     *   Draw Shapes   */

    // Set Background Color and Prep for Drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->use();

    // Green, Yellow, Red, & Grey Squares
    drawQuads();

    // Line from Start and End Points &
    // Obstical Walls
    drawLines();

    // Path Line
    drawPath();
    drawCircles();
}

void Display::genRobot(Box* b, double red, double green, double blue){

    // 1st Corner: lower left
    quads.push_back(R0*cos((b->xi[0])*PI) + b->x);
    quads.push_back(R0*sin((b->xi[0])*PI) + b->y);
    setVertexColor(quads, red, green, blue);

    // 2nd Corner: lower right
    quads.push_back(R0*cos((triRobo[0] + b->xi[0])*PI) + b->x);
    quads.push_back(R0*sin((triRobo[0] + b->xi[0])*PI) + b->y);
    setVertexColor(quads, red, green, blue);

    // 3rd Corner: upper right
    quads.push_back(R0*cos((triRobo[1] + b->xi[0])*PI) + b->x);
    quads.push_back(R0*sin((triRobo[1] + b->xi[0])*PI) + b->y );
    setVertexColor(quads, red, green, blue);

    // 4th Corner: upper left
    quads.push_back(R0*cos((b->xi[0])*PI) + b->x);
    quads.push_back(R0*sin((b->xi[0])*PI) + b->y);
    setVertexColor(quads, red, green, blue);

    // Convert vertices to normalized divece coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(quads[(quads.size()-5) - i*5], quads[(quads.size()-4) - i*5]);

    // Add vertices of the quads' outlines
    for (int j = 3; j>=0 ; j--) {
        quadOutlines.push_back(quads[(quads.size()-5) - j*5]);
        quadOutlines.push_back(quads[(quads.size()-4) - j*5]);
        setVertexColor(quadOutlines, 0.0, 0.0, 0.0);
    }
}

void Display::genRobot(Box* b, double x, double y, double theta, double red, double green, double blue){

    // 1st Corner: lower left
    quads.push_back(R0*cos((theta) * PI) + x);
    quads.push_back(R0*sin((theta) * PI) + y);
    setVertexColor(quads, red, green, blue);

    // 2nd Corner: lower right
    quads.push_back(R0*cos((triRobo[0] + theta) * PI) + x);
    quads.push_back(R0*sin((triRobo[0] + theta) * PI) + y);
    setVertexColor(quads, red, green, blue);

    // 3rd Corner: upper right
    quads.push_back(R0*cos((triRobo[1] + theta) * PI) + x);
    quads.push_back(R0*sin((triRobo[1] + theta) * PI) + y);
    setVertexColor(quads, red, green, blue);

    // 4th Corner: upper left
    quads.push_back(R0*cos((theta) * PI) + x);
    quads.push_back(R0*sin((theta) * PI) + y);
    setVertexColor(quads, red, green, blue);

    // Convert vertices to normalized divece coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(quads[(quads.size()-5) - i*5], quads[(quads.size()-4) - i*5]);

    // Add vertices of the quads' outlines
    for (int j = 3; j>=0 ; j--) {
        quadOutlines.push_back(quads[(quads.size()-5) - j*5]);
        quadOutlines.push_back(quads[(quads.size()-4) - j*5]);
        setVertexColor(quadOutlines, 0.0, 0.0, 0.0);
    }
}

void Display::putQuads(){

    Box* tmp;
    int i;
    if(step){
        for(i=0;i<Box::pAllLeaf->size()&&i<expansions.at(renderSteps);i++){
            tmp=Box::pAllLeaf->at(i);

            genQuad(tmp,epsilon);
            numQuads++;
        }

    }//NOTE: This draws ALL quads, including non leaves - this is overkill, and wastes time and space.
     //Can we do this better?
    else{
        for(i=0;i<Box::pAllLeaf->size();i++){
            tmp = Box::pAllLeaf->at(i);
            if(!tmp->isLeaf){
                continue;
            }
            genQuad(tmp,epsilon);
            numQuads++;
        }
    }
}





/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void Display::resizeGL(int width, int height) {
    //glViewport(0, 0, width, height);
}

/*
 *  SET CONTROL WINDOW
 *
 * Pass pointer that holds Display widget
 * so that Display can print information
 * to the user via the text display screen.
 */
void Display::setControlWindow(MainWindow* cw) {
    window = cw;
}


//===========================================================//
//======          Functions to Generate Shapes         ======//
//=== To generate shapes, OpenGL requires you to store    ===//
//=== the data for the vertices of each shape.            ===//
//=== I.e. the (x, y) coordinate of each vertex.          ===//
//=== However, in OpenGL, each vertex has its own color.  ===//
//=== Therefore, to draw one triangle, 15 values must be  ===//
//=== stored:                                             ===//
//=== x1 y1 r1 g1 b1 | x2 y2 r2 g2 b2 | x3 y3 r3 g3 b3    ===//
//===========================================================//
/*
 * TO NORMAL DEVICE COORDINATES
 *
 * Convert coordinates in the ranges 0 <= x <= boxWidth
 * and 0 <= y <= boxHeight to the Normalized Device
 * Coordinate Range used by OpenGL -1 <= x < 1 and
 * -1 <= y <= 1
 */
void Display::toNormalDeviceCoords(double& x, double& y) {
    x = ((2.0 * x) / boxWidth) - 1;
    y = ((2.0 * y) / boxHeight) - 1;
}

/*
 * SET VERTEX COLOR
 *
 * Inline function to easily set the color of a vertex
 * since OpenGL requires each vertex to have its own color
 */
inline void Display::setVertexColor(vector<double>& array, float r, float g, float b) {
    array.push_back(r);
    array.push_back(g);
    array.push_back(b);
}

/*
 * PERPENDICULAR VERTICES
 *
 * Used to give lines thickness.
 * The function accepts two points ('a' and 'b') and computes the vertices
 * of a rectangle, where 'a' and 'b' are the midpoints of the sides and
 * 'thickness' is the height.
 *
 *     [p1]-----[p2] \
 *      |         |   \
 *     [a]       [b]   > thickness
 *      |         |   /
 *     [p3]-----[p4] /
 *
 * 'thickness' is with respect to the 'boxWidth' and 'boxHeight' not the
 * normalized device coordinate range
 *
 * Precondition: 'a' and 'b' cannot have already been normalized
 * Postcondition: Perpendicular vertices have been added to specified
 *                vector and normalized
 *
 * This function uses vecters to find the vertices by converting 'a'
 * and 'b' into a vector, finding a perpendicular vector, giving it
 * the length of 'thickness', and adding/subtracting the values of
 * this vector to 'a' and 'b'
 */
void Display::perpVertices(float thickness, vector<double>& storage, double a_x, double a_y,
                       double b_x, double b_y, float red, float green, float blue) {
    thickness /= 2;
    double x = a_x - b_x;
    double y = a_y - b_y;
    //(*controlWin) << "\nSTAGE 1: x= " << x << ",  y= " << y << "\n";
    //double tempx = x, tempy = y;

    // Used to prevent overflow error
    float essentuallyZero = 0.001;

    if ((-essentuallyZero < x && x < essentuallyZero) &&
        (-essentuallyZero < y && y < essentuallyZero)) {
        x = 0; y = 0;
    } else if (-essentuallyZero < x && x < essentuallyZero) {
        x = 1 * thickness;
        y = 0;
    } else if (-essentuallyZero < y && y < essentuallyZero) {
        y = 1 * thickness;
        x = 0;
    } else {
        x = 1/x;
        y = 1/y;

        double temp_x = x, temp_y = y;
        x = thickness * (temp_x / sqrt(pow(temp_x, 2) + pow(temp_y, 2)));
        y = thickness * (temp_y / sqrt(pow(temp_x, 2) + pow(temp_y, 2)));
    }
    //(*controlWin) << "STAGE 2: x= " << x << ",  y= " << y << "\n";
    //(*controlWin) << "DOT PRODUCT= " << x * tempx - y * tempy << "\n\n";

    // Save vertices to 'storage'
    storage.push_back(a_x + x);
    storage.push_back(a_y - y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(a_x - x);
    storage.push_back(a_y + y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(b_x + x);
    storage.push_back(b_y - y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(b_x - x);
    storage.push_back(b_y + y);
    setVertexColor(storage, red, green, blue);

    // Set previous 4 vertices to normal device coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(storage[(storage.size()-5) - i*5],
                             storage[(storage.size()-4) - i*5]);
}

/*
 * GENERATE PATH
 *
 * Generates the shapes that constitute the path, a smooth line from
 * 'alpha' to 'beta' avoiding all obsticals
 */
void Display::genPath(vector<Box*>& path, double* alpha, double* beta, double R0)
{
    double red = clr_path[0], green = clr_path[1], blue = clr_path[2];
    float thickness = R0 < 1.5/2 ? 2*R0 : 1.5;

    vector<double> vertexHolder;

    vertexHolder.push_back(alpha[0]);        // beta x
    vertexHolder.push_back(alpha[1]);        // beta y

    for (int i = 0; i < path.size(); i++) {
        vertexHolder.push_back(path[i]->x);
        vertexHolder.push_back(path[i]->y);
    }

    vertexHolder.push_back(beta[0]);       // alpha x
    vertexHolder.push_back(beta[1]);       // alpha y

    // Draw a filled circle at each point (excluding the first and last)
    // in order to smooth out where two lines intersect
    for (int q = 1; q < vertexHolder.size()/2-1; q++)
        genFilledCircle(thickness/2, vertexHolder[2*q], vertexHolder[2*q+1],
                        red, green, blue);

    // Generate vertices for path line
    for (int n = 0; n < vertexHolder.size() / 2 - 1; n++)
        perpVertices(thickness, pathLineVertices,
                   vertexHolder[(n)*2], vertexHolder[(n)*2+1],
                   vertexHolder[(n+1)*2], vertexHolder[(n+1)*2+1],
                   red, green, blue);


    /*******************************************************************************************
     * // Print out origional 2 points and perpendicular vertices produces
     * (*controlWin) << "\t(" << vertexHolder[n*2] << ", " << vertexHolder[n*2+1] << ")\n";
     * (*controlWin) << "x= " << x << "   y= " << y << "\n";
     * (*controlWin) << "(" << (vertexHolder[n*2]+x) << ", " << (vertexHolder[n*2+1]-y) << ")\n";
     * (*controlWin) << "(" << (vertexHolder[n*2]-x) << ", " << (vertexHolder[n*2+1]+y) << ")\n";
     *
     * (*controlWin) << "\t(" << vertexHolder[(n+1)*2] << ", " << vertexHolder[(n+1)*2+1] << ")\n";
     * (*controlWin) << "(" << (vertexHolder[(n+1)*2]+x) << ", " << (vertexHolder[(n+1)*2+1]-y) << ")\n";
     * (*controlWin) << "(" << (vertexHolder[(n+1)*2]-x) << ", " << (vertexHolder[(n+1)*2+1]+y) << ")\n\n\n";
     *******************************************************************************************/
}

/*
 * GENERATE LINES
 *
 * Uses to draw obsticals, this function generates the shapes necessary
 * to create a line (with thickness)
 */
void Display::genLine(float thickness, double a_x, double a_y, double b_x, double b_y,
             float red, float green, float blue, bool smoothCorners)
{
    // Draw a filled circle on both sides to smooth out
    // where two lines intersect
    if (smoothCorners) {
        genFilledCircle(thickness/2, a_x, a_y, red, green, blue);
        genFilledCircle(thickness/2, b_x, b_y, red, green, blue);
    }

    // Calculate perpendicular vertices to give line thickness
    perpVertices(thickness, lines, a_x, a_y, b_x, b_y, red, green, blue);
}

/*
 * GENERATE QUADRILATERAL
 *
 * Generates a box and with the color of its status and
 * a black outline around the box
 */
void Display::genQuad(Box* b, double epsilon)
{

    // Get color based on Box's Status
    double red = clr_UNKNOWN[0], green = clr_UNKNOWN[1], blue = clr_UNKNOWN[2];

    switch(b->status)
    {
        case Box::FREE:
            // color is dark green, representing completed free angular ranges for both links
            // Note: the angular range of link i is the full circle iff LOWERi=0 and UPPERi=360 (i=1 or 2).
            if (b->xi[Box::LOWER] != 0 || b->xi[Box::UPPER] != 360) {
                red = clr_totalFREE[0];
                green = clr_totalFREE[1];
                blue = clr_totalFREE[2];
            }
            // green
            else {
                red = clr_partialFREE[0];
                green = clr_partialFREE[1];
                blue = clr_partialFREE[2];
            }
            break;
        case Box::STUCK:  //color is red
            red = clr_STUCK[0];
            green = clr_STUCK[1];
            blue = clr_STUCK[2];
            break;
        case Box::MIXED: //color is yellow (if box is epsilon-large)
            red = clr_MIXED[0];
            green = clr_MIXED[1];
            blue = clr_MIXED[2];
            if (b->height < epsilon || b->width < epsilon) { // color is gray (if box is epsilon-small)
                red = clr_EPS[0];
                green = clr_EPS[1];
                blue = clr_EPS[2];
            }
            break;
        case Box::UNKNOWN:
            //* << "UNKNOWN box status in genQuad\n";
        break;
    }

    // 1st Corner: lower left
    quads.push_back(b->x - b->width / 2);
    quads.push_back(b->y - b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 2nd Corner: lower right
    quads.push_back(b->x + b->width / 2);
    quads.push_back(b->y - b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 3rd Corner: upper right
    quads.push_back(b->x + b->width / 2);
    quads.push_back(b->y + b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 4th Corner: upper left
    quads.push_back(b->x - b->width / 2);
    quads.push_back(b->y + b->height / 2);
    setVertexColor(quads, red, green, blue);

    // Convert vertices to normalized divece coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(quads[(quads.size()-5) - i*5], quads[(quads.size()-4) - i*5]);

    // Add vertices of the quads' outlines
    for (int j = 3; j>=0 ; j--) {
        quadOutlines.push_back(quads[(quads.size()-5) - j*5]);
        quadOutlines.push_back(quads[(quads.size()-4) - j*5]);
        setVertexColor(quadOutlines, 0.0, 0.0, 0.0);
    }
}

/*
 * GENERATE WALLS
 *
 * Generate white lines representing obsticals
 */
void Display::genWalls(Box* b)
{
    // With all values set to 1.0, the color is white
    double red = clr_obstacle[0], green = clr_obstacle[1], blue = clr_obstacle[2];

    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
    {
        Wall* w = *iter;
        genLine(1.75, w->src->x, w->src->y, w->dst->x, w->dst->y, red, green, blue);
    }
}


void Display::genHollowCircle(double radius, double x, double y,
                     float red, float green, float blue)
{
    // Adjust radius to normalized device coordinates
    toNormalDeviceCoords(x, y);
    double radius_x = radius / (boxWidth/2.0);
    double radius_y = radius / (boxHeight/2.0);

    // Set manually to prevent floating point
    // error -> missing slice of circle
    hollowCircles.push_back(x + radius_x);    // theta = 0
    hollowCircles.push_back(y);
    setVertexColor(hollowCircles, red, green, blue);

    for (int div = 1; div < hollowCircleResolution; ++div) {
        double theta = div * (2 * M_PI) / hollowCircleResolution;

        hollowCircles.push_back(cos(theta) * radius_x + x);
        hollowCircles.push_back(sin(theta) * radius_y + y);
        setVertexColor(hollowCircles, red, green, blue);
    }

    hollowCircles.push_back(x + radius_x);    // theta = 2*pi
    hollowCircles.push_back(y);
    setVertexColor(hollowCircles, red, green, blue);
}

/*
 * GENERATE FILLED CIRCLE
 *
 * Generates a filled circle with a given radius and centered
 * at ('x', 'y')
 *
 * A perfectly smooth circle cannot be drawn, so a series of
 * lines must be used instead. The smoothness (number of
 * vertices) is determined by 'filledCircleResolution'
 */
void Display::genFilledCircle(double radius, double x, double y, float red, float green, float blue)
{
    // Convert to Normal Device Coordinates [-1.0, 1.0]
    toNormalDeviceCoords(x, y);
    double radius_x = radius / (boxWidth/2.0);
    double radius_y = radius / (boxHeight/2.0);


    // Set Vertices
    filledCircles.push_back(x);               // Center x
    filledCircles.push_back(y);               // Center y
    setVertexColor(filledCircles, red, green, blue);

    // Set manually to prevent floating point
    // error => incomplete circle
    filledCircles.push_back(x + radius_x);    // theta = 0
    filledCircles.push_back(y);
    setVertexColor(filledCircles, red, green, blue);

    for (int div = 1; div < filledCircleResolution; ++div) {
        double theta = div * (2 * M_PI) / filledCircleResolution;

        filledCircles.push_back(cos(theta) * radius_x + x);
        filledCircles.push_back(sin(theta) * radius_y + y);
        setVertexColor(filledCircles, red, green, blue);
    }

    filledCircles.push_back(x + radius_x);    // theta = 2*pi
    filledCircles.push_back(y);
    setVertexColor(filledCircles, red, green, blue);
}


//===========================================================//
//======            Functions to Draw Shapes           ======//
//=== To draw shapes, OpenGL requires the vertex data of  ===//
//=== each shape, the number of vertices, the way         ===//
//=== this data will be stored on the GPU, how it will    ===//
//=== be drawn to the screen, and much more.              ===//
//=== See http://learnopengl.com to get a better          ===//
//=== understanding of this section of code.              ===//
//===========================================================//




/*
 * DRAW LINES
 *
 * Uses vertices stored in 'lines' vector
 * to draw lines to the screen.
 */
void Display::drawLines() {
    if (!lines.size()) return;

    // Array of all vertices
    // Currently, values in vectors are stored in
    // contiguous memory slots
    double* coord_color = &lines[0];

    // Set Up
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, lines.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Draw Setup
    int numLines = lines.size()/20;
    int* count = new int[numLines];
    for (int i = 0; i < numLines; i++)
        count[i] = 4;
    int* indexOfFirstVert = new int[numLines];
    for (int j = 0; j < numLines; j++)
        indexOfFirstVert[j] = j * 4;

    // Draw!!
    /*
     * Note: look up 'GL_TRIANGLE_STRIP' to get a
     * better understanding of how lines are given
     * thickness.
     */
    glMultiDrawArrays(GL_TRIANGLE_STRIP, indexOfFirstVert, count, numLines);

    // Clean up
    delete[] count;
    delete[] indexOfFirstVert;
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * DRAW QUADRILATERALS
 *
 * Use the vertices stored in 'quads' vector
 * to draw quadrilaterals to the screen.
 * If 'hideBoxBoundary' == false, the outlines
 * stored in 'quadOutlines' vector are drawn.
 */
void Display::drawQuads() {
    if (!quads.size()) return;

    //== Draw Solid Boxes ==//

    // Array of all vertices
    double* coord_color = &quads[0];

    /*
     * A quadrilateral can't be made outright;
     * two triangles must be drawn instead.
     * For the vertices shared by the triangles,
     * we could list them in 'quads' multiple times
     * or we can use indices.
     *
     * What is happening in the following loop:
     *  Each 5 values in 'quads' correlates with one vertex
     *    (2 coordinate values & 3 color values).
     *  Each 20 values in 'quads' correlates with 1 quadrilateral.
     *  Every 4 vertices (20 values) are used to create
     *    2 triangles, which has a total of 6 vertices.
     *  Therefore, 6 indices are needed for every 4 vertices
     *  in 'quads'.
     */
    int* indices = new int[quads.size()/20 * 6];
    for (int i = 0; i < quads.size()/20; i++) {
        indices[i*6]     = i*4 + 1;
        indices[i*6 + 1] = i*4 + 3;
        indices[i*6 + 2] = i*4 + 0;
        indices[i*6 + 3] = i*4 + 1;
        indices[i*6 + 4] = i*4 + 2;
        indices[i*6 + 5] = i*4 + 3;
    }

    // Set Up
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, quads.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Transfer more data from CPU to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quads.size()/20*6*sizeof(GLint), indices, GL_STATIC_DRAW);

    // Draw!
    glDrawElements(GL_TRIANGLES, quads.size()/20*6, GL_UNSIGNED_INT, (GLvoid*)0);

    // Cleanup
    glDeleteBuffers(1, &ebo);
    delete[] indices;


    //== Draw Box Outlines ==//
    if (!hideBoxBoundary) {

        // Array of all vertices
        double* coord_color = &quadOutlines[0];

        // Transfer data from CPU to GPU
        glBufferData(GL_ARRAY_BUFFER, quadOutlines.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

        // Draw Setup
        int* count = new int[numQuads];
        for (int i = 0; i < numQuads; i++)
            count[i] = 4;
        int* indexOfFirstVert = new int[numQuads];
        for (int j = 0; j < numQuads; j++)
            indexOfFirstVert[j] = 4*j;

        // Draw!!
        glMultiDrawArrays(GL_LINE_LOOP, indexOfFirstVert, count, numQuads);

        // Cleanup
        delete[] count;
        delete[] indexOfFirstVert;
    }

    // More Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * DRAW CIRLCES
 *
 * Uses the vertices stored in 'filledCircles'
 * vector to draw solid circles and 'hollowCircles'
 * vector to draw circle outlines to screen.
 */
void Display::drawCircles() {

    //== Solid Circles ==//

    if (filledCircles.size()) {

        // Array of all vertices
        double* coord_color = &filledCircles[0];

        // Generate VAO
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Transfer data from CPU to GPU
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, filledCircles.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

        // Tell OpenGL where in the list of values
        // the coordinate values are stored
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                              (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Tell OpenGL where in the list of values
        // the color values are stored
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                              (GLvoid*)(2 * sizeof(GLdouble)));
        glEnableVertexAttribArray(1);

        // Draw Setup
        int numCircles = filledCircles.size() / (5 * (filledCircleResolution + 2));

        int* count = new int[numCircles];
        for (int i = 0; i < numCircles; i++)
            count[i] = filledCircleResolution+2;

        int* indexOfFirstVert = new int[numCircles];
        for (int j = 0; j < numCircles; j++)
            indexOfFirstVert[j] = j * (filledCircleResolution+2);

        // Draw!!
        /*
         * Note: look up 'GL_TRIANGLE_FAN' to get a
         * better understanding of how solid circles
         * are drawn
         */
        glMultiDrawArrays(GL_TRIANGLE_FAN, indexOfFirstVert, count,
                          numCircles);

        // Clean up
        delete[] count;
        delete[] indexOfFirstVert;
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }


    //== Hollow Circles ==//

    if (hollowCircles.size()) {
        // Array of all vertices
        double* coord_color = &hollowCircles[0];

        // Generate VAO
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Transfer data from CPU to GPU
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, hollowCircles.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

        // Tell OpenGL where in the list of values
        // the coordinate values are stored
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                              (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Tell OpenGL where in the list of values
        // the color values are stored
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                              (GLvoid*)(2 * sizeof(GLdouble)));
        glEnableVertexAttribArray(1);

        // Draw Setup
        int numCircles = hollowCircles.size() / (5 * (hollowCircleResolution + 1));
        int* count = new int[numCircles];
        for (int i = 0; i < numCircles; i++)
            count[i] = hollowCircleResolution+1;

        int* indexOfFirstVert = new int[numCircles];
        for (int j = 0; j < numCircles; j++)
            indexOfFirstVert[j] = j * (1 + hollowCircleResolution);

        // Draw!!
        /*
         * Note: look up 'GL_LINE_LOOP' to get a
         * better understanding of how hollow circles
         * are drawn
         */
        glMultiDrawArrays(GL_LINE_LOOP, indexOfFirstVert, count, numCircles);

        // Clean up
        delete[] count;
        delete[] indexOfFirstVert;
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}

/*
 * DRAW PATH
 *
 * Uses the vertices stored in 'pathLineVertices'
 * vector to draw path lines to screen.
 */
void Display::drawPath() {
    if (!pathLineVertices.size()) return;

    // Array of all vertices
    double* coord_color = &pathLineVertices[0];

    // Set Up
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pathLineVertices.size() * sizeof(GLdouble),
                 coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Draw Setup
    int* count = new int[pathLineVertices.size() / 20];
    for (int i = 0; i < pathLineVertices.size()/20; i++)
        count[i] = 4;

    int* indexOfFirstVert = new int[pathLineVertices.size() / 20];
    for (int j = 0; j < pathLineVertices.size()/20; j++)
        indexOfFirstVert[j] = 4*j;

    // Draw!!!!!
    /*
     * Note: look up 'GL_TRIANGLE_LOOP' to get a
     * better understanding of how the path is drawn
     */
    glMultiDrawArrays(GL_TRIANGLE_STRIP, indexOfFirstVert, count, pathLineVertices.size()/20);

    // Clean up
    delete[] count;
    delete[] indexOfFirstVert;
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
