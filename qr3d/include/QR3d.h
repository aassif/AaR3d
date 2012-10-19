#ifndef __R3D_QR3D__
#define __R3D_QR3D__

//#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaGlutWatermark>
//#include <AaReferenceFrame>
//#include "R3dImageRenderer3dGLSL.h"

#include <QMainWindow>
//#include <QMenuBar>
#include <QDockWidget>
//#include <QApplication>
//#include <QKeyEvent>
//#include <QFileDialog>
//#include <QGLViewer/qglviewer.h>
#include "QLutEditor.h"
#include "QImageRenderer.h"

namespace Aa
{
  namespace R3d
  {

    class QR3d : public QMainWindow
    {
      Q_OBJECT

      private:
        QImageRenderer * m_renderer;
        QLutEditor     * m_editor;
        QDockWidget    * m_editor_dock;
        R3d::Image     * m_image;
        R3d::Lut       * m_lut;

      public:
        QR3d ();
        virtual ~QR3d ();

      public slots:
        void openImage ();
        void setImage (R3d::Image *);
        void setImage (const QString & path, const dvec3 & scale = dvec3 (1));

        void openLut ();
        void saveLut ();
        void setLut (R3d::Lut *);
        void setLut (const QVector<QColor> &);
        void setLut (const QString & path);
    };

  } // namespace R3d
} // namespace Aa

#endif // __R3D_QR3D__

