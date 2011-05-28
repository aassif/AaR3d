#ifndef __R3D_IMAGE_RENDERER_3D_GLSL__
#define __R3D_IMAGE_RENDERER_3D_GLSL__

#include <AaGLSL>
#include "R3dImageRenderer3d.h"

namespace Aa
{
  namespace R3d
  {
    class ImageRenderer3dGLSL : public ImageRenderer3d
    {
      protected:
        int m_mode;
        GL::Program m_program;
        GLuint m_table;
        bool m_blinn;
        float m_shininess;
        Math::vR3 m_delta;
        
      protected:
        virtual void glSlice_ViewAligned_Cube (bool) const;

      public:
        ImageRenderer3dGLSL (const Image * = NULL, const Lut * = NULL);
        virtual ~ImageRenderer3dGLSL ();
        int mode () const {return m_mode;}
        void setMode (int m) {m_mode = m;}
        bool lighting () const {return m_blinn;}
        void setLighting (bool b) {m_blinn = b;}
        virtual void glPreDraw (bool = false) const;
        virtual void glPostDraw (bool = false) const;

        // glUniform variables.
        template <typename T> static bool SetUniform (int location,                      T v1);
        template <typename T> static bool SetUniform (GLuint program, const char * name, T v1);
        template <typename T> static bool SetUniform (int location,                      T v1, T v2);
        template <typename T> static bool SetUniform (GLuint program, const char * name, T v1, T v2);
        template <typename T> static bool SetUniform (int location,                      T v1, T v2, T v3);
        template <typename T> static bool SetUniform (GLuint program, const char * name, T v1, T v2, T v3);
        template <typename T> static bool SetUniform (int location,                      T v1, T v2, T v3, T v4);
        template <typename T> static bool SetUniform (GLuint program, const char * name, T v1, T v2, T v3, T v4);
        // glUniform arrays.
        template <typename T, int N> static bool SetUniform (int location,                      int count, const T * values);
        template <typename T, int N> static bool SetUniform (GLuint program, const char * name, int count, const T * values);
        // glUniform matrices.
        template <typename T, int N, int M> static bool SetUniform (int location,                      const T * values, int count = 1);
        template <typename T, int N, int M> static bool SetUniform (GLuint program, const char * name, const T * values, int count = 1);
    };
  } // namespace R3d
} // namespace Aa

namespace Aa
{
  namespace R3d
  {
#define AA_GLSL_UNIFORM1(T, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T> (int location, T v1)\
    {\
      F (location, v1);\
      return true;\
    }

    AA_GLSL_UNIFORM1 (GLint,   glUniform1i)
    AA_GLSL_UNIFORM1 (GLfloat, glUniform1f)

#undef AA_GLSL_UNIFORM1

    template <typename T>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, T v1)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T> (location, v1) : false;
    }

#define AA_GLSL_UNIFORM2(T, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T> (int location, T v1, T v2)\
    {\
      F (location, v1, v2);\
      return true;\
    }

    AA_GLSL_UNIFORM2 (GLint,   glUniform2i)
    AA_GLSL_UNIFORM2 (GLfloat, glUniform2f)

#undef AA_GLSL_UNIFORM2

    template <typename T>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, T v1, T v2)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T> (location, v1, v2) : false;
    }

#define AA_GLSL_UNIFORM3(T, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T> (int location, T v1, T v2, T v3)\
    {\
      F (location, v1, v2, v3);\
      return true;\
    }

    AA_GLSL_UNIFORM3 (GLint,   glUniform3i)
    AA_GLSL_UNIFORM3 (GLfloat, glUniform3f)

#undef AA_GLSL_UNIFORM3

    template <typename T>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, T v1, T v2, T v3)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T> (location, v1, v2, v3) : false;
    }

#define AA_GLSL_UNIFORM4(T, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T> (int location, T v1, T v2, T v3, T v4)\
    {\
      F (location, v1, v2, v3, v4);\
      return true;\
    }

    AA_GLSL_UNIFORM4 (GLint,   glUniform4i)
    AA_GLSL_UNIFORM4 (GLfloat, glUniform4f)

#undef AA_GLSL_UNIFORM4

    template <typename T>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, T v1, T v2, T v3, T v4)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T> (location, v1, v2, v3, v4) : false;
    }

#define AA_GLSL_UNIFORM_ARRAY(T, N, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T, N> (int location, int count, const T * values)\
    {\
      F (location, count, values);\
      return true;\
    }

    AA_GLSL_UNIFORM_ARRAY (GLint,   1, glUniform1iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 1, glUniform1fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   2, glUniform2iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 2, glUniform2fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   3, glUniform3iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 3, glUniform3fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   4, glUniform4iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 4, glUniform4fv)

#undef AA_GLSL_UNIFORM_ARRAY

    template <typename T, int N>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, int count, const T * values)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T, N> (location, count, values) : false;
    }

#define AA_GLSL_UNIFORM_MATRIX(T, N, M, F) \
    template <>\
    bool ImageRenderer3dGLSL::SetUniform<T, N, M> (int location, const T * values, int count)\
    {\
      F (location, count, GL_FALSE, values);\
      return true;\
    }

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 2, glUniformMatrix2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 3, glUniformMatrix2x3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 4, glUniformMatrix2x4fv)

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 2, glUniformMatrix3x2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 3, glUniformMatrix3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 4, glUniformMatrix3x4fv)

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 2, glUniformMatrix4x2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 3, glUniformMatrix4x3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 4, glUniformMatrix4fv)

#undef AA_GLSL_UNIFORM_MATRIX

    template <typename T, int N, int M>
    bool ImageRenderer3dGLSL::SetUniform (GLuint program, const char * name, const T * values, int count)
    {
      GLint location = glGetUniformLocation (program, name);
      return (location != -1) ? SetUniform<T, N, M> (location, values, count) : false;
    }
  } // namespace R3d
} // namespace Aa

#endif // __R3D_IMAGE_RENDERER_3D_GLSL__

