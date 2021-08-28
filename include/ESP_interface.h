/* ******************************************************* */
/* Author	: Yasmin El Margoushy						   */
/* Date		: 28 SEPT 2020								   */
/* Version	: V01										   */
/* ******************************************************* */
#ifndef ESP_INTERFACE_H
#define ESP_INTERFACE_H

void HESP_voidInit(void);
void HESP_voidWifiConnect(u8 * Copy_u8Name,u8 * Copy_u8Password);
void HESP_voidGEThttpReq(u8 * Copy_u8IP , u8 * Copy_u8Port, u8 * Copy_u8URL , u8 * Copy_u8Data , u16 Copy_u16Max);

#endif
