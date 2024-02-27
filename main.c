#include "page.h"
#include "table.h"
#include "tui.h"

int
main (void)
{
  tui_init ();
  table_init ();
  page_main ();
}
