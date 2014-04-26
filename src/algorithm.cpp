#include "algorithm.h"
double stdev(MAT &mat)
{
    UINT length = mat.nlength;
    double sum = 0;
    double avr = average(mat);
    if(mat.nlength==0)return 0;
    for(int i(0); i < length; i++ ) sum += pow(mat.GetValue(0,i)-avr, 2);
    sum /= length;
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
    if(matX.nlength!=matY.nlength)throw "wrong data.";//归一化之后……
    //cout<<"平均值X: "<<Ex<<endl<<"平均值Y: "<<Ey<<endl;
    double dSum(0);
    UINT nLength = matX.nlength;
    for(UINT i(0);i<nLength;i++)
    {
        dSum+=matX.GetValue(i)*matY.GetValue(i);
    }
    return (dSum-Ex*Ey*matX.nlength)/nLength;//   (1/n)sum(XiYi,1,n)-ExEy
}
double corcoe(MAT &matX,MAT &matY)
{
    return cov(matX,matY)/(stdev(matX)*stdev(matY));
}
void normalization(MAT &matN,MAT &matStd)
{
    double evStd  = stdev(matStd);
    double avrStd = average(matStd);
    double evN    = stdev(matN);
    double avrN   = average(matN);
    for(UINT i(0);i<matN._row;i++)
    {
       for(UINT j(0);j<matN._col;j++)
        {
            double norm = (matN.GetValue(i,j)-avrN)/evN;
            matN.SetValue(i,j,norm*evStd+avrStd);
        }
    }
}
