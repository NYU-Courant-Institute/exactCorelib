QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2-links-ompl
TEMPLATE = app


SOURCES += \
    main.cpp \
    polygon.cpp \
    MainWindow.cpp \
    Display.cpp \
    ./FIST/api_functions.cpp \
    ./FIST/compute.cpp \
    ./FIST/data.cpp \
    ./FIST/grid.cpp \
    ./FIST/vertex.cpp \
    ./FIST/memory.cpp \
    ./FIST/list.cpp \
    ./FIST/io_2D.cpp \
    ./FIST/io_3D.cpp \
    ./FIST/io_basic.cpp \
    ./FIST/io_dxf.cpp \
    ./FIST/io_parse.cpp \
    ./FIST/bridge.cpp \
    ./FIST/triangulate.cpp \
    ./FIST/simple.cpp \
    ./FIST/numerics.cpp \
    ./FIST/project.cpp \
    ./FIST/sgo.cpp \
    ./FIST/ear_clip.cpp \
    ./FIST/heap.cpp \
    ./FIST/clean_data.cpp \
    ./FIST/io_misc.cpp \
    ./FIST/arg_eval.cpp \
    ./FIST/desperate.cpp \
    ./FIST/quads.cpp \
    ./FIST/ipe_io.cpp \
    ./FIST/write_ipe.cpp \
    ./FIST/bottleneck.cpp \
    ./FIST/orientation.cpp \
    ./FIST/degenerate.cpp \
    ./FIST/misc.cpp \
    ./FIST/elapsed.cpp \
    ./mathtool/Basic.cpp \
    ./mathtool/Gauss.cpp \
    ./mathtool/Quaternion.cpp

HEADERS  += \
    main.h \
    intersection.h \
    polygon.h \
    segseg.h \
    MainWindow.h \
    Display.h \
    ./FIST/fpkernel.h \
    ./FIST/martin.h \
    ./FIST/defs.h \
    ./FIST/api_fist.h \
    ./FIST/ext_appl_defs.h \
    ./FIST/header.h \
    ./FIST/data.h \
    ./FIST/grid.h \
    ./FIST/io_dxf.h \
    ./FIST/numerics.h \
    ./FIST/sgo.h \
    ./FIST/ipe_io.h \
    ./mathtool/Basic.h \
    ./mathtool/Gauss.h \
    ./mathtool/Matrix.h \
    ./mathtool/Point.h \
    ./mathtool/Quaternion.h \
    ./mathtool/Vector.h \
    ./utils/Timer.h


FORMS    += MainWindow.ui

LIBS += -L"/opt/local/lib/" -lompl -lompl_app_base -lompl_app

INCLUDEPATH += \
    /opt/local/include/

CONFIG += c++11
