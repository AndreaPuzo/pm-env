#include "pm.h"

__PM_PUBL int pm_bus_ctor (struct pm_bus_t * bus, struct pm_cfg_t cfg)
{
  return 0 ;
}

__PM_PUBL void pm_bus_dtor (struct pm_bus_t * bus)
{}

__PM_PUBL void pm_bus_rst (struct pm_bus_t * bus, int lvl)
{
  pm_cpu_rst(&bus->cpu, lvl) ;
}

__PM_PUBL void pm_bus_clk (struct pm_bus_t * bus)
{
  pm_cpu_clk(&bus->cpu) ;
}

__PM_PUBL u_byte_t pm_bus_ldb (struct pm_bus_t * bus, u_word_t adr)
{
  return U_BYTE(0) ;
}
__PM_PUBL u_half_t pm_bus_ldh (struct pm_bus_t * bus, u_word_t adr)
{
  return U_HALF(0) ;
}
__PM_PUBL u_word_t pm_bus_ldw (struct pm_bus_t * bus, u_word_t adr)
{
  return U_WORD(0) ;
}

__PM_PUBL void pm_bus_stb (struct pm_bus_t * bus, u_word_t adr, u_byte_t dat)
{}
__PM_PUBL void pm_bus_sth (struct pm_bus_t * bus, u_word_t adr, u_half_t dat)
{}
__PM_PUBL void pm_bus_stw (struct pm_bus_t * bus, u_word_t adr, u_word_t dat)
{}

__PM_PUBL void pm_bus_int (struct pm_bus_t * bus, u_word_t num)
{
  pm_cpu_int(&bus->cpu, num) ;
}

__PM_PUBL u_word_t pm_bus_cks (struct pm_bus_t * bus)
{
  return bus->cpu.ck[0] ;
}
