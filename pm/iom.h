#ifndef __PM_IOM_H
# define __PM_IOM_H

/* -------------------------------------------------------------------------- *
 * Device Interface                                                           *
 * -------------------------------------------------------------------------- */

typedef int      ( * pm_dev_ctor_t ) (struct pm_dev_t * dev, struct pm_cfg_t cfg) ;
typedef void     ( * pm_dev_dtor_t ) (struct pm_dev_t * dev) ;
typedef void     ( * pm_dev_rst_t  ) (struct pm_dev_t * dev, int lvl) ;
typedef void     ( * pm_dev_clk_t  ) (struct pm_dev_t * dev) ;
typedef u_byte_t ( * pm_dev_ldb_t  ) (struct pm_dev_t * dev, u_word_t adr) ;
typedef u_half_t ( * pm_dev_ldh_t  ) (struct pm_dev_t * dev, u_word_t adr) ;
typedef u_word_t ( * pm_dev_ldw_t  ) (struct pm_dev_t * dev, u_word_t adr) ;
typedef void     ( * pm_dev_stb_t  ) (struct pm_dev_t * dev, u_word_t adr, u_byte_t dat) ;
typedef void     ( * pm_dev_sth_t  ) (struct pm_dev_t * dev, u_word_t adr, u_half_t dat) ;
typedef void     ( * pm_dev_stw_t  ) (struct pm_dev_t * dev, u_word_t adr, u_word_t dat) ;

struct pm_dev_t {
  struct pm_iom_t * bus  ;
  struct pm_cfg_t   cfg  ;
  u_word_t          sgn  ;
  u_word_t          irq  ;
  u_word_t          adr  ;
  u_word_t          len  ;
  pm_dev_ctor_t     ctor ;
  pm_dev_dtor_t     dtor ;
  pm_dev_rst_t      rst  ;
  pm_dev_clk_t      clk  ;
  pm_dev_ldb_t      ldb  ;
  pm_dev_ldh_t      ldh  ;
  pm_dev_ldw_t      ldw  ;
  pm_dev_stb_t      stb  ;
  pm_dev_sth_t      sth  ;
  pm_dev_stw_t      stw  ;
} ;

/* -------------------------------------------------------------------------- *
 * Input and Output Memory                                                    *
 * -------------------------------------------------------------------------- */

struct pm_iom_t {
  struct pm_bus_t * bus          ;
  struct pm_cfg_t   cfg          ;
  u_word_t          adr          ;
  u_word_t          len          ;
  u_word_t          irq          ;
  u_word_t          ipr          ;
  u_word_t          imr          ;
  u_word_t          isr          ;
  struct pm_dev_t * dev [ 0x20 ] ;
} ;

__PM_PUBL int      pm_iom_ctor (struct pm_iom_t * iom, struct pm_cfg_t cfg) ;
__PM_PUBL void     pm_iom_dtor (struct pm_iom_t * iom) ;
__PM_PUBL void     pm_iom_rst  (struct pm_iom_t * iom, int lvl) ;
__PM_PUBL void     pm_iom_clk  (struct pm_iom_t * iom) ;
__PM_PUBL u_byte_t pm_iom_ldb  (struct pm_iom_t * iom, u_word_t adr) ;
__PM_PUBL u_half_t pm_iom_ldh  (struct pm_iom_t * iom, u_word_t adr) ;
__PM_PUBL u_word_t pm_iom_ldw  (struct pm_iom_t * iom, u_word_t adr) ;
__PM_PUBL void     pm_iom_stb  (struct pm_iom_t * iom, u_word_t adr, u_byte_t dat) ;
__PM_PUBL void     pm_iom_sth  (struct pm_iom_t * iom, u_word_t adr, u_half_t dat) ;
__PM_PUBL void     pm_iom_stw  (struct pm_iom_t * iom, u_word_t adr, u_word_t dat) ;
__PM_PUBL void     pm_iom_int  (struct pm_iom_t * iom, struct pm_dev_t * dev) ;
__PM_PUBL u_word_t pm_iom_cks  (struct pm_iom_t * iom) ;
__PM_PUBL int      pm_iom_mnt  (struct pm_iom_t * iom, struct pm_dev_t * dev) ;
__PM_PUBL int      pm_iom_umn  (struct pm_iom_t * iom, struct pm_dev_t * dev) ;

#endif
