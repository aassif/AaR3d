#include <AaCmdLine>
#include <QApplication>
#include "QR3d.h"

using namespace std;
using namespace Aa;

int main (int argc, char ** argv)
{
  CmdLine::Parser p ("AaR3d 1.0", 0, 1);

  try
  {
    string lut = "";
    p.add ('l', "lut", "transfer function", &lut);

    double box [3] = {1.0, 1.0, 1.0};
    p.add ('b', "box", "bounding box scale", 3, box);

    //double ref [3] = {1.0, 1.0, 1.0};
    //p.add ('r', "ref", "reference frame",   3, ref);

    CmdLine::StringList arguments = p (argc, argv);
    
    QApplication application (argc, argv);

    R3d::QR3d r3d;

    if (! lut.empty ())
      r3d.setLut (lut);

    if (! arguments.empty ())
      r3d.setImage (arguments.front (), vec (box [0], box [1], box [2]));

    r3d.setWindowTitle ("AaR3d!");
    r3d.show ();

    return application.exec ();
  }
  catch (exception & e)
  {
    p.usage (cout);
    cerr << p.getCmd () << ": " << e.what () << endl;
    return 1;
  }
}

