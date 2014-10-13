import sys
import re
import os

OPERATOR_REGEX = '\+|-'

def readNextSetOfLines(lines):
    """Given a set of lines beginning with the input type, return a pair with n and the list of the next n lines, and remove the content of the tuple from the input"""
    nLines = []
    if lines[0].strip() == "End":
        return (0, [])
    i = 1
    title = lines[0]
    name = ""
    if title.startswith("Poly") or title.startswith("Wall"):
        name = title.split()[1].strip()[1:-1]
    while lines[i][0] == '"' or lines[i][0].isdigit():
        nLines.append(lines[i])
        i += 1
    lines[:len(nLines) + 1] = []
    return (len(nLines), nLines, name)

class Triple(tuple):
    """Triple class"""
    def __new__(cls, *args):
        # TODO: Check if triple
        return tuple.__new__(cls, args)
    def __add__(self, other):
        return Triple(*([sum(x) for x in zip(self, other)]))
    def __sub__(self, other):
        return self.__add__(-i for i in other)

def splitLineByOperator(string):
    """Given a string, returns a 3-tuple (base, operands, operators):
    base: the first operand. This could be a special operand (e.g., a polygon)
    operands: the other operands (all points)
    operators: the list of operators"""
    operands = [x.strip() for x in re.split(OPERATOR_REGEX, string)]
    operators = [x.strip() for x in re.findall(OPERATOR_REGEX, string)]
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

def reduceListOps(operands, operators, points2Indices, points):
    p = Triple(0, 0, 0)
    for j in zip(operands, operators):
        temp = string2Triple(j[0], points2Indices, points)
        if j[1] == '+':
            p += temp
        elif j[1] == '-':
            p -= temp
    return p

input = open('output-tmp.txt', 'r')
os.remove('output-tmp-py.txt')
output = open('output-tmp-py.txt', 'w')
lines = [x.strip(' \t\n\r') for x in input.readlines()]
listFaces = []
numPoints = 0
pointsLines = []
puts = []
while lines[0] != "End":
    if lines[0] == "Points":
        (n, pl, name) = readNextSetOfLines(lines)
        numPoints += n
        pointsLines += pl
    elif lines[0].startswith("Put"):
        puts.append(lines[0])
        lines[0:1] = []
    else:
        listFaces.append(readNextSetOfLines(lines))

# Processing the explicitly defined input points:
def parsePointsSection(pointsLines):
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
        p += reduceListOps(operands, operators, points2Indices, points)
        points.append(p)
    return (points2Indices, points)

# Processing the explicitly defined input points:
(points2Indices, points) = parsePointsSection(pointsLines)

faces = []
poly2faces = {}
for j in range(len(listFaces)):
    numFaces = listFaces[j][0]
    facesLines = listFaces[j][1]
    name = listFaces[j][2]
    start = len(faces)
    for i in range(numFaces):
        face = facesLines[i].split()
        faceType = int(face[0])
        face = face[1:]
        line = ' '.join(face)
        (base, operands, operators) = splitLineByOperator(line)
        p = reduceListOps(operands, operators, points2Indices, points)
        face = base.split()
        # use real indices
        for j in range(len(face)):
            if not face[j].isdigit():
                face[j] = str(points2Indices[face[j]])
        if p != Triple(0, 0, 0):
            for j in range(len(face)):
                basePoint = points[int(face[j]) - 1]
                newPoint = p + basePoint
                points.append(newPoint)
                face[j] = str(len(points))
        if faceType == 1:
            faces.append(face)
            base = points[int(face[0]) - 1]
            p = points[int(face[1]) - 1] + points[int(face[2]) - 1] + Triple(-base[0], -base[1], -base[2])
            points.append(p)
            faces.append([face[1], len(points), face[2]])
        elif faceType == 2 or faceType == 3:
            base = face[0]
            face = face[1:]
            for j in range(len(face) - 1):
                faces.append([base, face[j], face[j + 1]])
        elif faceType == 4:
            faces.append(face[:])
            face.reverse()
            faces.append(face[:])
        elif faceType == 5:
            faces.append(face[:])
            face.reverse()
            faces.append(face[:])
            face.reverse()
            base = points[int(face[0]) - 1]
            p = points[int(face[1]) - 1] + points[int(face[2]) - 1] + Triple(-base[0], -base[1], -base[2])
            points.append(p)
            otherFace = [face[1], len(points), face[2]]
            faces.append(otherFace[:])
            otherFace.reverse()
            faces.append(otherFace[:])
        else:
            faces.append(face)
    end = len(faces)
    poly2faces[name] = (start, end)

for i in puts:
    poly = i.split(' ', 1)[1]
    facesRange = poly2faces[poly.split()[0]]
    op = poly.split()[1]
    p = Triple(0, 0, 0) if len(poly.split()) < 3 else Triple(*[int(x) for x in poly.split()[2:]])
    for j in range(*facesRange):
        face = []
        for k in range(len(faces[j])):
            basePoint = points[int(faces[j][k]) - 1]
            newPoint = p + basePoint
            points.append(newPoint)
            face.append(str(len(points)))
        faces.append(face)

faces = ['0 ' + ' '.join([str(j) for j in i]) + "\n" for i in faces]
output.write(str(len(points)) + "\n")
output.write("\n".join([' '.join([str(j) for j in i]) for i in points]) + "\n")
output.write(str(len(faces)) + "\n")
output.write(''.join(faces))
output.write("0\n")
output.flush()
