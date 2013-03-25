//#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaReferenceFrame>
#include <AaFixedFunction>
//#include "R3dImageRenderer3dGLSL.h"

//#include <QMainWindow>
//#include <QMenuBar>
//#include <QDockWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QResource>
//#include <QFileDialog>
//#include <QGLViewer/qglviewer.h>
//#include "QLutEditor.h"
#include "QImageRenderer.h"

using namespace std;
using namespace Aa;

namespace Aa
{
  namespace R3d
  {

    inline std::string qresource_to_string (const QString & name)
    {
      QResource r (name);
      return std::string ((const char *) r.data (), r.size ());
    }

    inline QGLFormat qglformat_version (int major, int minor)
    {
      QGLFormat format;
      format.setVersion (major, minor);
      return format;
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QImageRenderer /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    QImageRenderer::QImageRenderer (QWidget * parent) :
      QGLViewer (parent),
      m_fast (false),
      m_image (NULL),
      m_texture (0),
      m_lut (NULL),
      m_renderer (NULL),
      m_fbo (NULL),
      m_timer (this)
    {
      m_timer.setInterval (40);
      m_timer.setSingleShot (true);
      connect (&m_timer, SIGNAL (timeout ()), this, SLOT (updateGL ()));
    }

    QImageRenderer::~QImageRenderer ()
    {
      //cout << ">>> " << __PRETTY_FUNCTION__ << endl;

      if (m_renderer != NULL)
        delete m_renderer;

      if (m_fbo != NULL)
        delete m_fbo;

      //cout << "<<< " << __PRETTY_FUNCTION__ << endl;
    }

    void QImageRenderer::setFast (bool f)
    {
      m_fast = f;
    }

    R3d::ImageRenderer3dGLSL * QImageRenderer::Factory (RendererType type)
    {
      switch (type)
      {
        case RENDERER_TEXTURE             : return new R3d::ImageRenderer3dGLSL;
        case RENDERER_POST_CLASSIFICATION : return new R3d::PostClassification;
        case RENDERER_PRE_INTEGRATION     : return new R3d::PreIntegration;
        case RENDERER_PHONG               : return new R3d::Phong;
        case RENDERER_RAINBOW             : return new R3d::PostClassification ("/Aa/R3d/Rainbow.fragment");
        case RENDERER_MIP                 : return new R3d::MIP (GL_MAX);
        case RENDERER_SURFACE             : return new R3d::Surface;
        default                           : return NULL;
      }
    }

    void QImageRenderer::setRenderer (RendererType type)
    {
      if (m_renderer != NULL)
      {
        delete m_renderer;
        m_renderer = NULL;
      }

      m_renderer = QImageRenderer::Factory (type);

      if (m_renderer != NULL)
      {
        m_renderer->setImage (m_texture);
        m_renderer->setLut (m_lut);
      }
    }

    void QImageRenderer::setImage (const R3d::Image * image)
    {
      if (m_image != NULL)
      {
        glDeleteTextures (1, &m_texture);
        m_texture = 0;
      }

      m_image = image;

      if (image != NULL)
      {
        // Texture.
        m_texture = image->glTex3d ();

        // Camera.
        vec3 p = image->position ();
        vec3 d = image->scale () * image->dims ();
        vec3 c = p + 0.5f * d;
        setSceneCenter (qglviewer::Vec (c[0], c[1], c[2]));
        setSceneRadius (0.5f * d.length ());
        showEntireScene ();
      }

      if (m_renderer != NULL)
      {
        m_renderer->setImage (m_texture);
        updateGL ();
      }
    }

    void QImageRenderer::setLut (const R3d::Lut * lut)
    {
      m_lut = lut;

      if (m_renderer != NULL)
      {
        m_renderer->setLut (lut);
        updateGL ();
      }
    }

    void QImageRenderer::init ()
    {
      glewInit ();
      //glClearColor (0.0, 0.0, 0.0, 0.0);
      glClearColor (1.0, 1.0, 1.0, 0.0);
      glEnable (GL_CULL_FACE);

      cout << glGetString (GL_VENDOR) << endl;
      cout << glGetString (GL_RENDERER) << endl;
      cout << glGetString (GL_VERSION) << endl;
      cout << glGetString (GL_EXTENSIONS) << endl;

      static const char * SHADERS [][2] =
      {
        {"/Aa/PassThru.vertex",                 ":/glsl/PassThru.VertexShader.glsl"},

        {"/Aa/Core",                            ":/glsl/Core.glsl"},
        {"/Aa/Core/Vertex",                     ":/glsl/core/Vertex.glsl"},
        {"/Aa/Core.vertex",                     ":/glsl/Core.VertexShader.glsl"},

        {"/Aa/FillColor.fragment",              ":/glsl/FillColor.FragmentShader.glsl"},
        {"/Aa/Phong",                           ":/glsl/Phong.glsl"},

        {"/Aa/R3d/Fragment",                    ":/glsl/R3d/Fragment.glsl"},

        {"/Aa/R3d/McSlicing.geometry",          ":/glsl/R3d/McSlicing.GeometryShader.glsl"},

        {"/Aa/R3d/Texture",                     ":/glsl/R3d/Texture.glsl"},
        {"/Aa/R3d/Texture.fragment",            ":/glsl/R3d/Texture.FragmentShader.glsl"},

        {"/Aa/R3d/PostClassification",          ":/glsl/R3d/PostClassification.glsl"},
        {"/Aa/R3d/PostClassification.fragment", ":/glsl/R3d/PostClassification.FragmentShader.glsl"},

        {"/Aa/R3d/PreIntegration",              ":/glsl/R3d/PreIntegration.glsl"},
        {"/Aa/R3d/PreIntegration.fragment",     ":/glsl/R3d/PreIntegration.FragmentShader.glsl"},

        {"/Aa/R3d/Gradient",                    ":/glsl/R3d/Gradient.glsl"},

        {"/Aa/R3d/Rainbow",                     ":/glsl/R3d/Rainbow.glsl"},
        {"/Aa/R3d/Rainbow.fragment",            ":/glsl/R3d/Rainbow.FragmentShader.glsl"},

        {"/Aa/R3d/Phong",                       ":/glsl/R3d/Phong.glsl"},
        {"/Aa/R3d/Phong.fragment",              ":/glsl/R3d/Phong.FragmentShader.glsl"},

        {"/Aa/R3d/Surface",                     ":/glsl/R3d/Surface.glsl"},
        {"/Aa/R3d/Surface.fragment",            ":/glsl/R3d/Surface.FragmentShader.glsl"}
      };

      for (AaUInt i = 0; i < sizeof (SHADERS) / sizeof (SHADERS [0]); ++i)
        GL::Program::SetString (SHADERS [i][0], qresource_to_string (SHADERS [i][1]));

#if 0
      for (AaUInt i = 0; i < 16; ++i)
      {
        std::cout << "////////////////////////////////////////////////////////////////////////////////" << endl
                  << SHADERS [i][0] << endl
                  << "////////////////////////////////////////////////////////////////////////////////" << endl
                  << SHADERS [i][1] << endl
                  << "////////////////////////////////////////////////////////////////////////////////" << endl
                  << GL::Program::String (SHADERS [i][0]) << endl;
      }
#endif

      // Renderer.
      this->setRenderer (RENDERER_PRE_INTEGRATION);
      //this->setRenderer (RENDERER_PHONG);
    }

    void QImageRenderer::resizeGL (int w, int h)
    {
      //cout << "QImageRenderer::resizeGL (" << w << ", " << h << ")\n";

      if (m_fbo != NULL)
      {
        delete m_fbo;
        m_fbo = NULL;
      }

      m_fbo = new QGLFramebufferObject (w, h, QGLFramebufferObject::Depth, GL_TEXTURE_2D, GL_RGBA32F);

      QGLViewer::resizeGL (w, h);
    }

    void QImageRenderer::draw ()
    {
      if (m_image != NULL && m_renderer != NULL && m_fbo != NULL)
      {
        if (m_fast || m_timer.isActive ())
          this->fastDraw ();
        else
        {
          m_fbo->bind ();
          glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          const qglviewer::Camera * camera = this->camera ();
          mat4 transform  = m_image->transform () * Scale (m_image->dims ());
          //cout << "transform = " << transform << endl;
          mat4 modelview  = GL::FixedFunction::ModelView  ();
          //cout << "modelview = " << modelview << endl;
          mat4 projection = GL::FixedFunction::Projection ();
          //cout << "projection = " << projection << endl;
          vec2 range      = vec<float> (camera->zNear (), camera->zFar ());
          m_renderer->glDraw (GL::CoreContext (NULL,
                                               modelview * transform,
                                               projection,
                                               range,
                                               false));

          m_fbo->release ();
          QRect r = QRect (QPoint (0, 0), m_fbo->size ());
          QGLFramebufferObject::blitFramebuffer (0, r, m_fbo, r);

          GLenum e1 = glGetError ();
          if (e1 != GL_NO_ERROR) cerr << gluErrorString (e1) << endl;
        }
      }
    }

    void QImageRenderer::fastDraw ()
    {
      m_timer.stop ();
      if (m_image != NULL && m_renderer != NULL && m_fbo != NULL)
      {
        m_fbo->bind ();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const qglviewer::Camera * camera = this->camera ();
        mat4 transform  = m_image->transform () * Scale (m_image->dims ());
        mat4 modelview  = GL::FixedFunction::ModelView  ();
        mat4 projection = GL::FixedFunction::Projection ();
        vec2 range      = vec<float> (camera->zNear (), camera->zFar ());
        m_renderer->glDraw (GL::CoreContext (NULL,
                                             modelview * transform,
                                             projection,
                                             range,
                                             true));

        m_fbo->release ();
        QRect r = QRect (QPoint (0, 0), m_fbo->size ());
        QGLFramebufferObject::blitFramebuffer (0, r, m_fbo, r);
      }
      m_timer.start ();
    }

    void QImageRenderer::keyPressEvent (QKeyEvent * e)
    {
      switch (e->key ())
      {
#if 0
        case Qt::Key_Space:
          m_renderer->setLighting (! m_renderer->lighting ());
          updateGL ();
          break;
#endif

        case Qt::Key_0: this->setRenderer (RENDERER_TEXTURE);             updateGL (); break;
        case Qt::Key_1: this->setRenderer (RENDERER_POST_CLASSIFICATION); updateGL (); break;
        case Qt::Key_2: this->setRenderer (RENDERER_PRE_INTEGRATION);     updateGL (); break;
        case Qt::Key_3: this->setRenderer (RENDERER_PHONG);               updateGL (); break;
        case Qt::Key_4: this->setRenderer (RENDERER_MIP);                 updateGL (); break;
        case Qt::Key_5: this->setRenderer (RENDERER_RAINBOW);             updateGL (); break;
        case Qt::Key_6: this->setRenderer (RENDERER_SURFACE);             updateGL (); break;

        case Qt::Key_Escape:
          qApp->exit ();
          break;
      }
    }

  } // namespace R3d
} // namespace Aa

