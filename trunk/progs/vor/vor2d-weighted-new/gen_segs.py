"""
Code for generating random non-colinear, non-intersecting line segments
 in a box.

Reference: Section 1.5 in "Computational Geometry in C" by Joesph O'Rourke
"""

import random
import numpy as np
from sys import argv

WIDTH = 1024
M_WEIGHT = 6.0 # The max. weight (resp. min. reciprocal weight) of a feature.

segs = []

def area(p, q, r):
    return np.linalg.det(np.array([[p[0], q[0], r[0]],
                                   [p[1], q[1], r[1]],
                                   [1, 1, 1]]))

def collinear(p, q, r):
    return area(p, q, r) == 0

def left(p, q, r):
    return area(p, q, r) > 0

def rcoor():
    return random.randint(0, WIDTH - 1)

def intersects(r, s):
    for seg in segs:
        p, q = seg
        if collinear(r, s, p) or \
           collinear(r, s, q) or \
           collinear(p, q, r) or \
           collinear(p, q, s):
            return false
        else:
            return (left(r, s, p) ^ left(r, s, q)) and \
                (left(p, q, r) ^ left(p, q, s)

def output_file(n):
    # Generate random points.
    f = open("output_" + ("w_" if len(argv) > 2 else "") + sn, 'w')
    f.write(str(2 * n) + "\n")
    while len(l) < n:
        x1, y1 = rcoor(), rcoor()
        x2, y2 = rcoor(), rcoor()
        if ((x1 != x2) || (y1 != y2)) and not intersects((x1, y1), (x2, y2)):
            f.write(str(x1) + " " + str(y1) + "\n")
            f.write(str(x2) + " " + str(y2) + "\n")

    # Write segments.
    f.write("\n" + str(n) + "\n")
    for i in range(0, n, 2):
        if len(argv) > 2 and argv[2] == "-w":
            f.write("w " + str(random.uniform(1.0 / M_WEIGHT, M_WEIGHT)) + "\n")
        f.write(str(i) + " " + str(i + 1) + "\n")

    f.close()

outputfile(int(argv[1]))
