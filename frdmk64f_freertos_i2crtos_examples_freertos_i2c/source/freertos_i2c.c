
#include "global.h"
 /*  Standard C Included Files */
#include <string.h>
#include <stdio.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_port.h"
#include "fsl_i2c.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
/* TODO: insert other definitions and declarations here. */
typedef long BaseType_t;
#define EVENT_BIT (1<<0)
#define I2C_BAUDRATE 100000U

#define MASK_UNITS          0x0F

#define MASK_TENS_OF_SECOND 0x70
#define MASK_TENS_OF_MINUTE 0x70
#define MASK_TENS_OF_HOUR   0x10

#define MASK_TENS_OF_DAY    0x30
#define MASK_TENS_OF_MONTH  0x10
#define MASK_TENS_OF_YEAR   0xF0

#define ASCII_MASK 0x30
#define REGISTER_SEC
#define REGISTER_MIN
#define REGISTER_HOUR
#define REGISTER_DAY
#define REGISTER_MONTH
#define REGISTER_YEAR


//volatile bool g_MasterCompletionFlag = false;
i2c_master_handle_t g_m_handle;
//i2c_master_transfer_t masterXfer;

//SemaphoreHandle_t semaphoreISR;
SemaphoreHandle_t mutex;
EventGroupHandle_t event;
TaskHandle_t init_i2c_handle;
TaskHandle_t ST_RTC_handle;
EventBits_t uxBits;

uint8_t * read_RTC (uint8_t reg);
void write_RTC (uint8_t reg, uint8_t *dato);

void write_hour(uint8_t *dato);
void write_date(uint8_t *dato);
void read_hour(uint8_t *dato);
void read_date(uint8_t *dato);

/*
 * @brief   Application entry point.
 */
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle,
        status_t status, void * userData)
{
//	PRINTF(" i2c_master_callback \n\r");

	BaseType_t reschedule;
	reschedule = pdFALSE;

	if (status == kStatus_Success)
	{
//		  xSemaphoreGiveFromISR(semaphoreISR, &reschedule); /* TODO: de aqui no pasa */
		xEventGroupSetBitsFromISR(event, EVENT_BIT, &reschedule);
	}
  //    xSemaphoreGiveFromISR(semaphoreISR_, &reschedule_); /* TODO: de aqui no pasa */
//    xEventGroupSetBits(event,EVENT_BIT);
//    portYIELD_FROM_ISR(pdTRUE == reschedule || pdTRUE == reschedule_ ? pdTRUE : pdFALSE);
    portYIELD_FROM_ISR(reschedule);
}


void init_i2c(void *arg)
{

	init_i2c_handle = xTaskGetCurrentTaskHandle();

    CLOCK_EnableClock(kCLOCK_I2c0);
	CLOCK_EnableClock(kCLOCK_PortB);

	port_pin_config_t config_i2c =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAlt2,
	        kPORT_UnlockRegister, };

	PORT_SetPinConfig(PORTB, 2, &config_i2c);
	PORT_SetPinConfig(PORTB, 3, &config_i2c);

	i2c_master_config_t masterConfig;
	I2C_MasterGetDefaultConfig(&masterConfig);

	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));

	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, NULL);

	NVIC_EnableIRQ(I2C0_IRQn);
	//NVIC_EnableIRQ(PORTB_IRQn);
	NVIC_SetPriority(I2C0_IRQn,5);
	//NVIC_SetPriority(PORTB_IRQn,5);

}

void write_hour(uint8_t *dato)
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;

	seconds = ( (dato[0] - ASCII_MASK) << 4 ) | ( dato[1] - ASCII_MASK ) ;
	write_RTC(0x00, seconds);

	minutes = ( (dato[2] - ASCII_MASK) << 4 ) | ( dato[3] - ASCII_MASK );
	write_RTC(0x01, minutes);

	hours   = ( (dato[4] - ASCII_MASK) << 4 ) | ( dato[5] - ASCII_MASK );
	write_RTC(0x02, hours);
}
void write_date(uint8_t *dato)
{
	uint8_t days;
	uint8_t months;
	uint8_t years;

	days   = ( (dato[0] - ASCII_MASK) << 4 ) | ( dato[1] - ASCII_MASK );
	write_RTC(0x04, days);

	months = ( (dato[2] - ASCII_MASK) << 4 ) | ( dato[3] - ASCII_MASK );
	write_RTC(0x05, months);

	years  = ( (dato[4] - ASCII_MASK) << 4 ) | ( dato[5] - ASCII_MASK );
	write_RTC(0x06, years);
}

void write_RTC(uint8_t reg, uint8_t *dato)
{
	static uint8_t dateTime[6];

	i2c_master_transfer_t masterXfer;

	uint8_t* data_buffer = dateTime;
	masterXfer.slaveAddress = 0x6F;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = (uint32_t)reg;
	masterXfer.subaddressSize = 1U;
	masterXfer.data = data_buffer;
	masterXfer.dataSize = 1U;
	masterXfer.flags = kI2C_TransferDefaultFlag;

    xSemaphoreTake(mutex, portMAX_DELAY);
	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
//	while (!g_MasterCompletionFlag)
//	{
//	}
//	g_MasterCompletionFlag = false;
//	xSemaphoreTake(semaphoreISR, portMAX_DELAY);
	xEventGroupWaitBits(event, EVENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
	xSemaphoreGive(mutex);

//	for(;;)
//	{
//		vTaskDelete(ST_RTC_handle);
////		vTaskDelay(1000);
//	}
	return 0;

}

void read_hour(uint8_t *time)
{
	uint8_t buffer;

	buffer = read_RTC(0x00);
	time[0] =   (buffer & MASK_UNITS         )      + ASCII_MASK;
	time[1] = ( (buffer & MASK_TENS_OF_SECOND)>>4 ) + ASCII_MASK;

	buffer = read_RTC(0x01);
	time[2] =   (buffer & MASK_UNITS         )      + ASCII_MASK;
	time[3] = ( (buffer & MASK_TENS_OF_MINUTE)>>4 ) + ASCII_MASK;

	buffer = read_RTC(0x02);
	time[4] =   (buffer & MASK_UNITS         )      + ASCII_MASK;
	time[5] = ( (buffer & MASK_TENS_OF_HOUR  )>>4 ) + ASCII_MASK;

}

void read_date(uint8_t *date)
{
	/* TODO: ¿buffer needs to be defined as a pointer? */
	uint8_t buffer;

	buffer = read_RTC(0x04);
	date[0] =   (buffer & MASK_UNITS          )      + ASCII_MASK;
	date[1] = ( (buffer & MASK_TENS_OF_DAY    )>>4 ) + ASCII_MASK;

	buffer = read_RTC(0x05);
	date[2] =   (buffer & MASK_UNITS          )      + ASCII_MASK;
	date[3] = ( (buffer & MASK_TENS_OF_MONTH  )>>4 ) + ASCII_MASK;

	buffer = read_RTC(0x06);
	date[4] =   (buffer & MASK_UNITS          )      + ASCII_MASK;
	date[5] = ( (buffer & MASK_TENS_OF_YEAR   )>>4 ) + ASCII_MASK;
}

uint8_t * read_RTC(uint8_t reg)
{
	uint8_t status;
	PRINTF(" Read_RTC \n\r");
//	i2c_master_handle_t g_m_handle_2;
//	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle_2, i2c_master_callback_2, NULL);

	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	uint8_t buffer; //1st option
//	uint8_t *buffer; //2nd option

		masterXfer.slaveAddress = 0x6F;
		masterXfer.direction = kI2C_Read;
		masterXfer.subaddress = (uint32_t)reg;
		masterXfer.subaddressSize = 1U;
		masterXfer.data = &buffer;
		masterXfer.dataSize = 1U;
		masterXfer.flags = kI2C_TransferDefaultFlag;

		xSemaphoreTake(mutex, portMAX_DELAY);
		status = I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);

		if (status != kStatus_Success)
		    {
			xEventGroupSetBits(event, EVENT_BIT);
		    }
	//	while (!g_MasterCompletionFlag)
	//	{
	//	}
	//	g_MasterCompletionFlag = false;
//		xSemaphoreTake(semaphoreISR, portMAX_DELAY);
		uxBits = xEventGroupWaitBits(event, EVENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

		if ((uxBits & EVENT_BIT) != 0)
		{
			/* xEventGroupWaitBits() returned because just EVENT_BIT was set. */
			PRINTF("EVENT_BIT was set\n\r");
		}

		xSemaphoreGive(mutex);

//		ones = buffer & MASK_ONES;

		PRINTF("%X\n\r", buffer);
//		PRINTF("%X\n\r", buffer); //1st option
//		PRINTF("%X\n\r", (uint8_t)buffer); //2nd option

		return buffer;
}

//void Read_RTC(void *arg)
//{
//	uint8_t status;
//	PRINTF(" Read_RTC \n\r");
////	i2c_master_handle_t g_m_handle_2;
////	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle_2, i2c_master_callback_2, NULL);
//
//	i2c_master_transfer_t masterXfer;
//	memset(&masterXfer, 0, sizeof(masterXfer));
//
//	uint8_t buffer;
////	uint8_t *buffer;
//
//
//		masterXfer.slaveAddress = 0x6F;
//		masterXfer.direction = kI2C_Read;
//		masterXfer.subaddress = (uint32_t)0x00;
//		masterXfer.subaddressSize = 1U;
//		masterXfer.data = &buffer;
//		masterXfer.dataSize = 1U;
//		masterXfer.flags = kI2C_TransferDefaultFlag;
//
//		xSemaphoreTake(mutex, portMAX_DELAY);
//		status = I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
//
//		if (status != kStatus_Success)
//		    {
//			xEventGroupSetBits(event, EVENT_BIT);
//		    }
//	//	while (!g_MasterCompletionFlag)
//	//	{
//	//	}
//	//	g_MasterCompletionFlag = false;
////		xSemaphoreTake(semaphoreISR, portMAX_DELAY);
//		uxBits = xEventGroupWaitBits(event, EVENT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
//
//		if ((uxBits & EVENT_BIT) != 0)
//		{
//			/* xEventGroupWaitBits() returned because just EVENT_BIT was set. */
//			PRINTF("EVENT_BIT was set\n\r");
//		}
//
//		xSemaphoreGive(mutex);
//		ones = buffer & MASK_ONES;
//
//		PRINTF("%X\n\r", buffer);
////		PRINTF("%X\n\r", (uint8_t)buffer);
//
//
//}
//
//int main(void) {
//
//  	/* Init board hardware. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
//  	/* Init FSL debug console. */
//    BOARD_InitDebugConsole();
//
//
////	semaphoreISR = xSemaphoreCreateBinary();
//    mutex = xSemaphoreCreateMutex();
//    event = xEventGroupCreate();
//
//    xTaskCreate(init_i2c, "init_i2c", configMINIMAL_STACK_SIZE+100, NULL, configMAX_PRIORITIES, NULL);
//	xTaskCreate(ST_RTC, "ST_RTC", configMINIMAL_STACK_SIZE+100, NULL, configMAX_PRIORITIES, NULL);
//	xTaskCreate(Read_RTC, "Read_RTC", configMINIMAL_STACK_SIZE+100, NULL, configMAX_PRIORITIES, NULL);
//	vTaskStartScheduler();
//
//	read_Datetime(SEGundos, {'1','2'});
//	read_Datetime(Minutos, {'1','2'});
//	read_Datetime(Horas, {'1','2'});
//
//    /* Force the counter to be placed into memory. */
//    volatile static int i = 0 ;
//    /* Enter an infinite loop, just incrementing a counter. */
//    while(1) {
//        i++ ;
//    }
//    return 0 ;
//}
