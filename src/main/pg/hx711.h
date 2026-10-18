/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include "drivers/io_types.h"
#include "pg/pg.h"

typedef struct hx711Config_s {
    ioTag_t sckTag;
    ioTag_t doutTag;
} hx711Config_t;
PG_DECLARE(hx711Config_t, hx711Config);
