#include "i2c.h"

void I2C_Init(){
	//Clock enable of GPIO and I2C1
	//PB8 => SCL PB9 => SDA
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |=RCC_APB1ENR_I2C1EN;
	
	//GPIO Init
	GPIOB->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_1 | GPIO_PUPDR_PUPD9_1;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2;
	
	//I2C Init
	// Fm mode 4Mhz, standard mode 100khz 7bit addressing mode
I2C1->CR1 &= ~(1 << 0);  // Disable I2C1 to configure it
I2C1->CR2 = (16 << 0);   // Set APB1 clock frequency to 16 MHz

// Configure clock control for standard mode (100 kHz)
I2C1->CCR = (80 << 0);   // Standard mode, 100kHz
I2C1->TRISE = 17;  
	//Peripheral enable
	I2C1->CR1 |= I2C_CR1_PE;

}

void I2C_Start(){
	//Start the I2C and wait for SB to be set
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C_SendAddress(uint8_t Address){
	//Send the Slave address
	I2C1->DR = (Address << 1); //Address with LSB reset (transmitter)
	while(!(I2C1->SR1 & I2C_SR1_ADDR));//wait for ADDR bit to be set
	volatile uint32_t temp = I2C1->SR1 | I2C1->SR2; //This has to be done to clear ADDR bit
}

void I2C_Transmit(uint8_t Data){
	//Checking if TRA = 1 which means transmission mode 
	if(I2C1->SR2 & I2C_SR2_TRA){
		while (!(I2C1->SR1 & I2C_SR1_TXE));//Waiting for TXE flag to be set
		I2C1->DR = Data;
		while (!(I2C1->SR1 & I2C_SR1_BTF));//Waiting for the BTF flag to be set
	}
}

void I2C_Stop(){
	I2C1->CR1 |= I2C_CR1_STOP;
}
