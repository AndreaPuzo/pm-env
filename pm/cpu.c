#include "pm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

__PM_PRIV u_byte_t _cpu_plb   (struct pm_cpu_t * cpu, u_word_t * adr) ;
__PM_PRIV u_half_t _cpu_plh   (struct pm_cpu_t * cpu, u_word_t * adr) ;
__PM_PRIV u_word_t _cpu_plw   (struct pm_cpu_t * cpu, u_word_t * adr) ;
__PM_PRIV void     _cpu_psb   (struct pm_cpu_t * cpu, u_word_t * adr, u_byte_t dat) ;
__PM_PRIV void     _cpu_psh   (struct pm_cpu_t * cpu, u_word_t * adr, u_half_t dat) ;
__PM_PRIV void     _cpu_psw   (struct pm_cpu_t * cpu, u_word_t * adr, u_word_t dat) ;
__PM_PRIV u_word_t _cpu_fch   (struct pm_cpu_t * cpu) ;
__PM_PRIV u_word_t _cpu_gecsr (struct pm_cpu_t * cpu, u_word_t adr, u_word_t shf, u_word_t msk) ;
__PM_PRIV void     _cpu_secsr (struct pm_cpu_t * cpu, u_word_t adr, u_word_t shf, u_word_t msk, u_word_t dat) ;
__PM_PRIV u_word_t _cpu_trs   (struct pm_cpu_t * cpu, u_word_t adr, u_word_t xwr) ;
__PM_PRIV int      _cpu_xhgsp (struct pm_cpu_t * cpu, u_word_t dst_pl, u_word_t src_pl) ;
__PM_PRIV int      _cpu_int   (struct pm_cpu_t * cpu, u_word_t fun, u_word_t dat) ;

#define _cpu_PL_set(cpu, dat) _cpu_secsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_PL, PM_CPU_SRM_PL, dat)
#define _cpu_PL_get(cpu)      _cpu_gecsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_PL, PM_CPU_SRM_PL)
#define _cpu_ID_set(cpu, dat) _cpu_secsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_ID, PM_CPU_SRM_ID, dat)
#define _cpu_ID_get(cpu)      _cpu_gecsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_ID, PM_CPU_SRM_ID)
#define _cpu_MS_set(cpu)      _cpu_secsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_MS, PM_CPU_SRM_MS, 0x1)
#define _cpu_MS_clr(cpu)      _cpu_secsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_MS, PM_CPU_SRM_MS, 0x0)
#define _cpu_MS_get(cpu)      _cpu_gecsr(cpu, PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_SR), PM_CPU_SRS_MS, PM_CPU_SRM_MS)

#define _cpu_RN_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RN, PM_CPU_SRM_RN, 0x1)
#define _cpu_RS_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RS, PM_CPU_SRM_RS, 0x1)
#define _cpu_EI_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_EI, PM_CPU_SRM_EI, 0x1)
#define _cpu_SI_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SI, PM_CPU_SRM_SI, 0x1)
#define _cpu_WI_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_WI, PM_CPU_SRM_WI, 0x1)
#define _cpu_SS_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SS, PM_CPU_SRM_SS, 0x1)
#define _cpu_SK_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SK, PM_CPU_SRM_SK, 0x1)
#define _cpu_PM_set(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_PM, PM_CPU_SRM_PM, 0x1)
#define _cpu_RN_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RN, PM_CPU_SRM_RN, 0x0)
#define _cpu_RS_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RS, PM_CPU_SRM_RS, 0x0)
#define _cpu_EI_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_EI, PM_CPU_SRM_EI, 0x0)
#define _cpu_SI_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SI, PM_CPU_SRM_SI, 0x0)
#define _cpu_WI_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_WI, PM_CPU_SRM_WI, 0x0)
#define _cpu_SS_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SS, PM_CPU_SRM_SS, 0x0)
#define _cpu_SK_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SK, PM_CPU_SRM_SK, 0x0)
#define _cpu_PM_clr(cpu, pl)  _cpu_secsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_PM, PM_CPU_SRM_PM, 0x0)
#define _cpu_RN_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RN, PM_CPU_SRM_RN)
#define _cpu_RS_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_RS, PM_CPU_SRM_RS)
#define _cpu_EI_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_EI, PM_CPU_SRM_EI)
#define _cpu_SI_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SI, PM_CPU_SRM_SI)
#define _cpu_WI_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_WI, PM_CPU_SRM_WI)
#define _cpu_SS_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SS, PM_CPU_SRM_SS)
#define _cpu_SK_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_SK, PM_CPU_SRM_SK)
#define _cpu_PM_get(cpu, pl)  _cpu_gecsr(cpu, PM_CPU_CSR(pl, PM_CPU_SR), PM_CPU_SRS_PM, PM_CPU_SRM_PM)

__PM_PUBL int pm_cpu_ctor (struct pm_cpu_t * cpu, struct pm_cfg_t cfg)
{
  _cpu_MS_clr(cpu) ;

  if (0 == cfg.argc || NULL == cfg.argv)
    return -1 ;

  cpu->cfg = cfg ;

  for (int argi = 0 ; argi < cfg.argc ; ++argi) {
    char * args = cfg.argv[argi] ;

    if (0 == strcmp(args, "-i") || 0 == strcmp(args, "--irq")) {
      u_word_t id ;
      char * endptr = NULL ;
 
      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &id)) {
        fprintf(stderr, "error: cpu: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      _cpu_ID_set(cpu, ( id & 0xF )) ;
    } else if (0 == strcmp(args, "-e") || 0 == strcmp(args, "--entry")) {
      u_word_t pc ;
      char * endptr = NULL ;

      if (0 != pm_str_to_uint(cfg.argv[++argi], &endptr, 0, &pc)) {
        fprintf(stderr, "error: cpu: expected unsigned integer for option %s\n", args) ;
        return -2 ;
      }

      cpu->pc[0] = pc ;
    } else if (0 == strcmp(args, "-m") || 0 == strcmp(args, "--master")) {
      _cpu_MS_set(cpu) ;
    } else {
      cpu->bus = (struct pm_bus_t *)args ;
    }
  }

  if (NULL == cpu->bus) {
    fprintf(stderr, "error: cpu: unmounted device\n") ;
    return -3 ;
  }

  return 0 ;
}

__PM_PUBL void pm_cpu_dtor (struct pm_cpu_t * cpu)
{}

__PM_PUBL void pm_cpu_rst (struct pm_cpu_t * cpu, int lvl)
{
  if (lvl < 0) {
    pm_cpu_dtor(cpu) ;
    
    if (0 != pm_cpu_ctor(cpu, cpu->cfg))
      return ;
  }

  const u_word_t MS = _cpu_MS_get(cpu) ;

  cpu->ck[0] = cpu->ck[1] = U_WORD(0) ;

  for (int idx = 0 ; idx < 0x20 ; ++idx) {
    cpu->csr[idx] = U_WORD(0x00000000) ;
    cpu->xpr[idx] = U_WORD(0x00000000) ;
    cpu->fpr[idx] = U_WORD(0x00000000) ;
  }

  if (0 == MS) {
    _cpu_MS_clr(cpu) ;
    pm_bus_rdy(bus, pm_cpu_id(cpu)) ;
  } else {
    _cpu_MS_set(cpu) ;
    _cpu_RN_set(cpu, PM_CPU_PL_M) ;
    pm_bus_bsy(bus, pm_cpu_id(cpu)) ;
  }

  _cpu_RS_set(cpu, PM_CPU_PL_M) ;
  pm_bus_int(cpu->bus, _cpu_ID_get(cpu)) ;
}

__PM_PUBL void pm_cpu_clk (struct pm_cpu_t * cpu)
{
  if (0 < cpu->ck[1]) {
    --cpu->ck[1] ;
    ++cpu->ck[0] ;
    return ;
  }

  cpu->xpr[0] = U_WORD(0x00000000) ;

  /* handle interrupts */

  u_word_t pl = _cpu_PL_get(cpu) ;
  int wait_for_int  = 0 ;

  for (int idx = 0 ; idx < 0x4 ; ++idx) {
    if (0 == _cpu_EI_get(cpu, idx)) {
      if (0 != _cpu_WI_get(cpu, idx) && pl == idx) {
        wait_for_int = 1 ;
      }

      continue ;
    }

    do {
      const u_word_t IMR = cpu->csr[PM_CPU_CSR(idx, PM_CPU_IMR)] ;
      u_word_t *     ipr = cpu->csr + PM_CPU_CSR(idx, PM_CPU_IPR) ;

      *ipr &= ~IMR ;

      if (0 == *ipr) {
        if (0 != _cpu_WI_get(cpu, idx) && pl == idx) {
          wait_for_int = 1 ;
        }

        break ;
      }

      int num ;

      for (num = 0 ; num < 0x20 ; ++num) {
        if (0 != ( ( *ipr >> num ) & 0x1 ))
          break ;
      }

      *ipr &= ~( U_WORD(1) << num ) ;

      if (0 == _cpu_int(cpu, 0, num)) {
        if (0 != _cpu_WI_get(cpu, idx) && pl == idx) {
          _cpu_WI_clr(cpu, idx) ;
          wait_for_int = 0 ;
        }

        break ;
      }
    } while (1) ;
  }

  if (0 != wait_for_int) {
    ++cpu->ck[0] ;
    return ;
  }

  /* fetch the next instruction */

  s_word_t ins = _cpu_fch(cpu) ;
  cpu->ins = ins ;

  /* decode the instruction */

  u_word_t o = (u_word_t)( ( ins >>  0 ) & 0x01F ) ;
  u_word_t a = (u_word_t)( ( ins >>  5 ) & 0x01F ) ;
  u_word_t b = (u_word_t)( ( ins >> 10 ) & 0x01F ) ;
  u_word_t c = (u_word_t)( ( ins >> 15 ) & 0x01F ) ;
  u_word_t u = (u_word_t)( ( ins >> 20 ) & 0xFFF ) ;
  u_word_t s = (u_word_t)  ( ins >> 20 ) ;

  /* execute the instruction */

  switch (o) {
  case 0x00 : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] + (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x01 : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] - (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x02 : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] * (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x03 : {
    s += cpu->xpr[c] ;

    if (0 == s) {
      pm_cpu_int(cpu, PM_CPU_INT_DZ) ;
      break ;
    }

    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] / (u_word_t)s
    ) ;
  } break ;

  case 0x04 : {
    s += cpu->xpr[c] ;

    if (0 == s) {
      pm_cpu_int(cpu, PM_CPU_INT_DZ) ;
      break ;
    }

    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] % (u_word_t)s
    ) ;
  } break ;

  case 0x05 : {
    cpu->xpr[a] = (u_word_t)(
      (s_word_t)cpu->xpr[b] * (s_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x06 : {
    s += cpu->xpr[c] ;

    if (0 == s) {
      pm_cpu_int(cpu, PM_CPU_INT_DZ) ;
      break ;
    }

    cpu->xpr[a] = (u_word_t)(
      (s_word_t)cpu->xpr[b] / (s_word_t)s
    ) ;
  } break ;

  case 0x07 : {
    s += cpu->xpr[c] ;

    if (0 == s) {
      pm_cpu_int(cpu, PM_CPU_INT_DZ) ;
      break ;
    }

    cpu->xpr[a] = (u_word_t)(
      (s_word_t)cpu->xpr[b] % (s_word_t)s
    ) ;
  } break ;

  case 0x08 : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] & (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x09 : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] | (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x0A : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] ^ (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x0B : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] << (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x0C : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)cpu->xpr[b] >> (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x0D : {
    cpu->xpr[a] = (u_word_t)(
      (S_word_t)cpu->xpr[b] >> (u_word_t)( cpu->xpr[c] + s )
    ) ;
  } break ;

  case 0x0E : {
    u = ( u << 4 ) | ( c >> 1 ) ;

    if (0 == ( c & 0x1 )) {
      cpu->xpr[a] = ( cpu->xpr[b] & U_WORD(0xFFFF0000) ) | ( u <<  0 ) ;
    } else {
      cpu->xpr[a] = ( cpu->xpr[b] & U_WORD(0x0000FFFF) ) | ( u << 16 ) ;
    }
  } break ;

  case 0x0F : {
    s = ( s << 4 ) | ( c >> 1 ) ;
    cpu->xpr[a] = cpu->pc[0] ;

    if (0 == ( c & 0x1 )) {
      cpu->pc[0]  = cpu->xpr[b] + s ;
    } else {
      cpu->pc[0] += (cpu->xpr[b] + s) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x10 : {
    if ((u_word_t)cpu->xpr[a] == (u_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x11 : {
    if ((u_word_t)cpu->xpr[a] != (u_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x12 : {
    if ((u_word_t)cpu->xpr[a] <  (u_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x13 : {
    if ((u_word_t)cpu->xpr[a] <= (u_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x14 : {
    if ((s_word_t)cpu->xpr[a] <  (s_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x15 : {
    if ((s_word_t)cpu->xpr[a] <= (s_word_t)cpu->xpr[b]) {
      cpu->pc[0] += ( cpu->xpr[c] + s ) * sizeof(u_word_t) ;
    }
  } break ;

  case 0x16 : {
    cpu->xpr[a] = (u_word_t)((s_word_t)
      (s_byte_t)pm_cpu_ldb(cpu, cpu->xpr[b] + cpu->xpr[c] + s)
    ) ;
  } break ;

  case 0x17 : {
    cpu->xpr[a] = (u_word_t)((s_word_t)
      (s_half_t)pm_cpu_ldh(cpu, cpu->xpr[b] + cpu->xpr[c] + s)
    ) ;
  } break ;

  case 0x18 : {
    cpu->xpr[a] = (u_word_t)(
      (u_byte_t)pm_cpu_ldb(cpu, cpu->xpr[b] + cpu->xpr[c] + s)
    ) ;
  } break ;

  case 0x19 : {
    cpu->xpr[a] = (u_word_t)(
      (u_half_t)pm_cpu_ldh(cpu, cpu->xpr[b] + cpu->xpr[c] + s)
    ) ;
  } break ;

  case 0x1A : {
    cpu->xpr[a] = (u_word_t)(
      (u_word_t)pm_cpu_ldw(cpu, cpu->xpr[b] + cpu->xpr[c] + s)
    ) ;
  } break ;

  case 0x1B : {
    pm_cpu_stb(cpu, cpu->xpr[b] + cpu->xpr[c] + s, (u_byte_t)cpu->xpr[a]) ;
  } break ;

  case 0x1C : {
    pm_cpu_sth(cpu, cpu->xpr[b] + cpu->xpr[c] + s, (u_half_t)cpu->xpr[a]) ;
  } break ;

  case 0x1D : {
    pm_cpu_stw(cpu, cpu->xpr[b] + cpu->xpr[c] + s, (u_word_t)cpu->xpr[a]) ;
  } break ;

  case 0x1E : {
    switch (c) {
    case 0x00 : {
      _cpu_psb(cpu, cpu->xpr + a, (u_byte_t)( cpu->xpr[b] + s )) ;
    } break ;

    case 0x01 : {
      _cpu_psh(cpu, cpu->xpr + a, (u_half_t)( cpu->xpr[b] + s )) ;
    } break ;

    case 0x02 : {
      _cpu_psw(cpu, cpu->xpr + a, (u_word_t)( cpu->xpr[b] + s )) ;
    } break ;

    case 0x03 : {
      cpu->xpr[b] = (u_word_t)((s_word_t)
        (s_byte_t)_cpu_plb(cpu, cpu->xpr + a)
      ) ;
    } break ;

    case 0x04 : {
      cpu->xpr[b] = (u_word_t)(
        (u_byte_t)_cpu_plb(cpu, cpu->xpr + a)
      ) ;
    } break ;

    case 0x05 : {
      cpu->xpr[b] = (u_word_t)(
        (u_half_t)_cpu_plh(cpu, cpu->xpr + a)
      ) ;
    } break ;

    case 0x06 : {
      cpu->xpr[b] = (u_word_t)(
        (u_word_t)_cpu_plw(cpu, cpu->xpr + a)
      ) ;
    } break ;

    case 0x07 : {
      cpu->xpr[b] = (u_word_t)((s_word_t)
        (s_half_t)_cpu_plh(cpu, cpu->xpr + a)
      ) ;
    } break ;

    case 0x08 : {
      const u_word_t PL = _cpu_PL_get(cpu) ;

      /* save the current frame pointer */

      _cpu_psw(cpu, cpu->xpr + a, cpu->xpr[b]) ;

      /* create a frame of [u] bytes */

      cpu->xpr[b]  = cpu->xpr[a] ;

      if (0 == _cpu_SK_get(cpu, PL)) {
        cpu->xpr[a] -= u ;
      } else {
        cpu->xpr[a] += u ;
      }
    } break ;

    case 0x09 : {
      /* restore the previous frame */

      cpu->xpr[a] = cpu->xpr[b] ;
      cpu->xpr[b] = _cpu_plw(cpu, cpu->xpr + a) ;
    } break ;

    case 0x0A : {
      cpu->xpr[a] = (u_word_t)(
        ~(u_word_t)cpu->xpr[b] + s
      ) ;
    } break ;

    case 0x0B : {
      switch ( u & 0x3 ) {
      case 0x0 : {
        u_word_t tmp = cpu->xpr[a] ;
        cpu->xpr[a]  = cpu->xpr[b] ;
        cpu->xpr[b]  = tmp         ;
      } break ;

      case 0x1 : {
        _cpu_int(cpu, 0, a) ;
      } break ;

      case 0x2 : {
        _cpu_int(cpu, 1, 0) ;
      } break ;

      case 0x3 : {
        u_word_t lvl =
          /* 1+4-bit internal bus identifier */
          ( ( ( a >> 0 ) & 0x1 ) << 4 ) | ( a >> 1 ) |
          /* 1+5-bit external bus identifier */
          ( ( ( u >> 2 ) & 0x1 ) << 5 ) | ( b >> 0 ) ;

        pm_bus_rst(cpu->bus, lvl) ;
      } break ;
      }
    } break ;

    case 0x0C : {
      cpu->xpr[b] = cpu->csr[a] & s ;
    } break ;

    case 0x0D : {
      const u_word_t PL = _cpu_PL_get(cpu) ;

      if (( a >> 3 ) < PL || PM_CPU_PL_U == PL) {
        pm_cpu_int(cpu, PM_CPU_INT_GP) ;
        break ;
      }

      cpu->csr[a] = cpu->xpr[b] | u ;
    } break ;

    case 0x0E : {
      const u_word_t SHF = ( u >> 0 ) & 0x7 ;
      const u_word_t MSK = ( u >> 4 ) & 0xF ;

      cpu->xpr[b] = ( cpu->csr[a] >> ( SHF << 2 ) ) & MSK ;
    } break ;

    case 0x0F : {
      const u_word_t PL = _cpu_PL_get(cpu) ;

      if (( a >> 3 ) < PL || PM_CPU_PL_U == PL) {
        pm_cpu_int(cpu, PM_CPU_INT_GP) ;
        break ;
      }

      const u_word_t SHF = ( u >> 0 ) & 0x7 ;
      const u_word_t MSK = ( u >> 4 ) & 0xF ;
      const u_word_t VAL = ( u >> 8 ) & 0xF ;

      cpu->csr[a] &= ~( U_WORD(0xF) << ( SHF << 2 ) ) ;
      cpu->csr[a] |=  ( ( cpu->xpr[b] | VAL ) & MSK ) << ( SHF << 2 ) ;
    } break ;

    case 0x10 : case 0x11 : case 0x12 : case 0x13 :
    case 0x14 : case 0x15 : case 0x16 : case 0x17 :
    case 0x18 : case 0x19 : case 0x1A : case 0x1B :
    case 0x1C : case 0x1D : case 0x1E : case 0x1F : {
      pm_cpu_int(cpu, PM_CPU_INT_UD) ;
    } break ;
    }
  } break ;

  case 0x1F : {
    pm_cpu_int(cpu, PM_CPU_INT_UD) ;
  } break ;
  }

  /* update the context */

  pl = _cpu_PL_get(cpu) ;

  if (0 != _cpu_SS_get(cpu, pl)) {
    pm_cpu_int(cpu, PM_CPU_INT_SS) ;
  }

  if (0 == _cpu_RN_get(cpu, pl)) {
    if (PM_CPU_PL_M != pl) {
      int res = -1 ;

      if (PM_CPU_PL_U == pl) {
        res = _cpu_xhgsp(cpu, PM_CPU_PL_S, pl) ;
      }

      if (PM_CPU_PL_S == pl && res < 0) {
        res = _cpu_xhgsp(cpu, PM_CPU_PL_H, pl) ;
      }

      if (PM_CPU_PL_H == pl && res < 0) {
        res = _cpu_xhgsp(cpu, PM_CPU_PL_M, pl) ;
      }

      if (res < 0) {
        cpu->bus->hlt = 1 ;
      }
    } else {
      cpu->bus->hlt = 1 ;
    }
  }

  --cpu->ck[1] ;
  ++cpu->ck[0] ;
}

#define _ld(typ)                           \
  do {                                     \
    cpu->ck[1] += U_WORD(2) ;              \
    adr = _cpu_trs(cpu, adr, 0x1) ;        \
    return pm_bus_ld##typ(cpu->bus, adr) ; \
  } while (0)

__PM_PUBL u_byte_t pm_cpu_ldb (struct pm_cpu_t * cpu, u_word_t adr)
{
  _ld(b) ;
}

__PM_PUBL u_half_t pm_cpu_ldh (struct pm_cpu_t * cpu, u_word_t adr)
{
  _ld(h) ;
}

__PM_PUBL u_word_t pm_cpu_ldw (struct pm_cpu_t * cpu, u_word_t adr)
{
  _ld(w) ;
}

#undef _ld

#define _st(typ)                         \
  do {                                   \
    cpu->ck[1] += U_WORD(2) ;            \
    adr = _cpu_trs(cpu, adr, 0x2) ;      \
    pm_bus_st##typ(cpu->bus, adr, dat) ; \
  } while (0)

__PM_PUBL void pm_cpu_stb (struct pm_cpu_t * cpu, u_word_t adr, u_byte_t dat)
{
  _st(b) ;
}

__PM_PUBL void pm_cpu_sth (struct pm_cpu_t * cpu, u_word_t adr, u_half_t dat)
{
  _st(h) ;
}

__PM_PUBL void pm_cpu_stw (struct pm_cpu_t * cpu, u_word_t adr, u_word_t dat)
{
  _st(w) ;
}

#undef _st

__PM_PUBL void pm_cpu_int (struct pm_cpu_t * cpu, u_word_t num)
{
  num &= PM_CPU_INT_MAX ;

  const u_word_t HS  = PM_CPU_INT_HI_MIN <= num && num <= PM_CPU_INT_HI_MAX ;
  const u_word_t SS  = PM_CPU_INT_SI_MIN <= num && num <= PM_CPU_INT_SI_MAX ;
  const u_word_t IDR = cpu->csr[PM_CPU_CSR(PM_CPU_PL_M, PM_CPU_IDR)] ;
  
  u_word_t pl = PM_CPU_PL_M ;

  if (0 == HS && 0 != ( ( IDR >> num ) & 0x1 )) {
    pl = PM_CPU_PL_H ;
  } else if (0 == SS && 0 != ( ( IDR >> num ) & 0x1 )) {
    pl = PM_CPU_PL_S ;
  }

  cpu->csr[PM_CPU_CSR(pl, PM_CPU_IPR)] |= U_WORD(1) << num ;
}

__PM_PUBL u_word_t pm_cpu_id (struct pm_cpu_t * cpu)
{
  return _cpu_ID_get(cpu) & 0x3 ;
}

#define _dec_sp(siz)                       \
  do {                                     \
    const u_word_t PL = _cpu_PL_get(cpu) ; \
                                           \
    if (0 == _cpu_SK_get(cpu, PL)) {       \
      adr  = *sp   ;                       \
      *sp += (siz) ;                       \
    } else {                               \
      *sp -= (siz) ;                       \
      adr  = *sp   ;                       \
    }                                      \
  } while (0)

__PM_PRIV u_byte_t _cpu_plb (struct pm_cpu_t * cpu, u_word_t * sp)
{
  u_word_t adr ;
  _dec_sp(sizeof(u_byte_t)) ;
  return pm_cpu_ldb(cpu, adr) ;
}

__PM_PRIV u_half_t _cpu_plh (struct pm_cpu_t * cpu, u_word_t * sp)
{
  u_word_t adr ;
  _dec_sp(sizeof(u_half_t)) ;
  return pm_cpu_ldh(cpu, adr) ;
}

__PM_PRIV u_word_t _cpu_plw (struct pm_cpu_t * cpu, u_word_t * sp)
{
  u_word_t adr ;
  _dec_sp(sizeof(u_word_t)) ;
  return pm_cpu_ldw(cpu, adr) ;
}

#undef _dec_sp

#define _inc_sp(siz)                       \
  do {                                     \
    const u_word_t PL = _cpu_PL_get(cpu) ; \
                                           \
    if (0 == _cpu_SK_get(cpu, PL)) {       \
      *sp -= (siz) ;                       \
      adr  = *sp   ;                       \
    } else {                               \
      adr  = *sp   ;                       \
      *sp += (siz) ;                       \
    }                                      \
  } while (0)

__PM_PRIV void _cpu_psb (struct pm_cpu_t * cpu, u_word_t * sp, u_byte_t dat)
{
  u_word_t adr ;
  _inc_sp(sizeof(u_byte_t)) ;
  pm_cpu_stb(cpu, adr, dat) ;
}

__PM_PRIV void _cpu_psh (struct pm_cpu_t * cpu, u_word_t * sp, u_half_t dat)
{
  u_word_t adr ;
  _inc_sp(sizeof(u_half_t)) ;
  pm_cpu_sth(cpu, adr, dat) ;
}

__PM_PRIV void _cpu_psw (struct pm_cpu_t * cpu, u_word_t * sp, u_word_t dat)
{
  u_word_t adr ;
  _inc_sp(sizeof(u_word_t)) ;
  pm_cpu_stw(cpu, adr, dat) ;
}

#undef _inc_sp

#define _ld()                          \
  do {                                 \
    cpu->ck[1] += U_WORD(2) ;          \
    adr = _cpu_trs(cpu, adr, 0x5) ;    \
    return pm_bus_ldw(cpu->bus, adr) ; \
  } while (0)

__PM_PRIV u_word_t _cpu_fch (struct pm_cpu_t * cpu)
{
  cpu->pc[1]   = cpu->pc[0] ;
  cpu->pc[0]  += sizeof(u_word_t) ;
  u_word_t adr = cpu->pc[1] ;
  _ld() ;
}

#undef _ld

__PM_PRIV u_word_t _cpu_gecsr (struct pm_cpu_t * cpu, u_word_t adr, u_word_t shf, u_word_t msk)
{
  return ( cpu->csr[adr] >> shf ) & msk ;
}

__PM_PRIV void _cpu_secsr (struct pm_cpu_t * cpu, u_word_t adr, u_word_t dat, u_word_t shf, u_word_t msk)
{
  cpu->csr[adr] &= ~( msk << shf ) ;
  cpu->csr[adr] |= ( dat & msk ) << shf ;
}

__PM_PRIV u_word_t _cpu_trs (struct pm_cpu_t * cpu, u_word_t adr, u_word_t xwr)
{
  const u_word_t PL = _cpu_PL_get(cpu) ;

  if (0 == _cpu_PM_get(cpu, PL))
    return adr ;

  const u_word_t OFS = ( adr >>  0 ) & U_WORD(0xFFF) ;
  const u_word_t PX1 = ( adr >> 12 ) & U_WORD(0x3FF) ;
  const u_word_t PX0 = ( adr >> 22 ) & U_WORD(0x3FF) ;

  adr = cpu->csr[PM_CPU_CSR(PL, PM_CPU_PDT)] ;

  _cpu_PM_clr(cpu, PL) ;
  adr = pm_cpu_ldw(cpu, adr + PX0 * sizeof(adr)) ;
  _cpu_PM_set(cpu, PL) ;

  if (0 == ( adr & 0x1 )) {
    pm_cpu_int(cpu, PM_CPU_INT_PF) ;
    return U_WORD(0x00000000) ;
  }

  if (( ( adr >> 1 ) & 0x3 ) < PL) {
    pm_cpu_int(cpu, PM_CPU_INT_GP) ;
    return U_WORD(0x00000000) ;
  }

  adr &= ~U_WORD(0xFFF) ;

  _cpu_PM_clr(cpu, PL) ;
  adr = pm_cpu_ldw(cpu, adr + PX1 * sizeof(adr)) ;
  _cpu_PM_set(cpu, PL) ;

  if (0 == ( adr & 0x1 )) {
    pm_cpu_int(cpu, PM_CPU_INT_PF) ;
    return U_WORD(0x00000000) ;
  }

  if (( ( adr >> 1 ) & 0x3 ) < PL) {
    pm_cpu_int(cpu, PM_CPU_INT_GP) ;
    return U_WORD(0x00000000) ;
  }

  if (xwr != ( ( ( adr >> 3 ) & 0x7 ) & xwr )) {
    pm_cpu_int(cpu, PM_CPU_INT_GP) ;
    return U_WORD(0x00000000) ;
  }

  return ( adr & ~U_WORD(0xFFF) ) | OFS ;
}

__PM_PRIV int _cpu_xhgsp (struct pm_cpu_t * cpu, const u_word_t DST_PL, const u_word_t SRC_PL)
{
  const u_word_t SRC_ISP = cpu->csr[PM_CPU_CSR(SRC_PL, PM_CPU_ISP)] ;
  const u_word_t DST_ISP = cpu->csr[PM_CPU_CSR(DST_PL, PM_CPU_ISP)] ;

  if (0 != ( SRC_ISP & U_WORD(0xFFF) ) || 0 != ( DST_ISP & U_WORD(0xFFF) )) {
    pm_cpu_int(cpu, PM_CPU_INT_GP) ;
    return -1 ;
  }

  /* temporary registers for source context */

  u_word_t src_pc0 = cpu->pc[0] ;
  u_word_t src_csr [ 0x8  ] ;
  u_word_t src_xpr [ 0x20 ] ;
  u_word_t src_fpr [ 0x20 ] ;

  for (int idx = 0 ; idx < 0x8 ; ++idx) {
    src_csr[idx] = cpu->csr[PM_CPU_CSR(SRC_PL, idx)] ;
  }

  for (int idx = 0 ; idx < 0x20 ; ++idx) {
    src_xpr[idx] = cpu->xpr[idx] ;
    src_fpr[idx] = cpu->fpr[idx] ;
  }

  /* switch to destination PL */
  _cpu_PL_set(cpu, DST_PL) ;

  /* load the destination context */

  cpu->pc[0] = pm_cpu_ldw(cpu, DST_ISP) ;

  for (int idx = 0 ; idx < 0x8 ; ++idx) {
    cpu->csr[PM_CPU_CSR(DST_PL, idx)] =
      pm_cpu_ldw(cpu, DST_ISP + (0x1 + idx) * sizeof(u_word_t)) ;
  }

  for (int idx = 0 ; idx < 0x20 ; ++idx) {
    cpu->xpr[idx] =
      pm_cpu_ldw(cpu, DST_ISP + (0x09 + idx) * sizeof(u_word_t)) ;
    cpu->fpr[idx] =
      pm_cpu_ldw(cpu, DST_ISP + (0x29 + idx) * sizeof(u_word_t)) ;
  }

  /* link the destination to the source context */
  pm_cpu_stw(cpu, DST_ISP, SRC_PL) ;

  /* switch to source PL */
  _cpu_PL_set(cpu, SRC_PL) ;

  /* save the source context */

  pm_cpu_stw(cpu, SRC_ISP, src_pc0) ;

  for (int idx = 0 ; idx < 0x8 ; ++idx) {
    pm_cpu_stw(cpu, SRC_ISP + (0x1 + idx) * sizeof(u_word_t),
      src_csr[idx]) ;
  }

  for (int idx = 0 ; idx < 0x20 ; ++idx) {
    pm_cpu_stw(cpu, SRC_ISP + (0x09 + idx) * sizeof(u_word_t),
      src_xpr[idx]) ;
    pm_cpu_stw(cpu, SRC_ISP + (0x29 + idx) * sizeof(u_word_t),
      src_fpr[idx]) ;
  }

  /* switch to destination PL */
  _cpu_PL_set(cpu, DST_PL) ;

  return 0 ;
}

__PM_PRIV int _cpu_int (struct pm_cpu_t * cpu, u_word_t fun, u_word_t dat)
{
  const u_word_t SRC_PL = _cpu_PL_get(cpu) ;
  
  if (0 == fun) {
    /* invoke the interrupt routine */

    if (0 == _cpu_EI_get(cpu, SRC_PL))
      return -1 ;

    const u_word_t IDT = cpu->csr[PM_CPU_CSR(SRC_PL, PM_CPU_IDT)] ;
    const u_word_t DSC = pm_cpu_ldw(cpu, IDT + ( dat << 1 ) * sizeof(u_word_t) + 0) ;
    const u_word_t ISR = pm_cpu_ldw(cpu, IDT + ( dat << 1 ) * sizeof(u_word_t) + 1) ;

    if (0 == ( DSC & 0x1 )) {
      pm_cpu_int(cpu, PM_CPU_INT_IF) ;
      return -1 ;
    }

    const u_word_t TYP    = ( DSC >> 1 ) & 0x7 ;
    const u_word_t MIN_PL = ( DSC >> 4 ) & 0x3 ;
    const u_word_t DST_PL = ( DSC >> 6 ) & 0x3 ;

    if (MIN_PL < SRC_PL || PM_CPU_PL_U == DST_PL) {
      pm_cpu_int(cpu, PM_CPU_INT_GP) ;
      return -1 ;
    }

    /* exchange the context */

    switch ( ( TYP >> 1 ) & 0x3 ) {
    case 0 : { /* abort */
      pm_cpu_int(cpu, PM_CPU_INT_MC) ;
    } return -1 ;

    case 2 : { /* fault */
      cpu->pc[0] = cpu->pc[1] ;
    } break ;
    }

    if (0 != _cpu_xhgsp(cpu, DST_PL, SRC_PL))
      return -1 ;

    cpu->pc[0] = ISR ;

    /* disable interrupts and set the serving flag */

    _cpu_EI_clr(cpu, DST_PL) ;
    _cpu_WI_clr(cpu, DST_PL) ;
    _cpu_SI_set(cpu, DST_PL) ;

    return 0 ;
  }

  if (1 == fun) {
    /* return from interrupt */

    if (PM_CPU_PL_U == SRC_PL) {
      pm_cpu_int(cpu, PM_CPU_INT_GP) ;
      return -1 ;
    }

    if (0 == _cpu_SI_get(cpu, SRC_PL)) {
      pm_cpu_int(cpu, PM_CPU_INT_GP) ;
      return -1 ;
    }

    /* exchange the context */

    const u_word_t SRC_ISP = cpu->csr[PM_CPU_CSR(SRC_PL, PM_CPU_ISP)] ;
    const u_word_t DST_PL  = pm_cpu_ldw(cpu, SRC_ISP) & 0x3 ;

    return _cpu_xhgsp(cpu, DST_PL, SRC_PL) ;
  }

  return -1 ;
}
