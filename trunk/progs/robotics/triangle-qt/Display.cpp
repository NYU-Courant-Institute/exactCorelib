// Header
#include "Display.h"

// Standard Library
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <ctime>


// Global Variables in triangle-qt.cpp
extern QuadTree* QT;
extern std::string fileName;
extern double alpha[3];
extern double beta[3];
extern double epsilon;
extern double triRobo[2];
extern double R0;
extern int QType;
extern double boxWidth;
extern double boxHeight;
extern Box* boxA;
extern Box* boxB;
extern bool noPath;
extern bool hideBoxBoundary;
extern bool hideBox;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern int animationSpeed;
extern vector<Box*> PATH;
extern std::vector<int> expansions;
// color coding variable ========================================
extern Color clr_totalFREE;
extern Color clr_partialFREE;
extern Color clr_MIXED;
extern Color clr_STUCK;
extern Color clr_EPS;
extern Color clr_UNKNOWN;
extern Color clr_start;
extern Color clr_goal;
extern Color clr_path;
extern Color clr_robot;
extern Color clr_obstacle;
extern Color clr_boundary;



// Member Variables
static int numQuads = 0;
static bool regenShapes = true;
int inc(0);
int renderSteps(1);
bool step(false);
// Stores Vertices of Shapes
static std::vector<Vertex>  hollowCircles;
static std::vector<Vertex>  filledCircles;
static std::vector<Vertex>  lines;
static std::vector<Vertex>  quads;
static std::vector<Vertex>  quadOutlines;
static std::vector<Vertex>  pathLineVertices;






/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
Display::~Display()
{}

/*
 * INITIALIZE GL
 *
 * Called once to prepare display window for
 * rendering.
 * Creates and initializes shader program
 */
GLuint fbo;
GLuint depthBuffer;			// Our handle to the depth render buffer
GLuint img;					// Our handle to a texture
void initBuffer() {

    // Setup our FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create the render buffer for depth
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, boxWidth, boxHeight);

    // Now setup a texture to render to
    glGenTextures(1, &img);
    glBindTexture(GL_TEXTURE_2D, img);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  boxWidth, boxHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // And attach it to the FBO so we can render to it
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, img, 0);

    // Attach the depth render buffer to the FBO as it's depth attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        exit(1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);	// Unbind the FBO for now
}
void Display::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    //initBuffer();
}


/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    renderScene();
    glFlush();
}


/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void Display::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Display::renderScene() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    putBoxes();
    glDisable(GL_BLEND);

    Pose start = Pose(alpha[0], alpha[1], alpha[2]);
    Pose goal = Pose(beta[0], beta[1], beta[2]);

    if (!noPath) {
        drawPath(start, goal);

        if (replayAnim) {
            replayAnim = false;

            showAnim = true;
            inc=0;
        }

        if(showAnim){
            usleep((99-animationSpeed)*5000);
            if (pauseAnim) {
                drawRobot(PATH.at(inc), clr_robot);
                update();
            }
            else {
                if(inc<PATH.size()){            //each regen, draw robot at next step in path
                    drawRobot(PATH.at(inc++), clr_robot);
                    update();
                } else if(inc==PATH.size()){   //draw robot in final box
                        drawRobot(goal, clr_goal);
                        inc++;
                } else if(inc==PATH.size()+1){     //draw robot at end position
                    showAnim=false;
                    inc=0;
                }
            }
        }
    }

    drawRobot(start, clr_start);
    drawRobot(goal, clr_goal);

    drawObstacles();
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//
void Display::putBoxes(){
    if(step){
        for(int i=0;i<Box::pAllLeaf->size()&&i<expansions.at(renderSteps);i++){
            Box* tmp=Box::pAllLeaf->at(i);
            drawBoxes(tmp, epsilon);
            numQuads++;
        }
    }
    //NOTE: This draws ALL quads, including non leaves - this is overkill, and wastes time and space.
    //Can we do this better?
    else{
        //for(int i=Box::pAllLeaf->size()-1;i>=0;--i){
        for(int i=0;i<Box::pAllLeaf->size();i++){
            Box* tmp = Box::pAllLeaf->at(i);
            if(!tmp->isLeaf) continue;
            drawBoxes(tmp, epsilon);
            numQuads++;
        }
    }
}
/*
 * DRAW Boxes(Quads)
 */
void Display::drawBoxes(Box* b, double epsilon) {
    if(hideBox) return ;

    Color clr_box = clr_MIXED;

    switch(b->status) {
        case Box::FREE:
            // color is dark green, representing completed free angular ranges for both links
            // Note: the angular range of link i is the full circle iff LOWERi=0 and UPPERi=360 (i=1 or 2).
            if (b->xi[Box::LOWER] != 0 || b->xi[Box::UPPER] != 360) {
                clr_box = clr_totalFREE;
            }
            // green
            else {
                clr_box = clr_partialFREE;
            }
            break;
        case Box::STUCK:  //color is red
            clr_box = clr_STUCK;
            break;
        case Box::MIXED: //color is yellow (if box is epsilon-large)
            if (b->height < epsilon || b->width < epsilon) { // color is gray (if box is epsilon-small)
                clr_box = clr_EPS;
            }
            break;
        case Box::UNKNOWN:
            //* << "UNKNOWN box status in genQuad\n";
        break;
    }
    // 1st Corner: lower left
    Pose lower_left = Pose(b->x - b->width / 2, b->y - b->height / 2);
    // 2nd Corner: lower right
    Pose lower_right = Pose(b->x + b->width / 2, b->y - b->height / 2);
    // 3rd Corner: upper right
    Pose upper_right = Pose(b->x + b->width / 2, b->y + b->height / 2);
    // 4th Corner: upper left
    Pose upper_left = Pose(Pose(b->x - b->width / 2, b->y + b->height / 2));

    glColor4fv(clr_box.rgba);
    glBegin(GL_POLYGON);
        glVertex2f( lower_left.x,  lower_left.y );
        glVertex2f( lower_right.x, lower_right.y );
        glVertex2f( upper_right.x, upper_right.y );
        glVertex2f( upper_left.x,  upper_left.y );
        glVertex2f( lower_left.x,  lower_left.y );
    glEnd();

    // 1 2 3 4
    if (!hideBoxBoundary) {
        glColor4fv(Color(0,0,0,1).rgba);
        glLineWidth(2.0);
        glBegin(GL_LINE_STRIP);
            glVertex2f( lower_left.x,  lower_left.y );
            glVertex2f( lower_right.x, lower_right.y );
            glVertex2f( upper_right.x, upper_right.y );
            glVertex2f( upper_left.x,  upper_left.y );
            glVertex2f( lower_left.x,  lower_left.y );
        glEnd();
        glLineWidth(1.0);
    }
}

/*
 * DRAW Obstacles
 */
void Display::drawObstacles() {
    Box* b = QT->pRoot;
    glLineWidth(2.0);
    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter) {
        Wall* w = *iter;
        glColor4fv(clr_obstacle.rgba);
        glBegin(GL_LINE_STRIP);
            glVertex2f(w->src->x, w->src->y);
            glVertex2f(w->dst->x, w->dst->y);
        glEnd();
    }
    glLineWidth(1.0);
}

/*
 * DRAW PATH
 */
void Display::drawPath(Pose start, Pose goal) {
    glColor4fv(clr_path.rgba);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(start.x, start.y);
        for (unsigned int i = 0; i < PATH.size(); ++i) {
            glVertex2f(PATH[i]->x, PATH[i]->y);
        }
        glVertex2f(goal.x, goal.y);
    glEnd();
    glLineWidth(1.0);
}

void Display::drawRobot(Box* b, Color c){
    drawTriangle(Pose(R0*cos((b->xi[0])*PI)+b->x, R0*sin((b->xi[0])*PI)+b->y),
                 Pose(R0*cos((triRobo[0]+b->xi[0])*PI)+b->x, R0*sin((triRobo[0]+b->xi[0])*PI)+b->y),
                 Pose(R0*cos((triRobo[1]+b->xi[0])*PI)+b->x, R0*sin((triRobo[1]+b->xi[0])*PI)+b->y),
                 c, true, true);
}

void Display::drawRobot(Pose p, Color c){
    drawTriangle(Pose(R0*cos((p.theta)*PI)+p.x, R0*sin((p.theta)*PI)+p.y),
                 Pose(R0*cos((triRobo[0]+p.theta)*PI)+p.x, R0*sin((triRobo[0]+p.theta)*PI)+p.y),
                 Pose(R0*cos((triRobo[1]+p.theta)*PI)+p.x, R0*sin((triRobo[1]+p.theta)*PI)+p.y),
                 c, false, true);
}

void Display::drawTriangle(Pose a, Pose b, Pose c, Color clr, bool draw_solid, bool draw_outline) {
    if (draw_solid) {
        glColor4fv(clr.rgba);
        glBegin(GL_TRIANGLES);
            glVertex2f( a.x, a.y );
            glVertex2f( b.x, b.y );
            glVertex2f( c.x, c.y );
        glEnd();
    }
    if (draw_outline) {
        glColor4fv(Color(0,0,0,1).rgba);
        glLineWidth(1.0);
        glBegin(GL_LINE_STRIP);
            glVertex2f( a.x, a.y );
            glVertex2f( b.x, b.y );
            glVertex2f( c.x, c.y );
            glVertex2f( a.x, a.y );
        glEnd();
        glLineWidth(1.0);
    }
}


/*
 * DRAW Cube
 */
void Display::drawCube(double length) {
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, 0);
        glVertex3f(-length,length,0);
        glVertex3f(-length,-length,0);
        glVertex3f(length,-length,0);
    glEnd();
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, length);
        glVertex3f(-length, length, length);
        glVertex3f(-length, -length, length);
        glVertex3f(length, -length, length);
    glEnd();
    glBegin (GL_LINES);
        glVertex3f(length,length,0);
        glVertex3f(length,length,length);
        glVertex3f(-length,length,0);
        glVertex3f(-length,length,length);
        glVertex3f(-length,-length,0);
        glVertex3f(-length,-length,length);
        glVertex3f(length,-length,0);
        glVertex3f(length,-length,length);
    glEnd();
}
