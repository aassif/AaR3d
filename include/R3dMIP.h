#ifndef R3D_MIP__H
#define R3D_MIP__H

#include <AaMeshVBO>
#include <AaMeshPrimitives>
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
          GL::Program          m_program;
          Mesh::BasicMeshVBO * m_vbo;

        public:
          ColorBox (const vec4 & c) :
            m_color (c),
            m_program (),
            m_vbo (NULL)
          {
            m_program.attach (GL_VERTEX_SHADER,   "/Aa/Core.vertex");
            m_program.attach (GL_FRAGMENT_SHADER, "/Aa/FillColor.fragment");
            m_program.link ();

            Mesh::BasicMesh * m = Mesh::BasicCube::Create ();
            m_vbo = new Mesh::BasicMeshVBO (m);
            delete m;
          }

          void glDraw (const GL::CoreContext & context) const
          {
            m_program.use ();
            m_program.set<GLfloat, 4, 4> ("aa_gl_modelview",  context.modelview  ());
            m_program.set<GLfloat, 4, 4> ("aa_gl_projection", context.projection ());
            m_program.set<GLfloat>       ("aa_gl_fill_color", m_color);
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

