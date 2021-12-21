#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "stm32f4xx_spi.h"
uint8_t SPIData = 0;
void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++)
		asm("nop");
}
void	CS_ON()
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);
}

void CS_OFF()
{
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

void SPI_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	SPI_InitTypeDef spiconf;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	spiconf.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spiconf.SPI_Mode = SPI_Mode_Master;
	spiconf.SPI_DataSize = SPI_DataSize_8b;
	spiconf.SPI_CPOL = SPI_CPOL_Low;
	spiconf.SPI_CPHA = SPI_CPHA_1Edge;
	spiconf.SPI_NSS = SPI_NSS_Soft;
	spiconf.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	spiconf.SPI_FirstBit = SPI_FirstBit_MSB;
	
	SPI_Init(SPI1, &spiconf);
	SPI_Cmd(SPI1, ENABLE);
}

int main()
{
	SPI_init();
	
	CS_ON();
	SPI_I2S_SendData(SPI1, 0x8F);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET){}
	SPI_I2S_SendData(SPI1, 0x00);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == SET) {}
	SPIData = SPI_I2S_ReceiveData(SPI1);
	CS_OFF();

	for (;;)
	{
	}
}
