TEMPLATE = app
TARGET = r3d
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
    src/QR3d.cc \
    src/QTransferFunction.cc \
    src/QTransferTable.cc \
    src/QImageRenderer.cc \
    src/r3d.cc

MOC_DIR = moc
HEADERS += \
    ../../toolkit/include/bits/AaToolkit.h \
    ../../toolkit/include/bits/AaException.hh \
    ../../toolkit/include/bits/AaMatrix.hh \
    ../../toolkit/include/bits/AaVector.hh \
    ../../opengl/include/bits/AaGLSL.h \
    ../../opengl/include/bits/AaScene.h \
    ../../opengl/include/bits/AaPrimitives.h \
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
    include/QR3d.h \
    include/QTransferFunction.h \
    include/QTransferTable.h \
    include/QImageRenderer.h

RCC_DIR = rcc
RESOURCES = qr3d.qrc

#RC_FILE = qr3d.rc

INCLUDEPATH += \
    ../../toolkit/include \
    ../../opengl/include \
    ../include \
    include \
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

TRANSLATIONS = qr3d_fr.ts

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
