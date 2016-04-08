"""
Code for generating random non-collinear, non-intersecting line segments
 in a box.

Reference: Section 1.5 in "Computational Geometry in C" by Joesph O'Rourke.
"""

import numpy as np
from sys import argv
from math import sqrt
from gen_infra import *

weight = M_WEIGHT
use_weights = False
use_matrix  = False
segs = []
distance_ub = float("inf")

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
    l_str = "l_" + str(weight) + "_" if (use_weights or use_matrix) else ""

    f = open("test_segs_" + w_str + m_str + d_str + l_str + str(n), 'w')
    f.write(str(2 * n) + "\n")
    while len(segs) < n:
        x1, y1 = rcoor(), rcoor()
        x2, y2 = rcoor(), rcoor()
        if not intersects_any((x1, y1), (x2, y2)) and \
           dist((x1, y1), (x2, y2)) <= distance_ub:
            f.write(str(x1) + " " + str(y1) + "\n")
            f.write(str(x2) + " " + str(y2) + "\n")
            segs += [((x1, y1), (x2, y2))]
    
    # Write segments.
    f.write("\n" + str(n) + "\n")
    for i in range(0, 2 * n, 2):
        if use_weights:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix:
            m = raniso(weight)
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
        if arg == '-l':
            weight = int(argv[i + 1])
            i += 1
        i += 1

parse_args(argv)
output_file(int(argv[1]))
