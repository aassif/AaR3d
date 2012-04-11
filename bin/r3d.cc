#include <AaCmdLine>
#include <QApplication>
#include "QR3d.h"

using namespace std;
using namespace Aa;

int main (int argc, char ** argv)
{
  //glutInit (&argc, argv);
  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutInitWindowSize (512, 512);
  //glutInitWindowPosition (64, 64);
  //glutCreateWindow ("R3d!");

  string lut = "default.lut";
  double box [3] = {1.0, 1.0, 1.0};
  //double ref [3] = {1.0, 1.0, 1.0};
  CmdLine::Parser p ("AaR3d 1.0", 1, 1);
  try
  {
    p.add ('l', "lut", "transfer function", &lut);
    p.add ('b', "box", "bounding box scale", 3, box);
    //p.add ('r', "ref", "reference frame",   3, ref);
    CmdLine::StringList arguments = p (argc, argv);
    //my_init (arguments.front (), lut, box, ref);
    
    QApplication application (argc, argv);
    R3d::QR3d r3d (arguments.front (), lut, vec (box [0], box [1], box [2]));
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

//#include "r3d.moc"

