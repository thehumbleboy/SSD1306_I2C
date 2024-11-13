#include "main.h"

int main(){
    I2C_Init();
    ssd1306_Init();
    delay(1000);
    
    ssd1306_UpdateScreen();
    ssd1306_Fill(Black);
    delay(100000);
    
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Weight:", Font_7x10, White);

    int counter = 0;
    int direction = 1;  // 1 for up, -1 for down
    char buffer[10]; // To store the counter as a string

    while(1){
        // Clear the previous value before writing the new one
        ssd1306_Fill(Black);
        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Weight:", Font_7x10, White);

        // Convert the counter to a string and display it
        sprintf(buffer, "%d gm", counter);
        ssd1306_SetCursor(0, 26);
        ssd1306_WriteString(buffer, Font_7x10, White);

        // Update the screen with the new content
        ssd1306_UpdateScreen();

        // Increment or decrement the counter based on direction
        counter += direction;

        // Change direction when limits are reached
        if(counter >= 15){
            direction = -1; // Start counting down
        }
        else if(counter <= 0){
            direction = 1;  // Start counting up
        }

        // Delay for 1 second
        delay(1000);
    }
}
