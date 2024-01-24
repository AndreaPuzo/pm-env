#include "pm.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
  const char cfgstr [] =
    "--cpu=0 [../bin/cfg/cpu.cfg] --irq 0x10 --entry 0x00000000 --master      ; configuration for master\n"
    "--cpu=1 [../bin/cfg/cpu.cfg] --irq 0x11 --entry 0x80000000               ; configuration for slave\n"
    "--cpu=2 [../bin/cfg/cpu.cfg] --irq 0x12 --entry 0x80000000               ; configuration for slave\n"
    "--cpu=3 [../bin/cfg/cpu.cfg] --irq 0x13 --entry 0x80000000               ; configuration for slave\n"
    "--ram=0 [../bin/cfg/ram.cfg] --irq 0x14 --adr 0x80000000 --len   1G --be ; main RAM bank 0 (L)\n"
    "--ram=1 [../bin/cfg/ram.cfg] --irq 0x15 --adr 0xC0000000 --len   1G --be ; main RAM bank 1 (H)\n"
    "--ram=2 [../bin/cfg/ram.cfg] --irq 0x16 --adr 0x00000000 --len   4K --be ; internal arguments RAM\n"
    "--ram=3 [../bin/cfg/ram.cfg] --irq 0x17 --adr 0x00001000 --len  60K --le ; external arguments RAM\n"
    "--iom=0 [../bin/cfg/iom.cfg] --irq 0x14 --adr 0x00010000 --len 256M      ; I/O memory bank 0\n"
    "--iom=1 [../bin/cfg/iom.cfg] --irq 0x15 --adr 0x20010000 --len 256M      ; I/O memory bank 1\n"
    "--iom=2 [../bin/cfg/iom.cfg] --irq 0x16 --adr 0x40010000 --len 256M      ; I/O memory bank 2\n"
    "--iom=3 [../bin/cfg/iom.cfg] --irq 0x17 --adr 0x60010000 --len 256M      ; I/O memory bank 3\n"
  ;

/*
  cpu:
    --irq interrupt
    --entry address
    --master
    bus

  ram:
    --irq interrupt
    --adr address
    --len length
    --le/--be
    bus

  iom:
    --irq interrupt
    --adr address
    --len length
    bus
*/

  struct pm_bus_t bus ;
  
  if (0 != pm_cfg_ctor(&bus.cfg, cfgstr)) {
    fprintf(stderr, "error: cannot initialize the configuration\n") ;
    return EXIT_FAILURE ;
  }

  pm_cfg_dump(&bus.cfg, stderr) ;
  pm_cfg_dtor(&bus.cfg) ;
  return EXIT_SUCCESS ;

  if (0 != pm_bus_ctor(&bus, bus.cfg)) {
    pm_bus_dtor(&bus) ;
    fprintf(stderr, "error: cannot configure the connection\n") ;
    return EXIT_FAILURE ;
  }

  pm_cfg_dump(&bus.cfg, stderr) ;
  pm_bus_dtor(&bus) ;

  fprintf(stderr, "HELLO WORLD!\n") ;
  return EXIT_SUCCESS ;
}
