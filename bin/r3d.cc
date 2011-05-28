#include <fstream>
#include <cstdlib> // exit
#include <AaText>
#include <AaCmdLine>
//#include <AaGlutWatermark>
#include <AaReferenceFrame>
#include "R3dImageRenderer3dGLSL.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include <QGLViewer/qglviewer.h>
#include "QLutEditor.h"

using namespace std;
using namespace Aa;
using namespace Aa::Math;

namespace Aa
{
  namespace R3d
  {

    class QImageRenderer : public QGLViewer
    {
      Q_OBJECT

      private:
        const R3d::Image         * m_image;
        const R3d::Lut           * m_lut;
        R3d::ImageRenderer3dGLSL * m_renderer;

      public:
        QImageRenderer (const R3d::Image * image, const R3d::Lut * lut, QWidget * parent = NULL) :
          QGLViewer (parent),
          m_image (image),
          m_lut (lut),
          m_renderer (NULL)
        {
        }

        virtual ~QImageRenderer ()
        {
          //cout << ">>> " << __PRETTY_FUNCTION__ << endl;

          if (m_renderer != NULL)
            delete m_renderer;

          //cout << "<<< " << __PRETTY_FUNCTION__ << endl;
        }

      public slots:
        void setImage (const R3d::Image * image)
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

        void setLut (const R3d::Lut * lut)
        {
          m_lut = lut;
          if (m_renderer != NULL)
          {
            m_renderer->setLut (lut);
            updateGL ();
          }
        }

      public:
        virtual void init ()
        {
          glewInit ();
          //glClearColor (0.0, 0.0, 0.0, 0.0);
          glClearColor (1.0, 1.0, 1.0, 0.0);
          glEnable (GL_CULL_FACE);

          // Renderer.
          m_renderer = new R3d::ImageRenderer3dGLSL (m_image, m_lut);
        }

        virtual void draw ()
        {
          if (m_renderer != NULL)
            m_renderer->glDraw (false);
        }

        virtual void fastDraw ()
        {
          if (m_renderer != NULL)
            m_renderer->glDraw (true);
        }

        virtual void keyPressEvent (QKeyEvent * e)
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
    };

    class QR3d : public QMainWindow
    {
      Q_OBJECT

      private:
        QImageRenderer   * m_renderer;
        QLutEditor       * m_editor;
        QDockWidget      * m_editor_dock;
        R3d::Image       * m_image;
        R3d::Lut         * m_lut;

      public:
        QR3d (const std::string & image, const std::string & lut, const vR3 & scale = vR3 (1, 1, 1)) :
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

        virtual ~QR3d ()
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

      public slots:
        void openImage ()
        {
          QString path = QFileDialog::getOpenFileName (this, tr(""), "/home/prof/aassif/data/import", tr("Images (*.b8 DICOMDIR)"));
          if (! path.isEmpty ()) setImage (path);
        }

        void setImage (R3d::Image * image, const vR3 & scale = vR3 (1, 1, 1))
        {
          if (m_image != NULL)
          {
            delete m_image;
            m_image = NULL;
          }

          if (image != NULL)
          {
            m_image = image;
            const vR3 & d = m_image->box ().dim ();
            float dx = scale.x * d.x;
            float dy = scale.y * d.y;
            float dz = scale.z * d.z;
            m_image->setBox (Math::Box (pR3 (-0.5 * dx, -0.5 * dy, -0.5 * dz), vR3 (dx, dy, dz)));
          }

          m_renderer->setImage (m_image);
          m_renderer->updateGL ();
        }

        void setImage (const std::string & path, const vR3 & scale = vR3 (1, 1, 1))
        {
          R3d::Image * image = R3d::ImageLoadB8 (path);
          setImage (image, scale);
        }

        void setImage (const QString & path, const vR3 & scale = vR3 (1, 1, 1))
        {
          setImage (path.toStdString (), scale);
        }

        void setLut (R3d::Lut * lut)
        {
          //cout << "void setLut (R3d::Lut *);" << endl;

          if (m_lut != NULL)
          {
            delete m_lut;
            m_lut = NULL;
          }

          m_lut = lut;
          m_renderer->setLut (m_lut);
          m_renderer->updateGL ();
        }

        void setLut (const QVector<QColor> & table)
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

        void setLut (const std::string & path)
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
    };

  } // namespace R3d
} // namespace Aa

#if 0
void my_init (const string & image, const string & lut, float scale [3], float frame [3])
{
  cout << glGetString (GL_VENDOR) << endl;
  cout << glGetString (GL_RENDERER) << endl;
  cout << glGetString (GL_VERSION) << endl;
  //GL::Extensions::print (cout);
  cout << glGetString (GL_EXTENSIONS) << endl;

  // Scene.
  g_scene = new GL::Scene ();
  g_scene->setBackground (0, 0, 0);
  //g_scene->setFrame (frame);
  //g_scene->addInput (new ReferenceFrame ());
  //g_scene->addInput (new SphereTest (pR3 (4, 8, 12), 3.0, 12, 24));

  // Transfer function.
  g_lut = new R3d::Lut (lut);

  // Data.
  g_image = R3d::ImageLoadB8 (image);
  //cout << g_image->box () << endl;
  const vR3 & d = g_image->box ().dim ();
  float dx = scale [0] * d.x;
  float dy = scale [1] * d.y;
  float dz = scale [2] * d.z;
  g_image->setBox (Box (pR3 (-0.5 * dx, -0.5 * dy, -0.5 * dz), vR3 (0.5 * dx, 0.5 * dy, 0.5 * dz)));

  // Camera.
  const Box & box = g_image->box ();
  const pR3 & pos = box.pos ();
  const vR3 & dim = box.dim ();
  g_scene->setTarget (pos + 0.5 * dim);
  g_scene->setCamera (pos + vR3 (0.5 * dim.x, 0.5 * dim.y, 4.0 * dim.z));

  // Renderer.
  g_renderer = new R3d::ImageRenderer3dGLSL (g_image, g_lut);
  //g_renderer->setLighting (true);
  g_scene->addInput (g_renderer);

  // Signature.
  g_watermark = new GL::Watermark ("R3d v0.1");
  g_scene->addInput (g_watermark);
}

void my_exit (int code)
{
  delete g_scene;
  exit (code);
}

void my_grab (const string & filename)
{
  int viewport [4];
  glGetIntegerv (GL_VIEWPORT, viewport);

  int larg = viewport [2]; if (larg & 1) ++larg;
  int haut = viewport [3]; if (haut & 1) ++haut;

  ofstream file (filename.c_str ());
  if (! file)
    cerr << "Cannot open file \"" << filename.c_str () << "\" !\n";
  else
    {
      file << "P6\n#\n" << larg << ' ' << haut << "\n255\n";

      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      //glutSwapBuffers ();
      glReadBuffer (GL_FRONT);

      char * pixels = new char [3 * larg * haut];
      glReadPixels (0, 0, larg, haut, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      glReadBuffer (GL_BACK);

      char * ppix = pixels + (3 * larg * (haut - 1));
      for (unsigned int j = haut; j-- ; ppix -= 3 * larg) file.write (ppix, 3 * larg);

      delete[] pixels;
      file.close ();
    }

  cout << "grabbed \"" << filename << "\".\n";
}

void my_display ()
{
  g_scene->glDraw ();
}

void my_idle ()
{
  glutPostRedisplay ();
}

void my_reshape (int w, int h)
{
  g_scene->glReshape (w, h);
}

void my_keyboard (unsigned char key, int, int)
{
  switch (key)
  {
  case 'q':
  case 'Q':
  case 27:
    my_exit (0);
    break;

  case 'g':
    my_grab ("r3d.ppm");
    break;

  case ' ':
    g_renderer->setLighting (! g_renderer->lighting ());
    glutPostRedisplay ();
    break;

  default:
    cout << "Key '" << key << "' [" << (int) key << "] is not assigned ...\n";
  }
}

void my_mouse (int button, int state, int x, int y)
{
  int modifiers = glutGetModifiers ();

  switch (state)
  {
  case GLUT_UP:
    g_scene->setMotionMode (GL::Scene::MOTION_NONE);
    glutPostRedisplay ();
    last_x = -1;
    last_y = -1;
    break;

  case GLUT_DOWN:
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
      switch (modifiers)
      {
        case 0:                 g_scene->setMotionMode (GL::Scene::MOTION_CAMERA_ROTATE); break;
        case GLUT_ACTIVE_CTRL:  g_scene->setMotionMode (GL::Scene::MOTION_CAMERA_FORWARD); break;
        case GLUT_ACTIVE_SHIFT: g_scene->setMotionMode (GL::Scene::MOTION_TARGET_ROTATE); break;
        default: break;
      }
      break;

    case GLUT_RIGHT_BUTTON:
      break;

    default:
      cerr << "this button [" << button << "] stinks!\n";
      break;
    }

    last_x = x;
    last_y = y;
    break;

  default:
    cerr << "this state stinks!\n";
    break;
  }
}

void my_motion (int x, int y)
{
  int dx = x - last_x; last_x = x;
  int dy = y - last_y; last_y = y;

  if (dx != 0 || dy != 0) g_scene->motion (dx, dy);
}
#endif

int main (int argc, char ** argv)
{
  //glutInit (&argc, argv);
  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutInitWindowSize (512, 512);
  //glutInitWindowPosition (64, 64);
  //glutCreateWindow ("R3d!");

  string lut = "default.lut";
  float box [3] = {1.0, 1.0, 1.0};
  float ref [3] = {1.0, 1.0, 1.0};
  CmdLine::Parser p ("AaR3d 1.0", 1, 1);
  try
  {
    p.add ('l', "lut", "transfer function", &lut);
    p.add ('b', "box", "bounding box scale", 3, box);
    //p.add ('r', "ref", "reference frame",   3, ref);
    CmdLine::StringList arguments = p (argc, argv);
    //my_init (arguments.front (), lut, box, ref);
    
    QApplication application (argc, argv);
    Aa::R3d::QR3d r3d (arguments.front (), lut, vR3 (box [0], box [1], box [2]));
    r3d.setWindowTitle ("R3d!");
    r3d.show ();
    return application.exec ();
  }
  catch (exception & e)
  {
    p.usage (cout);
    cerr << p.getCmd () << ": " << e.what () << endl;
    return 1;
  }

  //glutReshapeFunc (my_reshape);
  //glutKeyboardFunc (my_keyboard);
  //glutMouseFunc (my_mouse);
  //glutMotionFunc (my_motion);
  //glutDisplayFunc (my_display);
  //glutIdleFunc (my_idle);
  //glutMainLoop ();

}
#include "r3d.moc"

