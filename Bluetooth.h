#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define Rx  (1U<<5)
#define Tx  (1U<<4)

/*!
 * @fn      Bluetooth_Init
 * @brief   Initializes the bluetooth pins for Rx and Tx
*/
void Bluetooth_Init(void);
/*!
 * @fn      Bluetooth_ReceiveChar
 * @brief   Receives a character from the user using bluetooth module HC-05
 * @return  Returns a character that was sent from the user using bluetooth
*/
char Bluetooth_ReceiveChar(void);
/*!
 * @fn      Bluetooth_SendChar
 * @brief   Sends a character to the user using bluetooth module HC-05
 * @param   data    The character sent to the user
*/
void Bluetooth_SendChar(unsigned char data);
/*!
 * @fn      Bluetooth_SendString
 * @brief   Sends a whole string to the user using bluetooth module HC-05
 * @param   str     the string sent to the user
*/
void Bluetooth_SendString(char *str);
void Delay(unsigned long counter);
void UART5_Handler(void);

#endif //BLUETOOTH_H_
