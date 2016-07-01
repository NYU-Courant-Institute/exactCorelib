QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2-links-rrt-qt
TEMPLATE = app


SOURCES += \
    main.cpp \
    polygon.cpp \
    RRT.cpp \
    ./itree/EndPoint.cpp \
    ./itree/Interval.cpp \
    ./itree/RectKD.cpp \
    ./mathtool/Basic.cpp \
    ./mathtool/Gauss.cpp \
    ./mathtool/Quaternion.cpp \


HEADERS  += \
    main.h \
    intersection.h \
    polygon.h \
    prm-graph.h \
    RRT.h \
    segseg.h \
    ./graph/BaseGraph.h \
    ./graph/BasicDefns.h \
    ./graph/Defines.h \
    ./graph/Graph.h \
    ./graph/GraphAlgo.h \
    ./itree/EndPoint.h \
    ./itree/Interval.h \
    ./itree/MiddleStructure.h \
    ./itree/RectangleTree.h \
    ./itree/RectKD.h \
    ./mathtool/Basic.h \
    ./mathtool/Gauss.h \
    ./mathtool/Matrix.h \
    ./mathtool/Point.h \
    ./mathtool/Quaternion.h \
    ./mathtool/Vector.h \
    ./utils/Timer.h


FORMS    += MainWindow.ui

