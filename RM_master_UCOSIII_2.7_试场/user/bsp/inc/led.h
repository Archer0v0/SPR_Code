#ifndef __LED_H
#define __LED_H
//ָʾ��
#define LED0 PBout(9)
////����
#define	Pull_scan	 PCin(6)
#define Shoot		PEout(1)=1;delay_ms(160);PEout(1)=0;delay_ms(160);
//��
#define OPEN    =1;
#define CLOSE		=0;
#define UP_1		PDout(14)
#define UP_2		PDout(13)
#define CLIP		PDout(12)
#define GET			PCout(9)
//̧��
#define Up      UP_1 OPEN
#define Down    UP_1 CLOSE 
#define begain  UP_1 CLOSE UP_2 CLOSE CLIP CLOSE GET CLOSE 

//ȡ��
#define Get     GET OPEN delay_ms(200);CLIP OPEN delay_ms(200);UP_2 OPEN delay_ms(300);GET CLOSE
#define Throw   GET OPEN delay_ms(250);CLIP CLOSE delay_ms(100);GET CLOSE delay_ms(100);UP_2 CLOSE 
/**************************�ź�***************************/

//ȡ��

#define Left_scan	 PCin(10)
#define Right_scan PCin(11)
void LED_Init(void);	 				    
#endif
