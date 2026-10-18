/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <stdbool.h>
#include <stdint.h>
extern uint32_t testPrimask;
static inline uint32_t __get_PRIMASK(void) { return testPrimask; }
static inline void __disable_irq(void) { testPrimask=1; }
static inline void __set_PRIMASK(uint32_t value) { testPrimask=value; }
