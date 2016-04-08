"""
Code for generating random non-collinear, non-intersecting 
points and line segments in a box.
"""

import numpy as np
from sys import argv
from gen_infra import *

weight = M_WEIGHT
use_weights_pts = False
use_matrix_pts  = False
use_weights_sgs = False
use_matrix_sgs  = False
pts  = []
segs = []
distance_ub = float("inf")

def intersects_any(r, s):
    for seg in segs:
        if intersects_or_collinear(seg[0], seg[1], r, s):
            return True
    return False

def collinear_any(p):
    for seg in segs:
        if collinear(seg[0], seg[1], p):
            return True
    return False

def output_file(np, ns):
    global pts, segs
    
    # Generate random points.
    wp_str = "wp_" if use_weights_pts else ""
    mp_str = "mp_" if use_matrix_pts else ""
    ws_str = "ws_" if use_weights_sgs else ""
    ms_str = "ms_" if use_matrix_sgs else ""
    d_str  = "d_" + str(distance_ub) + "_" if distance_ub < float("inf") else ""
    l_str  = "l_" + str(weight) + "_" if (use_weights_pts or use_matrix_pts or use_weights_sgs or use_matrix_sgs) else ""
    
    f = open(
        "test_mixed_" + wp_str + mp_str + ws_str + ms_str + d_str + l_str + str(np) + "_pts_" + str(ns) + "_sgs", 'w')
    f.write(str(2 * ns + np) + "\n")
    while len(segs) < ns:
        x1, y1 = rcoor(), rcoor()
        x2, y2 = rcoor(), rcoor()
        if not intersects_any((x1, y1), (x2, y2)) and \
           dist((x1, y1), (x2, y2)) <= distance_ub:
            f.write(str(x1) + " " + str(y1) + "\n")
            f.write(str(x2) + " " + str(y2) + "\n")
            segs += [((x1, y1), (x2, y2))]
            
    while len(pts) < np:
        x, y = rcoor(), rcoor()
        if not ((x, y) in pts) and not collinear_any((x, y)):
            f.write(str(x) + " " + str(y) + "\n")
            pts += [(x, y)]

    # Write segments.
    f.write("\n" + str(ns + np) + "\n")
    for i in range(0, 2 * ns, 2):
        if use_weights_sgs:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix_sgs:
            m = raniso(weight)
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + " " + str(i + 1) + "\n")

    for i in range(2 * ns, 2 * ns + np):
        if use_weights_pts:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix_pts:
            m = raniso(weight)
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + "\n")

    f.close()

def parse_args(argv):
    global use_weights_pts, use_matrix_pts, use_weights_sgs, use_matrix_sgs
    global distance_ub, weight
    i = 3                # Omitting 'python' and feature counts.
    while i < len(argv):
        arg = argv[i]
        if arg == '-wp':
            use_weights_pts = True
        if arg == '-mp':  # TODO
            use_weights_pts = False
            use_matrix_pts  = True
        if arg == '-ws':
            use_weights_sgs = True
        if arg == '-ms':  # TODO
            use_weights_sgs = False
            use_matrix_sgs  = True
        if arg == '-d':
            distance_ub = int(argv[i + 1])
            i += 1
        if arg == '-l':
            weight = int(argv[i + 1])
            i += 1
        i += 1

parse_args(argv)
output_file(int(argv[1]), int(argv[2]))
