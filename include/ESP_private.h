/* ******************************************************* */
/* Author	: Yasmin El Margoushy						   */
/* Date		: 28 SEPT 2020								   */
/* Version	: V01										   */
/* ******************************************************* */
#ifndef ESP_PRIVATE_H
#define ESP_PRIVATE_H

static void HESP_voidWifiServerUrlTCP(u8 * Copy_u8Url , u8 * Copy_u8Port);
static void HESP_voidWifiCIPSEND(u8 * Copy_u8URL , u8 * Copy_u8IP , u8 * Copy_u8Port);
static u8 u8ESPValidateCmd(u8 * Copy_u8Data , u16 Copy_max);
#endif

