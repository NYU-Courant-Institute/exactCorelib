import random
import math
from math import pi
from sys import argv

WIDTH = 1024
SCALING = 384

def output_file(n):
    sn = str(n)

    # Generate random points.
    f = open("test_circ_"  + sn, 'w')
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(SCALING * math.cos(2 * pi * i/n) + WIDTH / 2) 
                + " " + str(SCALING * math.sin(2 * pi * i/n) + WIDTH / 2) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        f.write(str(i) + "\n")

    f.close()

output_file(int(argv[1]))
