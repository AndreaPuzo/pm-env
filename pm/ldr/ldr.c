#include "../pm.h"

__PM_PUBL int pm_ldr_ctor (struct pm_ldr_t * ldr, int argc, char ** argv)
{
}

__PM_PUBL void pm_ldr_dtor (struct pm_ldr_t * ldr)
{
}

__PM_PUBL int pm_ldr_dump (struct pm_ldr_t * ldr, FILE * out)
{}

__PM_PUBL struct pm_ldr_mod_t * pm_ldr_find (struct pm_ldr_t * ldr, const char * mod)
{
  struct pm_ldr_mod_t * cur = ldr->mod_fst ;

  while (NULL != cur) {
    if (0 == strcmp(cur->nam, mod))
      return cur ;
  }

  return NULL ;
}

__PM_PUBL int pm_ldr_open (struct pm_ldr_t * ldr, const char * mod)
{
  struct pm_ldr_mod_t res = pm_ldr_find(ldr, mod) ;

  if (NULL != res)
    return 0 ;

  struct pm_ldr_dir_t * cur = ldr->dir_fst ;

  while (NULL != cur) {
    res = pm_ldr_mod_ctor(ldr->mod_lst, cur, nam) ;

    if (NULL != res)
      break ;
  }

  if (NULL == res)
    return -1 ;

  ldr->mod_lst = res ;

  if (NULL == ldr->mod_fst) {
    ldr->mod_fst = res ;
  }

  return 0 ;
}

__PM_PUBL int pm_ldr_close (struct pm_ldr_t * ldr, const char * mod)
{
  struct pm_ldr_mod_t * res = pm_ldr_find(ldr, mod) ;

  if (NULL == res)
    return -1 ;

  if (res == ldr->mod_fst) {
    ldr->mod_fst = res->nxt ;
  } else if (res == ldr->mod_lst) {
    ldr->mod_lst = res->prv ;
  }

  pm_ldr_mod_dtor(res) ;

  return 0 ;
}

__PM_PUBL void * pm_ldr_ldsym (struct pm_ldr_t * ldr, const char * mod, const char * sym)
{
  struct pm_ldr_mod_t * res = pm_ldr_find(ldr, mod) ;

  if (NULL == res)
    return NULL ;

  return pm_ldr_mod_ldsym(res, sym) ;
}

__PM_PUBL void pm_ldr_ulsym (struct pm_ldr_t * ldr, const char * mod, const char * sym)
{
  struct pm_ldr_mod_t * res = pm_ldr_find(ldr, mod) ;

  if (NULL == res)
    return NULL ;

  pm_ldr_mod_ulsym(res, sym) ;
}
