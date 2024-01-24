#include "pm.h"
#include <stdlib.h>
#include <string.h>

__PM_PUBL int pm_ram_ctor (struct pm_ram_t * ram, struct pm_cfg_t cfg)
{
  ram->bus = NULL               ;
  ram->sr  = U_WORD(0x00000000) ;
  ram->adr = U_WORD(0x00000000) ;
  ram->len = U_WORD(0)          ;
  ram->buf = NULL               ;

  if (0 == cfg.argc || NULL == cfg.argv)
    return -1 ;

  ram->cfg = cfg ;
  ram->sr |= PM_RAM_SRF_RS ;

  for (int argi = 0 ; argi < cfg.argc ; ++argi) {
    char * args = cfg.argv[argi] ;

    if (0 == strcmp(args, "-i") || 0 == strcmp(args, "--irq")) {
      u_word_t id ;
      char * endptr = NULL ;
 
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &id)) {
        fprintf(stderr, "error: ram: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      ram->sr &= ~PM_RAM_SRF_ID ;
      ram->sr |= ( id & PM_RAM_SRM_ID ) << PM_RAM_SRS_ID ;
    } else if (0 == strcmp(args, "-b") || 0 == strcmp(args, "--buf")) {
      ram->buf = cfg.argv[++argi] ;
      ram->sr &= ~PM_RAM_SRF_RS   ;
    } else if (0 == strcmp(args, "-a") || 0 == strcmp(args, "--adr")) {
      u_word_t adr ;
      char * endptr = NULL ;
      
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &adr)) {
        fprintf(stderr, "error: ram: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      ram->adr = adr ;
    } else if (0 == strcmp(args, "-l") || 0 == strcmp(args, "--len")) {
      u_word_t len ;
      char * endptr = NULL ;
      
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &len)) {
        fprintf(stderr, "error: ram: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      if (NULL != endptr) {
        if (0 == strcmp(endptr, "K")) {
          len <<= 10 ;
        } else if (0 == strcmp(endptr, "M")) {
          len <<= 20 ;
        } else if (0 == strcmp(endptr, "G")) {
          len <<= 30 ;
        }
      }

      ram->len = len ;
    } else if (0 == strcmp(args, "--le") || 0 == strcmp(args, "--be")) {
      ram->sr &= ~PM_RAM_SRF_BO ;
      ram->sr |= ( ( ( 'b' == args[2] ) *  PM_RAM_SRM_BO ) << PM_RAM_SRS_BO ) ;
    } else {
      ram->bus = (struct pm_bus_t *)args ;
    }
  }

  if (NULL == ram->bus) {
    fprintf(stderr, "error: ram: unmounted device\n") ;
    return -3 ;
  }

  if (0 != ( ram->sr & PM_RAM_SRF_RS )) {
    ram->buf = (u_byte_t *)malloc(ram->len * sizeof(u_byte_t)) ;
  }

  if (NULL == ram->buf || ram->len < U_WORD(0x1000)) {
    fprintf(stderr, "error: ram: unexpected buffer or length\n") ;
    return -4 ;
  }

  ram->sr |= PM_RAM_SRF_RN ;

  return 0 ;
}

__PM_PUBL void pm_ram_dtor (struct pm_ram_t * ram)
{
  if (0 == ( ram->sr & PM_RAM_SRF_RS ))
    return ;

  free(ram->buf)  ;
  ram->buf = NULL ;
}

__PM_PUBL void pm_ram_rst (struct pm_ram_t * ram, int lvl)
{
  const int RST_HRD = lvl < 0 ;
  const int RST_ALL = ( lvl >> 10 ) & 0x1 ;
  const int RST_ID  = ( lvl >>  6 ) & 0xF ;

  if (0 == RST_ALL && RST_ID != pm_ram_id(ram))
    return ;

  if (0 != RST_HRD) {
    pm_ram_dtor(ram) ;

    if (0 != pm_ram_ctor(ram, ram->cfg))
      return ;
  }

  (void) memset(ram->buf, 0x00, ram->len) ;

  pm_ram_int(ram) ;
}

__PM_PUBL void pm_ram_clk (struct pm_ram_t * ram)
{}

__PM_PUBL u_byte_t pm_ram_ldb (struct pm_ram_t * ram, u_word_t adr)
{
  u_byte_t dat ;

  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    dat = pm_ldb_le(ram->buf) ;
  } else {
    dat = pm_ldb_be(ram->buf) ;
  }

  return dat ;
}

__PM_PUBL u_half_t pm_ram_ldh (struct pm_ram_t * ram, u_word_t adr)
{
  u_half_t dat ;

  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    dat = pm_ldh_le(ram->buf) ;
  } else {
    dat = pm_ldh_be(ram->buf) ;
  }

  return dat ;
}

__PM_PUBL u_word_t pm_ram_ldw (struct pm_ram_t * ram, u_word_t adr)
{
  u_word_t dat ;

  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    dat = pm_ldw_le(ram->buf) ;
  } else {
    dat = pm_ldw_be(ram->buf) ;
  }

  return dat ;
}

__PM_PUBL void pm_ram_stb (struct pm_ram_t * ram, u_word_t adr, u_byte_t dat)
{
  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    pm_stb_le(ram->buf, dat) ;
  } else {
    pm_stb_be(ram->buf, dat) ;
  }
}

__PM_PUBL void pm_ram_sth (struct pm_ram_t * ram, u_word_t adr, u_half_t dat)
{
  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    pm_sth_le(ram->buf, dat) ;
  } else {
    pm_sth_be(ram->buf, dat) ;
  }
}

__PM_PUBL void pm_ram_stw (struct pm_ram_t * ram, u_word_t adr, u_word_t dat)
{
  if (0 == ( ram->sr & PM_RAM_SRF_BO )) {
    pm_stw_le(ram->buf, dat) ;
  } else {
    pm_stw_be(ram->buf, dat) ;
  }
}

__PM_PUBL void pm_ram_int (struct pm_ram_t * ram)
{
  pm_bus_int(ram->bus, ( ram->sr >> PM_RAM_SRS_ID ) & PM_RAM_SRM_ID) ;
}

__PM_PUBL u_word_t pm_ram_id (struct pm_ram_t * ram)
{
  return ( ( ram->sr >> PM_RAM_SRS_ID ) & PM_RAM_SRM_ID ) & 0xF ;
}
