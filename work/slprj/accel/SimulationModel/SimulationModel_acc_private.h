#include "__cf_SimulationModel.h"
#ifndef RTW_HEADER_SimulationModel_acc_private_h_
#define RTW_HEADER_SimulationModel_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)   if(!(ptr)) {\
  ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
  }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((ptr));\
  (ptr) = (NULL);\
  }
#else
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((void *)(ptr));\
  (ptr) = (NULL);\
  }
#endif
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
extern real_T rt_urand_Upu32_Yd_f_pw_snf ( uint32_T * u ) ; boolean_T
SimulationModel_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr , int_T *
tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay , real_T * *
tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr , boolean_T isfixedbuf ,
boolean_T istransportdelay , int_T * maxNewBufSzPtr ) ; real_T
SimulationModel_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T tStart
, real_T * tBuf , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T
oldestIdx , int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) ; extern real32_T look1_iflf_binlcpw (
real32_T u0 , const real32_T bp0 [ ] , const real32_T table [ ] , uint32_T
maxIndex ) ; extern real_T look1_iftd_binlcpw ( real32_T u0 , const real32_T
bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) ; void dzlit4jl3n (
SimStruct * const S ) ; void i52bjeg5os ( SimStruct * const S ) ; void
aypgbjrknk ( SimStruct * const S ) ; void ojm4wgjuu3 ( SimStruct * const S )
; void goqiqomjle ( SimStruct * const S ) ; void e25kvjr4sq ( SimStruct *
const S ) ; void ozjg3pijmi ( SimStruct * const S ) ; void pube33w2wp (
SimStruct * const S ) ; void kdecc3epyk ( SimStruct * const S ) ; extern
boolean_T aypgbjrknkFNI ( SimStruct * const S , int_T controlPortIdx , int_T
tid ) ; extern boolean_T e25kvjr4sqFNI ( SimStruct * const S , int_T
controlPortIdx , int_T tid ) ; extern boolean_T kdecc3epykFNI ( SimStruct *
const S , int_T controlPortIdx , int_T tid ) ;
#endif
