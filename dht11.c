/**
 * Project         : DHT11 source file
 * Filename        : dht11.c
 * Version         : 1.4
 * Author          : Hariharan Gopalarishnan
 * Date            : June 29th, 2019
 * Target Platform : TM4C123GH6PM
 * IDE             : Code Composer Studio v7.4.0
 * System Clock    : 40 MHz
 */

/**
 * Copyright (c) 2019, Hariharan Gopalakrishnan
 * All rights reserved.
 *
 */

/**
 *MIT License
 *
 *Copyright (c) 2019 Hariharan Gopalakrishnan
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

/*!<
 * Pin Map
 *
 * OUT - PE3
 *
 */

#include "dht11.h"
#include "wait.h"

volatile uint8_t i = 0;
volatile uint16_t time[DATABITS + SETUPBITS];


bool UpdateRdy = 0;


#ifndef test1
//#define test1
#endif

void
initDHT11Hw ()
{
  // Configure System clock as 40Mhz
  SYSCTL_RCC_R =
    SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4
									     <<
									     SYSCTL_RCC_SYSDIV_S);

  // Enable GPIO port E
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;

  // PE3 as Digital O/P Pin

  GPIO_PORTE_DIR_R |= (1 << 3);
  GPIO_PORTE_DEN_R |= (1 << 3);
  DATA = 1;
  waitMicrosecond (1000000);	// 1 second
}


void
initWTimer1A ()
{
  SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R1;	// turn-on timer
  WTIMER1_CTL_R &= ~TIMER_CTL_TAEN;	// turn-off counter before reconfiguring
  WTIMER1_CFG_R = 4;		// configure as 32-bit counter (A only)
  WTIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR;	// Periodic up counter
}



void
initSampler ()
{
  GPIO_PORTE_IEV_R |= (1 << 3);	// Rising Edge detection

  /* Enable Interrupt */

  GPIO_PORTE_IM_R |= (1 << 3);
  NVIC_EN0_R |= (1 << (INT_GPIOE - 16));

}


void
readSensordata ()
{
  DATA = 0;

  waitMicrosecond (18000);	//~18ms

  DATA = 1;

  GPIO_PORTE_DIR_R &= ~(1 << 3);

  while (DATA);			// Wait for response

  initWTimer1A ();

  initSampler ();
}

void
getReading (float *temp, float *humidity)
{
  uint8_t j;
  uint8_t base;
  uint8_t dechumid = 0;
  uint8_t dectemp = 0;
  uint8_t data[DATABITS];	// 2 byte each data (temperature and humidity) + 1 byte parity
  readSensordata ();
  while (!UpdateRdy);
  for (j = 0; j < DATABITS; j++)
    {
      if (time[j + SETUPBITS] > LOW)
	data[j] = 1;
      else
	data[j] = 0;
    }

  for (j = INTHMDSTRT, base = ((RESOLUTION / 2) - 1); j <= INTHMDEND; j++)
    {
      *humidity += data[j] << base;
      base--;
    }

  for (j = DECHMDSTRT, base = ((RESOLUTION / 2) - 1); j <= DECHMDEND; j++)
    {
      dechumid += data[j] << base;
      base--;
    }
  for (j = DECTEMPSTRT, base = ((RESOLUTION / 2) - 1); j <= DECTEMPEND; j++)
    {
      dectemp += data[j] << base;
      base--;
    }
  for (j = INTTEMPSTRT, base = ((RESOLUTION / 2) - 1); j <= INTTEMPEND; j++)
    {
      *temp += data[j] << base;
      base--;
    }

  *humidity += ((float) dechumid / 10);	//float math can be avoided by multiplying integral part by 10 then adding fractional part
  *temp += ((float) dectemp / 10);	//TODO : Need to improve code to avoid decimal data beyond the first digit

}



void
gpioEISR ()
{
  if ((GPIO_PORTE_MIS_R & 0xFF) & (1 << 3))
    {
      GPIO_PORTE_ICR_R |= (1 << 3);
      time[i++] = WTIMER1_TAV_R;
      WTIMER1_CTL_R &= ~TIMER_CTL_TAEN;
      WTIMER1_TAV_R = 0;
      WTIMER1_CTL_R |= TIMER_CTL_TAEN;
      if (i == (DATABITS + SETUPBITS))
	{
	  UpdateRdy = 1;
	}
    }
}


int
main (void)
{
  float temperature = 0.0f;
  float humidity = 0.0f;
  initDHT11Hw ();
  getReading (&temperature, &humidity);



#ifdef test1
  OUTPUT = 1;
  OUTPUT = 0;
#endif

  while (1);

}
