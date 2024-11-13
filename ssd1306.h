#ifndef SSD1306_H
#define SSD1306_H

//--------------Includes-----------------
#include "stdint.h"
#include "i2c.h"
#include "font.h"

//--------------Macros-------------------
#define SSD1306_ADDR                                         0x3C
#define SSD1306_HEIGHT                                       0x40 //64
#define SSD1306_COM_LR_REMAP                                 0x00
#define SSD1306_COM_ALTERNATIVE_PIN_CONFIG                   0x01
#define SSD1306_WIDTH                                         128


//-------------Enums---------------------
typedef enum {
    Black = 0x00,   // Black color, no pixel
    White = 0x01,   // Pixel is set. Color depends on LCD
} SSD1306_COLOR;

//-------------Structs-------------------

typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
}SSD1306_t;

//-------------Function Declarations------------
void ssd1306_WriteCommand(uint8_t Command);
void ssd1306_Init();
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen();
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteString(const char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void delay(uint32_t Seconds);

//-------------Variables--------------


#endif