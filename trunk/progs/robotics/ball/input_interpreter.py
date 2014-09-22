import sys

input = open('output-tmp.txt', 'r')
output = open('output-tmp-py.txt', 'w')
lines = [x.strip(' \t\n\r') for x in input.readlines()]
numPoints = int(lines[0])
pointsLines = lines[1 : numPoints + 1]
numFaces = int(lines[numPoints + 1])
facesLines = lines[numPoints + 2 : numPoints + numFaces + 2]
if len(lines) <> numPoints + numFaces + 3 or int(lines[-1]) <> 0:
    print "Wrong input format"
    sys.exit(-1)

def addTriples(a, b):
    return (int(a[0]) + int(b[0]), int(a[1]) + int(b[1]), int(a[2]) + int(b[2]))

points2Indices = {}
points = []
for i in range(numPoints):
    point = pointsLines[i].split()
    if point[0][0] == "\"":
        points2Indices[point[0][1:-1]] = i + 1
        point = point[1:]
    it = iter(list(range(len(point))))
    p = (0, 0, 0)
    for j in it:
        if point[j].isdigit():
            p = addTriples(p, (int(point[j]), int(point[next(it)]), int(point[next(it)])))
        else:
            p = addTriples(p, points[points2Indices[point[j]] - 1])
    points.append(p)

faces = []
for i in range(numFaces):
    face = facesLines[i].split()
    faceType = int(face[0])
    face = face[1:]
    for i in range(len(face)):
        if not face[i].isdigit():
            face[i] = str(points2Indices[face[i]])
    if ((faceType == 0 or faceType == 1) and len(face) == 6) or faceType == 3:
        p = (face[-3], face[-2], face[-1])
        face = face[:-3]
        for i in range(len(face)):
            basePoint = points[int(face[i]) - 1]
            newPoint = addTriples(p, basePoint)
            points.append(newPoint)
            face[i] = str(len(points))
    f = str(faceType) + " " + " ".join(face)
    faces.append(f)

output.write(str(numPoints) + "\n")
output.write("\n".join([" ".join([str(j) for j in i]) for i in points]) + "\n")
output.write(str(numFaces) + "\n")
output.write("\n".join(faces))
output.write("\n0\n")
output.flush()
