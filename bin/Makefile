GCC_DEFAULT=    g++ -W -Wall -pedantic
GCC_DEBUG=      $(GCC_DEFAULT) -O0 -ggdb3 -pg
GCC_RELEASE=    $(GCC_DEFAULT) -O3 -ggdb3
#GCC=            $(GCC_DEBUG)
GCC=            $(GCC_RELEASE)

#QT4=/opt/QtSDK/Desktop/Qt/473/gcc
MOC=            moc-qt4 #$(QT4)/bin/moc

################################################################################
# Chemins ######################################################################
################################################################################

L_X11=          -L/usr/lib -lXmu -L/usr/lib/x86_64-linux-gnu -lXi
L_GLU=          -lGL -lGLU $(L_X11) # -lglut

# AaToolkit ####################################################################

P_AaToolkit=    $(HOME)/wrk/toolkit
I_AaToolkit=    -I$(P_AaToolkit)/include
L_AaToolkit=    -L$(P_AaToolkit)/lib -lAaToolkit

# AaFFmpeg #####################################################################

P_FFMPEG=       $(HOME)/3rdPartyLibs/ffmpeg
I_FFMPEG=       -I$(P_FFMPEG)
L_FFMPEG=       -L$(P_FFMPEG)/libavformat -lavformat\
                -L$(P_FFMPEG)/libavcodec  -lavcodec\
                -L$(P_FFMPEG)/libavutil   -lavutil\
                -L$(P_FFMPEG)/libswscale  -lswscale -lbz2 -lz

P_AaFFmpeg=     $(HOME)/wrk/ffmpeg
I_AaFFmpeg=     -I$(P_AaFFmpeg)/include        $(I_FFMPEG)
L_AaFFmpeg=     -L$(P_AaFFmpeg)/lib -lAaFFmpeg $(L_FFMPEG)

# AaGL #########################################################################

P_GLEW=         $(HOME)/3rdPartyLibs/glew-1.7.0
I_GLEW=         -I$(P_GLEW)/include
L_GLEW=         -L$(P_GLEW)/lib -lGLEW -lpthread $(L_GLU)

P_AaGL=         $(HOME)/wrk/opengl
I_AaGL=         -I$(P_AaGL)/include    $(I_GLEW) $(I_AaToolkit)
L_AaGL=         -L$(P_AaGL)/lib -lAaGL $(L_GLEW) $(L_AaToolkit)

# AaR3d ########################################################################

P_AaR3d=        $(HOME)/wrk/r3d
I_AaR3d=        -I$(P_AaR3d)/include     $(I_AaGL)
L_AaR3d=        -L$(P_AaR3d)/lib -lAaR3d $(L_AaGL)

################################################################################

I_Qt4=          -I/usr/include/qt4\
                -I/usr/include/qt4/QtCore\
                -I/usr/include/qt4/QtGui\
                -I/usr/include/qt4/QtXml\
                -I/usr/include/qt4/QtOpenGL
L_Qt4=          -L/usr/lib/x86_64-linux-gnu -lQtCore -lQtGui -lQtOpenGL -lQtXml

P_QGLViewer=    $(HOME)/3rdPartyLibs/libQGLViewer-2.3.14
I_QGLViewer=    -I$(P_QGLViewer)                       $(I_Qt4)
L_QGLViewer=    -L$(P_QGLViewer)/QGLViewer -lQGLViewer $(L_Qt4)

################################################################################
# Règles #######################################################################
################################################################################

EXE=      b8
HDR=      
SRC=      
OBJ=      $(HDR:%.h=%.moc.o) \
          $(SRC:%.cc=%.o)
ALL=      make.depend $(EXE)

all:		$(ALL)

#r3d:		r3d.o $(OBJ) ../lib/libAaR3d.so
#			$(GCC) $< $(OBJ) $(L_QGLViewer) $(L_AaR3d) $(L_AaFFmpeg) -o $@

b8:		b8.o ../lib/libAaR3d.so
			$(GCC) $< $(L_QGLViewer) $(L_AaR3d) $(L_AaFFmpeg) -o $@

#lut:		lut.o QLutEditor.moc.o QLutEditor.o ../lib/libAaR3d.so
#			$(GCC) $< QLutEditor.moc.o QLutEditor.o $(L_QGLViewer) $(L_AaR3d) $(L_AaFFmpeg) -o $@

%.o:			%.cc
			$(GCC) -c $(I_QGLViewer) $(I_AaR3d) $(I_AaFFmpeg) $< -o $@

%.moc.cc:			%.h
			$(MOC) $< -o $@

################################################################################
# Ménage #######################################################################
################################################################################

clean:
			rm -f $(EXE:%=%.o) $(HDR:%.h=%.moc.cc) $(OBJ) $(ALL)
			rm -f core *~ gmon.* a.out

################################################################################
# Dépendances ##################################################################
################################################################################

dep:
			g++ -MM $(I_QGLViewer) $(I_AaR3d) $(I_AaFFmpeg) $(SRC) $(EXE:%=%.cc) >make.depend

make.depend:		$(SRC) $(EXE:%=%.cc) Makefile
			g++ -MM $(I_QGLViewer) $(I_AaR3d) $(I_AaFFmpeg) $(SRC) $(EXE:%=%.cc) >make.depend

include make.depend

