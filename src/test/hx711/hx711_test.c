/* SPDX-License-Identifier: GPL-3.0-or-later */
// Executes the production driver with simulated GPIO and monotonic time.
#include <assert.h>
#include <stdio.h>
#include "platform.h"
#include "drivers/io.h"
#include "drivers/hx711.h"
#include "pg/hx711.h"

uint32_t testPrimask;
static uint32_t now, bits;
static unsigned pulses, reads, owners[3];
static bool ready, high;
static hx711Config_t config = { 1, 2 };
uint32_t millis(void) { return now; }
uint32_t micros(void) { return now * 1000; }
void delayMicroseconds(uint32_t us) { assert(us == 1); if (high) assert(testPrimask == 1); }
const hx711Config_t *hx711Config(void) { return &config; }
IO_t IOGetByTag(uint8_t tag) { return tag < 3 ? tag : 0; }
bool IOIsFreeOrPreinit(IO_t io) { return !owners[io]; }
void IOInit(IO_t io, unsigned owner, unsigned index) { (void)index; owners[io] = owner; }
void IOConfigGPIO(IO_t io, unsigned cfg) { (void)io; (void)cfg; }
void IOHi(IO_t io) { assert(io == 1 && testPrimask == 1); high = true; pulses++; }
void IOLo(IO_t io) { assert(io == 1); high = false; if (pulses == 25) ready = false; }
bool IORead(IO_t io) {
    assert(io == 2);
    if (!high) return !ready;
    assert(reads < 24);
    return !!(bits & (1u << (23 - reads++)));
}
static hx711Sample_t sample(void) { hx711Sample_t s; hx711GetSample(&s); return s; }
static void feed(int32_t value) {
    const uint32_t primask = testPrimask;
    bits = value & 0xffffff; ready = true; pulses = reads = 0;
    hx711Update();
    assert(pulses == 25 && reads == 24 && !high && testPrimask == primask);
    assert(sample().raw == value);
}
int main(void) {
    config.sckTag = 0; hx711Init(); assert(!(sample().flags & HX711_CONFIGURED));
    config.sckTag = config.doutTag = 1; hx711Init(); assert(!(sample().flags & HX711_CONFIGURED));
    config.doutTag = 2; owners[1] = 99; hx711Init(); assert(!(sample().flags & HX711_CONFIGURED));
    owners[1] = 0; hx711Init();
    assert(sample().flags == HX711_CONFIGURED && sample().ageMs == UINT32_MAX);
    hx711Update(); assert(pulses == 0); // Not-ready reads never generate clocks.
    now = 100; feed(0); assert(sample().sequence == 1 && sample().timeUs == 100000);
    feed(-1); feed(-8388608); assert(sample().flags & HX711_SATURATED);
    feed(8388607); assert(sample().flags & HX711_SATURATED);
    feed(-42000); assert(!(sample().flags & HX711_SATURATED));
    now += 250; assert(sample().flags & HX711_FRESH);
    now++; assert(!(sample().flags & HX711_FRESH));
    const uint32_t sequence = sample().sequence;
    for (unsigned i = 0; i < 80; i++) { now += 13; feed(100 + i); }
    for (unsigned i = 0; i < 10; i++) { now += 100; feed(-100 - (int)i); }
    assert(sample().sequence == sequence + 90);
    now = UINT32_MAX - 100; feed(10); now = 149; assert(sample().ageMs == 250 && (sample().flags & HX711_FRESH));
    now++; assert(!(sample().flags & HX711_FRESH));
    testPrimask = 1; feed(42); assert(testPrimask == 1);
    puts("PASS: HX711 readiness, 24-bit polarity, 25 pulses, IRQ protection, resources, rates and freshness/wrap");
    return 0;
}
