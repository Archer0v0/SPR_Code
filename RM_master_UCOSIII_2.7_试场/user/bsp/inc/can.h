#ifndef __CAN_YunTai_h
#define __CAN_YunTai_h
#include <stm32f4xx.h>

#define DRV01_RETURN_ID  0x201//42mmÄ¦²ÁÂÖ
#define DRV02_RETURN_ID  0x202//42mmÄ¦²ÁÂÖ
#define DRV03_RETURN_ID  0x203//17mm²¦µ¯
#define DRV04_RETURN_ID  0x204//42mm²¦µ¯
#define DRV05_RETURN_ID  0x205//42mmpitch
#define DRV06_RETURN_ID  0x206//42mmyaw
#define DRV07_RETURN_ID  0x207//17mmPITCH
#define DRV08_RETURN_ID  0x208//42mm·¢ÉäÂÖ

void CAN1_init(void);
void CAN2_init(void);
void Cmd_205_208(int16_t _current_205,int16_t _current_206,int16_t _current_207,int16_t _current_208);
void Cmd_201_204(int16_t _current_201,int16_t _current_202,int16_t _current_203,int16_t _current_204);
void Cmd_Rest(void);
void CAN2_SetMsg(int16_t _current_1,int16_t _current_2,
	int16_t _current_3,int16_t _current_4);
void CAN1_SetMsg(int16_t _current_1,int16_t _current_2);
	
void CAN2_TX_IRQHandler(void);
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);

void delay(unsigned int t);
#endif

