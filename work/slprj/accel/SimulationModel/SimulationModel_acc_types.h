#include "__cf_SimulationModel.h"
#ifndef RTW_HEADER_SimulationModel_acc_types_h_
#define RTW_HEADER_SimulationModel_acc_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#ifndef _DEFINED_TYPEDEF_FOR_EPosition_
#define _DEFINED_TYPEDEF_FOR_EPosition_
typedef enum { NO = 0 , LEFT , MIDDLE , RIGHT } EPosition ;
#endif
#ifndef _DEFINED_TYPEDEF_FOR_bus_RemainingPosition_
#define _DEFINED_TYPEDEF_FOR_bus_RemainingPosition_
typedef struct { real32_T Angle ; real32_T Distance ; } bus_RemainingPosition
;
#endif
#ifndef _DEFINED_TYPEDEF_FOR_bus_Robot_
#define _DEFINED_TYPEDEF_FOR_bus_Robot_
typedef struct { int32_T EncodersCount [ 2 ] ; int16_T Distance [ 6 ] ;
int8_T Bearing [ 6 ] ; uint8_T sl_padding0 [ 2 ] ; EPosition ScannerStatus ;
int8_T ScannerValues [ 3 ] ; uint8_T sl_padding1 [ 5 ] ;
bus_RemainingPosition Remaining ; } bus_Robot ;
#endif
#ifndef _DEFINED_TYPEDEF_FOR_EObstacles_
#define _DEFINED_TYPEDEF_FOR_EObstacles_
typedef enum { NONE = 0 , CORRECTION , AVOIDANCE } EObstacles ;
#endif
#ifndef _DEFINED_TYPEDEF_FOR_EMode_
#define _DEFINED_TYPEDEF_FOR_EMode_
typedef enum { DISTANCE = 0 , SPEED } EMode ;
#endif
#ifndef _DEFINED_TYPEDEF_FOR_bus_Command_
#define _DEFINED_TYPEDEF_FOR_bus_Command_
typedef struct { EMode Mode ; real32_T Angle ; real32_T Position ; uint8_T
sl_padding0 [ 4 ] ; } bus_Command ;
#endif
typedef struct gbxflxzjno_ gbxflxzjno ;
#endif
