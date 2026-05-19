// Main SPI Slave Interface with raspberry pi made with HAL LIB as Arduino API is unable...
#include "Main.h"
#include "stm32f1xx.h"

SPI_HandleTypeDef spi;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);


volatile uint8_t data_tx[3] = {0xAA, 0xBB, 0xCC};  // Example data to transmit
volatile instruction instruction_tx = Ready;


void SPIEvent(){
  readSPI = true;
  uint8_t bufferSize = 0;
  int aux = 0;
  if (HAL_SPI_Receive(&spi, (uint8_t*) &instruction_rx, 1, HAL_MAX_DELAY) == HAL_OK) {
    switch(instruction_rx){
      case DataMpu0Ac:
      case DataMpu1Ac:
      case DataMpu2Ac:
      case DataMpu3Ac:
      case DataMpu0Gy:
      case DataMpu1Gy:
      case DataMpu2Gy: 
      case DataMpu3Gy:
      case DataMpu0Mg:
      case DataMpu1Mg:
      case DataMpu2Mg:
      case DataMpu3Mg:
      case DataBMPAt:
      case DataBMPTp:
      case DataHx711:
      case DataBat:
      case LEDR:
      case LEDG:
      case LEDB:
        bufferSize = 1;
        data_tx[1] = Send;
        break;
      case Switch0On:
        digitalWrite(SW0, HIGH);
        break;
      case Switch1On:
        digitalWrite(SW1, HIGH);
        break;
      case Switch2On:
        digitalWrite(SW2, HIGH);
        break;
      case Switch3On:
        digitalWrite(SW3, HIGH);
        break;
      case Switch4On:
        digitalWrite(SW4, HIGH);
        break;
      case Switch5On:
        digitalWrite(SW5, HIGH);
        break;
      case Switch0Off:
        digitalWrite(SW0, LOW);
        break;
      case Switch1Off:
        digitalWrite(SW1, LOW);
        break;
      case Switch2Off:
        digitalWrite(SW2, LOW);
        break;
      case Switch3Off:
        digitalWrite(SW3, LOW);
        break;
      case Switch4Off:
        digitalWrite(SW4, LOW);
        break;
      case Switch5Off:
        digitalWrite(SW5, LOW);
        break;
      //case BuzzerOn:
      //  buzzerOn();
      //  break;
      //case BuzzerOff:
      //  buzzerOff();
      //  break;
    }
    if(bufferSize)
      HAL_SPI_Transmit(&spi, (uint8_t*) data_tx, bufferSize , HAL_MAX_DELAY);
  }
  HAL_SPI_Transmit(&spi,(uint8_t*) &instruction_tx , 1, HAL_MAX_DELAY);
}

void setSPI() {   
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();

    spi.Instance = SPI1;
    spi.Init.Mode = SPI_MODE_SLAVE; 
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&spi);

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_SPI_ENABLE(&spi);

    attachInterrupt(PA4, SPIEvent, FALLING);
    HAL_SPI_Transmit(&spi,(uint8_t*) &instruction_tx , 1, HAL_MAX_DELAY);
}