/**
  ******************************************************************************
  * @file    SPI/M25P64_FLASH/spi_flash.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   This file provides a set of functions needed to manage the
  *          communication between SPI peripheral and SPI M25P64 FLASH.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "config.h"
#include "spi_flash.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* SPI Flash supported commands */
/* SST25VF040B */
#define CMD_AAI       0xAF  /* AAI 连续编程指令 */
#define CMD_DISWR	  0x04	/* 禁止写, 退出AAI状态 */

#define CMD_EWRSR	  0x50	/* 允许写状态寄存器 */
#define CMD_WRSR      0x01  /* Write Status Register instruction */
#define CMD_WREN      0x06  /* Write enable instruction */
#define CMD_WRDI      0x04  /* Write enable instruction */
#define CMD_READ      0x03  /* Read from Memory instruction */
#define CMD_RDSR      0x05  /* Read Status Register instruction  */
#define CMD_RDID      0x90  /* Read identification OR 0xAB */
#define CMD_SE        0x20  /* Sector Erase instruction */
#define CMD_BE        0x52  /* Bulk Erase instruction */
#define CMD_CE        0x60  /* Chip Erase instruction */
#define CMD_BP        0x02  /* Byte-Program instruction */

#define WIP_FLAG      0x01  /* Write In Progress (WIP) flag */

#define DUMMY_BYTE    0xA5


#define SPI_FLASH_PAGESIZE    (4*1024)


/* Private macro -------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       SPI_FLASH_CS_GPIO->BRR = SPI_FLASH_CS
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      SPI_FLASH_CS_GPIO->BSRR = SPI_FLASH_CS


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI and GPIO clocks */
  RCC_APB2PeriphClockCmd(SPI_FLASH_CLK | SPI_FLASH_GPIO_CLK | SPI_FLASH_CS_GPIO_CLK, ENABLE);

  /* Configure SPI pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_FLASH_PIN_SCK  | SPI_FLASH_PIN_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_FLASH_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI_FLASH_PIN_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(SPI_FLASH_GPIO, &GPIO_InitStructure);
  
  /* Configure I/O for Flash Chip select */
  GPIO_SetBits(SPI_FLASH_CS_GPIO, SPI_FLASH_CS);	//预置为高
  GPIO_InitStructure.GPIO_Pin = SPI_FLASH_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_FLASH_CS_GPIO, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* SPI configuration */
  SPI_Cmd(SPI_FLASH, DISABLE); 		//必须先禁能,才能改变MODE
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//两线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//CPOL=0 时钟悬空低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//CPHA=0 数据捕获第1个
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//2分频=36M
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7
  SPI_Init(SPI_FLASH, &SPI_InitStructure);

  /* Enable the SPI  */
  SPI_Cmd(SPI_FLASH, ENABLE);

  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(CMD_DISWR);
  SPI_FLASH_CS_HIGH();

  SPI_FLASH_WaitForWriteEnd();

  //复位后BP1,BP0为1，芯片在写保护状态下
  SPI_FLASH_WriteStatus(0);	/* 解除所有BLOCK的写保护 */
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(CMD_SE);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the block to erase.
  * @retval None
  */
void SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* block Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send block Erase instruction */
  SPI_FLASH_SendByte(CMD_BE);
  /* Send BlockAddr high nibble address byte */
  SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
  /* Send BlockAddr medium nibble address byte */
  SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
  /* Send BlockAddr low nibble address byte */
  SPI_FLASH_SendByte(BlockAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}


/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void SPI_FLASH_ChipErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Chip Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Chip Erase instruction  */
  SPI_FLASH_SendByte(CMD_CE);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *  (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *   to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *   or less than "SPI_FLASH_PAGESIZE" value.
  * @retval None
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint32_t i;

  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(CMD_AAI);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  SPI_FLASH_SendByte(*pBuffer++);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  SPI_FLASH_WaitForWriteEnd();

  NumByteToWrite -= 1;

  for (i = 0; i < NumByteToWrite; i++)
  {
    SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(CMD_AAI);
    SPI_FLASH_SendByte(*pBuffer++);
	SPI_FLASH_CS_HIGH();
    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
  }

  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(CMD_DISWR);
  SPI_FLASH_CS_HIGH();

  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *   WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *   to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PAGESIZE;
  count = SPI_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        pBuffer += SPI_FLASH_PAGESIZE;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        pBuffer += SPI_FLASH_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(CMD_READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(DUMMY_BYTE);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint16_t SPI_FLASH_ReadID(void)
{
  uint16_t Temp = 0, Temp0 = 0, Temp1 = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(CMD_RDID);
  
  SPI_FLASH_SendByte(0);	//必须为0
  SPI_FLASH_SendByte(0);	//必须为0
  SPI_FLASH_SendByte(0);	//id地址

  /* Read a byte from the FLASH 厂家ID */
  Temp0 = SPI_FLASH_SendByte(DUMMY_BYTE);

  /* Read a byte from the FLASH 芯片ID */
  Temp1 = SPI_FLASH_SendByte(DUMMY_BYTE);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  Temp = (Temp0 << 8) | Temp1;

  return Temp;
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(CMD_READ);

  /* Send the 24-bit address of the address to read from ---------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *   has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(DUMMY_BYTE));
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *   from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while((SPI_FLASH->SR & SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_FLASH->DR = byte;

  /* Wait to receive a byte */
  while((SPI_FLASH->SR & SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return (SPI_FLASH->DR);
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *   received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  /* Loop while DR register in not emplty */
  while((SPI_FLASH->SR & SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI_FLASH peripheral */
  SPI_FLASH->DR = HalfWord;

  /* Wait to receive a Half Word */
  while((SPI_FLASH->SR & SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return (SPI_FLASH->DR);
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(CMD_WREN);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  写状态寄存器.（SST25VF016B)
  * @param  None
  * @retval None
  */
void SPI_FLASH_WriteStatus(uint8_t data)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* 允许写状态寄存器 */
  SPI_FLASH_SendByte(CMD_EWRSR);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* 允许写状态寄存器 */
  SPI_FLASH_SendByte(CMD_WRSR);
  SPI_FLASH_SendByte(data);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *   status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(CMD_RDSR);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = SPI_FLASH_SendByte(DUMMY_BYTE);

  }
  while ((flashstatus & WIP_FLAG) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
