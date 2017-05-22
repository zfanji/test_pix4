#ifndef __SPI_H
#define __SPI_H


#include "stm32f4xx_hal_conf.h"

#define SPI1_CLK_ENABLE()                __SPI1_CLK_ENABLE()
#define SPI1_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE()

#define SPI1_FORCE_RESET()               __SPI1_FORCE_RESET()
#define SPI1_RELEASE_RESET()             __SPI1_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPI1_SCK_PIN                     GPIO_PIN_5
#define SPI1_SCK_GPIO_PORT               GPIOA
#define SPI1_SCK_AF                      GPIO_AF5_SPI1

#define SPI1_MISO_PIN                    GPIO_PIN_6
#define SPI1_MISO_GPIO_PORT              GPIOA
#define SPI1_MISO_AF                     GPIO_AF5_SPI1

#define SPI1_MOSI_PIN                    GPIO_PIN_7
#define SPI1_MOSI_GPIO_PORT              GPIOA
#define SPI1_MOSI_AF                     GPIO_AF5_SPI1


void SPI1_Init(void);

#endif

