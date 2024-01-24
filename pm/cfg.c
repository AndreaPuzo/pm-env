#include "pm.h"
#include <stdlib.h>

__PM_PRIV char * _read_file (const char * fn)
{
  FILE * fp = fopen(fn, "r") ;

  if (NULL == fp) {
    fprintf(stderr, "error: cannot open file `%s`\n", fn) ;
    return NULL ;
  }

  if (EOF == fseek(fp, 0, SEEK_END)) {
    fprintf(stderr, "error: cannot seek the end of file %s\n", fn) ;
    goto _failure ;
  }

  long len = ftell(fp) ;

  if (len < 0) {
    fprintf(stderr, "error: cannot compute the length of file %s\n", fn) ;
    goto _failure ;
  }

  if (EOF == fseek(fp, 0, SEEK_SET)) {
    fprintf(stderr, "error: cannot seek the start of file %s\n", fn) ;
    goto _failure ;
  }

  char * buf = (char *)malloc(( len + 1 ) * sizeof(char)) ;

  if (NULL == buf) {
    fprintf(stderr, "error: cannot allocate the buffer for file %s\n", fn) ;
    goto _failure ;
  }

  if (len != fread(buf, sizeof(char), len, fp)) {
    fprintf(stderr, "error: cannot read the entire file %s\n", fn) ;
    free(buf) ;
    goto _failure ;
  }

  buf[len] = '\0' ;

_success :
  fclose(fp) ;
  return buf ;

_failure :
  fclose(fp) ;
  return NULL ;
}

__PM_PRIV char * _cfg_dup_tok (struct pm_cfg_t * cfg, const char * str, size_t len) ;
__PM_PRIV char * _cfg_get_tok (struct pm_cfg_t * cfg, char ** str) ;
__PM_PRIV int    _cfg_add_tok (struct pm_cfg_t * cfg, char * str) ;

__PM_PUBL int pm_cfg_ctor (struct pm_cfg_t * cfg, const char * str)
{
  cfg->argc = 0    ;
  cfg->argv = NULL ;

  if (NULL != str) {
    char * buf = NULL ;

    if (':' == *str) {
      buf = _read_file(str + 1) ;

      if (NULL == buf)
        return -1 ;
    }
    
    char * cur ;

    if (NULL == buf) {
      cur = (char *)str ;
    } else {
      cur = buf ;
    }

/*    
    if ('\'' == *cur || '\"' == *cur) {
      char   quote = *cur  ;
      char * start = ++cur ;

      while ('\0' != *cur && quote != *cur) {
        if ('\\' == *cur && quote == cur[1]) {
          ++cur ;
        }

        ++cur ;
      }

      if (quote != *cur) {
        fprintf(stderr, "error: unclosed string\n") ;
        return -2 ;
      }

      if ('\0' == cur[1]) {
        cur = start ;
      } else {
        cur = start - 1 ;
      }
    }
*/
    while ('\0' != *cur) {
      char * tok = _cfg_get_tok(cfg, &cur) ;

      if (NULL == tok)
        continue ;

      if (0 != _cfg_add_tok(cfg, tok)) {
        fprintf(stderr, "error: cannot add token `%s`\n", tok) ;
        free(tok) ;

        if (NULL != buf) {
          free(buf) ;
        }

        return -3 ;
      }
    }

    if (NULL != buf) {
      free(buf) ;
    }
  }

  if (0 != _cfg_add_tok(cfg, NULL)) {
    fprintf(stderr, "error: cannot add the end of tokens\n") ;
    return -4 ;
  }

  --cfg->argc ;
  return 0 ;
}

__PM_PUBL void pm_cfg_dtor (struct pm_cfg_t * cfg)
{
  for (int argi = 0 ; argi < cfg->argc ; ++argi) {
    char * args = cfg->argv[argi] ;

    if (NULL != args) {
      free(args) ;
    }
  }

  if (NULL != cfg->argv) {
    free(cfg->argv) ;
  }
}

__PM_PUBL int pm_cfg_dump (struct pm_cfg_t * cfg, FILE * out)
{
  int res = 0 ;

  res += fprintf(out, "Configuration at %p:\n", cfg) ;

  for (int argi = 0 ; argi < cfg->argc ; ++argi) {
    char * args = cfg->argv[argi] ;
 
    res += fprintf(out, "\t%u\t", argi) ;
    
    if (NULL == args) {
      res += fprintf(out, "NULL\n") ;
    } else {
      res += fprintf(out, "`") ;

      for (int i = 0 ; '\0' != args[i] ; ++i) {
        char chr [ 4 ] ;
        int  len = pm_chr_to_str(chr, args[i]) ;

        if (len < 0)
          continue ;

        res += fprintf(out, "%.*s", len, chr) ;
      }

      res += fprintf(out, "`\n") ;
    }
  }

  return res ;
}

#include <string.h>

__PM_PRIV char * _cfg_dup_tok (struct pm_cfg_t * cfg, const char * str, size_t len)
{
  size_t cnt = 0 ;
  char * cur = (char *)str ;

  for (cnt = 0 ; cnt < len ; ++cnt) {
    int chr = pm_str_to_chr(cur, &cur) ;

    if ('\0' == chr)
      break ;
  }

  char * buf = (char *)malloc(( cnt + 1 ) * sizeof(char)) ;

  if (NULL == buf)
    return buf ;

  cnt = 0 ;
  cur = (char *)str ;
  int chr ;
  
  for (cnt = 0 ; cnt < len ; ++cnt) {
    int chr = pm_str_to_chr(cur, &cur) ;

    if ('\0' == chr)
      break ;

    buf[cnt] = chr ;
  }

  buf[cnt] = '\0' ;

  return buf ;
}

/*
__PM_PRIV char * _cfg_dup_tok (struct pm_cfg_t * cfg, const char * str, size_t len)
{
  size_t dup_len = 0 ;

  for (size_t i = 0 ; i < len && '\0' != str[i] ; ++i) {
    char chr [ 4 ] ;
    int  res = pm_chr_to_str(chr, str[i]) ;

    if (res < 0)
      continue ;

    dup_len += res ;
  }

  char * dup_buf = (char *)malloc(dup_len * sizeof(char)) ;

  if (NULL == dup_buf)
    return dup_buf ;

  dup_len = 0 ;

  for (size_t i = 0 ; i < len && '\0' != str[i] ; ++i) {
    char chr [ 4 ] ;
    int  res = pm_chr_to_str(chr, str[i]) ;

    if (res < 0)
      continue ;

    (void) strncpy(dup_buf + dup_len, chr, res) ;
    dup_len += res ;
  }

  return dup_buf ;
}
*/

__PM_PRIV char * _cfg_get_tok (struct pm_cfg_t * cfg, char ** str)
{
  char * cur = *str ;
  
  while ('\n' == *cur || '\r' == *cur) {
    ++cur ;
  }

  while (' '  == *cur || '\t' == *cur) {
    ++cur ;
  }

  if (*cur < ' ' || '~' < *cur) {
    *str = cur ;
    return NULL ;
  }

  if (';' == *cur) {
    while ('\0' != *cur && '\n' != *cur && '\r' != *cur) {
      ++cur ;
    }

    *str = cur ;
    return NULL ;
  }

  char * tok   = cur  ;
  char   quote = '\0' ;

  while ('\0' != *cur && '\n' != *cur && '\r' != *cur) {
    if ('\0' == quote) {
      if (' ' == *cur || '\t' == *cur || ';' == *cur)
        break ;

      if ('\'' == *cur || '\"' == *cur) {
        quote = *cur ;
      }
    } else {
      if ('\\' == *cur && quote == cur[1]) {
        ++cur ;
      } else if (quote == *cur) {
        quote = '\0' ;
      }
    }

    ++cur ;
  }

  *str = cur ;

  if (tok == cur)
    return NULL ;

  if ('\'' == *tok || '\"' == *tok) {
    if (*tok == cur[-1])
      return _cfg_dup_tok(cfg, tok + 1, ( (size_t)cur - (size_t)tok ) - 2) ;
  }

  return _cfg_dup_tok(cfg, tok, (size_t)cur - (size_t)tok) ;
}

__PM_PRIV int _cfg_add_tok (struct pm_cfg_t * cfg, char * str)
{
  int     argc = cfg->argc + 1 ;
  char ** argv = (char **)realloc(cfg->argv, argc * sizeof(char *)) ;

  if (NULL == argv)
    return -1 ;

  argv[cfg->argc] = str ;

  cfg->argv = argv ;
  cfg->argc = argc ;

  return 0 ;
}
