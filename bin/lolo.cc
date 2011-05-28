#include <fstream>
#include <AaCmdLine>
#include <R3dImage.h>

using namespace std;

void pgm (const string & filename,
          unsigned int w, unsigned int h,
          const unsigned char * data)
{
  ofstream ofs (filename.c_str (), ios::out | ios::binary);
  ofs << "P5" << endl
      << w << ' ' << h << endl
      << "255" << endl;
  ofs.write ((char *) data, w * h);
}

int main (int argc, char ** argv)
{
  try
  {
    int plane = 0;
    Aa::CmdLine::Parser p ("Lolo");
    p.add ('p', "plane", "plane number", &plane, true);
    Aa::CmdLine::StringList params = p (argc, argv);

    for (Aa::CmdLine::StringList::const_iterator
           i = params.begin (); i != params.end ();)
    {
      const std::string & filename = *(i++);
      Aa::R3d::Image * image = Aa::R3d::ImageLoadB8 (filename);
      //cout << image->dx () << 'x' << image->dy () << 'x' << image->dz () << endl;
      pgm (filename + ".pgm", image->dx (), image->dy (), image->begin () + plane * image->dxdy ());
      delete image;
    }
  }
  catch (runtime_error & e)
  {
    cerr << e.what () << endl;
  }
}

