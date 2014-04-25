#include "predef.h"
#include "algorithm.h"

using namespace std;
#define DELTA_TIME 0.001

#define NOT_CONFIGURED  //下面宏填好之后把这行注释掉（不能用~之类的符号哦）
#define DATA_INPUT_TIME "~/workspace/sensordata/timestamp.txt"
#define DATA_INPUT_X "~/workspace/sensordata/AccdataX.txt"
#define DATA_INPUT_Y "~/workspace/sensordata/AccdataY.txt"
#define DATA_INPUT_Z "~/workspace/sensordata/AccdataZ.txt"
#define DATA_OUTPUT_VCX "~/workspace/sensordata/VcdataX.txt"
#define DATA_OUTPUT_VCY "~/workspace/sensordata/VcdataY.txt"
#define DATA_OUTPUT_VCZ "~/workspace/sensordata/VcdataZ.txt"
#define DATA_OUTPUT_POSX  "~/workspace/sensordata/PosdataX.txt"
#define DATA_OUTPUT_POSY  "~/workspace/sensordata/PosdataY.txt"
#define DATA_OUTPUT_POSZ  "~/workspace/sensordata/PosdataZ.txt"
#define DATA_OUTPUT_OPTIMIZE_ACCX "~/workspace/sensordata/AccdataX_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_ACCY "~/workspace/sensordata/AccdataY_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_ACCZ "~/workspace/sensordata/AccdataZ_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_VCX "~/workspace/sensordata/VcdataX_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_VCY "~/workspace/sensordata/VcdataY_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_VCZ "~/workspace/sensordata/VcdataZ_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_POSX  "~/workspace/sensordata/PosdataX_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_POSY  "~/workspace/sensordata/PosdataY_optimized.txt"
#define DATA_OUTPUT_OPTIMIZE_POSZ  "~/workspace/sensordata/PosdataZ_optimized.txt"


//---------------------output data format define----------------

#define MATLAB_DATA_GENERATE
//如果需要生成导入matlab的数据(用空格隔开的若干数字)则保留该行定义
#define HUMAN_READ_DATA(ID,DATA) "[%d]%.8lf\t",ID,DATA//较易读的格式

#ifndef MATLAB_DATA_GENERATE
#define AUTO_FORMAT_DATA(ID,DATA) HUMAN_READ_DATA(ID,DATA)
#else   
#define AUTO_FORMAT_DATA(ID,DATA) " %.7e  ",DATA
#endif

#ifndef  MATLAB_DATA_GENERATE
#define OUT_LINE(X) puts(X)//用OUT_LINE代替puts  生成matlab数据时不会写入
#else
#define OUT_LINE(X) 
#endif

