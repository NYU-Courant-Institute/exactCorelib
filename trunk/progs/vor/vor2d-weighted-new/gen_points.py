import numpy as np
from sys import argv
from gen_infra import *

weight = M_WEIGHT
use_weights = False
use_matrix  = False
pts = []

def output_file(n):
    global pts
    sn = str(n)

    # Generate random points.
    w_str = "w_" if use_weights else ""
    m_str = "m_" if use_matrix else ""
    l_str = "l_" + str(weight) + "_" if (use_weights or use_matrix) else ""

    f = open("test_" + w_str + m_str + l_str + str(n), 'w')
    f.write(sn + "\n")
    for i in range(n):
        pt = (rcoor(), rcoor())
        if not pt in pts:
            pts += [pt]
            f.write(str(pt[0]) + " " + str(pt[1]) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        if use_weights:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix:
            m = raniso(weight)
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + "\n")

    f.close()

def parse_args(argv):
    global use_weights, use_matrix, weight
    i = 2                # Omitting 'python' and file name.
    while i < len(argv):
        arg = argv[i]
        if arg == '-w':
            use_weights = True
        if arg == '-m':  # TODO
            use_weights = False
            use_matrix  = True
        if arg == '-l':
            weight = int(argv[i + 1])
            i += 1
        i += 1
            
parse_args(argv)
output_file(int(argv[1]))
