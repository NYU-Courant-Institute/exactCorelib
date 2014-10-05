import sys


def readNextSetOfLines(lines):
    """Given a set of lines beginning with a number n, return a pair with n and the list of the next n lines, and remove the content of the tuple from the input"""
    n = int(lines[0])
    nLines = lines[1 : n + 1]
    lines[:n + 1] = []
    return (n, nLines)

class Triple(tuple):
    def __new__(cls, *args):
        return tuple.__new__(cls, args)
    def __add__(self, other):
        return Triple(*([sum(x) for x in zip(self, other)]))
    def __sub__(self, other):
        return self.__add__(-i for i in other)

input = open('output-tmp.txt', 'r')
output = open('output-tmp-py.txt', 'w')
lines = [x.strip(' \t\n\r') for x in input.readlines()]
(numPoints, pointsLines) = readNextSetOfLines(lines)
(numFaces, facesLines) = readNextSetOfLines(lines)


points2Indices = {}
points = []
for i in range(numPoints):
    point = pointsLines[i].split()
    if point[0][0] == "\"":
        points2Indices[point[0][1:-1]] = i + 1
        point = point[1:]
    it = iter(list(range(len(point))))
    p = Triple(0, 0, 0)
    for j in it:
        if point[j].isdigit():
            p = p + Triple(int(point[j]), int(point[next(it)]), int(point[next(it)]))
        else:
            p = p + points[points2Indices[point[j]] - 1]
    points.append(p)

faces = []
for i in range(numFaces):
    face = facesLines[i].split()
    faceType = int(face[0])
    face = face[1:]
    for j in range(len(face)):
        if not face[j].isdigit():
            face[j] = str(points2Indices[face[j]])
    if ((faceType == 0 or faceType == 1) and len(face) == 6) or faceType == 3:
        p = (face[-3], face[-2], face[-1])
        face = face[:-3]
        for j in range(len(face)):
            basePoint = points[int(face[j]) - 1]
            newPoint = p + basePoint
            points.append(newPoint)
            face[j] = str(len(points))
    if faceType == 1:
        # f = str(faceType) + " " + " ".join(face) + "\n"
        faces.append("0 " + " ".join([str(x) for x in face]) + "\n")
        base = points[int(face[0]) - 1]
        p = points[int(face[1]) - 1] + points[int(face[2]) - 1] + Triple(-base[0], -base[1], -base[2])
        points.append(p)
        faces.append("0 " + str(face[1]) + " " + str(len(points)) + " " + str(face[2]) + "\n")
    elif faceType == 2 or faceType == 3:
        base = face[0]
        face = face[1:]
        for j in range(len(face) - 1):
            faces.append("0 " + str(base) + " " + str(face[j]) + " " + str(face[j + 1]) + "\n")
    else:
        faces.append(str(faceType) + " " + " ".join(face) + "\n")

output.write(str(len(points)) + "\n")
output.write("\n".join([" ".join([str(j) for j in i]) for i in points]) + "\n")
output.write(str(len(faces)) + "\n")
output.write("".join(faces))
output.write("0\n")
output.flush()
