/* ******************************************************* */
/* Author	: Yasmin El Margoushy						   */
/* Date		: 28 SEPT 2020								   */
/* Version	: V01										   */
/* ******************************************************* */
#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include "USART_interface.h"

#include"ESP_interface.h"
#include"ESP_private.h"
#include"ESP_config.h"

#include<stdio.h>


void HESP_voidInit(void)
{
	u8 Local_u8Result = 0;
	u8 u8Data[100];
	while(Local_u8Result == 0)
	{
		/* Stop Echo */
		MUSART1_voidTransmit("ATE0\r\n");
		/* Check if CMD was sent correctly */
		Local_u8Result = u8ESPValidateCmd(u8Data , 100);
 	}

	Local_u8Result = 0;

	while(Local_u8Result == 0)
	{
		/* Set Station mode */
		MUSART1_voidTransmit("AT+CWMODE=1\r\n");
		/* Check if CMD was sent correctly */
		Local_u8Result = u8ESPValidateCmd(u8Data , 100);
	}

}

void HESP_voidWifiConnect(u8 * Copy_u8Name,u8 * Copy_u8Password)
{
	u8 Local_u8Result=0;
	u8 u8Data[100];
	while(Local_u8Result==0)
	{
/*********** Connect On WiFi ***********/

		MUSART1_voidTransmit("AT+CWJAP_CUR=\"");
		MUSART1_voidTransmit(Copy_u8Name);
		MUSART1_voidTransmit("\",\"");
		MUSART1_voidTransmit(Copy_u8Password);
		MUSART1_voidTransmit("\"\r\n");
		Local_u8Result = u8ESPValidateCmd(u8Data , 100);
	}

}

void HESP_voidGEThttpReq(u8 * Copy_u8IP , u8 * Copy_u8Port, u8 * Copy_u8URL , u8 * Copy_u8Data , u16 Copy_u16Max)
{
	u8 Local_u8Result=0;
	while(Local_u8Result==0)
	{
		HESP_voidWifiCIPSEND(Copy_u8URL , Copy_u8IP , Copy_u8Port);

		MUSART1_voidTransmit("GET ");
		MUSART1_voidTransmit(Copy_u8URL);
		MUSART1_voidTransmit("\r\n");
		Local_u8Result = u8ESPValidateCmd(Copy_u8Data , Copy_u16Max);
	}
}

static void HESP_voidWifiServerUrlTCP(u8 * Copy_u8Url , u8 * Copy_u8Port)
{
	u8 Local_u8Result = 0;
	u8 u8Data[100];
	while(Local_u8Result==0)
	{
/*********** Connect On Server ***********/
		MUSART1_voidTransmit("AT+CIPSTART=\"TCP\",\"");
		MUSART1_voidTransmit(Copy_u8Url);
		MUSART1_voidTransmit("\",");
		MUSART1_voidTransmit(Copy_u8Port);
		MUSART1_voidTransmit("\r\n");

		Local_u8Result = u8ESPValidateCmd(u8Data , 100);
	}
	
}

static void HESP_voidWifiCIPSEND(u8 * Copy_u8URL , u8 * Copy_u8IP , u8 * Copy_u8Port)
{
	u8 Data_String3[15] ={0};
	u8 Copy_u8Length1=strlen(Copy_u8URL)+6;
	sprintf(Data_String3,"%s%d%s","AT+CIPSEND=",Copy_u8Length1,"\r\n");
	u8 Local_u8Result=0;
	u8 u8Data[100];
	while(Local_u8Result==0)
	{
/******** Send length of data **********/
		HESP_voidWifiServerUrlTCP(Copy_u8IP , Copy_u8Port);
		MUSART1_voidTransmit(Data_String3);
		Local_u8Result = u8ESPValidateCmd(u8Data , 100);
	}
}



static u8 u8ESPValidateCmd(u8 * Copy_u8Data , u16 Copy_max)
{
	u16 i=0;
	u8 Result = 0;
	while(i< Copy_max)
	{
		Copy_u8Data[i] = MUSART1_u8Receive();
		i++;
	}
	for(u8 i=0;i<100;i++)
	{
		if((Copy_u8Data[i]=='O' && Copy_u8Data[i+1]=='K')||(Copy_u8Data[i]=='C' && Copy_u8Data[i+1]=='O' && Copy_u8Data[i+2]=='N' && Copy_u8Data[i+3]=='N' && Copy_u8Data[i+4]=='E' && Copy_u8Data[i+5]=='C' && Copy_u8Data[i+6]=='T'))
		{
			Result = 1;
			break;
		}
	}

	return Result;

}
