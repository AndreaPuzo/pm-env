#ifndef __PM_LDR_DIR_H
# define __PM_LDR_DIR_H

struct pm_ldr_dir_t {
  struct pm_ldr_dir_t * prv ;
  struct pm_ldr_dir_t * nxt ;
  const char *          pth ;
} ;

__PM_PUBL struct pm_ldr_dir_t * pm_ldr_dir_ctor (struct pm_ldr_dir_t * prv, const char * pth) ;
__PM_PUBL void                  pm_ldr_dir_dtor (struct pm_ldr_dir_t * dir) ;
__PM_PUBL int                   pm_ldr_dir_dump (struct pm_ldr_dir_t * dir, FILE * out) ;

#endif
