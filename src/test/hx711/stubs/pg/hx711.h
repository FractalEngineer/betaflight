/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <stdint.h>
typedef struct { uint8_t sckTag, doutTag;  } hx711Config_t;
const hx711Config_t *hx711Config(void);
hx711Config_t *hx711ConfigMutable(void);
