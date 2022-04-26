/*
2022-04-20
Projekt JARNÍ: Ovládání LED diody pomocí UART
Matouš Páleník, 4B
Obsahuje: 1 LED dioda
				: UART komunikace pøes PuTTY
*/

#include "stm8s.h"
#include "assert.h"
#include <stdio.h> //standartní knihovna pro vstup a výstup, obsahuje getcgar i putchar

#define LED_PORT GPIOG 
#define LED_PIN GPIO_PIN_3
#define LED_OFF GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_ON GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

char znak; //char je stejné jako int8_t

void setup(void){
 CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz

 UART1_DeInit();
  UART1_Init((uint32_t)115200, //rychlost
					UART1_WORDLENGTH_8D, //poèet bitù ve slovì
					UART1_STOPBITS_1, //poèet stopbutù
					UART1_PARITY_NO, //bez kontroly parity
					UART1_SYNCMODE_CLOCK_DISABLE, //asynchronní režim
					UART1_MODE_TXRX_ENABLE //vysílání i pøíjem
	       ); 
	UART1_Cmd(DISABLE);  //celkovì zakáže UART1
  UART1_Cmd(ENABLE);  //celkovì povolí UART1
	
	//LED dioda
	GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	}

// hlavní smyèka programu
void main(void){
	setup();
	LED_OFF;
  while (1){
		znak = getchar(); //zaznamená znak
		putchar(znak); //vypíše znak
		
		if ((znak == 'o')||(znak == 'O')) { 
		//jakmile program zaznamená písmeno "o", ptá se,
		//zda za ním následuje "n" nebo "f" (pro slova ON a OFF)
			znak = getchar();
			putchar(znak);
			
			if ((znak == 'n')||(znak == 'N')) {
				znak = getchar();
				LED_ON;
				printf("\r\nLED ON\r\n"); //informace, že LED svítí
			}
			
			if ((znak == 'f')||(znak == 'F')) {
				znak = getchar();
				putchar(znak);
				
				if ((znak == 'f')||(znak == 'F')) {
					znak = getchar();
					LED_OFF;
					printf("\r\nLED OFF\r\n"); //informace, že LED nesvítí
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

