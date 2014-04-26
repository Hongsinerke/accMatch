#include "intg.h"

void OptimizeAcc(MAT &time , MAT &data , double EvThreshold)//优化加速度数据（归0化
{
    bool bAvrModifyed	= false;//平均值修正
    for(UINT i(10) , j(0) ; i<data._col ; )//10点一帧
    {
        cout<<"Frame "<<i/10<<":"<<endl<<fixed<<setprecision(9);
        MAT sub = data.SubMat(0,j,0,i);
        double avr = average(sub);
        double ev = stdev(sub);//看一下帧平均值和标准差
        cout<<"ev:"<<ev<<endl<<"avr:"<<avr<<endl<<"timepass:"<<time[j]-time[0]<<"-"<<time[i]-time[0]<<endl;

            if(ev>EvThreshold||sqrt(avr*avr)>0.21)//运动的标准差及平均值阈值
            {
                cout<<"STATE:MOVING"<<endl<<endl;
                if(bAvrModifyed)goto LB_CONTINUE;
                //第一个运动帧
                MAT subStop = data.SubMat(0,0,0,j);
                avr = average(subStop);
                data-=avr;//用静止平均值修正所有时刻加速度（用调零值修正？
                for(UINT k(0) ; k<j ; k++) 
					data[k]=0;//然后把静止区归0化
                bAvrModifyed=true;
        }
        else 
        {
            cout<<"STATE:STOP"<<endl<<endl;
            if( !bAvrModifyed ) goto LB_CONTINUE;//平均值修正之前先不要动
            for(UINT k(j) ; k<=i ; k++)//修正静止帧加速度至0
            {
                data[k] = 0;
            }   
        }
        LB_CONTINUE:j = i;
        i += 10;
        if(i>=data._col && i<data._col+9)i=data._col-1;
    }
    if(!bAvrModifyed)//没有运动帧……全0咯
    {
        for(UINT i(0) ; i<data._col ; i++)
			data[i]=0;
    }
//    for(UINT term(0);term<2;term++)//跑两趟
    for(UINT i(1) ; i<data._col ; i++)//第二趟，修正单运动帧（误差）
    {
        if(data[i]!=0 && data[i-1] == 0)//运动帧起始点
        {
            if(i+20<data._col && data[i+20]==0 && data[i+19] == 0)//20点之后是0,本帧是单运动帧
            {
                puts("Single Moving Frame");
                for(UINT j(i-1) ; j<=i+20 ; j++)
                {
                    data[j] = 0;
                }
            }
        }
    }
}

void OptimizeVc(MAT &data)
{
    bool bMovStart=false;
    for(UINT i(1),j(0);i<data._col;i++)
    {
        if(!bMovStart)//应该是不动的状态
        {
            if(data[i] == data[i-1])
			{
				data[i-1] = 0;
				continue;
			}
			//相等，直线，置0,下一点
            //不等，标起始
            data[i-1]=0;
            j = i-1;//j要选在速度为0的点
            bMovStart = true;
        }
        else //应该是正在运动的状态
        {
            if(data[i]!=data[i-1])continue;//不等，还在运动，下一点
            //相等，标结束，修正
            bool bRealStop = true;
            for(UINT k(0);k<2&&k+i<data._col;k++)//向后检查5点
            {
                if(data[i+k]!=data[i]){bRealStop=false;break;}//并非连续5点都相同，未真正静止
            }
            if(bRealStop)//真正静止了
            {
                puts("Optimize Vc");
                bMovStart=false;//不运动了
                //修正
                //区间[j,i]为修正区间
                double delta = data[i]-data[j];
                for(UINT t(j);t<=i;t++)//Vo = data[T]-(T-Tj)*((Vi-Vj)/(Ti-Tj))
                {
                    data[t] -= (t-j)*((data[i]-data[j])/(double)(i-j));
                }
                for(UINT k(i+1);k<data._col;k++)//后边的点全部移动基准数
                {
                    data[k]-=delta;
                }
                //这个地方i点被修正为0了，所以要跳过下一个点，下个点和下下个点相等，让他们相互比较并置0
                i++;
            }
        }
    }
}

int main(void)
{
#ifdef NOT_CONFIGURED
    return 1;
#endif
    MAT matTime(1,1001);
    matTime.ReadFromFile(DATA_INPUT_TIME);
    matTime *= 1e-9;
    MAT matAccX(1,1001);
    matAccX.ReadFromFile(DATA_INPUT_X);
    MAT matAccY(1,1001);
    matAccY.ReadFromFile(DATA_INPUT_Y);
    MAT matAccZ(1,1001);
    matAccZ.ReadFromFile(DATA_INPUT_Z);
    MAT matVcX = trapz(matTime,matAccX);//将加速度积分
    MAT matVcY = trapz(matTime,matAccY);
    MAT matVcZ = trapz(matTime,matAccZ);

    matVcX.SaveToFile(DATA_OUTPUT_VCX);
    matVcY.SaveToFile(DATA_OUTPUT_VCY);
    matVcZ.SaveToFile(DATA_OUTPUT_VCZ);

    MAT matPosX1 = trapz(matTime,matVcX);//将速度积分
    MAT matPosY1 = trapz(matTime,matVcY);
    MAT matPosZ1 = trapz(matTime,matVcZ);

    matPosX1.SaveToFile(DATA_OUTPUT_POSX);
    matPosY1.SaveToFile(DATA_OUTPUT_POSY);
    matPosZ1.SaveToFile(DATA_OUTPUT_POSZ);

    OptimizeAcc(matTime,matAccX,0.074);//归零化加速度
    OptimizeAcc(matTime,matAccY,0.074);
    OptimizeAcc(matTime,matAccZ,0.074);

    matAccX.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCX);
    matAccY.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCY);
    matAccZ.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCZ);
    //------------------------------------------
    matVcX = trapz(matTime,matAccX);//将加速度积分
    matVcY = trapz(matTime,matAccY);
    matVcZ = trapz(matTime,matAccZ);

    OptimizeVc(matVcX);
    OptimizeVc(matVcY);
    OptimizeVc(matVcZ);

    matVcX.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCX);
    matVcY.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCY);
    matVcZ.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCZ);

    MAT matPosX2 = trapz(matTime,matVcX);//将速度积分
    MAT matPosY2 = trapz(matTime,matVcY);
    MAT	matPosZ2 = trapz(matTime,matVcZ);

    matPosX2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSX);
    matPosY2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSY);
    matPosZ2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSZ);

	double matCorCoeX = CorCoe (matPosX1 , matPosX2);
	double matCorCoeY = CorCoe (matPosY1 , matPosY2);
	double matCorCoeZ = CorCoe (matPosZ1 , matPosZ2);

	int judge = Judge(matCorCoeX , matCorCoeY , matCorCoeZ); //这里我先判定到这
    
    return 0;
}

