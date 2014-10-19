#pragma once
// Parser.h
//      We read a text file following the conventions of "inputs/format.txt".
//
//      We must be sure that each triangular face follows the CCW convention
//              when seen from the "outside"

#include "./Vector.h"
#include <iostream>
#include <map>
#include <cstring>

string fileName("input1.txt");     // Input file name
string inputDir("inputs");     // Path for input files
double scale=1;        // scaling of input environment
double deltaX=0;      // x-translation of input environment
double deltaY=0;      // y-translation of input environment
double deltaZ=0;      // z-translation of input environment

using namespace std;

extern int fileProcessor(string inputfile);

void parseVertices(vector<Vector> &pts, map<string, int> &ptsNames, ifstream &ifs);
void parseConfigFile(Box* b);

void parseVertices(vector<Vector> &pts, map<string, int> &ptsNames, ifstream &ifs) {
  int nPt = 0;
  ifs >> nPt;
  cout << "nPt=" << nPt << endl;

  // declare a 256-char buffer
  for (int i = 0; i < nPt; ++i) {
    double x, y, z;
    // readline into buffer, and convert buffer into a input stream ifsbuffer
    ifs >> x >> y >> z;
    pts.push_back(Vector(x, y, z));
    // check if there is a string in the buffer, and if so, assigning this
    // to the map.
  }
}

void parseConfigFile(Box* b) {
  std::stringstream ss;
  ss << inputDir << "/" << fileName;  // create full file name
  std::string s = ss.str();
  s = "inputs/input1.txt";
  cout << "input file name = " << s << endl;

  fileProcessor(s);  // this will clean the input and put in
  // output-tmp.txt

  system("python input_interpreter.py");
  ifstream ifs("output-tmp-py.txt");
  if (!ifs) {
    cerr<< "cannot open input file" << endl;
    exit(1);
  }

  vector<Vector> pts;
  map<string, int> ptsNames;
  parseVertices(pts, ptsNames, ifs);

  int nPolyhedra = 0;
  while (true) {
    int numFaces;
    ifs >> numFaces;
    cout << numFaces << " faces" << endl;

    if (numFaces == 0) {
      break;
    }
    nPolyhedra++;

    for (int i = 0; i < numFaces; ++i) {
      vector<Corner*> ptVec;

      // readline into buffer, and convert buffer into input stream "ifsbuffer"
      int faceType = 0; // used to check whether the face is a quadrilateral or triangle
                        // faceType=0 means a triangle
                        // faceType=1 means a quadrilateral

      ifs >> faceType; // move this after the for-loop

      // declare temporary point xx, yy, zz
      // ifsBuffer >> xx, and if necessary, use hash map to convert into an index
      // ifsBuffer >> yy, and if necessary, use hash map to convert into an index
      // ifsBuffer >> zz, and if necessary, use hash map to convert into an index
      // if there are more stuff in the buffer, we will read the faceType:
      //     ifsBuffer >> faceType
      // xxOffset = yyOffset = zzOffset = 0;
      // if there are even more stuff in the buffer, we will read the offsets:
      //     ifsBuffer >> xxOffset
      //     ifsBuffer >> yyOffset
      //     ifsBuffer >> yyOffset

      //The following loop is removed:
      for (int j=0; j< 3; ++j){
        unsigned long pt;
        ifs >> pt;
        pt--;   // to get indexing from 0
        ptVec.push_back(new Corner(pts[pt].x * scale,
                                   pts[pt].y * scale,
                                   pts[pt].z * scale));
        b -> addCorner(ptVec.back());
      }
      // First triangle
      Edge* e1 = new Edge (ptVec[0], ptVec[1]);
      Edge* e2 = new Edge (ptVec[1], ptVec[2]);
      Edge* e3 = new Edge (ptVec[2], ptVec[0]);
      b -> addEdge(e1);
      b -> addEdge(e2);
      b -> addEdge(e3);

      Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
      b -> addWall(w);
      // Possible Second triangle:

      // read the code for faceType from ifsbuffer: if no code or code=1, do
      // nothing:
      if (faceType == 1) {   // i.e., a quadrilateral
        Corner fourthPt = *ptVec[1] + *ptVec[2] - *ptVec[0];// + xx,yy,zz Offsets
        Edge* e1 = new Edge (*ptVec[2], *ptVec[1]);
        Edge* e2 = new Edge (*ptVec[1], fourthPt);
        Edge* e3 = new Edge (fourthPt, *ptVec[2]);
        Wall* w = new Wall (*ptVec[1], fourthPt, *ptVec[2]);
        b -> addEdge(e1);
        b -> addEdge(e2);
        b -> addEdge(e3);

        b -> addWall(w);
      }

    }  // for i
  }  // while

  cout<< "nPolyhedra=" << nPolyhedra << endl;
  ifs.close();
}
