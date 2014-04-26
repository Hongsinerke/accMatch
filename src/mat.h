
#include "predef.h"

using namespace std;

/*
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef char TCHAR; 
typedef TCHAR* PTCHAR,PSTR,LPSTR
typedef const LPSTR LPCTSTR;
*/


class MAT //不是所有的函数都写了实现的
{
protected:
    double ** _pElements;//元素数组
    enum matType{single,inherit} type;
public:
    UINT nlength;//元素个数
    UINT _row;//行
    UINT _col;//列
    MAT();//空矩阵
    MAT(UINT nRow,UINT nCol);//x行x列的0矩阵
    MAT (const MAT &mat);
    MAT & operator = (MAT mat);
    double operator = (double num);
    MAT operator + (MAT &mat);
    MAT operator - (MAT &mat);
    void operator += (MAT &mat);
    void operator += (double num);
    void operator -= (MAT &mat);
    void operator -= (double num);
    void operator *= (double num);
    void operator /= (double num);
    void operator ^= (MAT &mat);
    friend MAT operator * (double num,MAT &mat);
    friend MAT operator * (MAT &mat,double num);//数乘
    MAT operator ^ (MAT &mat);//内积
    MAT operator [](UINT id);//返回MAT，重载类型转换符实现取元素
    operator double() const {if(!_col)return 0;else return _pElements[0][0];};//返回第一个元素，这样在矩阵只有一个元素时可自动转为数字
    bool IsEmpty(){if(_row&&_col)return true;else return false;}
    ~MAT();
    bool SaveToFile(LPCTSTR szFile);
    bool ReadFromFile(LPCTSTR szFile);
    MAT SubMat(UINT r1,UINT c1,UINT r2,UINT c2);
    double GetValue(UINT row,UINT col);
    void SetValue(UINT row,UINT col,double val);
    void CopyFromArray(double ar[],UINT length);

};
