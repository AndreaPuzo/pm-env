#ifndef __PM_RAM_H
# define __PM_RAM_H

/* -------------------------------------------------------------------------- *
 * Status Register                                                            *
 * -------------------------------------------------------------------------- */

/* Shifts                                                                     */

# define PM_RAM_SRS_ID 0    /* Interrupt ID                                   */
# define PM_RAM_SRS_RN 4    /* RuNning                                        */
# define PM_RAM_SRS_BO 5    /* Byte Order                                     */
# define PM_RAM_SRS_RS 6    /* ReServed (buffer is malloc'd)                  */

/* Masks                                                                      */

# define PM_RAM_SRM_ID 0xF
# define PM_RAM_SRM_RN 0x1
# define PM_RAM_SRM_BO 0x1
# define PM_RAM_SRM_RS 0x1

/* Flags                                                                      */

# define PM_RAM_SRF_ID ( PM_RAM_SRM_ID << PM_RAM_SRS_ID )
# define PM_RAM_SRF_RN ( PM_RAM_SRM_RN << PM_RAM_SRS_RN )
# define PM_RAM_SRF_BO ( PM_RAM_SRM_BO << PM_RAM_SRS_BO )
# define PM_RAM_SRF_RS ( PM_RAM_SRM_RS << PM_RAM_SRS_RS )

/* -------------------------------------------------------------------------- *
 * Random Access Memory                                                       *
 * -------------------------------------------------------------------------- */

struct pm_ram_t {
  struct pm_bus_t * bus ;
  struct pm_cfg_t   cfg ;
  u_word_t          sr  ;
  u_word_t          adr ;
  u_word_t          len ;
  u_byte_t *        buf ;
} ;

__PM_PUBL int      pm_ram_ctor (struct pm_ram_t * ram, struct pm_cfg_t cfg) ;
__PM_PUBL void     pm_ram_dtor (struct pm_ram_t * ram) ;
__PM_PUBL void     pm_ram_rst  (struct pm_ram_t * ram, int lvl) ;
__PM_PUBL void     pm_ram_clk  (struct pm_ram_t * ram) ;
__PM_PUBL u_byte_t pm_ram_ldb  (struct pm_ram_t * ram, u_word_t adr) ;
__PM_PUBL u_half_t pm_ram_ldh  (struct pm_ram_t * ram, u_word_t adr) ;
__PM_PUBL u_word_t pm_ram_ldw  (struct pm_ram_t * ram, u_word_t adr) ;
__PM_PUBL void     pm_ram_stb  (struct pm_ram_t * ram, u_word_t adr, u_byte_t dat) ;
__PM_PUBL void     pm_ram_sth  (struct pm_ram_t * ram, u_word_t adr, u_half_t dat) ;
__PM_PUBL void     pm_ram_stw  (struct pm_ram_t * ram, u_word_t adr, u_word_t dat) ;
__PM_PUBL void     pm_ram_int  (struct pm_ram_t * ram) ;

#endif
