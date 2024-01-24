#include "pm.h"

__PM_PUBL u_word_t pm_endian (void)
{
  u_word_t num = U_WORD(0x00FF00FF)  ;
  u_byte_t dat = *((u_byte_t *)&num) ;

  return
    __PM_ENDIAN_LE * ( 0x00 == dat ) +
    __PM_ENDIAN_BE * ( 0x00 != dat ) ;
}

__PM_PUBL u_byte_t pm_ldb_le (const u_byte_t * src)
{
  return src[0] ;
}

__PM_PUBL u_byte_t pm_ldb_be (const u_byte_t * src)
{
  return src[0] ;
}

__PM_PUBL u_half_t pm_ldh_le (const u_byte_t * src)
{
  u_half_t dst = U_HALF(0) ;

  if (__PM_ENDIAN_LE == pm_endian()) {
    dst = *(u_half_t *)src ;
  } else {
    dst |= (u_half_t)src[0] << 0 ;
    dst |= (u_half_t)src[1] << 8 ;
  }

  return dst ;
}

__PM_PUBL u_half_t pm_ldh_be (const u_byte_t * src)
{
  u_half_t dst = U_HALF(0) ;

  if (__PM_ENDIAN_BE == pm_endian()) {
    dst = *(u_half_t *)src ;
  } else {
    dst |= (u_half_t)src[0] << 8 ;
    dst |= (u_half_t)src[1] << 0 ;
  }

  return dst ;
}

__PM_PUBL u_word_t pm_ldw_le (const u_byte_t * src)
{
  u_word_t dst = U_WORD(0) ;

  if (__PM_ENDIAN_LE == pm_endian()) {
    dst = *(u_word_t *)src ;
  } else {
    dst |= (u_word_t)src[0] <<  0 ;
    dst |= (u_word_t)src[1] <<  8 ;
    dst |= (u_word_t)src[2] << 16 ;
    dst |= (u_word_t)src[3] << 24 ;
  }

  return dst ;
}

__PM_PUBL u_word_t pm_ldw_be (const u_byte_t * src)
{
  u_word_t dst = U_WORD(0) ;

  if (__PM_ENDIAN_BE == pm_endian()) {
    dst = *(u_word_t *)src ;
  } else {
    dst |= (u_word_t)src[0] << 24 ;
    dst |= (u_word_t)src[1] << 16 ;
    dst |= (u_word_t)src[2] <<  8 ;
    dst |= (u_word_t)src[3] <<  0 ;
  }

  return dst ;
}

__PM_PUBL void pm_stb_le (u_byte_t * dst, u_byte_t src)
{
  dst[0] = src ;
}

__PM_PUBL void pm_stb_be (u_byte_t * dst, u_byte_t src)
{
  dst[0] = src ;
}

__PM_PUBL void pm_sth_le (u_byte_t * dst, u_half_t src)
{
  if (__PM_ENDIAN_LE == pm_endian()) {
    *(u_half_t *)dst = src ;
  } else {
    dst[0] = (u_byte_t)( ( src >> 0 ) & 0xFF ) ;
    dst[1] = (u_byte_t)( ( src >> 8 ) & 0xFF ) ;
  }
}

__PM_PUBL void pm_sth_be (u_byte_t * dst, u_half_t src)
{
  if (__PM_ENDIAN_BE == pm_endian()) {
    *(u_half_t *)dst = src ;
  } else {
    dst[0] = (u_byte_t)( ( src >> 8 ) & 0xFF ) ;
    dst[1] = (u_byte_t)( ( src >> 0 ) & 0xFF ) ;
  }
}

__PM_PUBL void pm_stw_le (u_byte_t * dst, u_word_t src)
{
  if (__PM_ENDIAN_LE == pm_endian()) {
    *(u_word_t *)dst = src ;
  } else {
    dst[0] = (u_byte_t)( ( src >>  0 ) & 0xFF ) ;
    dst[1] = (u_byte_t)( ( src >>  8 ) & 0xFF ) ;
    dst[2] = (u_byte_t)( ( src >> 16 ) & 0xFF ) ;
    dst[3] = (u_byte_t)( ( src >> 24 ) & 0xFF ) ;
  }
}

__PM_PUBL void pm_stw_be (u_byte_t * dst, u_word_t src)
{
  if (__PM_ENDIAN_BE == pm_endian()) {
    *(u_half_t *)dst = src ;
  } else {
    dst[0] = (u_byte_t)( ( src >> 24 ) & 0xFF ) ;
    dst[1] = (u_byte_t)( ( src >> 16 ) & 0xFF ) ;
    dst[2] = (u_byte_t)( ( src >>  8 ) & 0xFF ) ;
    dst[3] = (u_byte_t)( ( src >>  0 ) & 0xFF ) ;
  }
}

#include <stdlib.h>
#include <limits.h>
#include <errno.h>

__PM_PUBL int pm_str_to_uint (const char * str, char ** endptr, int rad, u_word_t * num)
{
  if (NULL == str)
    return -1 ;

  unsigned long res = strtoul(str, endptr, rad) ;

  if (0 == res || ULONG_MAX == res) {
    if (ERANGE == errno)
      return -2 ;
  }

  if (U_WORD_MAX < res)
    return -3 ;

  *num = (u_word_t)res ;

  return 0 ;
}

__PM_PUBL int pm_str_to_sint (const char * str, char ** endptr, int rad, s_word_t * num)
{
  if (NULL == str)
    return -1 ;

  signed long res = strtol(str, endptr, rad) ;

  if (0 == *num || LONG_MIN == *num || LONG_MAX == *num) {
    if (ERANGE == errno)
      return -2 ;
  }

  if (res < S_WORD_MIN || S_WORD_MAX < res)
    return -3 ;

  *num = (s_word_t)res ;

  return 0 ;
}

__PM_PUBL int pm_str_to_chr (const char * str, char ** endptr)
{
  char * cur = (char *)str ;
  int chr = *cur++ ;

  if ('\\' == chr) {
    chr = *cur++ ;

    switch (chr) {
    case 'a'  : { chr = '\a'   ; } break ;
    case 'c'  : { chr = '\b'   ; } break ;
    case 't'  : { chr = '\t'   ; } break ;
    case 'n'  : { chr = '\n'   ; } break ;
    case 'v'  : { chr = '\v'   ; } break ;
    case 'f'  : { chr = '\f'   ; } break ;
    case 'r'  : { chr = '\r'   ; } break ;
    case 'e'  : { chr = '\x1B' ; } break ;
    case 's'  : { chr = ' '    ; } break ;
    case '!'  : { chr = '!'    ; } break ;
    case '?'  : { chr = '?'    ; } break ;
    case '\'' : { chr = '\''   ; } break ;
    case '\"' : { chr = '\"'   ; } break ;
    case 'g'  : { chr = '`'    ; } break ;
    case 'd'  : { chr = '\x7F' ; } break ;

    case '0'  :
    case 'b'  : case 'B' :
    case 'x'  : case 'X' :
    default   : {
      int rad =
        ( '0' == chr               ) *  8 +
        ( 'b' == chr || 'B' == chr ) *  2 +
        ( 'x' == chr || 'X' == chr ) * 16 ;

      if (0 == rad) {
        rad = 10 ;
      }

      u_word_t num ;

      if (0 != pm_str_to_uint(cur, &cur, rad, &num))
        return -1 ;

      chr = (char)( num & 0xFF ) ;
    } break ; 
    }
  }

  if (NULL != endptr) {
    *endptr = cur ;
  }

  return chr ;
}

__PM_PUBL int pm_chr_to_str (char * str, int chr)
{
  static const char hex [] = "0123456789ABCDEF" ;

  int n = 0 ;

  switch (chr) {
  case '\a'   : { str[n++] = '\\' ; str[n++] = 'a'  ; } break ;
  case '\b'   : { str[n++] = '\\' ; str[n++] = 'c'  ; } break ;
  case '\t'   : { str[n++] = '\\' ; str[n++] = 't'  ; } break ;
  case '\n'   : { str[n++] = '\\' ; str[n++] = 'n'  ; } break ;
  case '\v'   : { str[n++] = '\\' ; str[n++] = 'v'  ; } break ;
  case '\f'   : { str[n++] = '\\' ; str[n++] = 'f'  ; } break ;
  case '\r'   : { str[n++] = '\\' ; str[n++] = 'r'  ; } break ;
  case '\x1B' : { str[n++] = '\\' ; str[n++] = 'e'  ; } break ;
  case ' '    : { str[n++] = '\\' ; str[n++] = 's'  ; } break ;
  case '!'    : { str[n++] = '\\' ; str[n++] = '!'  ; } break ;
  case '?'    : { str[n++] = '\\' ; str[n++] = '?'  ; } break ;
  case '\''   : { str[n++] = '\\' ; str[n++] = '\'' ; } break ;
  case '\"'   : { str[n++] = '\\' ; str[n++] = '\"' ; } break ;
  case '`'    : { str[n++] = '\\' ; str[n++] = 'g'  ; } break ;
  case '\x7F' : { str[n++] = '\\' ; str[n++] = 'd'  ; } break ;
  default     : {
    if (' ' < chr && chr <= '~') {
      str[n++] = chr ;
    } else {
      str[n++] = '\\' ;
      str[n++] = 'x'  ;
      str[n++] = hex[( chr >> 4 ) & 0xF] ;
      str[n++] = hex[( chr >> 0 ) & 0xF] ;
    }
  } break ;
  }

  return n ;
}
