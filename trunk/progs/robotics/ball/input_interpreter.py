import sys
import re

def readNextSetOfLines(lines):
    """Given a set of lines beginning with a number n, return a pair with n and the list of the next n lines, and remove the content of the tuple from the input"""
    nLines = []
    n = 0
    if lines[0].strip() != "End":
        i = 1
        while not lines[i].startswith("Poly") and not lines[i].startswith("Points") and not lines[i].startswith ("End"):
            nLines.append(lines[i])
            n += 1
            i += 1
    lines[:n + 1] = []
    print n
    return (n, nLines)

class Triple(tuple):
    """Triple class"""
    def __new__(cls, *args):
        # TODO: Check if triple
        return tuple.__new__(cls, args)
    def __add__(self, other):
        return Triple(*([sum(x) for x in zip(self, other)]))
    def __sub__(self, other):
        return self.__add__(-i for i in other)

def splitLineByOperator(line):
    operands = [x.strip() for x in re.split('\+|-', line)]
    operators = [x.strip() for x in re.findall('\+|-', line)]
    base = operands[0]
    operands = operands[1:]
    return (base, operands, operators)

def string2Triple(s, points2Indices, points):
    """ Input a string that is either a variable name, or x, y, z values, return
    the Triple for the point"""
    s = s.split()
    if len(s) == 1:
        # name
        if s[0].isdigit():
            return points[int(s[0]) - 1]
        else:
            return points[points2Indices[s[0]] - 1]
    elif len(s) == 3:
        # coordinates
        return Triple(*map(int, s))
    else:
        raise Exception()

def reduceListOps(operands, operators):
    p = Triple(0, 0, 0)
    for j in zip(operands, operators):
        temp = string2Triple(j[0], points2Indices, points)
        if j[1] == '+':
            p += temp
        elif j[1] == '-':
            p -= temp
    return p

input = open('output-tmp.txt', 'r')
output = open('output-tmp-py.txt', 'w')
lines = [x.strip(' \t\n\r') for x in input.readlines()]
(numPoints, pointsLines) = readNextSetOfLines(lines)
listFaces = []
while lines[0] != "End":
    listFaces.append(readNextSetOfLines(lines))

points2Indices = {}
points = []
for i in range(numPoints):
    line = pointsLines[i]
    if line[0] == '"':
        line = line[1:]
        lst = line.split('"')
        name = lst[0]
        points2Indices[name] = i + 1
        line = lst[1]
    (base, operands, operators) = splitLineByOperator(line)
    p = string2Triple(base, points2Indices, points)
    p += reduceListOps(operands, operators)
    points.append(p)

faces = []
for j in range(len(listFaces)):
    numFaces = listFaces[j][0]
    facesLines = listFaces[j][1]
    for i in range(numFaces):
        face = facesLines[i].split()
        faceType = int(face[0])
        face = face[1:]
        line = ' '.join(face)
        if re.search('\+|-', line):
            (base, operands, operators) = splitLineByOperator(line)
            p = reduceListOps(operands, operators)
            face = base.split()
            # use real indices
            for j in range(len(face)):
                if not face[j].isdigit():
                    face[j] = str(points2Indices[face[j]])
            print face
            for j in range(len(face)):
                basePoint = points[int(face[j]) - 1]
                newPoint = p + basePoint
                points.append(newPoint)
                face[j] = str(len(points))
        else:
            for j in range(len(face)):
                if not face[j].isdigit():
                    face[j] = str(points2Indices[face[j]])
        if faceType == 1:
            faces.append('0 ' + ' '.join([str(x) for x in face]) + "\n")
            base = points[int(face[0]) - 1]
            p = points[int(face[1]) - 1] + points[int(face[2]) - 1] + Triple(-base[0], -base[1], -base[2])
            points.append(p)
            faces.append('0 ' + str(face[1]) + ' ' + str(len(points)) + ' ' + str(face[2]) + "\n")
        elif faceType == 2 or faceType == 3:
            base = face[0]
            face = face[1:]
            for j in range(len(face) - 1):
                faces.append('0 ' + str(base) + ' ' + str(face[j]) + ' ' + str(face[j + 1]) + "\n")
        else:
            faces.append(str(faceType) + ' ' + ' '.join(face) + "\n")

output.write(str(len(points)) + "\n")
output.write("\n".join([' '.join([str(j) for j in i]) for i in points]) + "\n")
output.write(str(len(faces)) + "\n")
output.write(''.join(faces))
output.write("0\n")
output.flush()
