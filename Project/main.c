// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "main.h"

#define HIGHSCORE_ADDR 0x0001

// Globals
uint16_t CAR_X_COORD = COLS/2;
uint16_t CAR_Y_COORD = ROWS - 20;
volatile bool ALERT_CAR = true;


//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

void moveCar(volatile PS2_DIR_t direction)
{
	
	switch(direction)
	{
		case PS2_DIR_LEFT:
			printf("Move car left\n\r");
			CAR_X_COORD = CAR_X_COORD - 1;
			return;
		case PS2_DIR_RIGHT:
			printf("Move car right\n\r");
			CAR_X_COORD = CAR_X_COORD + 1;
			return;
		
		default:
			return;
	}
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	uint8_t data;
	char input;
	bool isPaused = false;
	
	init_hardware();

	printf("\n\f");
  printf("**************************************\n\r");
  printf("* Final Project - Main \n\r");
  printf("**************************************\n\r");
 
	eeprom_byte_read(EEPROM_I2C_BASE, HIGHSCORE_ADDR, &data);
	printf("Data2: %u\n\r", data);
	printf("Running...\n\r");

	while(1)
	{
		/*
		// Check if paused
		input = getchar();
		if(input == ' ')
		{
			if(isPaused)
			{
				printf("Running...\n\r");
				isPaused = false;
			}
		  else
			{
				printf("Paused. Hit space bar to resume\n\r");
				isPaused = true;
			}
		}
		
		// Don't do anything else until unpaused
		if(isPaused)
		{
			continue;
		}
		*/
		if(ALERT_CAR)
		{
			printf("Alert Car\n\r"); 
			ALERT_CAR = false;
			lcd_draw_image(CAR_X_COORD,
								carWidthPixels, 
								CAR_Y_COORD,
								carHeightPixels,
								carBitmaps,
								LCD_COLOR_BLUE,
								LCD_COLOR_BLACK);
		}
	}
			
}
