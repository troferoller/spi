#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>
#include "stm32f4xx_spi.h"
#include "stm32f4xx_hal_def.h"
#ifdef __cplusplus
extern "C"
#endif
uint8_t SPIData = 0;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void SPI1_Init()
{
	HAL_Init();
	SPI_HandleTypeDef	        hspi;
	SPI_TypeDef                 *spi;
	__GPIOA_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;

	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
	
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	__SPI1_CLK_ENABLE();

	hspi.Instance = spi;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_LOW;				  
	hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 10;
	
	HAL_SPI_MspInit(&hspi);
}


int main(void)
{
	SPI1_Init();

	SPI_I2S_SendData(SPI1, 0x8F);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET) {}
	SPI_I2S_SendData(SPI1, 0x00);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == SET) {}
	SPIData = SPI_I2S_ReceiveData(SPI1);

	for (;;)
	{
	}
}
