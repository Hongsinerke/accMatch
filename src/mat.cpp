#include "mat.h"

double MAT::GetValue(UINT row,UINT col)
{
    if(row<_row && col<_col)
        return _pElements[row][col];
    else
        return 0;
}

double MAT::GetValue(UINT ID)
{
    if(ID<nlength)//0到N
        return GetValue(ID/_col,ID%_col);
    else
        return 0;
}

void MAT::SetValue(UINT row,UINT col,double val)
{
    if(row<=_row-1 && col<=_col-1)
        _pElements[row][col] = val;
}
MAT::MAT()//默认构造空矩阵  但是初始化第一个行指针 矩阵为1x0大小
{
    type = single;
    _pElements = new double*[1];
    _row = 1;
    _col = 0;
    nlength = 0;
}

MAT::MAT(UINT nRow,UINT nCol)//构造x行x列的0矩阵
{
    type = single;
    if(!nRow)
    {
        MAT();//空行？ 起码初始化行指针！
        return;
    }
    _row = nRow;
    _col = nCol;
    nlength = nCol*nRow;
    
    _pElements = new double*[nRow];
    for(UINT i(0);i<nRow;i++)
    {
        _pElements[i] = NULL;
        if(!nCol)continue;//允许X*0
        _pElements[i] = new double[nCol];
        for(UINT j(0);j<nCol;j++)_pElements[i][j]=0;
    }
}

MAT::MAT(const MAT &mat)//复制构造
{
    type = mat.type;
    _row = mat._row; 
    _col = mat._col;
    nlength = mat.nlength;

    _pElements = new double*[_row];

    if(mat.type==inherit)//这种矩阵是从下标来的
    {
        for(UINT i(0);i<_row;i++)
        {
            _pElements[i]=mat._pElements[i];//我们希望得到原矩阵元素的地址以便赋值
        }
    }
    else//是单独产生的
    {
        for(UINT i(0);i<_row;i++)//那么我们复制元素
        {
            _pElements[i] = NULL;
            if(!_col)continue;//允许X*0
            _pElements[i] = new double[_col];
            for(UINT j(0);j<_col;j++)_pElements[i][j]=mat._pElements[i][j];
        }

    }
}

MAT::~MAT()//析构
{
    if(!_row){throw "Invalid Matrix!";return;}//行不可能为0
    for(UINT i(0);i<_row;i++)
    {
        if(_pElements[i]&&type==single)//独立矩阵才能析构列元素
        {
            delete[] _pElements[i];
            _pElements[i]=NULL;
        }
    }
    delete[] _pElements;
    _pElements = NULL;
}

double MAT::operator = (double num)
{
    if(_row!=1 || _col!=1)return num;
    _pElements[0][0]=num;
    return num;
}
MAT & MAT::operator = (MAT mat)
{
    if(type==inherit)return *this;//继承来的矩阵不能从另一个mat赋值
     if(_pElements)
    {
        for(UINT i(0);i<_row;i++)
        {
            if(_pElements[i])delete[] _pElements[i];
        }
        delete[] _pElements;
    }
    _pElements = NULL;//相当于先析构
    _row = mat._row; 
    _col = mat._col;
    nlength = mat.nlength;
    _pElements = new double*[_row];
    for(UINT i(0);i<_row;i++)
    {
        _pElements[i] = new double[_col];
        for(UINT j(0);j<_col;j++)_pElements[i][j]=mat._pElements[i][j];
    } 
    return *this;
}


void MAT::CopyFromArray(double ar[],UINT length)
{
    if(type==inherit)return;//也不能从一个数组赋值，总之只能被赋一个double
    if(_pElements)
    {
        for(UINT i(0);i<_row;i++)
        {
            delete[] _pElements[i];
        }
        delete[] _pElements;
    }
    _pElements = NULL;
    _row = 1;
    _col = length;
    nlength = length;

    _pElements = new double*[1];
    _pElements[0] = new double[_col];
    for(UINT j(0);j<_col;j++)_pElements[0][j]=ar[j]; 
}
MAT MAT::operator [](UINT id)
{
    if((!_col)||(!_row))return MAT();
    MAT matRet;//先空

    if(_row>1)//本矩阵是二维
    {
        //MAT matRet(1,_col);
        if(id>=_row)return MAT();
        matRet._col = _col;
        matRet.nlength = _col;
        matRet.type = inherit;//继承类型，列元素是指向原矩阵的，不能被delete
        matRet._pElements[0] = _pElements[id];//指向本矩阵中某行的第一个元素
        /*
        for(int i(0);i<_col;i++)
        {
            matRet._pElements[0][i]=_pElements[id][i];
        }
        return matRet;
        */
    }
    else//本矩阵是一维
    {
        //MAT matRet(1,1);
        if(id>=_col)return MAT();
        /*
        matRet._pElements[0][0] = _pElements[0][id];
        return matRet;
        */
        matRet._col = 1;
        matRet.nlength = 1;
        matRet.type = inherit;
        matRet._pElements[0] = &_pElements[0][id];//指向本矩阵（单行）的第x个元素
    }
    return matRet;
}
MAT MAT::SubMat(UINT r1,UINT c1,UINT r2,UINT c2)
{
    if(r2<r1 || c2<c1 || r2>=_row || c2>=_col)return MAT();
    MAT matRet(r2-r1+1,c2-c1+1);
    for(UINT i(0);i<matRet._row;i++)
    {
        for(UINT j(0);j<matRet._col;j++)
        {
            matRet._pElements[i][j]=this->_pElements[i+r1][j+c1];
        }
    }
    return matRet;
}

bool MAT::SaveToFile(LPCTSTR szFile)
{
    ofstream fWrite(szFile);
    if(!fWrite)return false;
    try
    {
        for(UINT i(0);i<_row;i++)
        for(UINT j(0);j<_col;j++)
        {
            fWrite<<" "<<scientific<<setprecision(8)<<_pElements[i][j]<<" ";
        }
        fWrite.close();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool MAT::ReadFromFile(LPCTSTR szFile)
{
    ifstream fRead(szFile);
    if(!fRead)return false;
    try
    {
        for(UINT i(0);i<_row;i++)
        for(UINT j(0);j<_col;j++)
        {
            if(!fRead)throw "File Read Error";
            fRead>>_pElements[i][j];
        }
        fRead.close();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

MAT operator * (double num,MAT &mat)
{
    MAT matRet = mat;
    for(UINT i(0);i<mat._row;i++)
        for(UINT j(0);j<mat._col;j++)
        {
            matRet[i][j]=num*mat[i][j];
        }
    return matRet;
}
MAT operator * (MAT &mat,double num)
{
    MAT matRet = mat;
    for(UINT i(0);i<mat._row;i++)
        for(UINT j(0);j<mat._col;j++)
        {
            matRet[i][j]=num*mat[i][j];
        }
    return matRet;
}
void MAT::operator *= (double num)
{
    for(UINT i(0);i<_row;i++)
        for(UINT j(0);j<_col;j++)
        {
            this->SetValue(i,j,num*GetValue(i,j));
        }
}
void MAT::operator /= (double num)
{
    if(!num)return;
    this->operator *=(1.000/num);
}
void MAT::operator += (double num)
{
    for(UINT i(0);i<_row;i++)
        for(UINT j(0);j<_col;j++)
        {
            this->SetValue(i,j,num+GetValue(i,j));
        }   
}
void MAT::operator -= (double num)
{
    for(UINT i(0);i<_row;i++)
        for(UINT j(0);j<_col;j++)
        {
            this->SetValue(i,j,GetValue(i,j)-num);
        }   
}
