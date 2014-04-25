
#include "predef.h"

using namespace std;

/*
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef char TCHAR; 
typedef TCHAR* PTCHAR,PSTR,LPSTR
typedef const LPSTR LPCTSTR;
*/


class MAT //�������еĺ�����д��ʵ�ֵ�
{
protected:
    double ** _pElements;//Ԫ������
    enum matType{single,inherit} type;
public:
    UINT nlength;//Ԫ�ظ���
    UINT _row;//��
    UINT _col;//��
    MAT();//�վ���
    MAT(UINT nRow,UINT nCol);//x��x�е�0����
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
    friend MAT operator * (MAT &mat,double num);//����
    MAT operator ^ (MAT &mat);//�ڻ�
    MAT operator [](UINT id);//����MAT����������ת����ʵ��ȡԪ��
    operator double() const {if(!_col)return 0;else return _pElements[0][0];};//���ص�һ��Ԫ�أ������ھ���ֻ��һ��Ԫ��ʱ���Զ�תΪ����
    bool IsEmpty(){if(_row&&_col)return true;else return false;}
    ~MAT();
    bool SaveToFile(LPCTSTR szFile);
    bool ReadFromFile(LPCTSTR szFile);
    MAT SubMat(UINT r1,UINT c1,UINT r2,UINT c2);
    double GetValue(UINT row,UINT col);
    void SetValue(UINT row,UINT col,double val);
    void CopyFromArray(double ar[],UINT length);

};
