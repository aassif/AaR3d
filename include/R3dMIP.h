#ifndef R3D_MIP__H
#define R3D_MIP__H

#include <AaMeshVBO>
#include <AaMeshPrimitives>
#include <glsl/AaFillColor>
#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {
    namespace details
    {
      class ColorBox
      {
        private:
          vec4                 m_color;
          GLSL::FillColor      m_program;
          Mesh::BasicMeshVBO * m_vbo;

        public:
          ColorBox (const vec4 & c) :
            m_color (c),
            m_program ("/Aa/Core.vertex", "/Aa/FillColor.fragment"),
            m_vbo (NULL)
          {
            Mesh::BasicMesh * m = Mesh::BasicCube::Create ();
            m_vbo = new Mesh::BasicMeshVBO (m);
            delete m;
          }

          void glDraw (const GL::CoreContext & c)
          {
            m_program.use ();
            m_program.modelview  (c.modelview  ());
            m_program.projection (c.projection ());
            m_program.fill_color (m_color);
            m_vbo->draw ();
          }
      };
    }

    class AA_R3D_API MIP :
      public ImageRenderer3dGLSL
    {
      protected:
        GLenum m_mode;
        details::ColorBox m_box;
        
      public:
        static vec4 FillColor (GLenum mode);

      protected:
        virtual void glPreDraw  (const GL::CoreContext &);
        virtual void glPostDraw (const GL::CoreContext &);

      public:
        MIP (GLenum mode = GL_MAX);
        virtual ~MIP ();
    };
  } // namespace R3d
} // namespace Aa

#endif // R3D_MIP__H

