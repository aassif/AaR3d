#include <iostream>
#include <fstream>
#include <cmath>
#include <AaCmdLine>

using namespace std;
using namespace Aa;

class DataType
{
  public:
    int bits;
    bool little_endian;

  public:
    DataType (int b = 8, bool le = false) :
      bits (b),
      little_endian (le)
    {
    }
};

namespace Aa
{
  namespace CmdLine
  {
    template<> string StringParser::TypeId<DataType> () {return "8|12be|12le|16be|16le";}
  }
}

ostream & operator<< (ostream & o, const DataType & d)
{
  return o << d.bits << (d.little_endian ? "le" : "be");
}

istream & operator>> (istream & i, DataType & d)
{
  string word;
  i >> word >> ws;
  /**/ if (word == "8")    d = DataType (8, false);
  else if (word == "12")   d = DataType (12, false);
  else if (word == "12be") d = DataType (12, false);
  else if (word == "12le") d = DataType (12, true);
  else if (word == "16")   d = DataType (16, false);
  else if (word == "16be") d = DataType (16, false);
  else if (word == "16le") d = DataType (16, true);
  else i.setstate (ios::failbit);
  return i;
}

void b8_data_8 (istream & is, ostream & os, unsigned int n)
{
  int min = 255, max = 0;
  for (unsigned int i = 0; i < n; ++i)
  {
    unsigned char v = is.get ();
    if (v < min) min = v;
    if (v > max) max = v;
    os.put (v);
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
}

void b8_data_12le (istream & is, ostream & os, unsigned int n)
{
  int min = 255, max = 0;
  for (unsigned int i = 0; i < n; ++i)
  {
    unsigned char v1 = is.get ();
    unsigned char v2 = is.get ();
    unsigned char v = (v2 << 8 | v1) >> 4;
    if (v < min) min = v;
    if (v > max) max = v;
    os.put (v);
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
}

void b8_data_12be (istream & is, ostream & os, unsigned int n)
{
  int min = 255, max = 0;
  for (unsigned int i = 0; i < n; ++i)
  {
    unsigned char v1 = is.get ();
    unsigned char v2 = is.get ();
    unsigned char v = (v1 << 8 | v2) >> 4;
    if (v < min) min = v;
    if (v > max) max = v;
    os.put (v);
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
}

void b8_data_12 (istream & is, ostream & os, unsigned int n, bool little_endian)
{
  if (little_endian) b8_data_12le (is, os, n);
  else               b8_data_12be (is, os, n);
}

void b8_data_16le (istream & is, ostream & os, unsigned int n)
{
  int min = 255, max = 0;
  for (unsigned int i = 0; i < n; ++i)
  {
    /****************/ is.get ();
    unsigned char v2 = is.get ();
    if (v2 < min) min = v2;
    if (v2 > max) max = v2;
    os.put (v2);
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
}

void b8_data_16be (istream & is, ostream & os, unsigned int n)
{
  int min = 255, max = 0;
  for (unsigned int i = 0; i < n; ++i)
  {
    unsigned char v1 = is.get ();
    /****************/ is.get ();
    if (v1 < min) min = v1;
    if (v1 > max) max = v1;
    os.put (v1);
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
}

void b8_data_16 (istream & is, ostream & os, unsigned int n, bool little_endian)
{
  if (little_endian) b8_data_16le (is, os, n);
  else               b8_data_16be (is, os, n);
}

void b8 (const string & input,
         const string & output,
         unsigned short dims [3],
         const DataType & type,
         int skip)
{
  cout << "type = " << type << endl;

  ifstream ifs (input.c_str (), ios::in | ios::binary);
  ifs.seekg (skip, ios::beg);

  ofstream ofs (output.c_str (), ios::out | ios::binary);
  ofs.write ((const char *) &(dims[0]), 2);
  ofs.write ((const char *) &(dims[1]), 2);
  ofs.write ((const char *) &(dims[2]), 2);

  unsigned int n = dims [0] * dims [1] * dims [2];
  switch (type.bits)
  {
    case 8:  b8_data_8  (ifs, ofs, n); break;
    case 12: b8_data_12 (ifs, ofs, n, type.little_endian); break;
    case 16: b8_data_16 (ifs, ofs, n, type.little_endian); break;
  }
}

int main (int argc, char ** argv)
{
  string input;
  string output;
  unsigned short dims [3] = {0, 0, 0};
  DataType type;
  unsigned int skip = 0;
  CmdLine::Parser p ("B8");
  p.add ('i', "input",  "input file",  &input,  true);
  p.add ('o', "output", "output file", &output, true);
  p.add ('d', "dims",   "dimensions",  3, dims, true);
  p.add ('t', "type",   "data type",   &type);
  p.add ('s', "skip",   "skip bytes",  &skip);
  CmdLine::StringList args = p (argc, argv);
  b8 (input, output, dims, type, skip);
  return 0;
}

