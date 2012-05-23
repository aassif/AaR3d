#ifndef AA_R3D_SLICER__H
#define AA_R3D_SLICER__H

#include <AaGLSL>
#include "AaR3d.h"

namespace Aa
{
  namespace R3d
  {
    class Slicer
    {
      public:
        typedef GLbyte VertexIndex;
        typedef GLbyte EdgeIndex;

      public:
        Slicer ();
        virtual ~Slicer ();
        virtual void glDraw (GLfloat step) = 0;
    };

    class SlicerObjectX : public Slicer
    {
      private:
        GLfloat m_dx;

      public:
        SlicerObjectX (GLfloat dx);
        virtual ~SlicerObjectX ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerObjectY : public Slicer
    {
      private:
        GLfloat m_dy;

      public:
        SlicerObjectY (GLfloat dy);
        virtual ~SlicerObjectY ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerObjectZ : public Slicer
    {
      private:
        GLfloat m_dz;

      public:
        SlicerObjectZ (GLfloat dz);
        virtual ~SlicerObjectZ ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerViewNaive : public Slicer
    {
      public:
        SlicerViewNaive ();
        virtual ~SlicerViewNaive ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerViewTetra : public Slicer
    {
      private:
        static void Edge  (const vec3 & v0, const vec3 & v1, GLfloat z0, GLfloat z1, GLfloat z);
        static void Tetra (const vec3 V [8], const GLfloat Z [8], GLfloat z, int i0, int i1, int i2, int i3);

      public:
        SlicerViewTetra ();
        virtual ~SlicerViewTetra ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerViewCube : public Slicer
    {
      public:
        static const EdgeIndex TABLE [256][8];

      public:
        SlicerViewCube ();
        virtual ~SlicerViewCube ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerViewCubeGLSLv1 : public SlicerViewCube
    {
      private:
        Aa::GL::Program * m_program;

      public:
        SlicerViewCubeGLSLv1 (Aa::GL::Program *,
                              const std::string & source);
        virtual ~SlicerViewCubeGLSLv1 ();
        virtual void glDraw (GLfloat step);
    };

    class SlicerViewCubeGLSLv2 : public SlicerViewCube
    {
      private:
        Aa::GL::Program * m_program;
        GLuint            m_table_tex2d;

      public:
        SlicerViewCubeGLSLv2 (Aa::GL::Program *,
                              const std::string & source_vertex,
                              const std::string & source_geometry);
        virtual ~SlicerViewCubeGLSLv2 ();
        virtual void glDraw (GLfloat step);
    };

#if 0
    class SlicerManager : public Slicer
    {
      public:
        enum SlicerType
        {
          SLICER_NONE,
          // object-aligned slices.
          SLICER_OBJECT_X,
          SLICER_OBJECT_Y,
          SLICER_OBJECT_Z,
          // view-aligned slices.
          SLICER_VIEW_NAIVE,
          SLICER_VIEW_TETRA,
          SLICER_VIEW_CUBE,
          SLICER_VIEW_CUBE_GLSL
        };

      public:
        static SlicerType  Next (SlicerType);
        static std::string Name (SlicerType);

      public:
        SlicerType m_type;
        Slicer   * m_slicer;

      public:
        SlicerManager ();
        virtual ~SlicerManager ();
        virtual void glDraw (GLfloat step);

        void next ();
        std::string name () const;
    };
#endif

  } // namespace R3d
} // namespace Aa

#endif // AA_R3D_SLICER__H

