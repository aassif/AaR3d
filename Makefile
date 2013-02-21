GCC_DEFAULT=      g++ -fPIC -W -Wall -pedantic
GCC_DEBUG=        $(GCC_DEFAULT) -O0 -ggdb3 -pg
GCC_RELEASE=      $(GCC_DEFAULT) -O3 -ggdb3
#GCC=              $(GCC_DEBUG)
GCC=              $(GCC_RELEASE)

# Chemins #

#L_GLU=            -lGL -lGLU -lglut

P_AaToolkit=      $(HOME)/wrk/toolkit
I_AaToolkit=      -I$(P_AaToolkit)/include

P_GLEW=           $(HOME)/3rdPartyLibs/glew-1.9.0
I_GLEW=           -I$(P_GLEW)/include
#L_GLEW=           $(L_GLU) -L$(P_GLEW)/lib -lglew

P_AaGL=           $(HOME)/wrk/opengl
I_AaGL=           -I$(P_AaGL)/include    $(I_GLEW)
#L_AaGL=           -L$(P_AaGL)/lib -lAaGL $(L_GLEW)

V_AaR3d=          AaR3d-0.0.2
S_AaR3d=          src
H_AaR3d=          include
I_AaR3d=          -I$(H_AaR3d)

# Règles #

HDR=              $(wildcard $(H_AaR3d)/*.h $(H_AaR3d)/*.hh)
SRC=              $(wildcard $(S_AaR3d)/*.cc)
OBJ=              $(SRC:%.cc=%.o)
ALL=              make.depend lib/libAaR3d.a lib/libAaR3d.so

all:                    $(ALL)
#			make -i -C bin

lib/libAaR3d.a:		$(OBJ)
			ar rcs lib/libAaR3d.a $(OBJ)

lib/libAaR3d.so:	$(OBJ)
			g++ -shared $(OBJ) -o lib/libAaR3d.so

%.o:                    %.cc
			$(GCC) -c $(I_AaR3d) $(I_AaGL) $(I_AaToolkit) $< -o $@

# Ménage #

clean:
			rm -f $(OBJ) $(ALL)
			make -C bin clean
#			find . -name "*~" -exec rm -f {} \;

archive:                clean
			tar zcvf ~/tgz/$(V_AaR3d).tgz *

# Dépendances #

dep:
			g++ -MM $(I_AaR3d) $(I_AaGL) $(I_AaToolkit) $(SRC) >make.depend

make.depend:            $(HDR) $(SRC)
			g++ -MM $(I_AaR3d) $(I_AaGL) $(I_AaToolkit) $(SRC) >make.depend

include make.depend

