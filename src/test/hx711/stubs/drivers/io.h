/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef unsigned IO_t;
#define OWNER_HX711_SCK 1
#define OWNER_HX711_DOUT 2
#define IOCFG_OUT_PP 1
#define IOCFG_IPU 2
IO_t IOGetByTag(uint8_t tag);
bool IOIsFreeOrPreinit(IO_t io);
void IOInit(IO_t io, unsigned owner, unsigned index);
void IOConfigGPIO(IO_t io, unsigned cfg);
void IOLo(IO_t io);
void IOHi(IO_t io);
bool IORead(IO_t io);
