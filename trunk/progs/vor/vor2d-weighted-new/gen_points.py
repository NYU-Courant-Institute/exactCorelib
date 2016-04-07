import numpy as np
from sys import argv
from gen_infra import iur, rcoor, ransio

use_weights = False
use_matrix  = False

def output_file(n):
    sn = str(n)

    # Generate random points.
    w_str = "w_" if use_weights else ""
    m_str = "m_" if use_matrix else ""
    
    f = open("test_" + w_str + m_str + str(n), 'w')
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(rcoor()) + " " + str(rcoor()) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        if use_weights:
            f.write("w " + str(iur(M_WEIGHT)) + "\n")
        elif use_matrix:
            m = raniso()
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + "\n")

    f.close()

def parse_args(argv):
    global use_weights, use_matrix
    i = 2                # Omitting 'python' and file name.
    while i < len(argv):
        arg = argv[i]
        if arg == '-w':
            use_weights = True
        if arg == '-m':  # TODO
            use_weights = False
            use_matrix  = True
            
parse_args(argv)
output_file(int(argv[1]))
