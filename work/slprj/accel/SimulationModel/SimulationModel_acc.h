#include "__cf_SimulationModel.h"
#ifndef RTW_HEADER_SimulationModel_acc_h_
#define RTW_HEADER_SimulationModel_acc_h_
#include <stddef.h>
#include <float.h>
#ifndef SimulationModel_acc_COMMON_INCLUDES_
#define SimulationModel_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "SimulationModel_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { bus_Robot nnw5z3adq0 ; bus_Command g1xnxkjajm ; real_T
kp4qt5u3vg [ 2 ] ; real_T dhnhpwpwcu [ 2 ] ; real_T ne0saeqzzd [ 2 ] ; real_T
lpvzwhcvm5 [ 2 ] ; real_T hzel0jh1hz ; real_T aad3ovwdgb ; real_T cv3uad0zxl
; real_T calv4cy1gb ; real_T b0zj2jukt3 [ 2 ] ; real_T j1jyv0wup5 [ 2 ] ;
real_T egva0wdhrf [ 2 ] ; real_T msc3dmipyb ; real_T peycrooe1b ; real_T
nexu35ha2t ; real_T gg3stblm4n ; real_T bh2gcaa4x4 ; real_T d5e1cpso1e ;
real_T cqg1iyomud ; real_T ofistssdx0 ; real_T cyy1grzfzv [ 2 ] ; real_T
mcjtgtsjsl [ 6 ] ; real_T paul0ft0wo [ 6 ] ; real_T pry0w30yxp ; real_T
gp5y3sfuls ; real_T ptrxm4kumd ; real_T pxgiqyrw11 ; real_T haubgphjzy ;
real_T dq0pdq5anr [ 2 ] ; real_T ltnpkbmuwn [ 12 ] ; bus_RemainingPosition
mxdx1ko1mh ; real32_T a12hhaetep ; real32_T fcsdftkdcg [ 2 ] ; real32_T
pbyjwbxef3 [ 6 ] ; real32_T lfxtwcoffj [ 6 ] ; real32_T fsrwewfvf0 ; real32_T
pyl2rten2h ; real32_T cxrbwpzsxj ; real32_T myoow5tk04 [ 6 ] ; real32_T
pwsbve3pry [ 6 ] ; real32_T h4zmzhrzq5 ; real32_T h5wiwrcen4 ; real32_T
hf4i5iu3c0 ; real32_T brsvushqwt ; real32_T e53cscpha1 ; real32_T ml4hjhtkas
; real32_T lyhpfdxzli ; real32_T gd0sat5iwp ; real32_T gl4dvvfysr ; int32_T
dy5ynmhbbp [ 2 ] ; int32_T ge43y2mqsf [ 2 ] ; int32_T prcpmjg12s [ 2 ] ;
int32_T e51miuugdn [ 2 ] ; int32_T hzkmwbgtb3 [ 2 ] ; EPosition pwg3upht3f ;
EObstacles fwnwobiwpq ; int8_T f4hsqa0jms ; int8_T eskaca3nbd [ 3 ] ; int8_T
dybjuqqvbi ; boolean_T hoqgxwedcm ; boolean_T l2cvnpviec ; boolean_T
ophehakjsg ; boolean_T dvgchlgseu ; boolean_T eipp1vuv4b ; char
pad_eipp1vuv4b [ 2 ] ; } i1uqgqugni ; typedef struct { real_T fw5rd4c1zy ;
real_T jiqoi2zmvl ; real_T chz3gp5i2n ; bus_RemainingPosition aeiqre1e5y ;
real_T dsipy1syfg ; real_T e4m3x3tx4i ; real_T kaiz3sbvte ; real_T ccqqlt5eyj
; real_T fyimh0agoo ; real_T eyla0vbz24 ; struct { real_T modelTStart ; }
ko1xmo1ywx ; struct { real_T modelTStart ; } dxcfebgyzp ; struct { void *
TUbufferPtrs [ 12 ] ; } ceg5arytwt ; struct { void * TUbufferPtrs [ 12 ] ; }
pypgntob3t ; real32_T adntrj12vv [ 2 ] ; real32_T n3nvflxppx ; real32_T
oxgfyqehfp ; real32_T hfvgk4oz5g ; real32_T dpv0pu5mzt [ 2 ] ; uint32_T
bdbecmj4aa ; uint32_T khs0au0kyk ; struct { int_T Tail [ 6 ] ; int_T Head [ 6
] ; int_T Last [ 6 ] ; int_T CircularBufSize [ 6 ] ; int_T MaxNewBufSize ; }
a1kp5x3nqn ; struct { int_T Tail [ 6 ] ; int_T Head [ 6 ] ; int_T Last [ 6 ]
; int_T CircularBufSize [ 6 ] ; int_T MaxNewBufSize ; } eiba0oqcqs ; int8_T
kz5x54hhyq ; int8_T mio3q4pmtr ; int8_T ncpcelpqzn ; int8_T lmlkwzkql5 ;
int8_T pl0abfhmef ; int8_T gfgnrhtlid ; uint8_T maee1hzsyx ; uint8_T
cd0toyt3s1 ; boolean_T ki0igsdlpl ; boolean_T ebo5nx1l15 ; boolean_T
mehcmpsqbu ; boolean_T diy25b0gyo ; } eg1akajh2d ; typedef struct { real_T
pauqettjco [ 2 ] ; real_T ckmmshmzy1 ; real_T l4zix2mpjh [ 2 ] ; real_T
bvvt1ge5gd ; real_T ofrqyt0l3h ; } aegwe0fkew ; typedef struct { real_T
pauqettjco [ 2 ] ; real_T ckmmshmzy1 ; real_T l4zix2mpjh [ 2 ] ; real_T
bvvt1ge5gd ; real_T ofrqyt0l3h ; } jkxl3gmlsc ; typedef struct { boolean_T
pauqettjco [ 2 ] ; boolean_T ckmmshmzy1 ; boolean_T l4zix2mpjh [ 2 ] ;
boolean_T bvvt1ge5gd ; boolean_T ofrqyt0l3h ; } he203u5wqa ; typedef struct {
const real32_T hhy00wvytg ; const real32_T mqkh5jptgh ; } nyqryoamwl ;
#define m5pdxr3brg(S) ((nyqryoamwl *) _ssGetConstBlockIO(S))
struct gbxflxzjno_ { real_T P_0 [ 2 ] ; real_T P_1 ; real_T P_2 [ 2 ] ;
real_T P_3 ; real_T P_4 [ 2 ] ; real_T P_5 [ 2 ] ; real_T P_6 [ 2 ] ; real_T
P_7 [ 2 ] ; real_T P_8 [ 2 ] ; real_T P_9 [ 2 ] ; real_T P_10 [ 2 ] ; real_T
P_11 [ 2 ] ; real_T P_12 [ 2 ] ; real_T P_13 ; real_T P_14 ; real_T P_15 ;
real_T P_16 ; real_T P_17 [ 2 ] ; real_T P_18 ; real_T P_19 ; real_T P_20 ;
real_T P_21 ; real_T P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25 ; real_T
P_26 ; real_T P_27 ; real_T P_28 ; real_T P_29 ; real_T P_30 ; real_T P_31 ;
real_T P_32 ; real_T P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 ; real_T
P_37 ; real_T P_38 ; real_T P_39 ; real_T P_40 ; real_T P_41 [ 12 ] ; real_T
P_42 [ 12 ] ; real_T P_43 ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T
P_47 ; real_T P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ;
real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T
P_58 ; real_T P_59 ; real_T P_60 ; bus_RemainingPosition P_61 ; real32_T P_62
; real32_T P_63 ; real32_T P_64 ; real32_T P_65 ; real32_T P_66 ; real32_T
P_67 [ 6 ] ; real32_T P_68 [ 6 ] ; real32_T P_69 ; real32_T P_70 ; real32_T
P_71 ; real32_T P_72 ; char pad_P_72 [ 4 ] ; real_T P_73 [ 2 ] ; real32_T
P_74 [ 16 ] ; real_T P_75 [ 2 ] ; real32_T P_76 [ 16 ] ; real_T P_77 [ 2 ] ;
real32_T P_78 [ 2 ] ; real_T P_79 [ 2 ] ; real32_T P_80 [ 16 ] ; real_T P_81
[ 2 ] ; real32_T P_82 [ 16 ] ; real_T P_83 [ 2 ] ; real32_T P_84 [ 2 ] ;
real_T P_85 [ 2 ] ; real32_T P_86 [ 2 ] ; real_T P_87 [ 2 ] ; real32_T P_88 [
2 ] ; real32_T P_89 ; real32_T P_90 ; real32_T P_91 ; real32_T P_92 ;
real32_T P_93 ; real32_T P_94 ; real32_T P_95 ; real32_T P_96 ; real32_T P_97
; real32_T P_98 ; real32_T P_99 ; real32_T P_100 ; real32_T P_101 ; real32_T
P_102 ; real32_T P_103 ; real32_T P_104 ; real32_T P_105 ; real32_T P_106 ;
real32_T P_107 ; real32_T P_108 ; real32_T P_109 ; real32_T P_110 ; real32_T
P_111 ; real32_T P_112 ; real32_T P_113 ; real32_T P_114 ; real32_T P_115 ;
real32_T P_116 ; real32_T P_117 ; real32_T P_118 [ 12 ] ; real32_T P_119 [ 12
] ; boolean_T P_120 ; boolean_T P_121 ; char pad_P_121 [ 2 ] ; } ; extern
gbxflxzjno b2thciodju ; extern const nyqryoamwl k2qy01tdde ;
#endif
