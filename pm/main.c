#include "pm.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
  struct pm_cfg_t cfg ;

  const char str [] =
    "--irq 0x0                 ; interrupt number\n"
    "--adr 0xC0000000 --len 1G ; address space\n"
    "--be                      ; byte order\n"
    "--dev='SRAM1G\e0\\''      ; device buffer" ;

  if (0 != pm_cfg_ctor(&cfg, str)) {
    fprintf(stderr, "error: cannot initialize the configuration\n") ;
    return EXIT_FAILURE ;
  }

  pm_cfg_dump(&cfg, stderr) ;
  pm_cfg_dtor(&cfg) ;

  fprintf(stderr, "HELLO WORLD!\n") ;
  return EXIT_SUCCESS ;
}
