#include "stm32f4xx.h"
#include <stdio.h>

volatile uint16_t adc_value;

void uart_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (2 << 4) | (2 << 6);  
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);

    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_send(char *s)
{
    while (*s)
    {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *s++;
    }
}

void tim2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 8399;
    TIM2->ARR = 99;
    TIM2->CR2 |= TIM_CR2_MMS_1; 
    TIM2->CR1 |= TIM_CR1_CEN;
}

void adc_dma_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    GPIOA->MODER |= (3 << 0); 

    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = (uint32_t)&adc_value;
    DMA2_Stream0->NDTR = 1;
    DMA2_Stream0->CR =
        DMA_SxCR_MINC |
        DMA_SxCR_TCIE |
        DMA_SxCR_CIRC |
        DMA_SxCR_PSIZE_0 |
        DMA_SxCR_MSIZE_0;

    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    DMA2_Stream0->CR |= DMA_SxCR_EN;

    ADC1->SQR3 = 0;
    ADC1->CR2 =
        ADC_CR2_DMA |
        ADC_CR2_DDS |
        ADC_CR2_EXTEN_0 |
        (0b0110 << 24) |
        ADC_CR2_ADON;
}

void DMA2_Stream0_IRQHandler(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF0)
    {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;

        char buf[32];
        sprintf(buf, "ADC: %d\r\n", adc_value);
        uart_send(buf);
    }
}

int main(void)
{
    uart_init();
    tim2_init();
    adc_dma_init();

    while (1)
    {
    }
}
