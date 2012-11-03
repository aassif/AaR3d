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
#include <QTextStream>
#include "QR3d.h"

using namespace std;
using namespace Aa;
//using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    QR3d::QR3d () :
      m_renderer (NULL),
      m_transfer_function (NULL),
      m_dock (NULL),
      m_image (NULL),
      m_lut (NULL)
    {
      m_renderer = new QImageRenderer;
      setCentralWidget (m_renderer);

      m_transfer_function = new QTransferFunction;

      m_transfer_table = new QTransferTable;

      m_transfer_tabs = new QTabWidget;
      m_transfer_tabs->addTab (m_transfer_function, tr ("Transfer function"));
      m_transfer_tabs->addTab (m_transfer_table,    tr ("Transfer table"));

      m_dock = new QDockWidget (tr ("QTransferFunction"));
      m_dock->setWidget (m_transfer_tabs);
      addDockWidget (Qt::LeftDockWidgetArea, m_dock);

      QMenuBar * bar = menuBar ();
      QMenu * menuFile = bar->addMenu (tr ("&File"));
      menuFile->addAction (tr ("&Open"), this, SLOT (openImage ()));
      menuFile->addAction (tr ("Open &LUT"), this, SLOT (openLut ()));
      menuFile->addAction (tr ("&Save LUT"), this, SLOT (saveLut ()));
      menuFile->addSeparator ();
      menuFile->addAction (tr ("&Quit"), qApp, SLOT (quit ()));
      QMenu * menuView = bar->addMenu (tr ("&View"));
      menuView->addAction (m_dock->toggleViewAction ());

      connect (m_transfer_tabs, SIGNAL (currentChanged        (int)),
               this,            SLOT   (transfer_tabs_changed (int)));

      connect (m_transfer_function, SIGNAL (computed (const QVector<QColor> &)),
               this,                SLOT   (setLut   (const QVector<QColor> &)));

      connect (m_transfer_table, SIGNAL (computed (const QVector<QColor> &)),
               this,             SLOT   (setLut   (const QVector<QColor> &)));

      this->setLut (m_transfer_function->table ());
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

    void QR3d::transfer_tabs_changed (int index)
    {
      switch (index)
      {
        case 0: this->setLut (m_transfer_function->table ()); break;
        case 1: this->setLut (m_transfer_table   ->table ()); break;
      }
    }

    void QR3d::openImage ()
    {
      QString title  = tr ("");
      QString dir    = "/home/prof/aassif/data/import";
      QString filter = tr ("Images (*.b8 *.dcm DICOMDIR)");
      QString path = QFileDialog::getOpenFileName (this, title, dir, filter);
      if (! path.isEmpty ()) setImage (path);
    }

    void QR3d::setImage (R3d::Image * image)
    {
      if (m_image != NULL)
      {
        delete m_image;
        m_image = NULL;
      }

      m_image = image;

      m_renderer->setImage (m_image);
      m_renderer->updateGL ();
    }

    void QR3d::setImage (const QString & path, const dvec3 & scale)
    {
      R3d::Image * image = R3d::ImageLoadB8 (path.toStdString (), scale);
      setImage (image);
    }

    void QR3d::openLut ()
    {
      QString title  = tr ("");
      QString dir    = "/home/prof/aassif/data";
      QString filter = tr ("LUT (*.qlut)");
      QString path = QFileDialog::getOpenFileName (this, title, dir, filter);
      if (! path.isEmpty ()) setLut (path);
    }

    void QR3d::saveLut ()
    {
      QString title  = tr ("");
      QString dir    = "/home/prof/aassif/data";
      QString filter = tr ("LUT (*.qlut)");
      QString path = QFileDialog::getSaveFileName (this, title, dir, filter);
      if (! path.isEmpty ())
      {
        QDomDocument doc;
        doc.appendChild (m_transfer_function->dom (doc));

        QFile file (path);
        file.open (QFile::WriteOnly);
        QTextStream text (&file);

        doc.save (text, 2);
      }
    }

    void QR3d::setLut (R3d::Lut * lut)
    {
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

#if 0
    void QR3d::setLut (const QString & path)
    {
      QFile file (path);
      file.open (QFile::ReadOnly);
      QTextStream text (&file);
      QColor table [256];
      for (int i = 0; i < 256; ++i)
      {
        AaUInt r, g, b, a;
        text >> r >> g >> b >> a;
        table [i] = QColor (r, g, b, a);
      }
      m_editor->import (table);
    }
#endif

    void QR3d::setLut (const QString & path)
    {
      QFile file (path);
      file.open (QFile::ReadOnly);

      QDomDocument doc;
      doc.setContent (&file);

      m_transfer_function->init (doc.firstChildElement ("lut"));
    }

  } // namespace R3d
} // namespace Aa

