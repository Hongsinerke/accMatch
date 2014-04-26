#include "intg.h"

void OptimizeAcc(MAT &time , MAT &data , double EvThreshold)//�Ż����ٶ����ݣ���0��
{
    bool bAvrModifyed	= false;//ƽ��ֵ����
    for(UINT i(10) , j(0) ; i<data._col ; )//10��һ֡
    {
        cout<<"Frame "<<i/10<<":"<<endl<<fixed<<setprecision(9);
        MAT sub = data.SubMat(0,j,0,i);
        double avr = average(sub);
        double ev = stdev(sub);//��һ��֡ƽ��ֵ�ͱ�׼��
        cout<<"ev:"<<ev<<endl<<"avr:"<<avr<<endl<<"timepass:"<<time[j]-time[0]<<"-"<<time[i]-time[0]<<endl;

            if(ev>EvThreshold||sqrt(avr*avr)>0.21)//�˶��ı�׼�ƽ��ֵ��ֵ
            {
                cout<<"STATE:MOVING"<<endl<<endl;
                if(bAvrModifyed)goto LB_CONTINUE;
                //��һ���˶�֡
                MAT subStop = data.SubMat(0,0,0,j);
                avr = average(subStop);
                data-=avr;//�þ�ֹƽ��ֵ��������ʱ�̼��ٶȣ��õ���ֵ������
                for(UINT k(0) ; k<j ; k++) 
					data[k]=0;//Ȼ��Ѿ�ֹ����0��
                bAvrModifyed=true;
        }
        else 
        {
            cout<<"STATE:STOP"<<endl<<endl;
            if( !bAvrModifyed ) goto LB_CONTINUE;//ƽ��ֵ����֮ǰ�Ȳ�Ҫ��
            for(UINT k(j) ; k<=i ; k++)//������ֹ֡���ٶ���0
            {
                data[k] = 0;
            }   
        }
        LB_CONTINUE:j = i;
        i += 10;
        if(i>=data._col && i<data._col+9)i=data._col-1;
    }
    if(!bAvrModifyed)//û���˶�֡����ȫ0��
    {
        for(UINT i(0) ; i<data._col ; i++)
			data[i]=0;
    }
//    for(UINT term(0);term<2;term++)//������
    for(UINT i(1) ; i<data._col ; i++)//�ڶ��ˣ��������˶�֡����
    {
        if(data[i]!=0 && data[i-1] == 0)//�˶�֡��ʼ��
        {
            if(i+20<data._col && data[i+20]==0 && data[i+19] == 0)//20��֮����0,��֡�ǵ��˶�֡
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
        if(!bMovStart)//Ӧ���ǲ�����״̬
        {
            if(data[i] == data[i-1])
			{
				data[i-1] = 0;
				continue;
			}
			//��ȣ�ֱ�ߣ���0,��һ��
            //���ȣ�����ʼ
            data[i-1]=0;
            j = i-1;//jҪѡ���ٶ�Ϊ0�ĵ�
            bMovStart = true;
        }
        else //Ӧ���������˶���״̬
        {
            if(data[i]!=data[i-1])continue;//���ȣ������˶�����һ��
            //��ȣ������������
            bool bRealStop = true;
            for(UINT k(0);k<2&&k+i<data._col;k++)//�����5��
            {
                if(data[i+k]!=data[i]){bRealStop=false;break;}//��������5�㶼��ͬ��δ������ֹ
            }
            if(bRealStop)//������ֹ��
            {
                puts("Optimize Vc");
                bMovStart=false;//���˶���
                //����
                //����[j,i]Ϊ��������
                double delta = data[i]-data[j];
                for(UINT t(j);t<=i;t++)//Vo = data[T]-(T-Tj)*((Vi-Vj)/(Ti-Tj))
                {
                    data[t] -= (t-j)*((data[i]-data[j])/(double)(i-j));
                }
                for(UINT k(i+1);k<data._col;k++)//��ߵĵ�ȫ���ƶ���׼��
                {
                    data[k]-=delta;
                }
                //����ط�i�㱻����Ϊ0�ˣ�����Ҫ������һ���㣬�¸�������¸�����ȣ��������໥�Ƚϲ���0
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
    MAT matVcX = trapz(matTime,matAccX);//�����ٶȻ���
    MAT matVcY = trapz(matTime,matAccY);
    MAT matVcZ = trapz(matTime,matAccZ);

    matVcX.SaveToFile(DATA_OUTPUT_VCX);
    matVcY.SaveToFile(DATA_OUTPUT_VCY);
    matVcZ.SaveToFile(DATA_OUTPUT_VCZ);

    MAT matPosX1 = trapz(matTime,matVcX);//���ٶȻ���
    MAT matPosY1 = trapz(matTime,matVcY);
    MAT matPosZ1 = trapz(matTime,matVcZ);

    matPosX1.SaveToFile(DATA_OUTPUT_POSX);
    matPosY1.SaveToFile(DATA_OUTPUT_POSY);
    matPosZ1.SaveToFile(DATA_OUTPUT_POSZ);

    OptimizeAcc(matTime,matAccX,0.074);//���㻯���ٶ�
    OptimizeAcc(matTime,matAccY,0.074);
    OptimizeAcc(matTime,matAccZ,0.074);

    matAccX.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCX);
    matAccY.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCY);
    matAccZ.SaveToFile(DATA_OUTPUT_OPTIMIZE_ACCZ);
    //------------------------------------------
    matVcX = trapz(matTime,matAccX);//�����ٶȻ���
    matVcY = trapz(matTime,matAccY);
    matVcZ = trapz(matTime,matAccZ);

    OptimizeVc(matVcX);
    OptimizeVc(matVcY);
    OptimizeVc(matVcZ);

    matVcX.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCX);
    matVcY.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCY);
    matVcZ.SaveToFile(DATA_OUTPUT_OPTIMIZE_VCZ);

    MAT matPosX2 = trapz(matTime,matVcX);//���ٶȻ���
    MAT matPosY2 = trapz(matTime,matVcY);
    MAT	matPosZ2 = trapz(matTime,matVcZ);

    matPosX2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSX);
    matPosY2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSY);
    matPosZ2.SaveToFile(DATA_OUTPUT_OPTIMIZE_POSZ);

	double matCorCoeX = CorCoe (matPosX1 , matPosX2);
	double matCorCoeY = CorCoe (matPosY1 , matPosY2);
	double matCorCoeZ = CorCoe (matPosZ1 , matPosZ2);

	int judge = Judge(matCorCoeX , matCorCoeY , matCorCoeZ); //���������ж�����
    
    return 0;
}

