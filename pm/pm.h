#ifndef __PM_H
# define __PM_H

# include <stdint.h>

typedef uint8_t  u_byte_t ;
typedef uint16_t u_half_t ;
typedef uint32_t u_word_t ;

# define U_BYTE(n) UINT8_C(n)
# define U_HALF(n) UINT16_C(n)
# define U_WORD(n) UINT32_C(n)

# define U_BYTE_MIN U_BYTE(0)
# define U_BYTE_MAX UINT8_MAX
# define U_HALF_MIN U_HALF(0)
# define U_HALF_MAX UINT16_MAX
# define U_WORD_MIN U_WORD(0)
# define U_WORD_MAX UINT32_MAX

typedef  int8_t  s_byte_t ;
typedef  int16_t s_half_t ;
typedef  int32_t s_word_t ;

# define S_BYTE(n) INT8_C(n)
# define S_HALF(n) INT16_C(n)
# define S_WORD(n) INT32_C(n)

# define S_BYTE_MIN INT8_MIN
# define S_BYTE_MAX INT8_MAX
# define S_HALF_MIN INT16_MIN
# define S_HALF_MAX INT16_MAX
# define S_WORD_MIN INT32_MIN
# define S_WORD_MAX INT32_MAX

typedef    float f_word_t ;

# define F_WORD(n) n ## F 

# define __PM_VERSION_MAJOR U_WORD(0x00)
# define __PM_VERSION_MINOR U_WORD(0x00)
# define __PM_VERSION_PATCH U_WORD(0x0000)

# define __PM_VERSION              \
  (                                \
    ( __PM_VERSION_MAJOR << 24 ) | \
    ( __PM_VERSION_MINOR << 16 ) | \
    ( __PM_VERSION_PATCH <<  0 )   \
  )

# ifdef _WIN32
#  ifdef __PM_BUILD
#   define __PM_PUBL __attribute__((dllexport))
#  else
#   define __PM_PUBL __attribute__((dllimport))
#  endif
# else
#  define __PM_PUBL __attribute__((visibility("default")))
# endif

# define __PM_PRIV static

# define __PM_ENDIAN_LE U_WORD(0x01234567)
# define __PM_ENDIAN_BE U_WORD(0x67452301)
# define __PM_ENDIAN    __PM_ENDIAN_LE

__PM_PUBL u_word_t pm_endian (void) ;
__PM_PUBL u_byte_t pm_ldb_le (const u_byte_t * src) ;
__PM_PUBL u_byte_t pm_ldb_be (const u_byte_t * src) ;
__PM_PUBL u_half_t pm_ldh_le (const u_byte_t * src) ;
__PM_PUBL u_half_t pm_ldh_be (const u_byte_t * src) ;
__PM_PUBL u_word_t pm_ldw_le (const u_byte_t * src) ;
__PM_PUBL u_word_t pm_ldw_be (const u_byte_t * src) ;
__PM_PUBL void     pm_stb_le (u_byte_t * dst, u_byte_t src) ;
__PM_PUBL void     pm_stb_be (u_byte_t * dst, u_byte_t src) ;
__PM_PUBL void     pm_sth_le (u_byte_t * dst, u_half_t src) ;
__PM_PUBL void     pm_sth_be (u_byte_t * dst, u_half_t src) ;
__PM_PUBL void     pm_stw_le (u_byte_t * dst, u_word_t src) ;
__PM_PUBL void     pm_stw_be (u_byte_t * dst, u_word_t src) ;

__PM_PUBL int pm_str_to_uint (const char * str, char ** endptr, int rad, u_word_t * num) ;
__PM_PUBL int pm_str_to_sint (const char * str, char ** endptr, int rad, s_word_t * num) ;
__PM_PUBL int pm_str_to_chr  (const char * str, char ** endptr) ;
__PM_PUBL int pm_chr_to_str  (char * str, int chr) ;

# include "cfg.h"
# include "cpu.h"
# include "ram.h"
# include "iom.h"
# include "bus.h"

# define __PM__ __PM_VERSION

#endif
