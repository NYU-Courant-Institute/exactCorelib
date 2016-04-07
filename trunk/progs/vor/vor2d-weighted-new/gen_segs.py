"""
Code for generating random non-collinear, non-intersecting line segments
 in a box.

Reference: Section 1.5 in "Computational Geometry in C" by Joesph O'Rourke.
"""

import numpy as np
from sys import argv
from math import sqrt
from gen_infra import iur, rcoor, ransio

use_weights = False
use_matrix  = False
segs = []
distance_ub = float("inf")

# All coordinates are integral so this is exact.
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

def intersects_or_collinear(p, q, r, s):
    if (collinear(r, s, p) or \
        collinear(r, s, q) or \
        collinear(p, q, r) or \
        collinear(p, q, s)):
        return True
    else:
        return (left(r, s, p) ^ left(r, s, q)) and \
            (left(p, q, r) ^ left(p, q, s))

def intersects_any(r, s):
    for seg in segs:
        if intersects_or_collinear(seg[0], seg[1], r, s):
            return True
    return False

def output_file(n):
    global segs
    
    # Generate random points.
    w_str = "w_" if use_weights else ""
    m_str = "m_" if use_matrix else ""
    d_str = "d_" + str(distance_ub) + "_" if distance_ub < float("inf") else ""
    
    f = open("test_segs_" + w_str + m_str + d_str + str(n), 'w')
    f.write(str(2 * n) + "\n")
    while len(segs) < n:
        x1, y1 = rcoor(), rcoor()
        x2, y2 = rcoor(), rcoor()
        if ((x1 != x2) or (y1 != y2)) and \
           not intersects_any((x1, y1), (x2, y2)) and \
           sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2) <= distance_ub:
            f.write(str(x1) + " " + str(y1) + "\n")
            f.write(str(x2) + " " + str(y2) + "\n")
            segs += [((x1, y1), (x2, y2))]

    # Write segments.
    f.write("\n" + str(n) + "\n")
    for i in range(0, 2 * n, 2):
        if use_weights:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix:
            m = raniso()
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + " " + str(i + 1) + "\n")

    f.close()

def parse_args(argv):
    global use_weights, use_matrix, distance_ub
    i = 2                # Omitting 'python' and file name.
    while i < len(argv):
        arg = argv[i]
        if arg == '-w':
            use_weights = True
        if arg == '-m':  # TODO
            use_weights = False
            use_matrix  = True
        if arg == '-d':
            distance_ub = int(argv[i + 1])
            i += 1
        i += 1

parse_args(argv)
output_file(int(argv[1]))
