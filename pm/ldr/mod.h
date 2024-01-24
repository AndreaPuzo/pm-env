#ifndef __PM_LDR_MOD_H
# define __PM_LDR_MOD_H

struct pm_ldr_mod_t {
  struct pm_ldr_mod_t * prv ;
  struct pm_ldr_mod_t * nxt ;
  struct pm_ldr_dir_t * dir ;
  const char *          nam ;
  void *                ptr ;
  int                   ref ;
} ;

__PM_PUBL struct pm_ldr_mod_t * pm_ldr_mod_ctor  (struct pm_ldr_mod_t * prv, struct pm_ldr_dir_t * dir, const char * nam) ;
__PM_PUBL void                  pm_ldr_mod_dtor  (struct pm_ldr_mod_t * mod) ;
__PM_PUBL int                   pm_ldr_mod_dump  (struct pm_ldr_mod_t * mod, FILE * out) ;
__PM_PUBL void *                pm_ldr_mod_ldsym (struct pm_ldr_mod_t * mod, const char * sym) ;
__PM_PUBL void                  pm_ldr_mod_ulsym (struct pm_ldr_mod_t * mod, const char * sym) ;

#endif
