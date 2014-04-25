#include "predef.h"
#include "mat.h"
double average(MAT &mat);
double stdev(MAT &mat);
MAT trapz(MAT &matX,MAT &matY);
double normalization (MAT &mat , UINT row,UINT col );
double CorCoe ( MAT &matX , MAT &matY );
bool Judge(double rX , double rY , double rZ);