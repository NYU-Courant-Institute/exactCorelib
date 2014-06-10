/* **************************************
   File: ball.cpp

   Description:
   This is the entry point for the running the SSS algorithm
   for a ball robot amidst a collection of polyhedrons (obstacles).


   NOTE: see several examples of running this program in the Makefile.

   Format of input environment: see README FILE

   **Use WASD keys to rotate the scene**

   Author: Surin Ahn (June 2013)
   -- supervised by Professor Yap

NOTE:	Surin was a Regional Finalist (one out of 100 nationwide)
	from New York in the 2013 Siemens Science & Math Competition

   ************************************** */

#include "ball.h"

using namespace std;

//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
bool findPath(Box* a, Box* b, Octree* OT, int& ct) {
  bool isPath = false;
  toReset.clear();
  a->dist2Source = 0;
  dijkstraQueue dijQ;
  dijQ.push(a);
  toReset.push_back(a);
  while(!dijQ.empty()) {
    Box* current = dijQ.extract();
    current->visited = true;

    // if current is MIXED, try expand it and push the children that is
    // ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
    if (current->status == Box::MIXED) {
      if (OT->expand(current)) {
        ++ct;
        for (int i = 0; i < 8; ++i) {
	  // go through neighbors of each child to see if it's in source set
	  // if yes, this child go into the dijQ
	  bool isNeighborOfSourceSet = false;
	  Box * prev;
	  for (int j = 0; j < 6 && !isNeighborOfSourceSet; ++j) {
	    BoxIter* iter = new BoxIter(current->pChildren[i], j);
	    Box* n = iter->First();
	    while (n && n != iter->End()) {
	      if (n->dist2Source == 0) {
		isNeighborOfSourceSet = true;
		prev = n;
		break;
	      }
	      n = iter->Next();
	    }
	  }
	  if (isNeighborOfSourceSet) {
	    switch (current->pChildren[i]->getStatus()) {
	      //if it's FREE, also insert to source set
	    case Box::FREE:
	      current->pChildren[i]->dist2Source = 0;
	      current->pChildren[i]->prev = prev;
	      // fallthrough
	    case Box::MIXED:
	      dijQ.push(current->pChildren[i]);
	      toReset.push_back(current->pChildren[i]);
	      break;
	    case Box::STUCK:
	      cerr << "inside FindPath: STUCK case not treated" << endl;
	      break;
	    case Box::UNKNOWN:
	      cerr << "inside FindPath: UNKNOWN case not treated" << endl;
	      break;
	    }
	  }
	}
      }
    } else {
      // if current is not MIXED, then must be FREE
      // go through its neighbors and add FREE and MIXED ones to dijQ
      // also add FREE ones to source set
      //found path!
      if (current == b && b->dist2Source == 0) {
	isPath = true;
	break;
      }
      for (int i = 0; i < 6; ++i) {
	BoxIter* iter = new BoxIter(current, i);
	Box* neighbor = iter->First();
	while (neighbor && neighbor != iter->End()) {
	  if (!neighbor->visited && neighbor->dist2Source == -1 &&
	      (neighbor->status == Box::FREE || neighbor->status == Box::MIXED)) {
	    if (neighbor->status == Box::FREE) {
	      neighbor->dist2Source = 0;
	      neighbor->prev = current;
	    }
	    dijQ.push(neighbor);
	    toReset.push_back(neighbor);
	  }
	  neighbor = iter->Next();
	}
      }
    }
  }

  // these two fields are also used in dijkstraShortestPath
  // need to reset
  for (int i = 0; i < (int)toReset.size(); ++i) {
    toReset[i]->visited = false;
    toReset[i]->dist2Source = -1;
  }

  return isPath;
}

void animReplay() {
  iPathSeg = 0;
  finishedAnim = 0;
}

void resetRotationMatrix() {
  // Chee:
  // The vector view_rotate in GLUI is a 4x4 rotation matrix
  // We want to rotate the view by "yAng" about the vertical (y) axis:
  //
  // First we define yAng:
  float yAng = 3.14 / 4;    // 45degrees
  //
  // row 1:
  view_rotate[0] = cos(yAng);
  view_rotate[1] = .0;
  view_rotate[2] = sin(yAng);
  view_rotate[3] = 0.;
  // row 2:
  view_rotate[4] = 0.;
  view_rotate[5] = 1.;
  view_rotate[6] = 0.;
  view_rotate[7] = 0.;
  // row 3:
  view_rotate[8] = -sin(yAng);
  view_rotate[9] = 0.;
  view_rotate[10] = cos(yAng);
  view_rotate[11] = 0.;
  // row 4:
  view_rotate[12] = 0.;
  view_rotate[13] = 0.;
  view_rotate[14] = 0.;
  view_rotate[15] = 1.;
  viewRot->set_float_array_val(view_rotate);
}

void resetViewPoint() {
  resetRotationMatrix();
  obj_pos[0] = eye[0];
  obj_pos[1] = eye[1];
  obj_pos[2] = eye[2];
}

void resetTopViewPoint() {
  topViewPos[0] = (float) (boxWidth / 2.0);
  topViewPos[1] = (float) (boxWidth / 2.0);
  topViewPos[2] = (float) (boxWidth * 2.5);
}

void logNonInteractiveRun(bool noPath) {
  // do something...
  cout << "Non Interactive Run of Disc Robot" << endl;
  if (noPath) {
    cout << "No Path Found!" << endl;
  } else {
    cout << "Path was Found!" << endl;
  }
}

Octree* genEmptyTree() {
  Box* root = new Box(boxWidth/2, boxWidth/2, boxWidth/2, boxWidth);
  Box::r0 = R0;

  parseConfigFile(root);
  root->updateStatus();

  cout<<"inside genEmpty:  Qtype= " << QType << "\n";
  return new Octree(root, epsilon, QType, seed++);  // Note that seed keeps changing!
}

Box* findEnclosingFreeBox(Octree* octree, double coordinate[3], Box* box, int& expandCounter) {
  box = octree->getBox(coordinate[0], coordinate[1], coordinate[2]);
  while (box && !(box)->isFree()) {
    if (!octree->expand(box)) {
      return NULL; // Does not have a free box for the given resolution
    }
    ++expandCounter;
    box = octree->getBox(box, coordinate[0], coordinate[1], coordinate[2]);
  }
  return box;
}

void run() {
  //update from glui live variables
  string previousResults = outputStream.str();
  outputStream.str(string());
  outputStream.clear();
  outputStream << "Run " << runCounter++ << ":" << endl;
  fileName = editInput->get_text();
  inputDir = editDir->get_text();
  R0 = editRadius->get_float_val();
  epsilon = editEpsilon->get_float_val();
  alpha[0] = editAlphaX->get_float_val();
  alpha[1] = editAlphaY->get_float_val();
  alpha[2] = editAlphaZ->get_float_val();
  beta[0] = editBetaX->get_float_val();
  beta[1] = editBetaY->get_float_val();
  beta[2] = editBetaZ->get_float_val();
  QType = radioQType->get_int_val();

  outputStream<<"inside run:  Qtype = " << QType << "\n";

  Timer t;
  // start timer
  t.start();

  if (OT) {
    delete(OT);
  }

  OT = genEmptyTree();

  if (showAnim) {
    animReplay();
  }

  noPath = false;  // Confusing use of "noPath"
  int ct = 0;

  boxA = findEnclosingFreeBox(OT, alpha, boxA, ct);
  boxB = findEnclosingFreeBox(OT, beta, boxB, ct);
  if (boxA == NULL || boxB == NULL) {
    noPath = true;
  }
  if (QType == 0 || QType == 1) {
    while (!noPath && !OT->isConnect(boxA, boxB)) {
      if (!OT->expand()) { // should ct be passed to expand?
        noPath = true;
      }
      ++ct;
    }
    path = Graph::dijkstraShortestPath(boxA, boxB);
  } else if (QType == 2) {
    noPath = !findPath(boxA, boxB, OT, ct);
    boxA->prev = NULL;
    path.clear();
    path.push_back(boxB);
    while (path.back()->prev) {
      path.push_back(path.back()->prev);
    }
    for (int i = 0; i < (int)toReset.size(); ++i) {
      toReset[i]->prev = NULL;
    }
    vector<Box*> dijkstraShortestPath = Graph::dijkstraShortestPath(boxA, boxB);
    if (dijkstraShortestPath.back() == boxA) {
      path = dijkstraShortestPath;
    } else {
      cerr << "Something went wrong in the dijkstra path generation algorithm, defaulting to subdivision generated path" << endl;
    }
  }
  // stop timer
  t.stop();
  // print the elapsed time in millisec
  outputStream << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
  outputStream << ">>\n";
  outputStream << ">>     Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
  outputStream << ">>\n";

  if (!noPath) {
    outputStream << ">>     Path found !" << endl;
  } else {
    outputStream << ">>     No Path !" << endl;
  }
  outputStream << ">>\n";
  outputStream << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
  outputStream << "Expanded " << ct << " times" << endl;
  outputStream << "total Free boxes: " << freeCount << endl;
  outputStream << "total Stuck boxes: " << stuckCount << endl;
  outputStream << "total Mixed boxes < epsilon: " << mixSmallCount << endl;
  outputStream << "total Mixed boxes > epsilon: " << mixCount - ct - mixSmallCount << endl;
  freeCount = 0;
  stuckCount = 0;
  mixCount = 0;
  mixSmallCount = 0;
  outputStream << endl;
  outputStream << previousResults;
  //output->set_text(outputStream.str().c_str());
}

void drawPath(vector<Box*>& path) {
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(beta[0], beta[1], beta[2]);
  for (int i = 0; i < (int) path.size(); ++i) {
    glVertex3f(path[i]->x, path[i]->y, path[i]->z);
  }
  glVertex3f(alpha[0], alpha[1], alpha[2]);
  glEnd();
  glLineWidth(1.0);
}

void drawQuad(Box* b) {
  glLineWidth(1);
  switch (b->status) {
    case Box::FREE:
      glColor3f(0.25, 1, 0.25);
      break;
    case Box::STUCK:
      glColor3f(1, 0.25, 0.25);
      break;
    case Box::MIXED:
      glColor3f(1, 1, 0.25);
      if (b->width < epsilon) {
	glColor3f(0.5, 0.5, 0.5);
      }
      break;
    case Box::UNKNOWN:
      std::cout << "UNKNOWN in drawQuad" << std::endl;
      break;
  }

  if (!radioDrawOption->get_int_val()) {
    glPushMatrix();
    glTranslated(b->x, b->y, b->z);
    glutWireCube(b->width);
    glPopMatrix();
  }
}

void drawEdges(Box* b) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (list<Wall*>::iterator iter = b->Walls.begin(); iter != b->Walls.end(); ++iter) {
    Wall* w = *iter;
    double A[3] = {w->b->x - w->a->x, w->b->y - w->a->y, w->b->z - w->a->z};
    double B[3] = {w->c->x - w->a->x, w->c->y - w->a->y, w->c->z - w->a->z};
    double normal[3] = {A[1]*B[2] - B[1]*A[2], B[0]*A[2] - A[0]*B[2], A[0]*B[1] - A[1]*B[0]};
    glColor4f(1.0, 1.0, 1.0, transparency / 100.0);
    glBegin(GL_TRIANGLES);
    glNormal3d(normal[0], normal[1], normal[2]);
    glVertex3f(w->a->x, w->a->y, w->a->z);
    glVertex3f(w->b->x, w->b->y, w->b->z);
    glVertex3f(w->c->x, w->c->y, w->c->z);
    glEnd();
  }
  glLineWidth(1.0);
}

// Recursively get the leaf descendants of Box b
void treeTraverse(Box* b) {
  if (b) {
    if (b->isLeaf) {
      drawQuad(b);
      return;
    }
    for (int i = 0; i < 8; ++i) {
      treeTraverse(b->pChildren[i]);
    }
  }
}

void filledSphere(double radius, double x, double y, double z, double r, double g, double b) {
  glPushMatrix();
  glColor3d(r,g,b);
  glTranslated(x, y, z);
  glutSolidSphere(radius, 100, 100);
  glPopMatrix();
}

void renderScene(void) {
  treeTraverse(OT->pRoot);	// draws the octree


  // Drawing the axes:
  glLineWidth(3.0);	// we should visibly identify the 3 coordinate axes
  			// put an arrow head.
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(boxWidth * 2, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, boxWidth * 2, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, boxWidth * 2);
  glEnd();

  double r0 = min(R0, 5.0);

  glPolygonMode(GL_FRONT, GL_LINE);

  if (!noPath) {
    if (showAnim && !finishedAnim) {
      if (iPathSeg >= path.size() - 1) {
        finishedAnim = true;
      } else {
	float dx = path[iPathSeg + 1]->x - path[iPathSeg]->x;
	float dy = path[iPathSeg + 1]->y - path[iPathSeg]->y;
	float dz = path[iPathSeg + 1]->z - path[iPathSeg]->z;
	float segLength = sqrt(dx * dx + dy * dy + dz * dz);
	segCount = segLength / epsilon;
	float x = path[iPathSeg]->x + dx / segCount * inSegCount;
	float y = path[iPathSeg]->y + dy / segCount * inSegCount;
	float z = path[iPathSeg]->z + dz / segCount * inSegCount;
	filledSphere(R0, x, y, z, 0, 0, 1);
      }
    }
    drawPath(path);
  }
  filledSphere(R0, alpha[0], alpha[1], alpha[2], 1.0, 0.0, 0.498);  // start
  filledSphere(R0, beta[0], beta[1], beta[2], 0, 0, 1);  // goal
  drawEdges(OT->pRoot);

  glEnable(GL_LIGHTING);
  glutSwapBuffers();
}

void setUpView(float xy_aspect) {
  glClearColor(0.6, 0.8, 1.0, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glScalef(2.0/boxWidth, 2.0/boxWidth, 2.0/boxWidth);
}

void renderTopView(void) {
  setUpView(topViewXYAspect);
  gluLookAt(topViewPos[1], topViewPos[2], topViewPos[0], topViewPos[1], topViewPos[2] - 1, topViewPos[0], 1, 0, 0);
  renderScene();
}

void renderCustomView(void) {
  setUpView(customViewXYAspect);
  gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
  glMultMatrixf(view_rotate);

  renderScene();
}

void reshapeView(int windowID, float& viewXYAspect) {
  glutSetWindow(windowID);
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  glViewport(tx, ty, tw, th);

  viewXYAspect = (float)tw / (float)th;

  glutPostRedisplay();
}

void reshapeTopView(int width, int height) {
  reshapeView(topViewWindowID, topViewXYAspect);
}

void reshapeCustomView(int width, int height) {
  reshapeView(customViewWindowID, customViewXYAspect);
}

void setLightsForWindow(int windowID) {
  glutSetWindow(windowID);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glEnable(GL_DEPTH_TEST);
}

void idle (int v) {
  glutTimerFunc(1000.0 / frameRate, idle, 0);
  inSegCount++;
  if (inSegCount > segCount) {
    iPathSeg++;
    inSegCount = 1;
  }
  glutSetWindow(customViewWindowID);
  glutPostRedisplay();
  glutSetWindow(topViewWindowID);
  glutPostRedisplay();
}

// MAIN PROGRAM: ========================================
int main(int argc, char* argv[]) {
  if (argc > 1) interactive = atoi(argv[1]);  // Interactive (0) or no (>0)
  if (argc > 2) alpha[0] = atof(argv[2]);    // start x
  if (argc > 3) alpha[1] = atof(argv[3]);    // start y
  if (argc > 4) alpha[2] = atof(argv[4]);    // start z
  if (argc > 5) beta[0] = atof(argv[5]);    // goal x
  if (argc > 6) beta[1] = atof(argv[6]);    // goal y
  if (argc > 7) beta[2] = atof(argv[7]);    // goal z
  if (argc > 8) epsilon = atof(argv[8]);    // epsilon (resolution)
  if (argc > 9) R0      = atof(argv[9]);    // robot radius
  if (argc > 10) fileName = argv[10];     // Input file name
  if (argc > 11) boxWidth = atof(argv[11]);  // boxWidth
  if (argc > 12) windowPosX = atoi(argv[12]);  // window X pos
  if (argc > 13) windowPosY = atoi(argv[13]);  // window Y pos
  if (argc > 14) QType   = atoi(argv[14]);  // PriorityQ Type (random or no)
  if (argc > 15) seed   = atoi(argv[15]);    // for random number generator
  if (argc > 16) inputDir  = argv[16];    // path for input files
  if (argc > 17) deltaX  = atof(argv[17]);  // x-translation of input file
  if (argc > 18) deltaY  = atof(argv[18]);  // y-translation of input file
  if (argc > 19) deltaZ = atof(argv[19]);    // z-translation of input file
  if (argc > 20) scale  = atof(argv[20]);    // scaling of input file
  if (argc > 21) eye[0] = atof(argv[21]);
  if (argc > 22) eye[1] = atof(argv[22]);
  if (argc > 23) eye[2] = atof(argv[23]);
  if (argc > 24) at[0] = atof(argv[24]);
  if (argc > 25) at[1] = atof(argv[25]);
  if (argc > 26) at[2] = atof(argv[26]);
  if (argc > 27) up[0] = atof(argv[27]);
  if (argc > 28) up[1] = atof(argv[28]);
  if (argc > 29) up[2] = atof(argv[29]);
  if (argc > 30) transparency = atoi(argv[30]);
  if (argc > 31) frameRate = atoi(argv[31]);

  cout << "before interactive, Qtype= " << QType << endl;

  if (interactive > 0) {  // non-interactive
    logNonInteractiveRun(noPath);
    return 0;
  }

  // Else, set up for GLUT/GLUI interactive display:
  glutInit(&argc, argv);
  glutInitWindowSize(boxWidth, boxWidth);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowPosX + boxWidth * 2, windowPosY);
  topViewWindowID = glutCreateWindow("Top View");

  glutDisplayFunc(renderTopView);
  glutReshapeFunc(reshapeTopView);

  glutInitWindowPosition(windowPosX, windowPosY);
  customViewWindowID = glutCreateWindow("Custom View");

  glutDisplayFunc(renderCustomView);
  glutReshapeFunc(reshapeCustomView);

  glutTimerFunc(50, idle, 0);

  GLUI *glui = GLUI_Master.create_glui("control", 0, windowPosX + boxWidth + 20, windowPosY);

  // SETTING UP THE CONTROL PANEL:
  editInput = glui->add_edittext("Input file:", GLUI_EDITTEXT_TEXT);
  editInput->set_text((char*)fileName.c_str());
  editDir = glui->add_edittext("Input Directory:", GLUI_EDITTEXT_TEXT);
  editDir->set_text((char*)inputDir.c_str());

  GLUI_Panel * robot_box_panel = glui->add_panel("Robot Specs");
  editRadius = glui->add_edittext_to_panel(robot_box_panel,
             "Radius:", GLUI_EDITTEXT_FLOAT);
  editRadius->set_float_val(R0);
  editEpsilon = glui->add_edittext_to_panel(robot_box_panel,
              "Epsilon:", GLUI_EDITTEXT_FLOAT);
  editEpsilon->set_float_val(epsilon);

  // NOTE: this is a quick and dirty solution to the bug where Start and Goal
  //    configurations are interchanged!   
  //GLUI_Panel * alpha_box_panel = glui->add_panel("Alpha (start configuration)");
  GLUI_Panel * alpha_box_panel = glui->add_panel("Beta (goal configuration)");
  editAlphaX = glui->add_edittext_to_panel(alpha_box_panel,
             "x:", GLUI_EDITTEXT_FLOAT);
  editAlphaX->set_float_val(alpha[0]);
  editAlphaY = glui->add_edittext_to_panel(alpha_box_panel,
             "y:", GLUI_EDITTEXT_FLOAT);
  editAlphaY->set_float_val(alpha[1]);
  editAlphaZ = glui->add_edittext_to_panel(alpha_box_panel, "z:", GLUI_EDITTEXT_FLOAT);
  editAlphaZ->set_float_val(alpha[2]);

  // NOTE: this is a quick and dirty solution to the bug where Start and Goal
  //    configurations are interchanged!   
  // GLUI_Panel * beta_box_panel = glui->add_panel("Beta (goal configuration)");
  GLUI_Panel * beta_box_panel = glui->add_panel("Alpha (start configuration)");
  editBetaX = glui->add_edittext_to_panel(beta_box_panel,
            "x:", GLUI_EDITTEXT_FLOAT);
  editBetaX->set_float_val(beta[0]);
  editBetaY = glui->add_edittext_to_panel(beta_box_panel,
            "y:", GLUI_EDITTEXT_FLOAT);
  editBetaY->set_float_val(beta[1]);
  editBetaZ = glui->add_edittext_to_panel(beta_box_panel,
            "z:", GLUI_EDITTEXT_FLOAT);
  editBetaZ->set_float_val(beta[2]);

  editSeed = glui->add_edittext("seed:", GLUI_EDITTEXT_INT);
  editSeed->set_int_val(seed);

  GLUI_Panel* animation_panel = glui->add_panel("Animation Control");

  GLUI_Spinner* editFrameRate = glui->add_spinner_to_panel(animation_panel, "Frame Rate", GLUI_SPINNER_INT, &frameRate);
  editFrameRate->set_int_val(frameRate);
  editFrameRate->set_int_limits(1, 100);

  GLUI_Spinner* editTransparency = glui->add_spinner_to_panel(animation_panel, "Transparency", GLUI_SPINNER_INT, &transparency);
  editTransparency->set_int_val(transparency);
  editTransparency->set_int_limits(0, 100);

  GLUI_Button* buttonReplay = glui->add_button_to_panel(animation_panel, "Replay Animation", -1, (GLUI_Update_CB)animReplay);
  buttonReplay->set_name("replay");
  buttonReplay->set_w(1);

  glui->add_separator();
  GLUI_Button* buttonRun = glui->add_button("Run", -1, (GLUI_Update_CB)run);
  buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)

  // New column:
  glui->add_column(true);

  glui->add_separator();
  radioQType = glui->add_radiogroup();
  glui->add_radiobutton_to_group(radioQType, "Random");
  glui->add_radiobutton_to_group(radioQType, "BFS (Breadth First)");
  //glui->add_radiobutton_to_group(radioQType, "A-star");
  glui->add_radiobutton_to_group(radioQType, "Best First Search");
  radioQType->set_int_val(QType);

  glui->add_separator();
  radioDrawOption = glui->add_radiogroup(0, -1, (GLUI_Update_CB)renderCustomView);
  glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
  glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");

  glui->add_separator();

  viewRot = new GLUI_Rotation(glui, "Rotate", view_rotate);
  viewRot->set_spin(1.0);

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/
  setLightsForWindow(topViewWindowID);
  setLightsForWindow(customViewWindowID);

  /****************************************/
  /*          Initial Viewing Position     */
  /****************************************/
  eyeVector[0] = at[0] - eye[0];
  eyeVector[1] = at[1] - eye[1];
  eyeVector[2] = at[2] - eye[2];
  resetViewPoint();

  GLUI_Translation *trans_xy = new GLUI_Translation(glui, "Objects XY",
                GLUI_TRANSLATION_XY, obj_pos);
  trans_xy->set_speed(5);
  GLUI_Translation *trans_z =
    new GLUI_Translation(glui, "Translate Z", GLUI_TRANSLATION_Z, &obj_pos[2]);
  trans_z->set_speed(5);
  glui->add_button("Reset Custom", 0, (GLUI_Update_CB) resetViewPoint);
  GLUI_Translation *topViewTransXY = new GLUI_Translation(glui, "Top View XY",
                GLUI_TRANSLATION_XY, topViewPos);
  topViewTransXY->set_speed(5);
  GLUI_Translation *topViewZoom =
    new GLUI_Translation(glui, "Top View Zoom", GLUI_TRANSLATION_Z, &topViewPos[2]);
  trans_z->set_speed(5);
  glui->add_button("Reset Top", 0, (GLUI_Update_CB) resetTopViewPoint);

  //output = new GLUI_TextBox(glui, true);
  //output->set_h(300);
  //output->set_w(200);
  // Quit button
  glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

  glui->set_main_gfx_window(customViewWindowID);

  // PERFORM THE INITIAL RUN OF THE ALGORITHM
  //==========================================
  run();   // make it do something interesting from the start!!!

  // SHOULD WE STOP or GO INTERACTIVE?
  //==========================================
  if (interactive > 0) {  // non-interactive
    logNonInteractiveRun(noPath);
    return 0;
  } else {
    glutMainLoop();
  }

  return 0;
}
