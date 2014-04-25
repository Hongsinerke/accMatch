#include "mat.h"

double MAT::GetValue(UINT row,UINT col)
{
    if(row<=_row-1 && col<=_col-1)
        return _pElements[row][col];
    else
        return 0;
}

void MAT::SetValue(UINT row,UINT col,double val)
{
    if(row<=_row-1 && col<=_col-1)
        _pElements[row][col] = val;
}
MAT::MAT()//Ĭ�Ϲ���վ���  ���ǳ�ʼ����һ����ָ�� ����Ϊ1x0��С
{
    type = single;
    _pElements = new double*[1];
    _row = 1;
    _col = 0;
    nlength = 0;
}

MAT::MAT(UINT nRow,UINT nCol)//����x��x�е�0����
{
    type = single;
    if(!nRow)
    {
        MAT();//���У� �����ʼ����ָ�룡
        return;
    }
    _row = nRow;
    _col = nCol;
    nlength = nCol*nRow;
    
    _pElements = new double*[nRow];
    for(UINT i(0);i<nRow;i++)
    {
        _pElements[i] = NULL;
        if(!nCol)continue;//����X*0
        _pElements[i] = new double[nCol];
        for(UINT j(0);j<nCol;j++)_pElements[i][j]=0;
    }
}

MAT::MAT(const MAT &mat)//���ƹ���
{
    type = mat.type;
    _row = mat._row; 
    _col = mat._col;
    nlength = mat.nlength;

    _pElements = new double*[_row];

    if(mat.type==inherit)//���־����Ǵ��±�����
    {
        for(UINT i(0);i<_row;i++)
        {
            _pElements[i]=mat._pElements[i];//����ϣ���õ�ԭ����Ԫ�صĵ�ַ�Ա㸳ֵ
        }
    }
    else//�ǵ���������
    {
        for(UINT i(0);i<_row;i++)//��ô���Ǹ���Ԫ��
        {
            _pElements[i] = NULL;
            if(!_col)continue;//����X*0
            _pElements[i] = new double[_col];
            for(UINT j(0);j<_col;j++)_pElements[i][j]=mat._pElements[i][j];
        }

    }
}

MAT::~MAT()//����
{
    if(!_row){throw "Invalid Matrix!";return;}//�в�����Ϊ0
    for(UINT i(0);i<_row;i++)
    {
        if(_pElements[i]&&type==single)//�����������������Ԫ��
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
    if(type==inherit)return *this;//�̳����ľ����ܴ���һ��mat��ֵ
     if(_pElements)
    {
        for(UINT i(0);i<_row;i++)
        {
            if(_pElements[i])delete[] _pElements[i];
        }
        delete[] _pElements;
    }
    _pElements = NULL;//�൱��������
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
    if(type==inherit)return;//Ҳ���ܴ�һ�����鸳ֵ����ֻ֮�ܱ���һ��double
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
    MAT matRet;//�ȿ�

    if(_row>1)//�������Ƕ�ά
    {
        //MAT matRet(1,_col);
        if(id>=_row)return MAT();
        matRet._col = _col;
        matRet.nlength = _col;
        matRet.type = inherit;//�̳����ͣ���Ԫ����ָ��ԭ����ģ����ܱ�delete
        matRet._pElements[0] = _pElements[id];//ָ�򱾾�����ĳ�еĵ�һ��Ԫ��
        /*
        for(int i(0);i<_col;i++)
        {
            matRet._pElements[0][i]=_pElements[id][i];
        }
        return matRet;
        */
    }
    else//��������һά
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
        matRet._pElements[0] = &_pElements[0][id];//ָ�򱾾��󣨵��У��ĵ�x��Ԫ��
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