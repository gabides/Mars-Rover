#include "__cf_SimulationModel.h"
#include <math.h>
#include "SimulationModel_acc.h"
#include "SimulationModel_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T SimulationModel_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr
, int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay ,
real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr , boolean_T
isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr ) { int_T
testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf =
* tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T SimulationModel_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real32_T look1_iflf_binlcpw (
real32_T u0 , const real32_T bp0 [ ] , const real32_T table [ ] , uint32_T
maxIndex ) { real32_T frac ; uint32_T iRght ; uint32_T iLeft ; uint32_T bpIdx
; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = 0.0F ; } else if ( u0 < bp0 [
maxIndex ] ) { bpIdx = maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while
( iRght - iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else {
iLeft = bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [
iLeft ] ) / ( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft =
maxIndex - 1U ; frac = 1.0F ; } return ( table [ iLeft + 1U ] - table [ iLeft
] ) * frac + table [ iLeft ] ; } real_T look1_iftd_binlcpw ( real32_T u0 ,
const real32_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; uint32_T iRght ; uint32_T iLeft ; uint32_T bpIdx ; if ( u0 <=
bp0 [ 0U ] ) { iLeft = 0U ; frac = 0.0 ; } else if ( u0 < bp0 [ maxIndex ] )
{ bpIdx = maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght -
iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft =
bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) /
( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ;
frac = 1.0 ; } return ( table [ iLeft + 1U ] - table [ iLeft ] ) * frac +
table [ iLeft ] ; } void dzlit4jl3n ( SimStruct * const S ) { eg1akajh2d *
_rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ; _rtDW ->
mehcmpsqbu = true ; } void i52bjeg5os ( SimStruct * const S ) { eg1akajh2d *
_rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ; _rtDW ->
mehcmpsqbu = false ; } void aypgbjrknk ( SimStruct * const S ) { i1uqgqugni *
_rtB ; gbxflxzjno * _rtP ; eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( i1uqgqugni * ) _ssGetBlockIO ( S ) ) ; _rtB -> gd0sat5iwp = ( (
( real32_T ) _rtB -> e51miuugdn [ 0 ] - ( real32_T ) _rtB -> hzkmwbgtb3 [ 0 ]
) * _rtP -> P_63 - ( ( real32_T ) _rtB -> e51miuugdn [ 1 ] - ( real32_T )
_rtB -> hzkmwbgtb3 [ 1 ] ) * _rtP -> P_63 ) * _rtP -> P_64 * _rtP -> P_65 ;
_rtDW -> pl0abfhmef = 4 ; } boolean_T aypgbjrknkFNI ( SimStruct * const S ,
int_T controlPortIdx , int_T tid ) { aypgbjrknk ( S ) ; UNUSED_PARAMETER (
controlPortIdx ) ; UNUSED_PARAMETER ( tid ) ; return ( 1 ) ; } void
lbhya3y4gf ( SimStruct * const S ) { } void ojm4wgjuu3 ( SimStruct * const S
) { eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ;
_rtDW -> diy25b0gyo = true ; } void goqiqomjle ( SimStruct * const S ) {
eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ;
_rtDW -> diy25b0gyo = false ; } void e25kvjr4sq ( SimStruct * const S ) {
real32_T b43b51vuaj ; i1uqgqugni * _rtB ; gbxflxzjno * _rtP ; eg1akajh2d *
_rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
gbxflxzjno * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( i1uqgqugni * )
_ssGetBlockIO ( S ) ) ; b43b51vuaj = look1_iflf_binlcpw ( _rtB -> ml4hjhtkas
, _rtP -> P_68 , _rtP -> P_67 , 5U ) ; _rtB -> lyhpfdxzli = _rtB ->
ml4hjhtkas / b43b51vuaj + _rtP -> P_69 ; _rtDW -> gfgnrhtlid = 4 ; }
boolean_T e25kvjr4sqFNI ( SimStruct * const S , int_T controlPortIdx , int_T
tid ) { e25kvjr4sq ( S ) ; UNUSED_PARAMETER ( controlPortIdx ) ;
UNUSED_PARAMETER ( tid ) ; return ( 1 ) ; } void oms0bmrmxp ( SimStruct *
const S ) { } void ozjg3pijmi ( SimStruct * const S ) { eg1akajh2d * _rtDW ;
_rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ; _rtDW -> ebo5nx1l15 =
true ; } void pube33w2wp ( SimStruct * const S ) { eg1akajh2d * _rtDW ; _rtDW
= ( ( eg1akajh2d * ) ssGetRootDWork ( S ) ) ; _rtDW -> ebo5nx1l15 = false ; }
void kdecc3epyk ( SimStruct * const S ) { i1uqgqugni * _rtB ; gbxflxzjno *
_rtP ; eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * ) ssGetRootDWork ( S ) )
; _rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( i1uqgqugni
* ) _ssGetBlockIO ( S ) ) ; _rtB -> hf4i5iu3c0 = ( ( ( real32_T ) _rtB ->
ge43y2mqsf [ 0 ] - ( real32_T ) _rtB -> prcpmjg12s [ 0 ] ) * _rtP -> P_71 + (
( real32_T ) _rtB -> ge43y2mqsf [ 1 ] - ( real32_T ) _rtB -> prcpmjg12s [ 1 ]
) * _rtP -> P_71 ) * _rtP -> P_72 ; _rtDW -> lmlkwzkql5 = 4 ; } boolean_T
kdecc3epykFNI ( SimStruct * const S , int_T controlPortIdx , int_T tid ) {
kdecc3epyk ( S ) ; UNUSED_PARAMETER ( controlPortIdx ) ; UNUSED_PARAMETER (
tid ) ; return ( 1 ) ; } void fwdr0mq5pq ( SimStruct * const S ) { } real_T
rt_urand_Upu32_Yd_f_pw_snf ( uint32_T * u ) { uint32_T lo ; uint32_T hi ; lo
= * u % 127773U * 16807U ; hi = * u / 127773U * 2836U ; if ( lo < hi ) { * u
= 2147483647U - ( hi - lo ) ; } else { * u = lo - hi ; } return ( real_T ) *
u * 4.6566128752457969E-10 ; } static void mdlOutputs ( SimStruct * S , int_T
tid ) { real_T p52jbskdeh [ 6 ] ; boolean_T pdmvjeyryc ; real_T hy55tmuzec ;
real32_T ns4t3cd3lg ; real32_T btmrxnmxsz ; real32_T az5vfifu4o ; real_T
aj4f4vq1zs ; int16_T dbpn0de3n5 [ 6 ] ; int8_T mv4xouaqhk [ 6 ] ; real32_T
h4ntytgssr ; real32_T gbbichatfx ; real_T fczcnhpr00 ; int32_T i ; real32_T
jdk4f4pwns_idx_0 ; real32_T jdk4f4pwns_idx_1 ; real_T ojdmecpaod_idx_1 ;
real32_T tmp ; int8_T prchuksdft_p ; i1uqgqugni * _rtB ; gbxflxzjno * _rtP ;
aegwe0fkew * _rtX ; eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * )
ssGetRootDWork ( S ) ) ; _rtX = ( ( aegwe0fkew * ) ssGetContStates ( S ) ) ;
_rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( i1uqgqugni *
) _ssGetBlockIO ( S ) ) ; _rtB -> kp4qt5u3vg [ 0 ] = _rtX -> pauqettjco [ 0 ]
; _rtB -> kp4qt5u3vg [ 1 ] = _rtX -> pauqettjco [ 1 ] ; if ( ssIsSampleHit (
S , 1 , 0 ) ) { _rtB -> dhnhpwpwcu [ 0 ] = _rtB -> kp4qt5u3vg [ 0 ] ; _rtB ->
ne0saeqzzd [ 0 ] = _rtB -> dhnhpwpwcu [ 0 ] ; _rtB -> lpvzwhcvm5 [ 0 ] = _rtB
-> ne0saeqzzd [ 0 ] ; _rtB -> dhnhpwpwcu [ 1 ] = _rtB -> kp4qt5u3vg [ 1 ] ;
_rtB -> ne0saeqzzd [ 1 ] = _rtB -> dhnhpwpwcu [ 1 ] ; _rtB -> lpvzwhcvm5 [ 1
] = _rtB -> ne0saeqzzd [ 1 ] ; _rtB -> hoqgxwedcm = _rtP -> P_121 ; _rtB ->
hzel0jh1hz = ssGetTaskTime ( S , 1 ) ; ssCallAccelRunBlock ( S , 15 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; } hy55tmuzec = _rtX -> ckmmshmzy1 ; _rtB ->
aad3ovwdgb = _rtP -> P_14 * _rtX -> ckmmshmzy1 ; if ( ssIsSampleHit ( S , 1 ,
0 ) ) { _rtB -> cv3uad0zxl = _rtB -> aad3ovwdgb ; ssCallAccelRunBlock ( S , 9
, 0 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 8 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> calv4cy1gb = _rtDW -> fw5rd4c1zy ; aj4f4vq1zs
= _rtDW -> jiqoi2zmvl ; jdk4f4pwns_idx_0 = _rtDW -> adntrj12vv [ 0 ] ;
jdk4f4pwns_idx_1 = _rtDW -> adntrj12vv [ 1 ] ; } _rtB -> b0zj2jukt3 [ 0 ] =
_rtP -> P_18 * _rtX -> l4zix2mpjh [ 0 ] ; _rtB -> b0zj2jukt3 [ 1 ] = _rtP ->
P_18 * _rtX -> l4zix2mpjh [ 1 ] ; if ( ssIsSampleHit ( S , 1 , 0 ) ) {
ojdmecpaod_idx_1 = muDoubleScalarFloor ( muDoubleScalarRound ( _rtB ->
b0zj2jukt3 [ 0 ] / _rtP -> P_19 ) * _rtP -> P_19 * _rtB -> egva0wdhrf [ 0 ] )
; if ( muDoubleScalarIsNaN ( ojdmecpaod_idx_1 ) || muDoubleScalarIsInf (
ojdmecpaod_idx_1 ) ) { ojdmecpaod_idx_1 = 0.0 ; } else { ojdmecpaod_idx_1 =
muDoubleScalarRem ( ojdmecpaod_idx_1 , 4.294967296E+9 ) ; } _rtB ->
dy5ynmhbbp [ 0 ] = ojdmecpaod_idx_1 < 0.0 ? - ( int32_T ) ( uint32_T ) -
ojdmecpaod_idx_1 : ( int32_T ) ( uint32_T ) ojdmecpaod_idx_1 ; _rtB ->
fcsdftkdcg [ 0 ] = ( real32_T ) _rtB -> dy5ynmhbbp [ 0 ] * _rtB -> a12hhaetep
* _rtP -> P_91 ; jdk4f4pwns_idx_0 = _rtB -> fcsdftkdcg [ 0 ] -
jdk4f4pwns_idx_0 ; ojdmecpaod_idx_1 = muDoubleScalarFloor (
muDoubleScalarRound ( _rtB -> b0zj2jukt3 [ 1 ] / _rtP -> P_19 ) * _rtP ->
P_19 * _rtB -> egva0wdhrf [ 1 ] ) ; if ( muDoubleScalarIsNaN (
ojdmecpaod_idx_1 ) || muDoubleScalarIsInf ( ojdmecpaod_idx_1 ) ) {
ojdmecpaod_idx_1 = 0.0 ; } else { ojdmecpaod_idx_1 = muDoubleScalarRem (
ojdmecpaod_idx_1 , 4.294967296E+9 ) ; } _rtB -> dy5ynmhbbp [ 1 ] =
ojdmecpaod_idx_1 < 0.0 ? - ( int32_T ) ( uint32_T ) - ojdmecpaod_idx_1 : (
int32_T ) ( uint32_T ) ojdmecpaod_idx_1 ; _rtB -> fcsdftkdcg [ 1 ] = (
real32_T ) _rtB -> dy5ynmhbbp [ 1 ] * _rtB -> a12hhaetep * _rtP -> P_91 ;
ns4t3cd3lg = _rtB -> fcsdftkdcg [ 1 ] - jdk4f4pwns_idx_1 ; jdk4f4pwns_idx_1 =
( jdk4f4pwns_idx_0 + ns4t3cd3lg ) * _rtP -> P_92 ; ns4t3cd3lg = ( ns4t3cd3lg
- jdk4f4pwns_idx_0 ) * _rtP -> P_93 ; btmrxnmxsz = _rtDW -> n3nvflxppx ;
jdk4f4pwns_idx_0 = _rtP -> P_94 * ns4t3cd3lg + _rtDW -> n3nvflxppx ; _rtB ->
msc3dmipyb = jdk4f4pwns_idx_1 * muSingleScalarCos ( jdk4f4pwns_idx_0 ) +
aj4f4vq1zs ; jdk4f4pwns_idx_1 *= muSingleScalarSin ( jdk4f4pwns_idx_0 ) ;
_rtB -> peycrooe1b = jdk4f4pwns_idx_1 + _rtDW -> chz3gp5i2n ; if ( _rtB ->
l2cvnpviec && _rtB -> ophehakjsg ) { ssSetStopRequested ( S , 1 ) ; } _rtB ->
dq0pdq5anr [ 0 ] = _rtB -> msc3dmipyb ; _rtB -> dq0pdq5anr [ 1 ] = _rtB ->
peycrooe1b ; ssCallAccelRunBlock ( S , 13 , 1 , SS_CALL_MDL_OUTPUTS ) ;
pdmvjeyryc = _rtB -> hoqgxwedcm ; } { real_T * * uBuffer = ( real_T * * ) &
_rtDW -> ceg5arytwt . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & _rtDW -> ceg5arytwt . TUbufferPtrs [ 6 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & p52jbskdeh [ 0 ] ; int_T
* iw_Tail = & _rtDW -> a1kp5x3nqn . Tail [ 0 ] ; int_T * iw_Head = & _rtDW ->
a1kp5x3nqn . Head [ 0 ] ; int_T * iw_Last = & _rtDW -> a1kp5x3nqn . Last [ 0
] ; int_T * iw_CircularBufSize = & _rtDW -> a1kp5x3nqn . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 6 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_25 > 0.0 )
? _rtP -> P_25 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
SimulationModel_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , *
uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail [ i1 ] ,
iw_Head [ i1 ] , _rtP -> P_26 , 1 , ( boolean_T ) ( ssIsMinorTimeStep ( S )
&& ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; tBuffer ++ ; uBuffer
++ ; } } } for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> pbyjwbxef3 [ i ] = (
real32_T ) p52jbskdeh [ i ] ; } if ( ssIsSampleHit ( S , 1 , 0 ) ) { for ( i
= 0 ; i < 6 ; i ++ ) { tmp = muSingleScalarFloor ( _rtB -> pbyjwbxef3 [ i ] )
; if ( muSingleScalarIsNaN ( tmp ) || muSingleScalarIsInf ( tmp ) ) { tmp =
0.0F ; } else { tmp = muSingleScalarRem ( tmp , 65536.0F ) ; } dbpn0de3n5 [ i
] = ( int16_T ) ( tmp < 0.0F ? ( int32_T ) ( int16_T ) - ( int16_T ) (
uint16_T ) - tmp : ( int32_T ) ( int16_T ) ( uint16_T ) tmp ) ; } } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> pypgntob3t . TUbufferPtrs [ 0 ] ;
real_T * * tBuffer = ( real_T * * ) & _rtDW -> pypgntob3t . TUbufferPtrs [ 6
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T
* y0 = & p52jbskdeh [ 0 ] ; int_T * iw_Tail = & _rtDW -> eiba0oqcqs . Tail [
0 ] ; int_T * iw_Head = & _rtDW -> eiba0oqcqs . Head [ 0 ] ; int_T * iw_Last
= & _rtDW -> eiba0oqcqs . Last [ 0 ] ; int_T * iw_CircularBufSize = & _rtDW
-> eiba0oqcqs . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 6 ; i1 ++ ) {
tMinusDelay = ( ( _rtP -> P_27 > 0.0 ) ? _rtP -> P_27 : 0.0 ) ; tMinusDelay =
simTime - tMinusDelay ; y0 [ i1 ] = SimulationModel_acc_rt_TDelayInterpolate
( tMinusDelay , 0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , &
iw_Last [ i1 ] , iw_Tail [ i1 ] , iw_Head [ i1 ] , _rtP -> P_28 , 1 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; tBuffer ++ ; uBuffer ++ ; } } } for ( i = 0 ; i < 6 ; i
++ ) { _rtB -> lfxtwcoffj [ i ] = ( real32_T ) p52jbskdeh [ i ] ; } if (
ssIsSampleHit ( S , 1 , 0 ) ) { for ( i = 0 ; i < 6 ; i ++ ) { tmp =
muSingleScalarFloor ( _rtB -> lfxtwcoffj [ i ] ) ; if ( muSingleScalarIsNaN (
tmp ) || muSingleScalarIsInf ( tmp ) ) { tmp = 0.0F ; } else { tmp =
muSingleScalarRem ( tmp , 256.0F ) ; } mv4xouaqhk [ i ] = ( int8_T ) ( tmp <
0.0F ? ( int32_T ) ( int8_T ) - ( int8_T ) ( uint8_T ) - tmp : ( int32_T ) (
int8_T ) ( uint8_T ) tmp ) ; } ssCallAccelRunBlock ( S , 12 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 11 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> nnw5z3adq0 . EncodersCount [ 0 ] = _rtB ->
dy5ynmhbbp [ 0 ] ; _rtB -> nnw5z3adq0 . EncodersCount [ 1 ] = _rtB ->
dy5ynmhbbp [ 1 ] ; for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> nnw5z3adq0 .
Distance [ i ] = dbpn0de3n5 [ i ] ; _rtB -> nnw5z3adq0 . Bearing [ i ] =
mv4xouaqhk [ i ] ; } _rtB -> nnw5z3adq0 . ScannerStatus = _rtB -> pwg3upht3f
; _rtB -> nnw5z3adq0 . ScannerValues [ 0 ] = _rtB -> eskaca3nbd [ 0 ] ; _rtB
-> nnw5z3adq0 . ScannerValues [ 1 ] = _rtB -> eskaca3nbd [ 1 ] ; _rtB ->
nnw5z3adq0 . ScannerValues [ 2 ] = _rtB -> eskaca3nbd [ 2 ] ; _rtB ->
nnw5z3adq0 . Remaining = _rtDW -> aeiqre1e5y ; if ( ssIsMajorTimeStep ( S ) )
{ if ( pdmvjeyryc ) { if ( ! _rtDW -> ki0igsdlpl ) { ssCallAccelRunBlock ( S
, 1 , 0 , SS_CALL_RTW_GENERATED_ENABLE ) ; ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_RTW_GENERATED_ENABLE ) ; _rtDW -> ki0igsdlpl = true ; } } else { if (
_rtDW -> ki0igsdlpl ) { ssCallAccelRunBlock ( S , 1 , 0 ,
SS_CALL_RTW_GENERATED_DISABLE ) ; ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_RTW_GENERATED_DISABLE ) ; _rtDW -> ki0igsdlpl = false ; } } } if (
_rtDW -> ki0igsdlpl ) { ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_OUTPUTS ) ; if (
ssIsMajorTimeStep ( S ) ) { srUpdateBC ( _rtDW -> kz5x54hhyq ) ; } }
ssCallAccelRunBlock ( S , 7 , 0 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtDW ->
maee1hzsyx != 0 ) { _rtDW -> oxgfyqehfp = _rtB -> h5wiwrcen4 ; } if ( ( _rtB
-> dvgchlgseu && ( _rtDW -> mio3q4pmtr <= 0 ) ) || ( ( ! _rtB -> dvgchlgseu )
&& ( _rtDW -> mio3q4pmtr == 1 ) ) ) { _rtDW -> oxgfyqehfp = _rtB ->
h5wiwrcen4 ; } az5vfifu4o = _rtP -> P_96 * _rtB -> h4zmzhrzq5 + _rtDW ->
oxgfyqehfp ; if ( az5vfifu4o > _rtP -> P_98 ) { jdk4f4pwns_idx_0 = _rtP ->
P_98 ; } else if ( az5vfifu4o < _rtP -> P_99 ) { jdk4f4pwns_idx_0 = _rtP ->
P_99 ; } else { jdk4f4pwns_idx_0 = az5vfifu4o ; } ssCallAccelRunBlock ( S , 5
, 0 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtDW -> cd0toyt3s1 != 0 ) { _rtDW ->
hfvgk4oz5g = _rtB -> e53cscpha1 ; } if ( ( _rtB -> eipp1vuv4b && ( _rtDW ->
ncpcelpqzn <= 0 ) ) || ( ( ! _rtB -> eipp1vuv4b ) && ( _rtDW -> ncpcelpqzn ==
1 ) ) ) { _rtDW -> hfvgk4oz5g = _rtB -> e53cscpha1 ; } jdk4f4pwns_idx_1 =
_rtP -> P_100 * _rtB -> brsvushqwt + _rtDW -> hfvgk4oz5g ; if (
jdk4f4pwns_idx_1 > _rtP -> P_102 ) { h4ntytgssr = _rtP -> P_102 ; } else if (
jdk4f4pwns_idx_1 < _rtP -> P_103 ) { h4ntytgssr = _rtP -> P_103 ; } else {
h4ntytgssr = jdk4f4pwns_idx_1 ; } gbbichatfx = jdk4f4pwns_idx_0 + h4ntytgssr
; if ( gbbichatfx > _rtP -> P_104 ) { gbbichatfx = _rtP -> P_104 ; } else {
if ( gbbichatfx < _rtP -> P_105 ) { gbbichatfx = _rtP -> P_105 ; } }
h4ntytgssr = jdk4f4pwns_idx_0 - h4ntytgssr ; if ( h4ntytgssr > _rtP -> P_106
) { h4ntytgssr = _rtP -> P_106 ; } else { if ( h4ntytgssr < _rtP -> P_107 ) {
h4ntytgssr = _rtP -> P_107 ; } } jdk4f4pwns_idx_0 = _rtP -> P_108 *
jdk4f4pwns_idx_1 ; if ( jdk4f4pwns_idx_1 > _rtP -> P_110 ) { jdk4f4pwns_idx_1
-= _rtP -> P_110 ; } else if ( jdk4f4pwns_idx_1 >= _rtP -> P_109 ) {
jdk4f4pwns_idx_1 = 0.0F ; } else { jdk4f4pwns_idx_1 -= _rtP -> P_109 ; }
aj4f4vq1zs = ( jdk4f4pwns_idx_0 != jdk4f4pwns_idx_1 ) ; jdk4f4pwns_idx_1 =
muSingleScalarSign ( jdk4f4pwns_idx_1 ) ; jdk4f4pwns_idx_0 = _rtP -> P_111 *
_rtB -> brsvushqwt ; tmp = muSingleScalarSign ( jdk4f4pwns_idx_0 ) ; if (
muSingleScalarIsNaN ( tmp ) || muSingleScalarIsInf ( tmp ) ) { tmp = 0.0F ; }
else { tmp = muSingleScalarRem ( tmp , 256.0F ) ; } if ( jdk4f4pwns_idx_1 <
128.0F ) { prchuksdft_p = ( int8_T ) jdk4f4pwns_idx_1 ; } else { prchuksdft_p
= 0 ; } if ( ( ( aj4f4vq1zs != 0.0 ) && ( ( tmp < 0.0F ? ( int32_T ) ( int8_T
) - ( int8_T ) ( uint8_T ) - tmp : ( int32_T ) ( int8_T ) ( uint8_T ) tmp )
== prchuksdft_p ) ) > _rtP -> P_29 ) { _rtB -> fsrwewfvf0 = m5pdxr3brg ( S )
-> hhy00wvytg ; } else { _rtB -> fsrwewfvf0 = jdk4f4pwns_idx_0 ; } _rtB ->
mxdx1ko1mh . Angle = _rtB -> brsvushqwt ; _rtB -> mxdx1ko1mh . Distance =
_rtB -> h4zmzhrzq5 ; jdk4f4pwns_idx_1 = _rtP -> P_113 * az5vfifu4o ; if (
az5vfifu4o > _rtP -> P_115 ) { az5vfifu4o -= _rtP -> P_115 ; } else if (
az5vfifu4o >= _rtP -> P_114 ) { az5vfifu4o = 0.0F ; } else { az5vfifu4o -=
_rtP -> P_114 ; } aj4f4vq1zs = ( jdk4f4pwns_idx_1 != az5vfifu4o ) ;
az5vfifu4o = muSingleScalarSign ( az5vfifu4o ) ; if ( az5vfifu4o < 128.0F ) {
prchuksdft_p = ( int8_T ) az5vfifu4o ; } else { prchuksdft_p = 0 ; }
az5vfifu4o = _rtP -> P_116 * _rtB -> h4zmzhrzq5 ; tmp = muSingleScalarSign (
az5vfifu4o ) ; if ( muSingleScalarIsNaN ( tmp ) || muSingleScalarIsInf ( tmp
) ) { tmp = 0.0F ; } else { tmp = muSingleScalarRem ( tmp , 256.0F ) ; } if (
( ( aj4f4vq1zs != 0.0 ) && ( ( tmp < 0.0F ? ( int32_T ) ( int8_T ) - ( int8_T
) ( uint8_T ) - tmp : ( int32_T ) ( int8_T ) ( uint8_T ) tmp ) ==
prchuksdft_p ) ) > _rtP -> P_30 ) { _rtB -> pyl2rten2h = m5pdxr3brg ( S ) ->
mqkh5jptgh ; } else { _rtB -> pyl2rten2h = az5vfifu4o ; } _rtB -> bh2gcaa4x4
= look1_iftd_binlcpw ( gbbichatfx , _rtP -> P_118 , _rtP -> P_41 , 11U ) ;
_rtB -> d5e1cpso1e = look1_iftd_binlcpw ( h4ntytgssr , _rtP -> P_119 , _rtP
-> P_42 , 11U ) ; } _rtB -> nexu35ha2t = _rtP -> P_33 * _rtX -> bvvt1ge5gd ;
_rtB -> gg3stblm4n = _rtP -> P_39 * _rtX -> ofrqyt0l3h ; aj4f4vq1zs = _rtP ->
P_43 * _rtB -> nexu35ha2t * _rtP -> P_44 * _rtB -> j1jyv0wup5 [ 0 ] ;
ojdmecpaod_idx_1 = _rtP -> P_43 * _rtB -> gg3stblm4n * _rtP -> P_44 * _rtB ->
j1jyv0wup5 [ 1 ] ; fczcnhpr00 = ( aj4f4vq1zs + ojdmecpaod_idx_1 ) * _rtP ->
P_45 ; _rtB -> ofistssdx0 = ( ojdmecpaod_idx_1 - aj4f4vq1zs ) / _rtB ->
cqg1iyomud ; _rtB -> cyy1grzfzv [ 0 ] = fczcnhpr00 * muDoubleScalarCos (
hy55tmuzec ) * _rtP -> P_49 ; _rtB -> cyy1grzfzv [ 1 ] = fczcnhpr00 *
muDoubleScalarSin ( hy55tmuzec ) * _rtP -> P_49 ; if ( ssIsSampleHit ( S , 1
, 0 ) ) { for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> mcjtgtsjsl [ i ] = (
real32_T ) _rtDW -> e4m3x3tx4i * _rtB -> myoow5tk04 [ i ] + _rtB ->
myoow5tk04 [ i ] ; _rtB -> paul0ft0wo [ i ] = ( real32_T ) _rtDW ->
kaiz3sbvte * _rtB -> pwsbve3pry [ i ] + _rtB -> pwsbve3pry [ i ] ; }
hy55tmuzec = ( real_T ) _rtB -> dybjuqqvbi - _rtB -> pry0w30yxp ; fczcnhpr00
= hy55tmuzec - _rtB -> calv4cy1gb ; if ( muDoubleScalarAbs ( fczcnhpr00 ) >
_rtP -> P_58 ) { _rtB -> ptrxm4kumd = _rtB -> gp5y3sfuls * muDoubleScalarSign
( fczcnhpr00 ) + _rtB -> calv4cy1gb ; } else { _rtB -> ptrxm4kumd =
hy55tmuzec ; } _rtB -> cxrbwpzsxj = btmrxnmxsz + ns4t3cd3lg ; }
UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID2 ( SimStruct * S ,
int_T tid ) { real_T ejyyyislur ; real_T ihy3wqob54 ; i1uqgqugni * _rtB ;
gbxflxzjno * _rtP ; _rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) ) ;
_rtB = ( ( i1uqgqugni * ) _ssGetBlockIO ( S ) ) ; _rtB -> j1jyv0wup5 [ 0 ] =
_rtP -> P_21 + _rtP -> P_22 ; _rtB -> j1jyv0wup5 [ 1 ] = _rtP -> P_21 + _rtP
-> P_23 ; _rtB -> egva0wdhrf [ 0 ] = _rtP -> P_20 / _rtB -> j1jyv0wup5 [ 0 ]
; _rtB -> egva0wdhrf [ 1 ] = _rtP -> P_20 / _rtB -> j1jyv0wup5 [ 1 ] ; _rtB
-> a12hhaetep = _rtP -> P_90 ; _rtB -> ophehakjsg = _rtP -> P_120 ; _rtB ->
cqg1iyomud = ( _rtP -> P_46 + _rtP -> P_47 ) * _rtP -> P_48 ; _rtB ->
pry0w30yxp = _rtP -> P_56 ; _rtB -> gp5y3sfuls = _rtP -> P_57 ; ejyyyislur =
_rtP -> P_59 ; ihy3wqob54 = _rtP -> P_60 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { i1uqgqugni * _rtB ;
gbxflxzjno * _rtP ; eg1akajh2d * _rtDW ; _rtDW = ( ( eg1akajh2d * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) )
; _rtB = ( ( i1uqgqugni * ) _ssGetBlockIO ( S ) ) ; if ( ssIsSampleHit ( S ,
1 , 0 ) ) { _rtDW -> fw5rd4c1zy = _rtB -> ptrxm4kumd ; _rtDW -> jiqoi2zmvl =
_rtB -> msc3dmipyb ; _rtDW -> adntrj12vv [ 0 ] = _rtB -> fcsdftkdcg [ 0 ] ;
_rtDW -> adntrj12vv [ 1 ] = _rtB -> fcsdftkdcg [ 1 ] ; _rtDW -> n3nvflxppx =
_rtB -> cxrbwpzsxj ; _rtDW -> chz3gp5i2n = _rtB -> peycrooe1b ; } { real_T *
* uBuffer = ( real_T * * ) & _rtDW -> ceg5arytwt . TUbufferPtrs [ 0 ] ;
real_T * * tBuffer = ( real_T * * ) & _rtDW -> ceg5arytwt . TUbufferPtrs [ 6
] ; real_T simTime = ssGetT ( S ) ; { int_T i1 ; const real_T * u0 = & _rtB
-> mcjtgtsjsl [ 0 ] ; int_T * iw_Tail = & _rtDW -> a1kp5x3nqn . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> a1kp5x3nqn . Head [ 0 ] ; int_T * iw_Last = &
_rtDW -> a1kp5x3nqn . Last [ 0 ] ; int_T * iw_CircularBufSize = & _rtDW ->
a1kp5x3nqn . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 6 ; i1 ++ ) {
iw_Head [ i1 ] = ( ( iw_Head [ i1 ] < ( iw_CircularBufSize [ i1 ] - 1 ) ) ? (
iw_Head [ i1 ] + 1 ) : 0 ) ; if ( iw_Head [ i1 ] == iw_Tail [ i1 ] ) { if ( !
SimulationModel_acc_rt_TDelayUpdateTailOrGrowBuf ( & iw_CircularBufSize [ i1
] , & iw_Tail [ i1 ] , & iw_Head [ i1 ] , & iw_Last [ i1 ] , simTime - _rtP
-> P_25 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
a1kp5x3nqn . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ iw_Head
[ i1 ] ] = simTime ; ( * uBuffer ++ ) [ iw_Head [ i1 ] ] = u0 [ i1 ] ; } } }
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> pypgntob3t . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> pypgntob3t .
TUbufferPtrs [ 6 ] ; real_T simTime = ssGetT ( S ) ; { int_T i1 ; const
real_T * u0 = & _rtB -> paul0ft0wo [ 0 ] ; int_T * iw_Tail = & _rtDW ->
eiba0oqcqs . Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> eiba0oqcqs . Head [ 0
] ; int_T * iw_Last = & _rtDW -> eiba0oqcqs . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> eiba0oqcqs . CircularBufSize [ 0 ] ; for ( i1
= 0 ; i1 < 6 ; i1 ++ ) { iw_Head [ i1 ] = ( ( iw_Head [ i1 ] < (
iw_CircularBufSize [ i1 ] - 1 ) ) ? ( iw_Head [ i1 ] + 1 ) : 0 ) ; if (
iw_Head [ i1 ] == iw_Tail [ i1 ] ) { if ( !
SimulationModel_acc_rt_TDelayUpdateTailOrGrowBuf ( & iw_CircularBufSize [ i1
] , & iw_Tail [ i1 ] , & iw_Head [ i1 ] , & iw_Last [ i1 ] , simTime - _rtP
-> P_27 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
eiba0oqcqs . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ iw_Head
[ i1 ] ] = simTime ; ( * uBuffer ++ ) [ iw_Head [ i1 ] ] = u0 [ i1 ] ; } } }
if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtDW -> aeiqre1e5y = _rtB -> mxdx1ko1mh
; _rtDW -> maee1hzsyx = 0U ; _rtDW -> oxgfyqehfp += _rtP -> P_97 * _rtB ->
pyl2rten2h ; _rtDW -> mio3q4pmtr = ( int8_T ) _rtB -> dvgchlgseu ; _rtDW ->
cd0toyt3s1 = 0U ; _rtDW -> hfvgk4oz5g += _rtP -> P_101 * _rtB -> fsrwewfvf0 ;
_rtDW -> ncpcelpqzn = ( int8_T ) _rtB -> eipp1vuv4b ; _rtDW -> e4m3x3tx4i = (
_rtP -> P_51 - _rtP -> P_50 ) * rt_urand_Upu32_Yd_f_pw_snf ( & _rtDW ->
bdbecmj4aa ) + _rtP -> P_50 ; _rtDW -> kaiz3sbvte = ( _rtP -> P_54 - _rtP ->
P_53 ) * rt_urand_Upu32_Yd_f_pw_snf ( & _rtDW -> khs0au0kyk ) + _rtP -> P_53
; ssCallAccelRunBlock ( S , 16 , 142 , SS_CALL_MDL_UPDATE ) ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { i1uqgqugni * _rtB ; gbxflxzjno
* _rtP ; aegwe0fkew * _rtX ; jkxl3gmlsc * _rtXdot ; _rtXdot = ( ( jkxl3gmlsc
* ) ssGetdX ( S ) ) ; _rtX = ( ( aegwe0fkew * ) ssGetContStates ( S ) ) ;
_rtP = ( ( gbxflxzjno * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( i1uqgqugni *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> pauqettjco [ 0 ] = _rtB -> cyy1grzfzv [
0 ] ; _rtXdot -> pauqettjco [ 1 ] = _rtB -> cyy1grzfzv [ 1 ] ; _rtXdot ->
ckmmshmzy1 = _rtB -> ofistssdx0 ; _rtXdot -> l4zix2mpjh [ 0 ] = _rtB ->
nexu35ha2t ; _rtXdot -> l4zix2mpjh [ 1 ] = _rtB -> gg3stblm4n ; _rtXdot ->
bvvt1ge5gd = 0.0 ; _rtXdot -> bvvt1ge5gd += _rtP -> P_31 * _rtX -> bvvt1ge5gd
; _rtXdot -> bvvt1ge5gd += _rtP -> P_32 * _rtB -> bh2gcaa4x4 ; _rtXdot ->
ofrqyt0l3h = 0.0 ; _rtXdot -> ofrqyt0l3h += _rtP -> P_37 * _rtX -> ofrqyt0l3h
; _rtXdot -> ofrqyt0l3h += _rtP -> P_38 * _rtB -> d5e1cpso1e ; } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 1328642199U
) ; ssSetChecksumVal ( S , 1 , 349049537U ) ; ssSetChecksumVal ( S , 2 ,
3327401536U ) ; ssSetChecksumVal ( S , 3 , 1784154911U ) ; { mxArray *
slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" ) ;
char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1
, & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField
( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; }
else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "8.6" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( eg1akajh2d ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( i1uqgqugni ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
gbxflxzjno ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetDefaultParam ( S , (
real_T * ) & b2thciodju ) ; _ssSetConstBlockIO ( S , & k2qy01tdde ) ;
rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 1 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 2 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 6 + 0 ] = (
SysOutputFcn ) aypgbjrknkFNI ; callSysFcns [ 6 + 1 ] = ( SysOutputFcn )
e25kvjr4sqFNI ; childS = ssGetSFunction ( S , 3 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) kdecc3epykFNI ; childS = ssGetSFunction ( S , 4 ) ;
callSysFcns = ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ]
= ( SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 5 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 6 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 7 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 8 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 9 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
