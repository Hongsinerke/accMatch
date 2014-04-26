#include "algorithm.h"
double stdev(MAT &mat)
{
    UINT length = mat.nlength;
    double sum = 0;
    double avr = average(mat);
    if(mat.nlength==0)return 0;
    for(int i(0); i < length; i++ ) sum += pow(mat.GetValue(0,i)-avr, 2)/length;
    return (sqrt(sum));
}
double average(MAT &mat)
{ 
    DEBUG_MAT_INFO(mat);
    DEBUG_MAT_ELEMENTS(mat);
    if(mat.nlength==0)return 0;
    double sum=0;
    for(int i(0);i<mat._row;i++)
    {
        cout<<"before loop "<<sum<<endl;
        for(int j(0);j<mat._col;j++)
        {
            double rt = mat.GetValue(i,j);
            sum+=(mat.GetValue(i,j)/mat.nlength);
            cout<<"loop "<<sum<<"  calc: "<<rt<<"/"<<mat.nlength<<endl;
        }
        cout<<"after loop "<<sum<<endl;
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

double cov(MAT &matX,MAT &matY)
{
    double Ex = average(matX);
    double Ey = average(matY);
    cout<<"平均值X: "<<Ex<<endl<<"平均值Y: "<<Ey<<endl;
    MAT matAvr(matX.nlength,matY.nlength);
    DEBUG_MAT_INFO(matAvr);
    for(UINT i(0);i<matX.nlength;i++)
    {
        for(UINT j(0);j<matY.nlength;j++)
        {
            matAvr[i][j]=(matX.GetValue(i)-Ex)*(matY.GetValue(j)-Ey);
        }//构造存在所有X Y组合的矩阵
    }
    DEBUG_MAT_INFO(matAvr);
    DEBUG_MAT_ELEMENTS(matAvr);
    cout<<"协方差: "<<scientific<<average(matAvr)<<endl;
    return average(matAvr);//最后的平均值E{[X-E(X)]*[Y-E(Y)]}
}
double cor(MAT &matX,MAT &matY)
{
    cout<<"标准差X: "<<stdev(matX)<<"\n标准差Y: "<<stdev(matY)<<endl;
    return cov(matX,matY)/(stdev(matX)*stdev(matY));
}
