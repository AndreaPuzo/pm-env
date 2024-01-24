#ifndef __PM_LDR_LDR_H
# define __PM_LDR_LDR_H

# include "dir.h"
# include "mod.h"

struct pm_ldr_t {
  struct pm_ldr_dir_t * dir_fst ;
  struct pm_ldr_dir_t * dir_lst ;
  struct pm_ldr_mod_t * mod_fst ;
  struct pm_ldr_mod_t * mod_lst ;
} ;

__PM_PUBL int                   pm_ldr_ctor  (struct pm_ldr_t * ldr, int argc, char ** argv) ;
__PM_PUBL void                  pm_ldr_dtor  (struct pm_ldr_t * ldr) ;
__PM_PUBL int                   pm_ldr_dump  (struct pm_ldr_t * ldr, FILE * out) ;
__PM_PUBL struct pm_ldr_mod_t * pm_ldr_find  (struct pm_ldr_t * ldr, const char * mod) ;
__PM_PUBL int                   pm_ldr_open  (struct pm_ldr_t * ldr, const char * mod) ;
__PM_PUBL int                   pm_ldr_close (struct pm_ldr_t * ldr, const char * mod) ;
__PM_PUBL void *                pm_ldr_ldsym (struct pm_ldr_t * ldr, const char * mod, const char * sym) ;
__PM_PUBL void                  pm_ldr_ulsym (struct pm_ldr_t * ldr, const char * mod, const char * sym) ;

#endif
