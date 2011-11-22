#include <fstream>
//#include <AaText>
//#include <AaCmdLine>
//#include <AaGlutWatermark>
//#include <AaReferenceFrame>
//#include "R3dImageRenderer3dGLSL.h"

//#include <QMainWindow>
#include <QMenuBar>
//#include <QDockWidget>
#include <QApplication>
#include <QFileDialog>
#include "QR3d.h"

using namespace std;
using namespace Aa;
//using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    QR3d::QR3d (const std::string & image,
                const std::string & lut,
                const Math::vR3 & scale) :
      m_renderer (NULL),
      m_editor (NULL),
      m_editor_dock (NULL),
      m_image (NULL),
      m_lut (NULL)
    {
      m_renderer = new QImageRenderer (NULL, NULL);
      setCentralWidget (m_renderer);

      m_editor = new QLutEditor (0, 256);
      m_editor_dock = new QDockWidget (tr("QLutEditor"));
      m_editor_dock->setWidget (m_editor);
      addDockWidget (Qt::LeftDockWidgetArea, m_editor_dock);

      QMenuBar * bar = menuBar ();
      QMenu * menuFile = bar->addMenu (tr("&File"));
      menuFile->addAction (tr("&Open"), this, SLOT (openImage ()));
      menuFile->addAction (tr("&Quit"), qApp, SLOT (quit ()));
      QMenu * menuView = bar->addMenu (tr("&View"));
      menuView->addAction (m_editor_dock->toggleViewAction ());

      connect (m_editor, SIGNAL (computed (const QVector<QColor> &)),
               this,     SLOT   (setLut   (const QVector<QColor> &)));

      setImage (image, scale);
      setLut (lut);
    }

    QR3d::~QR3d ()
    {
      //cout << ">>> " << __PRETTY_FUNCTION__ << endl;

      if (m_image != NULL)
      {
        //m_renderer->setImage (NULL);
        delete m_image;
        m_image = NULL;
      }

      if (m_lut != NULL)
      {
        //m_renderer->setLut (NULL);
        delete m_lut;
        m_lut = NULL;
      }

      //cout << "<<< " << __PRETTY_FUNCTION__ << endl;
    }

    void QR3d::openImage ()
    {
      static const QString title  = tr("");
      static const QString dir    = "/home/prof/aassif/data/import";
      static const QString filter = tr("Images (*.b8 DICOMDIR)");
      QString path = QFileDialog::getOpenFileName (this, title, dir, filter);
      if (! path.isEmpty ()) setImage (path);
    }

    void QR3d::setImage (R3d::Image * image, const Math::vR3 & scale)
    {
      if (m_image != NULL)
      {
        delete m_image;
        m_image = NULL;
      }

      if (image != NULL)
      {
        m_image = image;
        const Math::vR3 & d = m_image->box ().dim ();
        float dx = scale.x * d.x;
        float dy = scale.y * d.y;
        float dz = scale.z * d.z;
        m_image->setBox (Math::Box (Math::pR3 (-0.5 * dx, -0.5 * dy, -0.5 * dz), Math::vR3 (dx, dy, dz)));
      }

      m_renderer->setImage (m_image);
      m_renderer->updateGL ();
    }

    void QR3d::setImage (const std::string & path, const Math::vR3 & scale)
    {
      R3d::Image * image = R3d::ImageLoadB8 (path);
      setImage (image, scale);
    }

    void QR3d::setImage (const QString & path, const Math::vR3 & scale)
    {
      setImage (path.toStdString (), scale);
    }

    void QR3d::openLut ()
    {
    }

    void QR3d::setLut (R3d::Lut * lut)
    {
      //cout << "void setLut (R3d::Lut *);" << endl;

      if (m_lut != NULL)
      {
        delete m_lut;
        m_lut = NULL;
      }

      m_lut = lut;
      m_renderer->setFast (true);
      m_renderer->setLut (m_lut);
      m_renderer->updateGL ();
      m_renderer->setFast (false);
    }

    void QR3d::setLut (const QVector<QColor> & table)
    {
      //cout << "void setLut (const QVector<QColor> &);" << endl;

      GLubyte t [256][4];
      for (int i = 0; i < 256; ++i)
      {
        t [i][0] = table [i].red   ();
        t [i][1] = table [i].green ();
        t [i][2] = table [i].blue  ();
        t [i][3] = table [i].alpha ();
      }

      R3d::Lut * lut = new R3d::Lut (t);
      setLut (lut);
    }

    void QR3d::setLut (const std::string & path)
    {
      //cout << "void setLut (const std::string &);" << endl;
      std::ifstream ifs (path.c_str ());
      QColor table [256];
      for (int i = 0; i < 256; ++i)
      {
        int r, g, b, a;
        ifs >> r >> g >> b >> a;
        table [i] = QColor (r, g, b, a);
      }
      m_editor->import (table);
    }

  } // namespace R3d
} // namespace Aa

