/*
2022-04-20
Projekt JARN�: Ovl�d�n� LED diody pomoc� UART
Matou� P�len�k, 4B
Obsahuje: 1 LED dioda
				: UART komunikace p�es PuTTY
*/

#include "stm8s.h"
#include "assert.h"
#include <stdio.h> //standartn� knihovna pro vstup a v�stup, obsahuje getcgar i putchar

#define LED_PORT GPIOG 
#define LED_PIN GPIO_PIN_3
#define LED_OFF GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_ON GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

char znak; //char je stejn� jako int8_t

void setup(void){
 CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz

 UART1_DeInit();
  UART1_Init((uint32_t)115200, //rychlost
					UART1_WORDLENGTH_8D, //po�et bit� ve slov�
					UART1_STOPBITS_1, //po�et stopbut�
					UART1_PARITY_NO, //bez kontroly parity
					UART1_SYNCMODE_CLOCK_DISABLE, //asynchronn� re�im
					UART1_MODE_TXRX_ENABLE //vys�l�n� i p��jem
	       ); 
	UART1_Cmd(DISABLE);  //celkov� zak�e UART1
  UART1_Cmd(ENABLE);  //celkov� povol� UART1
	
	//LED dioda
	GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	}

// hlavn� smy�ka programu
void main(void){
	setup();
	LED_OFF;
  while (1){
		znak = getchar(); //zaznamen� znak
		putchar(znak); //vyp�e znak
		
		if ((znak == 'o')||(znak == 'O')) { 
		//jakmile program zaznamen� p�smeno "o", pt� se,
		//zda za n�m n�sleduje "n" nebo "f" (pro slova ON a OFF)
			znak = getchar();
			putchar(znak);
			
			if ((znak == 'n')||(znak == 'N')) {
				znak = getchar();
				LED_ON;
				printf("\r\nLED ON\r\n"); //informace, �e LED sv�t�
			}
			
			if ((znak == 'f')||(znak == 'F')) {
				znak = getchar();
				putchar(znak);
				
				if ((znak == 'f')||(znak == 'F')) {
					znak = getchar();
					LED_OFF;
					printf("\r\nLED OFF\r\n"); //informace, �e LED nesv�t�
				}
			}
		}
  }
}

 
char putchar (char c)
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);

  return (c);
}

char getchar (void)
{

  int c = 0;
  /* Loop until the Read data register flag is SET */
  while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    c = UART1_ReceiveData8();
  return (c);
}

