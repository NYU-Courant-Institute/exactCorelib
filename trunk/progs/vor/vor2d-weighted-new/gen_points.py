from random import randint
from sys import argv

WIDTH = 1024;

def rcoor():
    return randint(0, WIDTH - 1)

def output_file(n):
    sn = str(n)

    # Generate random points.
    f = open("output_" + sn, 'w')
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(rcoor()) + " " + str(rcoor()) + "\n")

    # Write points.
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(i) + "\n")

    f.close()

output_file(int(argv[1]))
