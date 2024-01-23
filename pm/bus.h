#ifndef __PM_BUS_H
# define __PM_BUS_H

struct pm_bus_t {
  int             hlt         ;
  struct pm_cpu_t cpu         ;
  struct pm_ram_t ram [ 0x4 ] ;
/* struct pm_iom_t iom [ 0x4 ] ; */
} ;

__PM_PUBL int      pm_bus_ctor (struct pm_bus_t * bus, struct pm_cfg_t cfg) ;
__PM_PUBL void     pm_bus_dtor (struct pm_bus_t * bus) ;
__PM_PUBL void     pm_bus_rst  (struct pm_bus_t * bus, int lvl) ;
__PM_PUBL void     pm_bus_clk  (struct pm_bus_t * bus) ;
__PM_PUBL u_byte_t pm_bus_ldb  (struct pm_bus_t * bus, u_word_t adr) ;
__PM_PUBL u_half_t pm_bus_ldh  (struct pm_bus_t * bus, u_word_t adr) ;
__PM_PUBL u_word_t pm_bus_ldw  (struct pm_bus_t * bus, u_word_t adr) ;
__PM_PUBL void     pm_bus_stb  (struct pm_bus_t * bus, u_word_t adr, u_byte_t dat) ;
__PM_PUBL void     pm_bus_sth  (struct pm_bus_t * bus, u_word_t adr, u_half_t dat) ;
__PM_PUBL void     pm_bus_stw  (struct pm_bus_t * bus, u_word_t adr, u_word_t dat) ;
__PM_PUBL void     pm_bus_int  (struct pm_bus_t * bus, u_word_t num) ;
__PM_PUBL u_word_t pm_bus_cks  (struct pm_bus_t * bus) ;

#endif
