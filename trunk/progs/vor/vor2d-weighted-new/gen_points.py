import random
from sys import argv

WIDTH = 1024
M_WEIGHT = 6.0

def rcoor():
    return random.randint(0, WIDTH - 1)

def output_file(n):
    sn = str(n)

    # Generate random points.
    f = open("output_" + ("w_" if len(argv) > 2 else "") + sn, 'w')
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(rcoor()) + " " + str(rcoor()) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        if len(argv) > 2 and argv[2] == "-w":
            f.write("w " + str(random.uniform(1.0 / M_WEIGHT, M_WEIGHT)) + "\n")
        f.write(str(i) + "\n")

    f.close()

output_file(int(argv[1]))
