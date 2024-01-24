#include "../pm.h"
#include <stdlib.h>
#include <dlfcn.h>

__PM_PUBL struct pm_ldr_mod_t * pm_ldr_mod_ctor (struct pm_ldr_mod_t * prv, struct pm_ldr_dir_t * dir, const char * nam)
{
  char pth [ 0x1000 ] ;
  (void) sprintf(pth, sizeof(pth), "%s/%s", dir->pth, nam) ;

  void * ptr = dlopen(pth, RTLD_LAZY) ;

  if (NULL == ptr)
    return NULL ;

  struct pm_ldr_mod_t * mod = (struct pm_ldr_mod_t *)malloc(sizeof(struct pm_ldr_mod_t)) ;

  if (NULL == mod)
    return mod ;

  mod->prv = prv  ;
  mod->nxt = NULL ;
  mod->dir = dir  ;
  mod->nam = nam  ;
  mod->ptr = ptr  ;
  mod->ref = 0    ;

  return mod ;
}

__PM_PUBL void pm_ldr_mod_dtor (struct pm_ldr_mod_t * mod)
{
  if (0 != mod->ref)
    return ;

  if (NULL == mod)
    return ;

  if (NULL != mod->prv) {
    mod->prv->nxt = mod->nxt ;
  }

  if (NULL != mod->nxt) {
    mod->nxt->prv = mod->prv ;
  }

  if (NULL != mod->ptr) {
    (void) dlclose(mod->ptr) ;
  }

  free(mod) ;
}

__PM_PUBL int pm_ldr_mod_dump (struct pm_ldr_mod_t * mod, FILE * out)
{
  return fprintf(out, "%p:\tPREV %p\tNEXT %p\tPATH %p\tNAME `%s`\tDATA %p\tREFC %d",
    mod, mod->prv, mod->nxt, mod->dir, mod->nam, mod->ptr, mod->ref) ;
}

__PM_PUBL void * pm_ldr_mod_ldsym (struct pm_ldr_mod_t * mod, const char * sym)
{
  if (NULL == mod->ptr)
    return NULL ;

  void * ptr = dlsym(mod->ptr, sym) ;

  if (NULL == ptr)
    return ptr ;

  ++mod->ref ;
  return ptr ;
}

__PM_PUBL void pm_ldr_mod_ulsym (struct pm_ldr_mod_t * mod, const char * sym)
{
  (void)sym ;

  if (0 == mod->ref)
    return ;

  --mod->ref ;
}
