#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "USART_interface.h"
#include "FPEC_interface.h"
#include "ESP_interface.h"

#include<stdio.h>
#include<string.h>

void Parser_voidParseRecord(u8* Copy_u8BufData);

u8 Global_u8Data[1800];
u8  u8RecBuffer[50]   ;
u8 status='0';
u8 Page=1;
u8  u8RecCounter    = 0;
u32 j=0;
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void func(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08002000;

	addr_to_call = *(Function_t*)(0x08002004);
	addr_to_call();
}

void Erase_buffer(void)
{
	for(u8 i = 0 ; i < 50 ; i++)
	{
		u8RecBuffer[i]=0;
	}
}
void Delay(u32 lim)
{
	for(j = 0 ; j < lim ; j++)
	{
		asm("NOP");
	}
}

void main(void)
{

	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,14); /* USART1 */
	RCC_voidEnableClock(RCC_APB2,2);  /* PortA  */
	RCC_voidEnableClock(RCC_AHB,4);   /* FPEC   */

	MGPIO_VidSetPinDirection(GPIOA,9,0b1010);   /* TX AFPP */
	MGPIO_VidSetPinDirection(GPIOA,10,0b0100);  /* RX Input Floating */

	MUSART1_voidInit();

	HESP_voidInit();
	//HESP_voidWifiConnect("TP-LINK_EA7B93","ehnayehnay");
	//HESP_voidWifiConnect("TEdata184B83","batee5aa");
	HESP_voidWifiConnect("Yasmin El Margoushy","123 456 789");
	Delay(1000000);
	while(1)
	{
		HESP_voidGEThttpReq("162.253.155.226","80", "http://otadevils.freevar.com/status.txt",Global_u8Data , 300);

		for(j=0 ; j<300 ; j++)
		{
			if ((Global_u8Data[j])==':')
			{
				status=Global_u8Data[j+1];
				break;
			}
		}

		if(status=='0')
		{
			func();
		}
		else if (status=='1')
		{
			if(Page==1)
			{
				FPEC_voidEraseAppArea();
			}

			u8 string[50] = {0};
			sprintf(string,"%s%d","http://otadevils.freevar.com/start.php?page=",Page*25);
			HESP_voidGEThttpReq("162.253.155.226","80", string , Global_u8Data , 100);
			Delay(1000000);
			HESP_voidGEThttpReq("162.253.155.226","80", "http://otadevils.freevar.com/block.txt",Global_u8Data , 1800);

			for(j=0 ; j<1800 ; j++)
			{
				if (Global_u8Data[j] == ':')
				{
					if((u8RecBuffer[0]==':') && ((u8RecBuffer[1]=='0')||(u8RecBuffer[1]=='1')))
					{
						Parser_voidParseRecord(u8RecBuffer);
					}
					u8RecCounter=0;
					Erase_buffer();
					u8RecBuffer[u8RecCounter]= Global_u8Data[j];
				}
				else if((Global_u8Data[j] == 'C') && (Global_u8Data[j+1] == 'L') && (Global_u8Data[j+2] == 'O') && (Global_u8Data[j+3] == 'S'))
				{
					Parser_voidParseRecord(u8RecBuffer);
					u8RecCounter=0;
					Erase_buffer();
				}
				else
				{
					u8RecCounter++;
					u8RecBuffer[u8RecCounter]= Global_u8Data[j];
				}
			}
			Page++;
		}
	}
}
