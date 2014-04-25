#include "algorithm.h"
double stdev(MAT &mat)
{
    UINT length		= mat.nlength;
    double sum		= 0;
    double avr		= average(mat);

    if(mat.nlength == 0)return 0;
    for(int i(0); i < length; i++ ) sum += pow(mat.GetValue(0,i)-avr, 2)/length;
    return (sqrt(sum));
}
double average(MAT &mat)
{
    double sum = 0;
    if(mat.nlength == 0)return 0;
    for(int i(0) ; i<mat._row ; i++)
    {
        for(int j(0);j<mat._col;j++)
        {
            sum += mat.GetValue(i,j) / mat.nlength;
        }
    }
    return sum;
}
MAT trapz(MAT &matX,MAT &matY)
{
    MAT matRet(1,matX.nlength);//估计也用不到二维矩阵……这里先这样吧(不会写)
    if(matX.nlength!=matY.nlength || matX._row!=1 || matY._row!=1)
    {
        return MAT();
    }
    for(int i(1);i<matX.nlength;i++)
    {
        double val = 
            (matX.GetValue(0,i) - matX.GetValue(0,i-1))/*delta x*/
           *(matY.GetValue(0,i) + matY.GetValue(0,i-1))/*delta h*/
            /2 + matRet.GetValue(0,i-1);/*prev sum*/
        matRet.SetValue(0,i,val);
    }
    return matRet;
}

double normalization (MAT &mat , UINT row,UINT col )	//归一化
{
	UINT length = mat.nlength;
	double	sum = 0 , 
			Average = average(mat) , 
			Stdev = stdev(mat);
	return ( ( mat.GetValue ( row , col ) - Average ) / Stdev);
}

double CorCoe ( MAT &matX , MAT &matY )        //相关系数
{
	double	AverageX = average(matX) , 
			AverageY = average(matY) ,
			x , 
			y ,
			sum_xy	= 0 ,
			pow_x	= 0 ,
			pow_y	= 0 ;

	UINT length = matX.nlength;		//一样的数量就只取一次了

	for(int i(0) ; i<length ; i++)
	{
		x		= matX.GetValue(0,i);
		y		= matY.GetValue(0,i);
		sum_xy		+= x*y;
		pow_x	+= x*x;
		pow_y	+= y*y;
	}

	double r =	(sum_xy - length * AverageX * AverageY) / 
				sqrt ( 
						(pow_x - length * AverageX * AverageX) * 
						(pow_y - length * AverageY * AverageY) 
					 );
	return r;
}

bool Judge(double rX , double rY , double rZ)
{
	double n = 0.8;   //设定的检测范围用的，请先修改赋值再用
	if(n < rX < 1 && n < rY < 1 && n < rZ < 1) return true;
	else return false;
}