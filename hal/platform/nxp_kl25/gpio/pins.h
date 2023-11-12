/**
* \file            gpio_platform_specific.h
* \brief           Include file with peripheral specific settings for the GPIO module
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#ifndef PINS_H
#define PINS_H

#define GPIO_PIN_A0 0x100
#define GPIO_PIN_A1 0x101
#define GPIO_PIN_A2 0x102
#define GPIO_PIN_A3 0x103
#define GPIO_PIN_A4 0x104
#define GPIO_PIN_A5 0x105
#define GPIO_PIN_A6 0x106
#define GPIO_PIN_A7 0x107
#define GPIO_PIN_A8 0x108
#define GPIO_PIN_A9 0x109
#define GPIO_PIN_A10 0x10A
#define GPIO_PIN_A11 0x10B
#define GPIO_PIN_A12 0x10C
#define GPIO_PIN_A13 0x10D
#define GPIO_PIN_A14 0x10E
#define GPIO_PIN_A15 0x10F
#define GPIO_PIN_A16 0x110
#define GPIO_PIN_A17 0x111
#define GPIO_PIN_A18 0x112
#define GPIO_PIN_A19 0x113
#define GPIO_PIN_A20 0x114
#define GPIO_PIN_A21 0x115
#define GPIO_PIN_A22 0x116
#define GPIO_PIN_A23 0x117
#define GPIO_PIN_A24 0x118
#define GPIO_PIN_A25 0x119
#define GPIO_PIN_A26 0x11A
#define GPIO_PIN_A27 0x11B
#define GPIO_PIN_A28 0x11C
#define GPIO_PIN_A29 0x11D
#define GPIO_PIN_A30 0x11E
#define GPIO_PIN_A31 0x11F

#if FSL_FEATURE_SOC_PORT_COUNT >= 2
#define GPIO_PIN_B0 0x200
#define GPIO_PIN_B1 0x201
#define GPIO_PIN_B2 0x202
#define GPIO_PIN_B3 0x203
#define GPIO_PIN_B4 0x204
#define GPIO_PIN_B5 0x205
#define GPIO_PIN_B6 0x206
#define GPIO_PIN_B7 0x207
#define GPIO_PIN_B8 0x208
#define GPIO_PIN_B9 0x209
#define GPIO_PIN_B10 0x20A
#define GPIO_PIN_B11 0x20B
#define GPIO_PIN_B12 0x20C
#define GPIO_PIN_B13 0x20D
#define GPIO_PIN_B14 0x20E
#define GPIO_PIN_B15 0x20F
#define GPIO_PIN_B16 0x210
#define GPIO_PIN_B17 0x211
#define GPIO_PIN_B18 0x212
#define GPIO_PIN_B19 0x213
#define GPIO_PIN_B20 0x214
#define GPIO_PIN_B21 0x215
#define GPIO_PIN_B22 0x216
#define GPIO_PIN_B23 0x217
#define GPIO_PIN_B24 0x218
#define GPIO_PIN_B25 0x219
#define GPIO_PIN_B26 0x21A
#define GPIO_PIN_B27 0x21B
#define GPIO_PIN_B28 0x21C
#define GPIO_PIN_B29 0x21D
#define GPIO_PIN_B30 0x21E
#define GPIO_PIN_B31 0x21F
#endif

#if FSL_FEATURE_SOC_PORT_COUNT >= 3
#define GPIO_PIN_C0 0x300
#define GPIO_PIN_C1 0x301
#define GPIO_PIN_C2 0x302
#define GPIO_PIN_C3 0x303
#define GPIO_PIN_C4 0x304
#define GPIO_PIN_C5 0x305
#define GPIO_PIN_C6 0x306
#define GPIO_PIN_C7 0x307
#define GPIO_PIN_C8 0x308
#define GPIO_PIN_C9 0x309
#define GPIO_PIN_C10 0x30A
#define GPIO_PIN_C11 0x30B
#define GPIO_PIN_C12 0x30C
#define GPIO_PIN_C13 0x30D
#define GPIO_PIN_C14 0x30E
#define GPIO_PIN_C15 0x30F
#define GPIO_PIN_C16 0x310
#define GPIO_PIN_C17 0x311
#define GPIO_PIN_C18 0x312
#define GPIO_PIN_C19 0x313
#define GPIO_PIN_C20 0x314
#define GPIO_PIN_C21 0x315
#define GPIO_PIN_C22 0x316
#define GPIO_PIN_C23 0x317
#define GPIO_PIN_C24 0x318
#define GPIO_PIN_C25 0x319
#define GPIO_PIN_C26 0x31A
#define GPIO_PIN_C27 0x31B
#define GPIO_PIN_C28 0x31C
#define GPIO_PIN_C29 0x31D
#define GPIO_PIN_C30 0x31E
#define GPIO_PIN_C31 0x31F
# endif

#if FSL_FEATURE_SOC_PORT_COUNT >= 4
#define GPIO_PIN_D0 0x400
#define GPIO_PIN_D1 0x401
#define GPIO_PIN_D2 0x402
#define GPIO_PIN_D3 0x403
#define GPIO_PIN_D4 0x404
#define GPIO_PIN_D5 0x405
#define GPIO_PIN_D6 0x406
#define GPIO_PIN_D7 0x407
#define GPIO_PIN_D8 0x408
#define GPIO_PIN_D9 0x409
#define GPIO_PIN_D10 0x40A
#define GPIO_PIN_D11 0x40B
#define GPIO_PIN_D12 0x40C
#define GPIO_PIN_D13 0x40D
#define GPIO_PIN_D14 0x40E
#define GPIO_PIN_D15 0x40F
#define GPIO_PIN_D16 0x410
#define GPIO_PIN_D17 0x411
#define GPIO_PIN_D18 0x412
#define GPIO_PIN_D19 0x413
#define GPIO_PIN_D20 0x414
#define GPIO_PIN_D21 0x415
#define GPIO_PIN_D22 0x416
#define GPIO_PIN_D23 0x417
#define GPIO_PIN_D24 0x418
#define GPIO_PIN_D25 0x419
#define GPIO_PIN_D26 0x41A
#define GPIO_PIN_D27 0x41B
#define GPIO_PIN_D28 0x41C
#define GPIO_PIN_D29 0x41D
#define GPIO_PIN_D30 0x41E
#define GPIO_PIN_D31 0x41F
# endif

#if FSL_FEATURE_SOC_PORT_COUNT >= 5
#define GPIO_PIN_E0 0x500
#define GPIO_PIN_E1 0x501
#define GPIO_PIN_E2 0x502
#define GPIO_PIN_E3 0x503
#define GPIO_PIN_E4 0x504
#define GPIO_PIN_E5 0x505
#define GPIO_PIN_E6 0x506
#define GPIO_PIN_E7 0x507
#define GPIO_PIN_E8 0x508
#define GPIO_PIN_E9 0x509
#define GPIO_PIN_E10 0x50A
#define GPIO_PIN_E11 0x50B
#define GPIO_PIN_E12 0x50C
#define GPIO_PIN_E13 0x50D
#define GPIO_PIN_E14 0x50E
#define GPIO_PIN_E15 0x50F
#define GPIO_PIN_E16 0x510
#define GPIO_PIN_E17 0x511
#define GPIO_PIN_E18 0x512
#define GPIO_PIN_E19 0x513
#define GPIO_PIN_E20 0x514
#define GPIO_PIN_E21 0x515
#define GPIO_PIN_E22 0x516
#define GPIO_PIN_E23 0x517
#define GPIO_PIN_E24 0x518
#define GPIO_PIN_E25 0x519
#define GPIO_PIN_E26 0x51A
#define GPIO_PIN_E27 0x51B
#define GPIO_PIN_E28 0x51C
#define GPIO_PIN_E29 0x51D
#define GPIO_PIN_E30 0x51E
#define GPIO_PIN_E31 0x51F
# endif

#endif