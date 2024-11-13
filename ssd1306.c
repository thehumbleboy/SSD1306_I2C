#include "ssd1306.h"

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

SSD1306_t SSD1306;

void ssd1306_WriteCommand(uint8_t Command){
	I2C_Start();
	I2C_SendAddress(SSD1306_ADDR);
	I2C_Transmit(0x00);
	I2C_Transmit(Command);
	I2C_Stop();
}

void ssd1306_Init(){
			delay(1000);
	
// Essential SSD1306 initialization commands

	   ssd1306_WriteCommand( 0xAE);   // Display off
     ssd1306_WriteCommand( 0x20);   // Set Memory Addressing Mode
     ssd1306_WriteCommand( 0x10);   // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
     ssd1306_WriteCommand( 0xB0);   // Set Page Start Address for Page Addressing Mode,0-7
     ssd1306_WriteCommand( 0xC8);   // Set COM Output Scan Direction
     ssd1306_WriteCommand( 0x00);   // Set low column address
     ssd1306_WriteCommand( 0x10);   // Set high column address
     ssd1306_WriteCommand( 0x40);   // Set start line address
     ssd1306_WriteCommand( 0x81);   // set contrast control register
     ssd1306_WriteCommand( 0xFF);
     ssd1306_WriteCommand( 0xA1);   // Set segment re-map 0 to 127
     ssd1306_WriteCommand( 0xA6);   // Set normal display

     ssd1306_WriteCommand( 0xA8);   // Set multiplex ratio(1 to 64)
     ssd1306_WriteCommand( SSD1306_HEIGHT - 1);

     ssd1306_WriteCommand( 0xA4);   // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
     ssd1306_WriteCommand( 0xD3);   // Set display offset
     ssd1306_WriteCommand( 0x00);   // No offset
     ssd1306_WriteCommand( 0xD5);   // Set display clock divide ratio/oscillator frequency
     ssd1306_WriteCommand( 0xF0);   // Set divide ratio
     ssd1306_WriteCommand( 0xD9);   // Set pre-charge period
     ssd1306_WriteCommand( 0x22);

     ssd1306_WriteCommand( 0xDA);   // Set com pins hardware configuration
     ssd1306_WriteCommand( SSD1306_COM_LR_REMAP << 5 | SSD1306_COM_ALTERNATIVE_PIN_CONFIG << 4 | 0x02);   

     ssd1306_WriteCommand( 0xDB);   // Set vcomh
     ssd1306_WriteCommand( 0x20);   // 0x20,0.77xVcc
     ssd1306_WriteCommand( 0x8D);   // Set DC-DC enable
     ssd1306_WriteCommand( 0x14);   //
     ssd1306_WriteCommand( 0xAF);   // Turn on SSD1306 panel

delay(1000);
		 
		     // Clear screen
//    ssd1306_Fill(Black);

//    // Flush buffer to screen
//    ssd1306_UpdateScreen();

//    // Set default values for screen object
//    SSD1306.CurrentX = 0;
//    SSD1306.CurrentY = 0;

//    SSD1306.Initialized = 1;
}

void ssd1306_Fill(SSD1306_COLOR color)
{
    // Fill screenbuffer with a constant value (color)
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

void ssd1306_UpdateScreen()
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        ssd1306_WriteCommand(0xB0 + i);
        ssd1306_WriteCommand(0x00);
        ssd1306_WriteCommand(0x10);

        I2C_Start();
				I2C_SendAddress(SSD1306_ADDR);
				// Send the control byte (0x40) to indicate data transmission
        I2C_Transmit(0x40);

        // Send one page of data (one row of pixels) from the buffer
        for (uint16_t j = 0; j < SSD1306_WIDTH; j++) {
            I2C_Transmit(SSD1306_Buffer[SSD1306_WIDTH * i + j]);
        }
				I2C_Stop();
    }
}

char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;

    // Check remaining space on current line
    if ((SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth)) || (SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))) {
        // Not enough space on current line
        return 0;
    }




    // Translate font to screenbuffer
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            }
            else
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    SSD1306.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR)!color;
    }

    // Draw in the correct color
    if (color == White)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

char ssd1306_WriteString(const char* str, FontDef Font, SSD1306_COLOR color)
{
    // Write until null-byte
    while (*str)
    {
        if (ssd1306_WriteChar(*str, Font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void delay(uint32_t Seconds){
	for (uint32_t i=0;i<Seconds;i++){
	
	}
}