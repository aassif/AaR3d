#ifndef __AA_R3D__
#define __AA_R3D__

#if defined( _WIN32 ) && defined( AA_R3D_BUILD_SHARED )
  #ifdef AaR3d_EXPORTS // Merci CMake :)
    #define AA_R3D_API    __declspec(dllexport)
    #define AA_R3D_EXTERN
  #else
    #define AA_R3D_API    __declspec(dllimport)
    #define AA_R3D_EXTERN extern
  #endif
#else
  #define AA_R3D_API
  #define AA_R3D_EXTERN
#endif

#endif // __AA_R3D__

