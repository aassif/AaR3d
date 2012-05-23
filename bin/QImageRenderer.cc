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
//#include <QFileDialog>
//#include <QGLViewer/qglviewer.h>
//#include "QLutEditor.h"
#include "QImageRenderer.h"

using namespace std;
using namespace Aa;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QImageRenderer /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    QImageRenderer::QImageRenderer (const R3d::Image * image,
                                    const R3d::Lut   * lut,
                                    QWidget          * parent) :
      QGLViewer (parent),
      m_fast (false),
      m_image (image),
      m_lut (lut),
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

    void QImageRenderer::setRenderer (RendererType type)
    {
      if (m_renderer != NULL)
      {
        delete m_renderer;
        m_renderer = NULL;
      }

      string vertex   = GL::Program::ReadSource ("shaders/PassThru.VertexShader.glsl");
      string geometry = GL::Program::ReadSource ("shaders/McSlicing.GeometryShader.glsl");
      string fragment;

      switch (type)
      {
        case RENDERER_TEXTURE :
          fragment = GL::Program::ReadSource ("shaders/Texture3d.FragmentShader.glsl");
          m_renderer = new R3d::ImageRenderer3dGLSL (vertex, geometry, fragment);
          break;

        case RENDERER_POST_CLASSIFICATION:
          fragment = GL::Program::ReadSource ("shaders/PostClassification.FragmentShader.glsl");
          m_renderer = new R3d::PostClassification (vertex, geometry, fragment);
          break;

        case RENDERER_PRE_INTEGRATION:
          fragment = GL::Program::ReadSource ("shaders/PreIntegration.FragmentShader.glsl");
          m_renderer = new R3d::PreIntegration (vertex, geometry, fragment);
          break;

        case RENDERER_BLINN:
          fragment = GL::Program::ReadSource ("shaders/Blinn.FragmentShader.glsl");
          m_renderer = new R3d::Blinn (vertex, geometry, fragment);
          break;

        case RENDERER_RAINBOW:
          fragment = GL::Program::ReadSource ("shaders/Rainbow.FragmentShader.glsl");
          m_renderer = new R3d::Rainbow (vertex, geometry, fragment);
          break;

        case RENDERER_MIP:
          fragment = GL::Program::ReadSource ("shaders/Texture3d.FragmentShader.glsl");
          m_renderer = new R3d::MIP (vertex, geometry, fragment, GL_MAX);
          break;

        default:
          break;
      }

      if (m_renderer != NULL)
      {
        m_renderer->setImg (m_image);
        m_renderer->setLut (m_lut);
      }
    }

    void QImageRenderer::setImage (const R3d::Image * image)
    {
      m_image = image;
      if (image != NULL)
      {
        // Camera.
        const dbox3 & box = image->box ();
        const dvec3 & pos = box.pos ();
        const dvec3 & dim = box.dim ();
        setSceneBoundingBox (qglviewer::Vec (pos[0],          pos[1],          pos[2]),
                             qglviewer::Vec (pos[0] + dim[0], pos[1] + dim[1], pos[2] + dim[2]));

        showEntireScene ();
      }

      if (m_renderer != NULL)
      {
        m_renderer->setImg (image);
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
      glClearColor (0.0, 0.0, 0.0, 0.0);
      //glClearColor (1.0, 1.0, 1.0, 0.0);
      glEnable (GL_CULL_FACE);

      cout << glGetString (GL_VENDOR) << endl;
      cout << glGetString (GL_RENDERER) << endl;
      cout << glGetString (GL_VERSION) << endl;
      cout << glGetString (GL_EXTENSIONS) << endl;

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
      if (m_renderer != NULL && m_fbo != NULL)
      {
        m_fbo->bind ();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_fast || m_timer.isActive ())
          this->fastDraw ();
        else
          m_renderer->glDraw (false);

        m_fbo->release ();
        QRect r = QRect (QPoint (0, 0), m_fbo->size ());
        QGLFramebufferObject::blitFramebuffer (0, r, m_fbo, r);
      }
    }

    void QImageRenderer::fastDraw ()
    {
      if (m_renderer != NULL)
      {
        m_timer.stop ();
        m_renderer->glDraw (true);
        m_timer.start ();
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

