#include "predef.h"
#include "mat.h"
double average(MAT &mat);//平均值
double stdev(MAT &mat);//标准差
MAT trapz(MAT &matX,MAT &matY);//梯形积分
double cov(MAT &matX,MAT &matY);//协方差
double corcoe(MAT &matX,MAT &matY);//相关系数
void normalization(MAT &matN,MAT &matStd);//归一化
/*将matN归一化到matStd的标准差和平均值*/
