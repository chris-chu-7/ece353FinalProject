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

static volatile uint16_t	PS2_X_DATA = 0;
static volatile uint16_t	PS2_Y_DATA = 0;
static volatile PS2_DIR_t PS2_DIR = PS2_DIR_CENTER;

//*****************************************************************************
// Returns the most current direction that was pressed.
//*****************************************************************************
PS2_DIR_t ps2_get_direction(void)
{
	if(PS2_X_DATA > PS2_ADC_HIGH_THRESHOLD)
	{
		return PS2_DIR_LEFT;
	} 
	else if(PS2_X_DATA < PS2_ADC_LOW_THRESHOLD)
	{
		return PS2_DIR_RIGHT;
	}
	else if(PS2_Y_DATA > PS2_ADC_HIGH_THRESHOLD)
	{
		return PS2_DIR_UP;
	}
	else if(PS2_Y_DATA < PS2_ADC_LOW_THRESHOLD)
	{
		return PS2_DIR_DOWN;
	}
	else
	{
		return PS2_DIR_CENTER;
	}
}

//*****************************************************************************
// TIMER2 ISR is used to determine when to move the car
//*****************************************************************************
void TIMER2A_Handler(void)
{	
	moveCar(PS2_DIR);
	ALERT_CAR = true;
	// Clear the interrupt
	TIMER2->ICR |= TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// TIMER4 ISR is used to trigger the ADC
//*****************************************************************************
void TIMER3A_Handler(void)
{	
	// Trigger sample sequencer 2
	ADC0->PSSI |= ADC_PSSI_SS2;
	// Clear the interrupt
	TIMER3->ICR |= TIMER_ICR_TATOCINT; 
}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{
	PS2_X_DATA = ADC0->SSFIFO2;
	PS2_Y_DATA = ADC0->SSFIFO2;
	
	PS2_DIR = ps2_get_direction();
	
	// Clear the interrupt
  ADC0->ISC |= ADC_ISC_IN2;
}

