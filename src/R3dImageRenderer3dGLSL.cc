#include <fstream>
#include <cmath>
#include <AaGLSL>
#include <AaText>
#include "R3dImageRenderer3dGLSL.h"

#include <sstream> // glGetUniformLocation
#include <cstdlib> // GLSL

//#define R3D_GLSL_VERTEX
#define R3D_GLSL_GEOMETRY
#define R3D_GLSL_FRAGMENT

#define R3D_GLSL (defined (R3D_GLSL_GEOMETRY) || defined (R3D_GLSL_VERTEX) || defined (R3D_GLSL_FRAGMENT))

using namespace std;
using namespace Aa::Math;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {
    ImageRenderer3dGLSL::ImageRenderer3dGLSL (const Image * img, const Lut * lut) :
      ImageRenderer3d (img, lut),
      m_mode (2),
      m_program (),
      m_blinn (false),
      m_shininess (8.0),
      m_delta (0.01, 0.01, 0.01)
    {
      cout << "ImageRenderer3dGLSL (Powered by GLSL!)\n";

#if R3D_GLSL
  #ifdef R3D_GLSL_GEOMETRY
      m_program.attach (GL_VERTEX_SHADER,       "PassThru.VertexShader.glsl");
      m_program.attach (GL_GEOMETRY_SHADER_ARB, "McSlicing.GeometryShader.glsl");

      //GLint max;
      //glGetIntegerv (GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max);
      //cout << "max = " << max << endl;
      GLuint id = m_program.id ();
      glProgramParameteriEXT (id, GL_GEOMETRY_INPUT_TYPE_EXT,   GL_POINTS);
      glProgramParameteriEXT (id, GL_GEOMETRY_OUTPUT_TYPE_EXT,  GL_TRIANGLE_STRIP);
      glProgramParameteriEXT (id, GL_GEOMETRY_VERTICES_OUT_EXT, 6); // FIXME

      glGenTextures (1, &m_table);
      glBindTexture (GL_TEXTURE_2D, m_table);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE8I_EXT, 8, 256, 0, GL_LUMINANCE_INTEGER_EXT, GL_BYTE, TABLE);
  #else
    #ifdef R3D_GLSL_VERTEX
      m_program.attach (GL_VERTEX_SHADER, "McSlicing.VertexShader.glsl");
    #endif
  #endif

  #ifdef R3D_GLSL_FRAGMENT
      // Fragment shader.
      m_program.attach (GL_FRAGMENT_SHADER, "PostClassification+Blinn.FragmentShader.glsl");
  #endif

      m_program.link ();
      m_program.validate ();
#endif
    }

    ImageRenderer3dGLSL::~ImageRenderer3dGLSL ()
    {
    }

    void ImageRenderer3dGLSL::glSlice_ViewAligned_Cube (bool motion) const
    {
#ifdef R3D_GLSL_GEOMETRY
      GLfloat m3d [16];
      glGetFloatv (GL_MODELVIEW_MATRIX, m3d);

      // Bounding box depth per vertex.
      GLfloat vertex_depths [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vertex_depths [i] = (i & 1 ? m3d [ 2] : 0)
                          + (i & 2 ? m3d [ 6] : 0)
                          + (i & 4 ? m3d [10] : 0) + m3d [14];

      // Maximum and minimum depths.
      GLfloat zMin = vertex_depths [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = vertex_depths [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      GLfloat edge_depths [12][2];
      for (int i = 0; i < 12; ++i)
      {
        edge_depths [i][0] = vertex_depths [EDGES [i][0]];
        edge_depths [i][1] = vertex_depths [EDGES [i][1]];
      }

      glActiveTexture (GL_TEXTURE3);
      glBindTexture (GL_TEXTURE_2D, m_table);
      m_program.set<GLint>      ("mc_slicing_table",         3);
      m_program.set<GLfloat, 1> ("mc_slicing_vertex_depths", 8, (const GLfloat *) vertex_depths);
      m_program.set<GLfloat, 2> ("mc_slicing_edge_depths",  12, (const GLfloat *) edge_depths);

      // Back-to-Front.
      glBegin (GL_POINTS);
      GLfloat dz = m_steps [motion ? 1 : 0];
      GLfloat z0 = zMin + 0.5 * fmod (zMax - zMin, dz);
      for (GLfloat z = z0; z <= zMax; z += dz)
        glVertexAttrib1f (0, z);
      glEnd ();
#else
  #ifdef R3D_GLSL_VERTEX
      GLfloat m3d [16];
      glGetFloatv (GL_MODELVIEW_MATRIX, m3d);

      // Bounding box depth per vertex.
      GLfloat vertex_depths [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vertex_depths [i] = (i & 1 ? m3d [ 2] : 0)
                          + (i & 2 ? m3d [ 6] : 0)
                          + (i & 4 ? m3d [10] : 0) + m3d [14];

      // Maximum and minimum depths.
      GLfloat zMin = vertex_depths [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = vertex_depths [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      GLfloat edge_depths [12][2];
      for (int i = 0; i < 12; ++i)
      {
        edge_depths [i][0] = vertex_depths [EDGES [i][0]];
        edge_depths [i][1] = vertex_depths [EDGES [i][1]];
      }

      m_program.set<GLfloat, 2> ("mc_slicing_edge_depths", 12, (const GLfloat *) edge_depths);

      GLint mc_slicing_z = m_program.location ("mc_slicing_z");
      if (mc_slicing_z == -1) {cerr << "mc_slicing_z" << endl; exit (1);}

      // Back-to-Front.
      GLfloat dz = m_steps [motion ? 1 : 0];
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, dz); z <= zMax; z += dz)
        {
          glUniform1fARB (mc_slicing_z, z);

          // This bitfield tells us which vertices of the bounding box
          // have already been passed by the "clipping" plane.
          int tableIndex = 0;
          for (int k = 8; k--;)
            if (vertex_depths [k] < z)
              tableIndex |= (1 << k);

          const EdgeIndex * t = TABLE [tableIndex];
          if (t[0] != -1)
            {
              glBegin (m_outlined ? GL_LINE_STRIP : GL_TRIANGLE_STRIP);

              // At least 3 vertices.
              glVertexAttrib1s (0, t[0]);
              glVertexAttrib1s (0, t[1]);
              glVertexAttrib1s (0, t[2]);
              // And maybe more...
              for (int k = 3; t[k] != -1;) glVertexAttrib1s (0, t[k++]);

              glEnd ();
            }
        }
  #else
      ImageRenderer3d::glSlice_ViewAligned_Cube (motion);
  #endif
#endif
    }

    void ImageRenderer3dGLSL::glPreDraw (bool motion) const
    {
#if R3D_GLSL
      // Enable GLSL program.
      m_program.use ();
#endif

#ifdef R3D_GLSL_FRAGMENT
      m_program.set<GLint>   ("mode",            m_mode);
      m_program.set<GLint>   ("image3d",         0);
      m_program.set<GLint>   ("lut1d",           1);
      m_program.set<GLint>   ("lut2d",           2);
      m_program.set<GLfloat> ("mc_slicing_step", m_steps [motion ? 1 : 0]);
      m_program.set<GLint>   ("blinn",           !motion && m_blinn);
      m_program.set<GLfloat> ("shininess",       m_shininess);
      m_program.set<GLfloat> ("delta",           m_delta.x, m_delta.y, m_delta.z);
#else
      glActiveTexture (GL_TEXTURE0);
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glEnable (GL_TEXTURE_3D);
#endif
    }

    void ImageRenderer3dGLSL::glPostDraw (bool) const
    {
#if R3D_GLSL
      glUseProgram (0);
#endif
      //glDisable (GL_TEXTURE_1D);
      //glDisable (GL_TEXTURE_2D);
      //glDisable (GL_TEXTURE_3D);
      //glActiveTexture (GL_TEXTURE0);
    }
  } // namespace R3d
} // namespace Aa

