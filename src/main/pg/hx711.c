/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "platform.h"
#ifdef USE_HX711
#include "pg/pg.h"
#include "pg/pg_ids.h"
#include "pg/hx711.h"

// Zero defaults leave both resources unassigned.
PG_REGISTER(hx711Config_t, hx711Config, PG_HX711_CONFIG, 0);
#endif
