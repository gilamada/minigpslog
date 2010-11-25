#ifndef __CONFIG_H__
#define __CONFIG_H__

/* SPI Flash  SST25VF040B */
#define SPI_FLASH                 SPI1
#define SPI_FLASH_CLK             RCC_APB2Periph_SPI1
#define SPI_FLASH_GPIO            GPIOA
#define SPI_FLASH_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_PIN_SCK         GPIO_Pin_5
#define SPI_FLASH_PIN_MISO        GPIO_Pin_6
#define SPI_FLASH_PIN_MOSI        GPIO_Pin_7
#define SPI_FLASH_CS              GPIO_Pin_4
#define SPI_FLASH_CS_GPIO         GPIOA
#define SPI_FLASH_CS_GPIO_CLK     RCC_APB2Periph_GPIOA

/* 128*64液晶屏 */
#define  BUS_NO			8			//定义总线起始的GPIO
#define  LCM_PORT		GPIOD
#define  LCM_EN			GPIO_Pin_3
#define  LCM_RST		GPIO_Pin_4
#define  LCM_DI			GPIO_Pin_5
#define  LCM_RW			GPIO_Pin_6

//语音
#define VOICE_PORT		GPIOF
#define VOICE_PORT_CLK	RCC_APB2Periph_GPIOF
#define VOICE_PINS		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define VOICE_MASK		0xfff0
#define	VOICE_START_BIT	0

//键盘
#define	KEY_PORT			GPIOC
#define	KEY_PORT_CLK		RCC_APB2Periph_GPIOC
#define	KEY_BACK_PIN		GPIO_Pin_0
#define	KEY_SELECT_PIN		GPIO_Pin_1
#define	KEY_UP_PIN			GPIO_Pin_2
#define	KEY_DOWN_PIN		GPIO_Pin_3	
#define	KEY_LIFT_PIN		GPIO_Pin_4
#define	KEY_RIGHT_PIN		GPIO_Pin_5
#define	KEY_MASK			0x003f
#define KEY_START_BIT		0

//LED指示灯
#define LED_PORT		GPIOF
#define LED_PORT_CLK	RCC_APB2Periph_GPIOF
#define LED_PINS		GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define	LED_MASK		0xff0f
#define	LED_START_BIT	4

//DS18B20
#define DS_PORT			GPIOA
#define DS_DQIO			GPIO_Pin_1
#define DS_RCC_PORT		RCC_APB2Periph_GPIOA

//LED显示板
#define DISP_ADDR_PORT		GPIOG
#define DISP_ADDR_PORT_CLK	RCC_APB2Periph_GPIOG
#define DISP_ADDR_PIN		GPIO_Pin_All
#define DISP_DATA_PORT		GPIOF
#define DISP_DATA_PORT_CLK	RCC_APB2Periph_GPIOF
#define DISP_DATA_PIN		0x00ff

#endif
