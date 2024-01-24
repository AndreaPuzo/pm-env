#include "pm.h"
#include <string.h>

__PM_PUBL int pm_iom_ctor (struct pm_iom_t * iom, struct pm_cfg_t cfg)
{
  iom->bus = NULL               ;
  iom->irq = U_WORD(0x0)        ;
  iom->adr = U_WORD(0x00000000) ;
  iom->len = U_WORD(0)          ;

  for (int id = 0 ; id < 0x20 ; ++id) {
    iom->dev[id] = NULL ;
  }

  if (0 == cfg.argc || NULL == cfg.argv)
    return -1 ;

  iom->cfg = cfg ;

  for (int argi = 0 ; argi < cfg.argc ; ++argi) {
    char * args = cfg.argv[argi] ;

    if (0 == strcmp(args, "-i") || 0 == strcmp(args, "--irq")) {
      u_word_t id ;
      char * endptr = NULL ;
 
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &id)) {
        fprintf(stderr, "error: iom: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      iom->irq = id ;
    } else if (0 == strcmp(args, "-a") || 0 == strcmp(args, "--adr")) {
      u_word_t adr ;
      char * endptr = NULL ;
      
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &adr)) {
        fprintf(stderr, "error: iom: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      iom->adr = adr ;
    } else if (0 == strcmp(args, "-l") || 0 == strcmp(args, "--len")) {
      u_word_t len ;
      char * endptr = NULL ;
      
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &len)) {
        fprintf(stderr, "error: iom: expected unsigned integer for option %s\n", args) ;
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

      iom->len = len ;
    } else {
      iom->bus = (struct pm_bus_t *)args ;
    }
  }

  if (NULL == iom->bus) {
    fprintf(stderr, "error: iom: unmounted device\n") ;
    return -3 ;
  }

  if (iom->len <= U_WORD(0x10)) {
    fprintf(stderr, "error: iom: unexpected length\n") ;
    return -4 ;
  }

  return 0 ;
}

__PM_PUBL void pm_iom_dtor (struct pm_iom_t * iom)
{
  for (int id = 0 ; id < 0x20 ; ++id) {
    struct pm_dev_t * dev = iom->dev[id] ;

    if (NULL == dev)
      continue ;

    (void) pm_iom_umn(iom, dev) ;
  }
}

__PM_PUBL void pm_iom_rst (struct pm_iom_t * iom, int lvl)
{
  const int RST_HRD = lvl < 0 ;
  const int RST_ALL = ( lvl >> 10 ) & 0x1 ;
  const int RST_ID  = ( lvl >>  6 ) & 0xF ;

  if (0 == RST_ALL && RST_ID != pm_iom_id(iom))
    return ;

  if (0 != RST_HRD) {
    pm_iom_dtor(iom) ;

    if (0 != pm_iom_ctor(iom, iom->cfg))
      return ;
  }
  
  for (int id = 0 ; id < 0x20 ; ++id) {
    struct pm_dev_t * dev = iom->dev[id] ;

    if (NULL == dev || NULL == dev->rst)
      continue ;

    dev->rst(dev, lvl) ;
  }

  iom->ipr = U_WORD(0x00000000) ;
  iom->imr = U_WORD(0x00000000) ;
  iom->isr = U_WORD(0x00)       ;

  pm_bus_int(iom->bus, iom->irq) ;
}

__PM_PUBL void pm_iom_clk (struct pm_iom_t * iom)
{
  for (int id = 0 ; id < 0x20 ; ++id) {
    struct pm_dev_t * dev = iom->dev[id] ;

    if (NULL == dev || NULL == dev->clk)
      continue ;

    dev->clk(dev) ;
  }

  iom->ipr &= ~iom->imr ;

  if (0 == iom->ipr)
    return ;

  for (int id = 0 ; id < 0x20 ; ++id) {
    if (0 != ( ( iom->ipr >> id ) & 0x1 )) {
      iom->isr = id ;
      pm_bus_int(iom->bus, iom->irq) ;
      break ;
    }
  }
}

#define _ldtyp(dst, src, siz)          \
  do {                                 \
    int shf ;                          \
    if (0 == siz) {                    \
      shf = ( adr & 0x3 ) << 3 ;       \
      dst = (u_byte_t)( src >> shf ) ; \
    } else if (1 == siz) {             \
      shf = ( adr & 0x1 ) << 4 ;       \
      dst = (u_half_t)( src >> shf ) ; \
    } else {                           \
      dst = src ;                      \
    }                                  \
  } while (0)

#define _ld(typ, siz)                          \
  do {                                         \
    if (                                       \
      U_WORD(0x00000000) <= adr &&             \
      adr <= U_WORD(0x00000010)                \
    ) {                                        \
      if (0x0 <= adr && adr <= 0x3) {          \
        dat = iom->irq & 0xF ;                 \
      } else if (0x4 <= adr && adr <= 0x7) {   \
        _ldtyp(dat, iom->ipr, siz) ;           \
      } else if (0x8 <= adr && adr <= 0xC) {   \
        _ldtyp(dat, iom->imr, siz) ;           \
      } else {                                 \
        dat = iom->isr & 0x1F ;                \
      }                                        \
                                               \
      return dat ;                             \
    }                                          \
                                               \
    for (int id = 0 ; id < 0x20 ; ++id) {      \
      struct pm_dev_t * dev = iom->dev[id] ;   \
                                               \
      if (NULL == dev || NULL == dev->ld##typ) \
        continue ;                             \
                                               \
      u_word_t ofs = adr - dev->adr ;          \
                                               \
      if (ofs < dev->len) {                    \
        dat = dev->ld##typ(dev, ofs) ;         \
        return dat ;                           \
      }                                        \
    }                                          \
  } while (0)

__PM_PUBL u_byte_t pm_iom_ldb (struct pm_iom_t * iom, u_word_t adr)
{
  u_byte_t dat = U_BYTE(0) ;
  _ld(b, 0) ;
  return dat ;
}

__PM_PUBL u_half_t pm_iom_ldh (struct pm_iom_t * iom, u_word_t adr)
{
  u_half_t dat = U_HALF(0) ;
  _ld(h, 1) ;
  return dat ;
}

__PM_PUBL u_word_t pm_iom_ldw (struct pm_iom_t * iom, u_word_t adr)
{
  u_byte_t dat = U_WORD(0) ;
  _ld(w, 2) ;
  return dat ;
}

#undef _ld
#undef _ldtyp

#define _sttyp(dst, src, siz)             \
  do {                                    \
    int shf ;                             \
    if (0 == siz) {                       \
      shf = ( adr & 0x3 ) << 3 ;          \
      dst &= ~( U_WORD(0xFF) << shf ) ;   \
      dst |= ( (u_word_t)src << shf ) ;   \
    } else if (1 == siz) {                \
      shf = ( adr & 0x1 ) << 4 ;          \
      dst &= ~( U_WORD(0xFFFF) << shf ) ; \
      dst |= ( (u_word_t)src << shf ) ;   \
    } else {                              \
      dst = src ;                         \
    }                                     \
  } while (0)

#define _st(typ, siz)                          \
  do {                                         \
    if (                                       \
      U_WORD(0x00000000) <= adr &&             \
      adr <= U_WORD(0x00000010)                \
    ) {                                        \
      if (0x0 <= adr && adr <= 0x3) {          \
        iom->irq = dat & 0xF ;                 \
      } else if (0x4 <= adr && adr <= 0x7) {   \
        /* read-only */                        \
      } else if (0x8 <= adr && adr <= 0xC) {   \
        _sttyp(iom->imr, dat, siz) ;           \
      } else {                                 \
        /* read-only */                        \
      }                                        \
                                               \
      return ;                                 \
    }                                          \
                                               \
    for (int id = 0 ; id < 0x20 ; ++id) {      \
      struct pm_dev_t * dev = iom->dev[id] ;   \
                                               \
      if (NULL == dev || NULL == dev->st##typ) \
        continue ;                             \
                                               \
      u_word_t ofs = adr - dev->adr ;          \
                                               \
      if (ofs < dev->len) {                    \
        dev->st##typ(dev, ofs, dat) ;          \
        return ;                               \
      }                                        \
    }                                          \
  } while (0)

__PM_PUBL void pm_iom_stb (struct pm_iom_t * iom, u_word_t adr, u_byte_t dat)
{
  _st(b, 0) ;
}

__PM_PUBL void pm_iom_sth (struct pm_iom_t * iom, u_word_t adr, u_half_t dat)
{
  _st(h, 1) ;
}

__PM_PUBL void pm_iom_stw (struct pm_iom_t * iom, u_word_t adr, u_word_t dat)
{
  _st(w, 2) ;
}

#undef _st
#undef _sttyp

__PM_PUBL void pm_iom_int (struct pm_iom_t * iom, struct pm_dev_t * dev)
{
  iom->ipr |= U_WORD(0x1) << dev->irq ;
}

__PM_PUBL u_word_t pm_iom_cks (struct pm_iom_t * iom)
{
  return pm_bus_cks(iom->bus) ;
}

__PM_PUBL u_word_t pm_iom_id (struct pm_iom_t * iom)
{
  return iom->irq & 0xF ;
}

#define _overlapping(a, b, c, d) ( 0 == ( b < c || a > d ) )

__PM_PUBL int pm_iom_mnt (struct pm_iom_t * iom, struct pm_dev_t * dev)
{
  if (NULL == dev || 0 == dev->len)
    return -1 ;

  int id ;

  for (id = 0 ; id < 0x20 ; ++id) {
    if (NULL == iom->dev[id])
      break ;
  }

  if (0x20 == id)
    return -2 ;

  u_word_t start = dev->adr ;
  u_word_t end   = start + dev->len - 1 ;

  for (int i = 0 ; i < 0x20 ; ++i) {
    if (NULL == iom->dev[i])
      continue ;

    u_word_t dev_start = iom->dev[i]->adr ;
    u_word_t dev_end   = dev_start + iom->dev[i]->len - 1 ;

    if (_overlapping(dev_start, dev_end, start, end))
      return -3 ;
  }

  dev->bus     = iom ;
  dev->irq     = id  ;
  iom->dev[id] = dev ;

  return 0 ;
}

#undef _overlapping

__PM_PUBL int pm_iom_umn (struct pm_iom_t * iom, struct pm_dev_t * dev)
{
  if (NULL == dev)
    return -1 ;

  for (int id = 0 ; id < 0x20 ; ++id) {
    if (dev == iom->dev[id]) {
      if (NULL != dev->rst) {
        dev->rst(dev, -1) ;
      }

      iom->dev[id] = NULL ;
      return 0 ;
    }
  }

  return -1 ;
}
