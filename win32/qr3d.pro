TEMPLATE = app
TARGET = qr3d
QT *= xml opengl

include (qr3d.pri)

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = bin/debug
    } else {
        DESTDIR = bin/release
    }
}
else {
    DESTDIR = bin
}

OBJECTS_DIR = obj
SOURCES += \
    ../../toolkit/src/AaException.cc \
#    ../../toolkit/src/AaText.cc \
    ../../opengl/src/AaGLSL.cc \
    ../../opengl/src/AaScene.cc \
    ../../opengl/src/AaPrimitives.cc \
    ../src/R3dSlicer.cc \
    ../src/R3dRainbow.cc \
    ../src/R3dPreIntegration.cc \
    ../src/R3dPostClassification.cc \
    ../src/R3dPadding.cc \
    ../src/R3dMIP.cc \
    ../src/R3dLut.cc \
    ../src/R3dImageRenderer3dGLSL.cc \
    ../src/R3dImageRenderer3d.cc \
    ../src/R3dImageRenderer2d.cc \
    ../src/R3dImageRenderer.cc \
    ../src/R3dImage.cc \
    ../src/R3dBlinn.cc \
    ../bin/QR3d.cc \
    ../bin/QLutEditor.cc \
    ../bin/QImageRenderer.cc \
    ../bin/r3d.cc

MOC_DIR = moc
HEADERS += \
    ../../toolkit/include/bits/AaException.h \
#    ../../toolkit/include/bits/AaGridIterator.hh \
    ../../toolkit/include/bits/AaMatrix.hh \
#    ../../toolkit/include/bits/AaText.h \
    ../../toolkit/include/bits/AaVector.hh \
    ../../opengl/include/bits/AaGLSL.h \
    ../../opengl/include/bits/AaScene.h \
    ../../opengl/include/bits/AaPrimitives.h \
#    ../../mesh/include/bits/AaMesh.hh \
#    ../../mesh/include/bits/AaMeshPolygonizer.hh \
#    ../../mesh/include/bits/AaMeshNOFF.hh \
    ../include/AaR3d.h \
    ../include/R3dSlicer.h \
    ../include/R3dRainbow.h \
    ../include/R3dPreIntegration.h \
    ../include/R3dPostClassification.h \
    ../include/R3dPadding.h \
    ../include/R3dMIP.h \
    ../include/R3dLut.h \
    ../include/R3dImageRenderer3dGLSL.h \
    ../include/R3dImageRenderer3d.h \
    ../include/R3dImageRenderer2d.h \
    ../include/R3dImageRenderer.h \
    ../include/R3dImage.h \
    ../include/R3dBlinn.h \
    ../bin/QR3d.h \
    ../bin/QLutEditor.h \
    ../bin/QImageRenderer.h

RCC_DIR = rcc
RESOURCES = ../qr3d.qrc

#RC_FILE = qr3d.rc

INCLUDEPATH += \
    ../../toolkit/include \
    ../../opengl/include \
#    ../../mesh/include \
    ../include \
    $${GLEW}/include \
    $${QGLVIEWER}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += \
            -L$${GLEW}/lib -lglew32s -lopengl32 \
            -L$${QGLVIEWER}/QGLViewer/debug -lQGLViewerd2
    }
    else {
        LIBS += \
            -L$${GLEW}/lib -lglew32s -lopengl32 \
            -L$${QGLVIEWER}/QGLViewer/release -lQGLViewer2
    }
}
else {
    LIBS += \
        -L$${GLEW}/lib -lGLEW \
        -L$${QGLVIEWER}/QGLViewer -lQGLViewer
}

TRANSLATIONS = ../qr3d_fr.ts

#DEFINES += AA_TOOLKIT_BUILD_SHARED
DEFINES += GLEW_STATIC

OTHER_FILES += \
    ../glsl/Texture3d.glsl \
    ../glsl/Texture3d.FragmentShader.glsl \
    ../glsl/Rainbow.glsl \
    ../glsl/Rainbow.FragmentShader.glsl \
    ../glsl/PreIntegration.glsl \
    ../glsl/PreIntegration.FragmentShader.glsl \
    ../glsl/PostClassification.glsl \
    ../glsl/PostClassification.FragmentShader.glsl \
    ../glsl/PassThru.VertexShader.glsl \
    ../glsl/McSlicing.VertexShader.glsl \
    ../glsl/McSlicing.GeometryShader.glsl \
    ../glsl/Blinn.glsl \
    ../glsl/Blinn.FragmentShader.glsl
