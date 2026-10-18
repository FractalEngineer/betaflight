/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <stdbool.h>
#include <stdint.h>
#define HX711_CONFIGURED 1
#define HX711_FRESH 2
#define HX711_SATURATED 4

typedef struct hx711Sample_s {
    uint8_t flags;
    int32_t raw;
    uint32_t sequence;
    uint32_t ageMs;
    uint32_t timeUs;
} hx711Sample_t;
void hx711Init(void);
void hx711Update(void);
void hx711GetSample(hx711Sample_t *sample);
