#include "MovieProgram.h"
#include <gtkmm.h>

int main ( int argc, char* argv[] )
{
  /* starts Gtk */
  Gtk::Main app(argc,argv);

  menu m;
  //movie1 m1;
  Gtk::Main::run(m);

  return 0;
}
