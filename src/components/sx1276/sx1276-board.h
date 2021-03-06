/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1276 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __SX1276_ARCH_H__
#define __SX1276_ARCH_H__
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef enum {
    TIMER_TxTimeout = 0x01,
    TIMER_RxTimeout = 0x02,
    TIMER_RxTimeoutSyncWord = 0x03

} SX1276_TimerEvent_t;

/*!
 * \brief Timer object description
 */
typedef	struct {
    SX1276_TimerEvent_t id;
    uint32_t timeout;
} TimerEvent_t;

/*!
 * \brief Radio hardware registers initialization definition
 *
 * \remark Can be automatically generated by the SX1276 GUI (not yet implemented)
 */
//#define RADIO_INIT_REGISTERS_VALUE                \
//{                                                 \
//    { MODEM_FSK , REG_LNA                , 0x23 },\
//    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
//    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
//    { MODEM_FSK , REG_AFCFEI             , 0x01 },\
//    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
//    { MODEM_FSK , REG_OSC                , 0x07 },\
//    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
//    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
//    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
//    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
//    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
//    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
//    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
//    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
//    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
//    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
//}                                                 \
//
////#define RF_MID_BAND_THRESH                          525000000

/*!
 * \brief Initializes the radio I/Os pins interface
 */
void SX1276IoInit( void );

/*!
 * \brief Initializes DIO IRQ handlers
 *
 * \param [IN] irqHandlers Array containing the IRQ callback functions
 */
void SX1276IoIrqInit( void );

/*!
 * \brief De-initializes the radio I/Os pins interface. 
 *
 * \remark Useful when going in MCU lowpower modes
 */
void SX1276IoDeInit( void );

/*!
 * \brief Gets the board PA selection configuration
 *
 * \param [IN] channel Channel frequency in Hz
 * \retval PaSelect RegPaConfig PaSelect value
 */
uint8_t SX1276GetPaSelect( uint32_t channel );

/*!
 * \brief Set the RF Switch I/Os pins in Low Power mode
 *
 * \param [IN] status enable or disable
 */
void SX1276SetAntSwLowPower( bool status );

/*!
 * \brief Initializes the RF Switch I/Os pins interface
 */
void SX1276AntSwInit( void );

/*!
 * \brief De-initializes the RF Switch I/Os pins interface 
 *
 * \remark Needed to decrease the power consumption in MCU lowpower modes
 */
void SX1276AntSwDeInit( void );

/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] rxTx [1: Tx, 0: Rx]
 */
void SX1276SetAntSw( uint8_t rxTx );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX1276CheckRfFrequency( uint32_t frequency );

/*!
 * \brief Resets the SX1276
 */
void SX1276Reset( void );

/*!
 * Radio hardware and global parameters
 */
extern SX1276_t SX1276;

/*!
 * \brief Initializes the timer object
 *
 * \remark TimerSetValue function must be called before starting the timer.
 *         this function initializes timestamp and reload value at 0.
 *
 * \param [IN] obj          Structure containing the timer object parameters
 * \param [IN] callback     Function callback called at the end of the timeout
 */
void TimerInit( TimerEvent_t *timer);


/*!
 * \brief Starts and adds the timer object to the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStart( TimerEvent_t *obj );


/*!
 * \brief Stops and removes the timer object from the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStop( TimerEvent_t *obj );


/*!
 * \brief Set timer new timeout value
 *
 * \param [IN] obj   Structure containing the timer object parameters
 * \param [IN] value New timer timeout value
 */
void TimerSetValue( TimerEvent_t *obj, uint32_t value );


typedef uint32_t TimerTime_t;
/*!
 * \brief Read the current time
 *
 * \retval time returns current time
 */
TimerTime_t TimerGetCurrentTime( void );

/*!
 * \brief Return the Time elapsed since a fix moment in Time
 *
 * \param [IN] savedTime    fix moment in Time
 * \retval time             returns elapsed time
*/
TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime );


/*!
 * \brief Read the current time
 *
 * \retval time returns current time
 */
TimerTime_t TimerGetCurrentTime( void );


/*!
 * \brief Return the Time elapsed since a fix moment in Time
 *
 * \param [IN] savedTime    fix moment in Time
 * \retval time             returns elapsed time
*/
TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime );

/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );


void SX1276OnDio0Irq( void );
void SX1276OnDio1Irq( void );
void SX1276OnDio2Irq( void );
void SX1276OnDio3Irq( void );
void SX1276OnDio4Irq( void );
void SX1276OnDio5Irq( void );

#endif // __SX1276_ARCH_H__
