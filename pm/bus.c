#include "pm.h"

__PM_PUBL int pm_bus_ctor (struct pm_bus_t * bus, struct pm_cfg_t cfg)
{
  return 0 ;
}

__PM_PUBL void pm_bus_dtor (struct pm_bus_t * bus)
{
  for (int id = 0 ; id < 0x4 ; ++id) {
    pm_cpu_dtor(bus->cpu + id) ;
    pm_ram_dtor(bus->ram + id) ;
    pm_iom_dtor(bus->iom + id) ;
  }
}

__PM_PUBL void pm_bus_rst (struct pm_bus_t * bus, int lvl)
{
  int id = pm_bus_mst(bus) ;
 
  for (int id = 0 ; id < 0x4 ; ++id) {
    pm_cpu_rst(bus->cpu + id, lvl) ;
    pm_ram_rst(bus->ram + id, lvl) ;
    pm_iom_rst(bus->iom + id, lvl) ;
  }
}

__PM_PUBL void pm_bus_clk (struct pm_bus_t * bus)
{
  for (int id = 0 ; id < 0x4 ; ++id) {
    if (0 != pm_bus_is_bsy(bus, id)) {
      pm_cpu_clk(bus->cpu + id) ;
    }

    pm_ram_clk(bus->ram + id) ;
    pm_iom_clk(bus->iom + id) ;
  }
}

#define _ld(typ)                                           \
  do {                                                     \
    for (int id = 0 ; id < 0x4 ; ++id) {                   \
      struct pm_ram_t * ram = bus->ram + id ;              \
                                                           \
      u_word_t ofs = adr - ram->adr ;                      \
                                                           \
      if (ofs < ram->len && ofs + sizeof(dat) <= ram->len) \
        return pm_ram_ld##typ(ram, ofs) ;                  \
    }                                                      \
                                                           \
    for (int id = 0 ; id < 0x4 ; ++id) {                   \
      struct pm_iom_t * iom = bus->iom + id ;              \
                                                           \
      u_word_t ofs = adr - iom->adr ;                      \
                                                           \
      if (ofs < iom->len)                                  \
        return pm_iom_ld##typ(iom, ofs) ;                  \
    }                                                      \
  } while (0)

__PM_PUBL u_byte_t pm_bus_ldb (struct pm_bus_t * bus, u_word_t adr)
{
  u_byte_t dat = U_BYTE(0) ;
  _ld(b) ;
  return dat ;
}
__PM_PUBL u_half_t pm_bus_ldh (struct pm_bus_t * bus, u_word_t adr)
{
  u_half_t dat = U_HALF(0) ;
  _ld(h) ;
  return dat ;
}
__PM_PUBL u_word_t pm_bus_ldw (struct pm_bus_t * bus, u_word_t adr)
{
  u_word_t dat = U_WORD(0) ;
  _ld(w) ;
  return dat ;
}

#undef _ld

#define _st(typ)                                             \
  do {                                                       \
    for (int id = 0 ; id < 0x4 ; ++id) {                     \
      struct pm_ram_t * ram = bus->ram + id ;                \
                                                             \
      u_word_t ofs = adr - ram->adr ;                        \
                                                             \
      if (ofs < ram->len && ofs + sizeof(dat) <= ram->len) { \
        pm_ram_st##typ(ram, ofs, dat) ;                      \
        return ;                                             \
      }                                                      \
    }                                                        \
                                                             \
    for (int id = 0 ; id < 0x4 ; ++id) {                     \
      struct pm_iom_t * iom = bus->iom + id ;                \
                                                             \
      u_word_t ofs = adr - iom->adr ;                        \
                                                             \
      if (ofs < iom->len) {                                  \
        pm_iom_st##typ(iom, ofs, dat) ;                      \
        return ;                                             \
      }                                                      \
    }                                                        \
  } while (0)

__PM_PUBL void pm_bus_stb (struct pm_bus_t * bus, u_word_t adr, u_byte_t dat)
{
  _st(b) ;
}

__PM_PUBL void pm_bus_sth (struct pm_bus_t * bus, u_word_t adr, u_half_t dat)
{
  _st(h) ;
}

__PM_PUBL void pm_bus_stw (struct pm_bus_t * bus, u_word_t adr, u_word_t dat)
{
  _st(w) ;
}

#undef _st

__PM_PUBL void pm_bus_int (struct pm_bus_t * bus, u_word_t num)
{
  int id = pm_bus_mst(bus) ;
  pm_cpu_int(bus->cpu + id, num) ;
}

__PM_PUBL u_word_t pm_bus_cks (struct pm_bus_t * bus)
{
  int id = pm_bus_mst(bus) ;
  return bus->cpu[id].ck[0] ;
}

__PM_PUBL void pm_bus_hlt (struct pm_bus_t * bus, u_word_t id)
{
  id &= 0x3 ;

  if (pm_bus_mst(bus) == id) {
    bus->ctr &= ~( 0xF << 0 ) ;
    bus->ctr |=  ( 0xF << 4 ) ;
  } else {
    bus->ctr &= ~( 0x1 << ( 0 + id ) ) ;
    bus->ctr |=  ( 0x1 << ( 4 + id ) ) ;
  }
}

__PM_PUBL void pm_bus_rdy (struct pm_bus_t * bus, u_word_t id)
{
  bus->ctr |=  ( 0x1 << ( 4 + ( id & 0x3 ) ) ) ;
}

__PM_PUBL void pm_bus_bsy (struct pm_bus_t * bus, u_word_t id)
{
  bus->ctr &= ~( 0x1 << ( 4 + ( id & 0x3 ) ) ) ;
}

__PM_PUBL int pm_bus_rint (struct pm_bus_t * bus, struct pm_cpu_t * cpu, int id, u_word_t num)
{
  id &= 0x3 ;

  if (0 != pm_bus_is_bsy(bus, id))
    return -1 ;

  if (pm_bus_mst(bus) != pm_cpu_id(cpu)) {
    pm_bus_int(bus, PM_CPU_INT_GP) ;
    return -2 ;
  }

  pm_cpu_int(bus->cpu + id, num) ;
  pm_bus_bsy(bus, id) ;

  return 0 ;
}
