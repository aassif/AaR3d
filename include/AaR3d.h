#ifndef __AA_R3D__
#define __AA_R3D__

<<<<<<< HEAD
#if defined( _WIN32 ) && defined( AA_R3D_BUILD_SHARED )
=======
#if defined (_WIN32) && defined (AA_R3D_BUILD_SHARED)
>>>>>>> e9b9ed37e179889ee5e9bd4344bbbf2922a89142
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

