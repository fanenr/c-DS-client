#include "tui.h"
#include "util.h"
#include <jansson.h>

int
main (void)
{
  check_and_init ();
  
  show_menu ();
}
