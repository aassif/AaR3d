//#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaReferenceFrame>
//#include "R3dImageRenderer3dGLSL.h"

//#include <QMainWindow>
//#include <QMenuBar>
//#include <QDockWidget>
#include <QApplication>
//#include <QKeyEvent>
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
      m_image (image),
      m_lut (lut),
      m_renderer (NULL)
    {
    }

    QImageRenderer::~QImageRenderer ()
    {
      //cout << ">>> " << __PRETTY_FUNCTION__ << endl;

      if (m_renderer != NULL)
        delete m_renderer;

      //cout << "<<< " << __PRETTY_FUNCTION__ << endl;
    }

    void QImageRenderer::setImage (const R3d::Image * image)
    {
      m_image = image;
      if (image != NULL)
      {
        // Camera.
        const Box & box = image->box ();
        const pR3 & pos = box.pos ();
        const vR3 & dim = box.dim ();
        setSceneBoundingBox (qglviewer::Vec (pos.x,         pos.y,         pos.z),
                             qglviewer::Vec (pos.x + dim.x, pos.y + dim.y, pos.z + dim.z));

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
      //glClearColor (0.0, 0.0, 0.0, 0.0);
      glClearColor (1.0, 1.0, 1.0, 0.0);
      glEnable (GL_CULL_FACE);

      // Renderer.
      m_renderer = new R3d::ImageRenderer3dGLSL (m_image, m_lut);
    }

    void QImageRenderer::draw ()
    {
      if (m_renderer != NULL)
        m_renderer->glDraw (false);
    }

    void QImageRenderer::fastDraw ()
    {
      if (m_renderer != NULL)
        m_renderer->glDraw (true);
    }

    void QImageRenderer::keyPressEvent (QKeyEvent * e)
    {
      switch (e->key ())
      {
        case Qt::Key_Space:
          m_renderer->setLighting (! m_renderer->lighting ());
          updateGL ();
          break;

        case Qt::Key_1: m_renderer->setMode (0); updateGL (); break;
        case Qt::Key_2: m_renderer->setMode (1); updateGL (); break;
        case Qt::Key_3: m_renderer->setMode (2); updateGL (); break;

        case Qt::Key_Escape:
          qApp->exit ();
          break;
      }
    }

  } // namespace R3d
} // namespace Aa

