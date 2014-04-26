
#include "predef.h"

using namespace std;

/*
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef char TCHAR; 
typedef TCHAR* PTCHAR,PSTR,LPSTR
typedef const LPSTR LPCTSTR;
*/

#define DEBUG_MAT_INFO(M)  cout<<"---------Matrix Info--------"<<endl<<M._row<<" rows x "<<M._col<<" cols\nlength = "<<M.nlength<<endl<<"First: "<<M.GetValue(0)<<"  Last: "<<M.GetValue(M.nlength-1)<<endl<<"----------End Info----------"<<endl
#define DEBUG_MAT_ELEMENTS(M) cout<<"----------Elements-------"<<endl;\
    for(UINT i(0);i<M._row;i++)\
    {\
        cout<<"→   ";\
        for(UINT j(0);j<M._col;j++)\
        {\
            cout<<M.GetValue(i,j)<<"  \t";\
        }\
        cout<<"←  \n";\
    }\
cout<<"------------End Elements-----------"<<endl

class MAT 
{
protected:
    double ** _pElements;//元素数组
    enum matType{single,inherit} type;
    //USHORT dim;//维度，用于下标层数判断
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
    double GetValue(UINT ID);//直接取第N个元素
    void SetValue(UINT row,UINT col,double val);
    void CopyFromArray(double ar[],UINT length);
};
