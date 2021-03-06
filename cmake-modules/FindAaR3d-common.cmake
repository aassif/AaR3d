# Library.
find_library(AAR3D_LIBRARY_RELEASE
    NAMES   ${AAR3D_LIBRARY_BASE_NAME}
            ${AAR3D_LIBRARY_BASE_NAME}-x64
    PATHS   "${AAR3D_PATH}/lib/${GENERATOR_DIR}/${ARCHITECTURE_DIR}"
)

find_library(AAR3D_LIBRARY_DEBUG
    NAMES   ${AAR3D_LIBRARY_BASE_NAME}d
            ${AAR3D_LIBRARY_BASE_NAME}d-x64
    PATHS   "${AAR3D_PATH}/lib/${GENERATOR_DIR}/${ARCHITECTURE_DIR}/debug"
)

if(NOT AAR3D_LIBRARY_DEBUG)
    set(AAR3D_LIBRARY_DEBUG "AAR3D_LIBRARY_DEBUG-NOPATH")
endif(NOT AAR3D_LIBRARY_DEBUG)

set(AAR3D_LIBRARY
    debug     "${AAR3D_LIBRARY_DEBUG}"
    optimized "${AAR3D_LIBRARY_RELEASE}"
)

# Include.
find_path(AAR3D_INCLUDE_DIR
    NAMES
    AaR3d.h
    R3dImage.h
    R3dImageRenderer.h
    R3dImageRenderer2d.h
    R3dImageRenderer3d.h
    R3dImageRenderer3dGLSL.h
    R3dLut.h
    R3dMIP.h
    R3dPadding.h
    R3dPhong.h
    R3dPostClassification.h
    R3dPreIntegration.h
    R3dRainbow.h
    R3dSlicer.h
    PATHS
    ${AAR3D_PATH}/include
)

if(AAR3D_INCLUDE_DIR AND AAR3D_LIBRARY)
    set(AAR3D_FOUND TRUE)
endif(AAR3D_INCLUDE_DIR AND AAR3D_LIBRARY)

if(AAR3D_FOUND)
    if(NOT AAR3D_FIND_QUIETLY)
        message(STATUS "Found AaR3d : ${AAR3D_LIBRARY}")
    endif(NOT AAR3D_FIND_QUIETLY)
else(AAR3D_FOUND)
    if(AAR3D_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find AaR3d")
    endif(AAR3D_FIND_REQUIRED)
endif(AAR3D_FOUND)
