//#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaReferenceFrame>
//#include "R3dImageRenderer3dGLSL.h"

//#include <QMainWindow>
//#include <QMenuBar>
//#include <QDockWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
//#include <QFileDialog>
//#include <QGLViewer/qglviewer.h>
//#include "QLutEditor.h"
#include "QImageRenderer.h"

using namespace std;
using namespace Aa;
//using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    inline string resource (const QString & name)
    {
      QFile file (name);
      file.open (QFile::ReadOnly);
      QByteArray bytes = file.readAll ();
      return bytes.data ();
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
        case RENDERER_BLINN               : return new R3d::Blinn;
        case RENDERER_RAINBOW             : return new R3d::Rainbow;
        case RENDERER_MIP                 : return new R3d::MIP (GL_MAX);
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
        {"/AaR3d/PassThru.vertex",             ":/glsl/PassThru.VertexShader.glsl"},

        {"/AaR3d/McSlicing.geometry",          ":/glsl/McSlicing.GeometryShader.glsl"},

        {"/AaR3d/Texture3d",                   ":/glsl/Texture3d.glsl"},
        {"/AaR3d/Texture3d.fragment",          ":/glsl/Texture3d.FragmentShader.glsl"},

        {"/AaR3d/PostClassification",          ":/glsl/PostClassification.glsl"},
        {"/AaR3d/PostClassification.fragment", ":/glsl/PostClassification.FragmentShader.glsl"},

        {"/AaR3d/PreIntegration",              ":/glsl/PreIntegration.glsl"},
        {"/AaR3d/PreIntegration.fragment",     ":/glsl/PreIntegration.FragmentShader.glsl"},

        {"/AaR3d/Blinn",                       ":/glsl/Blinn.glsl"},
        {"/AaR3d/Blinn.fragment",              ":/glsl/Blinn.FragmentShader.glsl"},

        {"/AaR3d/Rainbow",                     ":/glsl/Rainbow.glsl"},
        {"/AaR3d/Rainbow.fragment",            ":/glsl/Rainbow.FragmentShader.glsl"}
      };

      for (AaUInt i = 0; i < 12; ++i)
        GL::Program::SetString (SHADERS [i][0], resource (SHADERS [i][1]));

      // Renderer.
      this->setRenderer (RENDERER_PRE_INTEGRATION);
      //this->setRenderer (RENDERER_BLINN);
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
        m_fbo->bind ();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_fast || m_timer.isActive ())
          this->fastDraw ();
        else
        {
          glPushMatrix ();
          GL::MultMatrix (m_image->transform ());
          m_renderer->glDraw (false);
          glPopMatrix ();
        }

        m_fbo->release ();
        QRect r = QRect (QPoint (0, 0), m_fbo->size ());
        QGLFramebufferObject::blitFramebuffer (0, r, m_fbo, r);
      }
    }

    void QImageRenderer::fastDraw ()
    {
      if (m_image != NULL && m_renderer != NULL && m_fbo != NULL)
      {
        m_fbo->bind ();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_timer.stop ();

        glPushMatrix ();
        GL::MultMatrix (m_image->transform ());
        m_renderer->glDraw (true);
        glPopMatrix ();

        m_timer.start ();

        m_fbo->release ();
        QRect r = QRect (QPoint (0, 0), m_fbo->size ());
        QGLFramebufferObject::blitFramebuffer (0, r, m_fbo, r);
      }
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
        case Qt::Key_3: this->setRenderer (RENDERER_BLINN);               updateGL (); break;
        case Qt::Key_4: this->setRenderer (RENDERER_MIP);                 updateGL (); break;
        case Qt::Key_5: this->setRenderer (RENDERER_RAINBOW);             updateGL (); break;

        case Qt::Key_Escape:
          qApp->exit ();
          break;
      }
    }

  } // namespace R3d
} // namespace Aa

