#include "../pm.h"
#include <stdlib.h>

__PM_PUBL struct pm_ldr_dir_t * pm_ldr_dir_ctor (struct pm_ldr_dir_t * prv, const char * pth)
{
  if (NULL == pth)
    return NULL ;

  struct pm_ldr_dir_t * dir = (struct pm_ldr_dir_t *)malloc(sizeof(struct pm_ldr_dir_t)) ;

  if (NULL == dir)
    return dir ;

  dir->prv = prv  ;
  dir->nxt = NULL ;
  dir->pth = pth  ;

  return dir ;
}

__PM_PUBL void pm_ldr_dir_dtor (struct pm_ldr_dir_t * dir)
{
  if (NULL == dir)
    return ;

  if (NULL != dir->prv) {
    dir->prv->nxt = dir->nxt ;
  }

  if (NULL != dir->nxt) {
    dir->nxt->prv = dir->prv ;
  }

  free(dir) ;
}

__PM_PUBL int pm_ldr_dir_dump (struct pm_ldr_dir_t * dir, FILE * out)
{
  return fprintf(out, "%p:\tPREV %p\tNEXT %p\tPATH `%s`",
    dir, dir->prv, dir->nxt, dir->pth) ;
}
