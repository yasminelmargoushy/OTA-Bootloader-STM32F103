#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "USART_interface.h"
#include "USART_private.h"
#include "USART_config.h"


void MUSART1_voidInit(void)
{
	/*	baud rate = 115200		*/
	USART1 -> BRR = 0x45;

	SET_BIT((USART1-> CR[0]), 3);			/* Enabling Transmitter */
	SET_BIT((USART1-> CR[0]), 2);			/* Enabling Receiver */
	SET_BIT((USART1-> CR[0]), 13);			/* Enabling USART */
	
	USART1 -> SR = 0;						/* Clearing status register */
}

void MUSART1_voidTransmit(u8 arr[])
{
	u8 i = 0;
	while(arr[i] != '\0')
	{
		USART1 -> DR = arr[i];
		while((GET_BIT((USART1 -> SR), 6)) == 0);
		i++;
	}
	
}

u8 MUSART1_u8Receive(void)
{
	u16 TimeOut = 0 ;
	u8 Loc_u8ReceivedData = 0 ;
	while(GET_BIT( USART1 -> SR , 5) == 0)
	{
		TimeOut++ ;
		if(TimeOut == 2100)
		{
			break;
		}
	}
	if (Loc_u8ReceivedData == 0)
	{
		/* Masking */
		Loc_u8ReceivedData = 0xFF & (USART1 -> DR) ;
	}
	return(Loc_u8ReceivedData) ;
}




















