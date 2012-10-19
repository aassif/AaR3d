#include <QApplication>
#include "QLutEditor.h"

using namespace std;

int main (int argc, char ** argv)
{
  QApplication application (argc, argv);
  Aa::R3d::QLutEditor editor;
  editor.setWindowTitle ("QLutEditor");
  editor.show ();
  return application.exec ();
}

