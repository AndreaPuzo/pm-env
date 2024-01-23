#ifndef __PM_CFG_H
# define __PM_CFG_H

# include <stdio.h>

struct pm_cfg_t {
  int     argc ;
  char ** argv ;
} ;

__PM_PUBL int  pm_cfg_ctor (struct pm_cfg_t * cfg, const char * str) ;
__PM_PUBL void pm_cfg_dtor (struct pm_cfg_t * cfg) ;
__PM_PUBL int  pm_cfg_dump (struct pm_cfg_t * cfg, FILE * out) ;

#endif
