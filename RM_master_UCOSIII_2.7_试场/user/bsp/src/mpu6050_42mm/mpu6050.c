#include "mpu6050.h"
#include "inv_mpu.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//MPU6050 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Init(void)
{ 
	u8 res;
	IIC_Init();//��ʼ��IIC����
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);						//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X001);	//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50000);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    	����,�������
int gyroADC_42mm_X_offset=0,gyroADC_42mm_Y_offset=0,gyroADC_42mm_Z_offset=0;
MPU6050_42mm_RAW_DATA    MPU6050_42mm_Raw_Data; 
MPU6050_42mm_REAL_DATA   MPU6050_42mm_Real_Data;
float yaw=0,pitch=0,roll=0;
int MPU6050_42mm_ReadData(void)
{
		u8 buf[14];
    if(MPU_Read_Len(0X68
			,0x3B,14,buf) == 0xff)
    {
//        printf("error 1J\r\n");
        return 0xff;
    }
    else
    {
//        MPU6050_Raw_Data.Accel_Y = (buf[2]<<8 | buf[3]);
//        MPU6050_Raw_Data.Accel_Z = (buf[4]<<8 | buf[5]);
//        MPU6050_Raw_Data.Temp  =   (buf[6]<<8 | buf[7]);
        MPU6050_42mm_Raw_Data.Gyro_X = (buf[8]<<8 | buf[9]);
        MPU6050_42mm_Raw_Data.Gyro_Y = (buf[10]<<8 | buf[11]);
        MPU6050_42mm_Raw_Data.Gyro_Z = (buf[12]<<8 | buf[13]);


        //��ԭ������ת��Ϊʵ��ֵ�����㹫ʽ���Ĵ����������й�
//        MPU6050_Real_Data.Accel_X = (float)(MPU6050_Raw_Data.Accel_X)/8192.0; //��datasheet 30 of 47
//        MPU6050_Real_Data.Accel_Y = (float)(MPU6050_Raw_Data.Accel_Y)/8192.0; //��datasheet 30 of 47
//        MPU6050_Real_Data.Accel_Z = (float)(MPU6050_Raw_Data.Accel_Z)/8192.0; //��datasheet 30 of 47
//        MPU6050_Real_Data.Temp =   (float)(MPU6050_Raw_Data.Temp)/340.0+36.53;//��datasheet 31 of 47
        MPU6050_42mm_Real_Data.Gyro_X = (float)(MPU6050_42mm_Raw_Data.Gyro_X - gyroADC_42mm_X_offset)/65.5;     //��datasheet 32 of 47
        MPU6050_42mm_Real_Data.Gyro_Y = (float)(MPU6050_42mm_Raw_Data.Gyro_Y - gyroADC_42mm_Y_offset)/65.5;     //��datasheet 32 of 47
        MPU6050_42mm_Real_Data.Gyro_Z = (float)(MPU6050_42mm_Raw_Data.Gyro_Z - gyroADC_42mm_Z_offset)/65.5; 			//��datasheet 32 of 47
		}

    return 0;
}

void MPU6050_42mm_Gyro_calibration(void)
{
	    u16 i;
    float x_temp=0,y_temp=0,z_temp=0;

    for(i=0; i<1000; i++)
    {
        MPU6050_42mm_ReadData();
        delay_ms(3);
        x_temp=x_temp+MPU6050_42mm_Raw_Data.Gyro_X;
        y_temp=y_temp+MPU6050_42mm_Raw_Data.Gyro_Y;
        z_temp=z_temp+MPU6050_42mm_Raw_Data.Gyro_Z;
    }

    x_temp=x_temp/1000.00;
    y_temp=y_temp/1000.00;
    z_temp=z_temp/1000.00;

    gyroADC_42mm_X_offset=(int)x_temp;
    gyroADC_42mm_Y_offset=(int)y_temp;
    gyroADC_42mm_Z_offset=(int)z_temp;
}

u8 MPU_Get_Gyroscope()
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		MPU6050_42mm_Raw_Data.Gyro_X=((u16)buf[0]<<8)|buf[1];  
		MPU6050_42mm_Raw_Data.Gyro_Y=((u16)buf[2]<<8)|buf[3];  
		MPU6050_42mm_Raw_Data.Gyro_Z=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer()
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		MPU6050_42mm_Raw_Data.Accel_X=((u16)buf[0]<<8)|buf[1];  
		MPU6050_42mm_Raw_Data.Accel_Y=((u16)buf[2]<<8)|buf[3];  
		MPU6050_42mm_Raw_Data.Accel_Z=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//����,�������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//��������
		if(IIC_Wait_Ack())		//�ȴ�ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(0);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}


