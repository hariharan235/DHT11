/**
 * Project         : DHT11 header file
 * Filename        : dht11.h
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


#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>
#include <stdbool.h>

#include "tm4c123gh6pm.h"

#define DATA (*((volatile uint32_t *)(0x42000000 + (0x400243FC-0x40000000)*32 + 3*4))) //PE3

#define LOW 3200  //  LOW x = Clock speed * 80 millseconds ; TODO : Need to be generic based on system clock i.e  Here x is 3200 ( = 40 MHz clock * 80 millisconds)
#define DATABITS 40
#define SETUPBITS 3
#define RESOLUTION 16

#define INTHMDSTRT 0
#define INTHMDEND 7
#define DECHMDSTRT 8
#define DECHMDEND 15
#define INTTEMPSTRT 16
#define INTTEMPEND 23
#define DECTEMPSTRT 24
#define DECTEMPEND 31
#define PARITYSTRT 32
#define PARITYEND 39

#endif /* DHT11_H_ */
