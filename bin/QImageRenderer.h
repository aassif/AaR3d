#ifndef __R3D_QIMAGE_RENDERER__
#define __R3D_QIMAGE_RENDERER__

//#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaReferenceFrame>

//#include <QMainWindow>
//#include <QMenuBar>
//#include <QDockWidget>
//#include <QApplication>
#include <GL/glew.h>
//#include <QKeyEvent>
#include <QGLFramebufferObject>
//#include <QFileDialog>
#include <QGLViewer/qglviewer.h>
//#include "QLutEditor.h"
#include "R3dImageRenderer3dGLSL.h"

namespace Aa
{
  namespace R3d
  {

    class QImageRenderer : public QGLViewer
    {
      Q_OBJECT

      private:
        bool                       m_fast;
        const R3d::Image         * m_image;
        const R3d::Lut           * m_lut;
        R3d::ImageRenderer3dGLSL * m_renderer;
        QGLFramebufferObject     * m_fbo;
        QTimer                     m_timer;

      public:
        QImageRenderer (const R3d::Image *, const R3d::Lut *, QWidget * parent = NULL);
        virtual ~QImageRenderer ();

      protected:
        virtual void resizeGL (int w, int h);

      public slots:
        void setFast (bool);
        void setImage (const R3d::Image *);
        void setLut (const R3d::Lut *);

      public:
        virtual void init ();
        virtual void draw ();
        virtual void fastDraw ();
        virtual void keyPressEvent (QKeyEvent *);
    };

  } // namespace R3d
} // namespace Aa

#endif // __R3D_QIMAGE_RENDERER__

