#ifndef __PM_CPU_H
# define __PM_CPU_H

/* -------------------------------------------------------------------------- *
 * Hardware and Software Interrupts                                           *
 * -------------------------------------------------------------------------- */

# define PM_CPU_INT_MC 0x00 /* Machine Check                                  */
# define PM_CPU_INT_RS 0x01 /* machine ReSet                                  */
# define PM_CPU_INT_SS 0x02 /* Single Step execution                          */
# define PM_CPU_INT_BK 0x03 /* BreaKpoint                                     */
# define PM_CPU_INT_UD 0x04 /* UnDefined instruction                          */
# define PM_CPU_INT_DZ 0x05 /* Division by Zero                               */
# define PM_CPU_INT_GP 0x06 /* General Protection                             */
# define PM_CPU_INT_BF 0x07 /* Bus Fault                                      */
# define PM_CPU_INT_IF 0x08 /* Interrupt Fault                                */
# define PM_CPU_INT_PF 0x09 /* Page Fault                                     */
# define PM_CPU_INT_R0 0x0A /* Reserved 0                                     */
# define PM_CPU_INT_R1 0x0B /* Reserved 1                                     */

# define PM_CPU_INT_SI(n) ( 0x0C | ( (n) & 0x3 ) ) /* Software Interrupt [n]  */
# define PM_CPU_INT_HI(n) ( 0x10 | ( (n) & 0xF ) ) /* Hardware Interrupt [n]  */

# define PM_CPU_INT_SI_MIN 0x0C
# define PM_CPU_INT_SI_MAX 0x0F
# define PM_CPU_INT_HI_MIN 0x10
# define PM_CPU_INT_HI_MAX 0x1F

# define PM_CPU_INT_MIN    0x00
# define PM_CPU_INT_MAX    0x1F

/* -------------------------------------------------------------------------- *
 * Status Register                                                            *
 * -------------------------------------------------------------------------- */

/* Shifts                                                                     */

# define PM_CPU_SRS_PL 0    /* Privilege Level                                */
# define PM_CPU_SRS_RN 2    /* RuNning                                        */
# define PM_CPU_SRS_RS 3    /* ReSet Interrupt                                */
# define PM_CPU_SRS_EI 4    /* Enable Interrupts                              */
# define PM_CPU_SRS_SI 5    /* Serving Interrupt                              */
# define PM_CPU_SRS_WI 6    /* Wait for Interrupt                             */
# define PM_CPU_SRS_SS 7    /* Single Step execution                          */
# define PM_CPU_SRS_SK 8    /* StacK direction                                */
# define PM_CPU_SRS_PM 9    /* Paged Memory                                   */

/* Masks                                                                      */

# define PM_CPU_SRM_PL 0x3
# define PM_CPU_SRM_RN 0x1
# define PM_CPU_SRM_RS 0x1
# define PM_CPU_SRM_EI 0x1
# define PM_CPU_SRM_SI 0x1
# define PM_CPU_SRM_WI 0x1
# define PM_CPU_SRM_SS 0x1
# define PM_CPU_SRM_SK 0x1
# define PM_CPU_SRM_PM 0x1

/* Flags                                                                      */

# define PM_CPU_SRF_PL ( PM_CPU_SRM_PL << PM_CPU_SRS_PL )
# define PM_CPU_SRF_RN ( PM_CPU_SRM_RN << PM_CPU_SRS_RN )
# define PM_CPU_SRF_RS ( PM_CPU_SRM_RS << PM_CPU_SRS_RS )
# define PM_CPU_SRF_EI ( PM_CPU_SRM_EI << PM_CPU_SRS_EI )
# define PM_CPU_SRF_SI ( PM_CPU_SRM_SI << PM_CPU_SRS_SI )
# define PM_CPU_SRF_WI ( PM_CPU_SRM_WI << PM_CPU_SRS_WI )
# define PM_CPU_SRF_SS ( PM_CPU_SRM_SS << PM_CPU_SRS_SS )
# define PM_CPU_SRF_SK ( PM_CPU_SRM_SK << PM_CPU_SRS_SK )
# define PM_CPU_SRF_PM ( PM_CPU_SRM_PM << PM_CPU_SRS_PM )

/* -------------------------------------------------------------------------- *
 * Privilege Levels                                                           *
 * -------------------------------------------------------------------------- */

# define PM_CPU_PL_M 0      /* Machine                                        */
# define PM_CPU_PL_H 1      /* Hardware Supervisor                            */
# define PM_CPU_PL_S 2      /* Software Supervisor                            */
# define PM_CPU_PL_U 3      /* User                                           */

/* -------------------------------------------------------------------------- *
 * Control and Status Registers                                               *
 * -------------------------------------------------------------------------- */

# define PM_CPU_SR  0x0     /* Status Register                                */
# define PM_CPU_IPR 0x1     /* Interrupt Pending Requests                     */
# define PM_CPU_IMR 0x2     /* Interrupt Mask Requests                        */
# define PM_CPU_IDT 0x3     /* Interrupt Descriptors Table                    */
# define PM_CPU_ISP 0x4     /* Interrupt Status Page                          */
# define PM_CPU_IDR 0x5     /* Interrupt Delegate Requests                    */
# define PM_CPU_PDT 0x6     /* Page Descriptors Table                         */
# define PM_CPU_RSV 0x7     /* ReSerVed                                       */

# define PM_CPU_CSR(pl, idx) ( ( pl << 3 ) | (idx) )

/* -------------------------------------------------------------------------- *
 * General Purpose and Floating-Point Registers                               *
 * -------------------------------------------------------------------------- */

# define PM_CPU_ZR  0x00    /* hardwired to ZeRo                              */
# define PM_CPU_LA  0x01    /* Link Address                                   */
# define PM_CPU_SP  0x02    /* Stack Pointer                                  */
# define PM_CPU_FP  0x03    /* Frame Pointer                                  */
# define PM_CPU_R0  0x04    /* Reserved 0                                     */
# define PM_CPU_R1  0x05    /* Reserved 1                                     */
# define PM_CPU_T8  0x06    /* Temporary 8                                    */
# define PM_CPU_T9  0x07    /* Temporary 9                                    */
# define PM_CPU_A0  0x08    /* function Argument 0                            */
# define PM_CPU_A1  0x09    /* function Argument 1                            */
# define PM_CPU_A2  0x0A    /* function Argument 2                            */
# define PM_CPU_A3  0x0B    /* function Argument 3                            */
# define PM_CPU_A4  0x0C    /* function Argument 4                            */
# define PM_CPU_A5  0x0D    /* function Argument 5                            */
# define PM_CPU_A6  0x0E    /* function Argument 6                            */
# define PM_CPU_A7  0x0F    /* function Argument 7                            */
# define PM_CPU_T0  0x10    /* Temporary 0                                    */
# define PM_CPU_T1  0x11    /* Temporary 1                                    */
# define PM_CPU_T2  0x12    /* Temporary 2                                    */
# define PM_CPU_T3  0x13    /* Temporary 3                                    */
# define PM_CPU_T4  0x14    /* Temporary 4                                    */
# define PM_CPU_T5  0x15    /* Temporary 5                                    */
# define PM_CPU_T6  0x16    /* Temporary 6                                    */
# define PM_CPU_T7  0x17    /* Temporary 7                                    */
# define PM_CPU_S0  0x18    /* function Saved 0                               */
# define PM_CPU_S1  0x19    /* function Saved 1                               */
# define PM_CPU_S2  0x1A    /* function Saved 2                               */
# define PM_CPU_S3  0x1B    /* function Saved 3                               */
# define PM_CPU_S4  0x1C    /* function Saved 4                               */
# define PM_CPU_S5  0x1D    /* function Saved 5                               */
# define PM_CPU_S6  0x1E    /* function Saved 6                               */
# define PM_CPU_S7  0x1F    /* function Saved 7                               */

/* -------------------------------------------------------------------------- *
 * Central Processing Unit                                                    *
 * -------------------------------------------------------------------------- */

struct pm_cpu_t {
  struct pm_bus_t * bus          ;
  struct pm_cfg_t   cfg          ;
  u_word_t          ins          ;
  u_word_t          pc  [ 0x2  ] ;
  u_word_t          ck  [ 0x2  ] ;
  u_word_t          csr [ 0x20 ] ;
  u_word_t          xpr [ 0x20 ] ;
  u_word_t          fpr [ 0x20 ] ;
} ;

__PM_PUBL int      pm_cpu_ctor (struct pm_cpu_t * cpu, struct pm_cfg_t cfg) ;
__PM_PUBL void     pm_cpu_dtor (struct pm_cpu_t * cpu) ;
__PM_PUBL void     pm_cpu_rst  (struct pm_cpu_t * cpu, int lvl) ;
__PM_PUBL void     pm_cpu_clk  (struct pm_cpu_t * cpu) ;
__PM_PUBL u_byte_t pm_cpu_ldb  (struct pm_cpu_t * cpu, u_word_t adr) ;
__PM_PUBL u_half_t pm_cpu_ldh  (struct pm_cpu_t * cpu, u_word_t adr) ;
__PM_PUBL u_word_t pm_cpu_ldw  (struct pm_cpu_t * cpu, u_word_t adr) ;
__PM_PUBL void     pm_cpu_stb  (struct pm_cpu_t * cpu, u_word_t adr, u_byte_t dat) ;
__PM_PUBL void     pm_cpu_sth  (struct pm_cpu_t * cpu, u_word_t adr, u_half_t dat) ;
__PM_PUBL void     pm_cpu_stw  (struct pm_cpu_t * cpu, u_word_t adr, u_word_t dat) ;
__PM_PUBL void     pm_cpu_int  (struct pm_cpu_t * cpu, u_word_t num) ;

#endif
