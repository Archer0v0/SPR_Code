#include "mpu6050_i2c.h"
#include "stdio.h"
/*----I2C1----SCL----PB10---*/
/*----I2C1----SDA----PB11---*/

#define IIC_SCL_H()      GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define IIC_SCL_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define IIC_SDA_H()      GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define IIC_SDA_L()      GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define IIC_SDA_Read()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)

void IIC_delay(unsigned int t)
{
    int i;
    for( i=0; i<t; i++)
    {
        int a = 6;//6
        while(a--);
    }
}



void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef   gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &gpio);
}

void IIC_SDA_Out(void)
{
    GPIO_InitTypeDef   gpio;

    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &gpio);
}

void IIC_SDA_In(void)
{
    GPIO_InitTypeDef   gpio;

    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
}

void IIC_start(void)
{
    IIC_SDA_Out();
    IIC_SDA_H();
    IIC_SCL_H();
    IIC_delay(1);
    IIC_SDA_L();
    IIC_delay(1);
    IIC_SCL_L();
}

void IIC_stop(void)
{
    IIC_SDA_Out();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_delay(1);
    IIC_SCL_H();
    IIC_SDA_H();
    IIC_delay(1);
}

void IIC_ack(u8 re)
{
    IIC_SDA_Out();
    if(re)
        IIC_SDA_H();
    else
        IIC_SDA_L();
    IIC_SCL_H();
    IIC_delay(1);
    IIC_SCL_L();
    IIC_delay(1);
}

int IIC_WaitAck(void)
{
    u16 Out_Time=1000;

    IIC_SDA_H();
    IIC_SDA_In();
    IIC_delay(1);
    IIC_SCL_H();
    IIC_delay(1);
    while(IIC_SDA_Read())
    {
        if(--Out_Time)
        {
            IIC_stop();
//			printf("error 2A\r\n");
            return 0xff;
        }
    }
    IIC_SCL_L();
    return 0;
}

void IIC_WriteBit(u8 Temp)
{
    u8 i;
    IIC_SDA_Out();
    IIC_SCL_L();
    for(i=0; i<8; i++)
    {
        if(Temp&0x80)
        {
            IIC_SDA_H();
        }
        else
        {
            IIC_SDA_L();
        }
        Temp<<=1;
        IIC_delay(1);
        IIC_SCL_H();
        IIC_delay(1);
        IIC_SCL_L();
    }
}

u8 IIC_ReadBit(void)
{
    u8 i,Temp=0;
    IIC_SDA_In();
    for(i=0; i<8; i++)
    {
        IIC_SCL_L();
        IIC_delay(1);
        IIC_SCL_H();
        Temp<<=1;
        if(IIC_SDA_Read())
            Temp++;
        IIC_delay(1);
    }
    IIC_SCL_L();
    return Temp;
}

//???,????0,????0xff
int IIC_WriteData(u8 dev_addr,u8 reg_addr,u8 data)
{
    IIC_start();

    IIC_WriteBit(dev_addr);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2B\r\n");
        return 0xff;
    }

    IIC_WriteBit(reg_addr);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2C\r\n");
        return 0xff;
    }

    IIC_WriteBit(data);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2D\r\n");
        return 0xff;
    }

    IIC_stop();
    return 0;
}

//???,????0,????0xff
int IIC_ReadData(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count)
{
    u8 i;

    IIC_start();

    IIC_WriteBit(dev_addr);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2F\r\n");
        return 0xff;
    }

    IIC_WriteBit(reg_addr);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2G\r\n");
        return 0xff;
    }

    IIC_start();

    IIC_WriteBit(dev_addr+1);
    if(IIC_WaitAck() == 0xff)
    {
//        printf("error 2H\r\n");
        return 0xff;
    }

    for(i=0; i<(count-1); i++)
    {
        *pdata=IIC_ReadBit();
        IIC_ack(0);
        pdata++;
    }

    *pdata=IIC_ReadBit();
    IIC_ack(1);

    IIC_stop();

    return 0;
}

