// Parser.h
//      We read a text file following the conventions of "inputs/format.h".
//
//      We must be sure that each triangular face follows the CCW convention
//              when seen from the "outside"

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

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
//   leaving us at the beginning of a token (or EOF)
//   (This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line (std::ifstream & in) {
  int c;

  do {
    c = in.get();
    while (c == '#') {
      do {// ignore the rest of this line
        c = in.get();
      } while (c != '\n');
      c = in.get(); // now, reach the beginning of the next line
    }
  } while (c == ' ' || c == '\t' || c == '\n');  //ignore white spaces and newlines

  if (c == EOF)
    std::cout << "unexpected end of file." << std::endl;

  in.putback(c);  // this is non-white and non-comment char!
  return c;
}//skip_comment_line

// skips '\' followed by '\n'
//   NOTE: this assumes a very special file format (e.g., our BigInt File format)
//   in which the only legitimate appearance of '\' is when it is followed
//   by '\n' immediately!
int skip_backslash_new_line (std::istream & in) {
  int c = in.get();

  while (c == '\\') {
    c = in.get();

    if (c == '\n') {
      c = in.get();
    } else { // assuming the very special file format noted above!
      cout<< "continuation line \\ must be immediately followed by new line.\n";
    }
  }//while
  return c;
}//skip_backslash_new_line

/* ********************************************************************** */

void addTriangle(Box* b, Corner c0, Corner c1, Corner c2) {
  Edge* e1 = new Edge(c0, c1);
  Edge* e2 = new Edge(c1, c2);
  Edge* e3 = new Edge(c2, c0);
  Wall* w = new Wall(c0, c1, c2);
  b->addEdge(e1);
  b->addEdge(e2);
  b->addEdge(e3);
  b->addWall(w);
}

void parseConfigFile(Box* b) {
  std::stringstream ss;
  ss << inputDir << "/" << fileName;  // create full file name
  std::string s = ss.str();
  cout << "input file name = " << s << endl;

  fileProcessor(s);  // this will clean the input and put in
  // output-tmp.txt

  ifstream ifs("output-tmp.txt");
  if (!ifs) {
    cerr<< "cannot open input file" << endl;
    exit(1);
  }

  // First, get to the beginning of the first token:
  skip_comment_line(ifs);

  int nPt = 0;
  int nPolyhedra = 0;
  ifs >> nPt;
  cout << "nPt=" << nPt << endl;

  vector<double> pts(nPt * 3);
  map<string, int> ptsNames;
  // declare a 256-char buffer 
  for (int i = 0; i < nPt; ++i) {
    // readline into buffer, and convert buffer into a input stream ifsbuffer
    ifs >> pts[i * 3] >> pts[i * 3 + 1] >> pts[i * 3 + 2];
    // check if there is a string in the buffer, and if so, assigning this
    // to the map.
  }

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
        int pt;
        ifs >> pt; 
        pt--;   // to get indexing from 0
        ptVec.push_back(new Corner(pts[pt*3]*scale+deltaX,// + xxOffset
                                   pts[pt*3+1]*scale+deltaY,// + yyOffset
                                   pts[pt*3+2]*scale+deltaZ));// + zzOffset
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
      /* addTriangle(b, *ptVec[0], *ptVec[1], *ptVec[2]); */
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

        /* addTriangle(b, *ptVec[2], *ptVec[1], fourthPt); */
      }

    }//for i
  }//while

  cout<< "nPolyhedra=" << nPolyhedra << endl;
  ifs.close();
}
