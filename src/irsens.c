#include <avr/io.h>

#include "iomap.h"
#include "lcd.h"

#include "fixed/fix8.h"

static fix8_t avg2 = F8(127);

static const fix8_t alpha2 = F8(0.1);
static const fix8_t oneminusalpha2 = F8(0.9);

void irsens_init() {
	IRSENS_DDR = 0;
}

void irsens_update() {
	uint8_t v = ~IRSENS_PIN;
	uint8_t low = 255;
	uint8_t high = 255;
	int8_t i;

	lcd_printf(8, "IRSNS %c%c%c%c%c%c%c%c",
		   v & BIT(7) ? 'X' : 'o',
		   v & BIT(6) ? 'X' : 'o',
		   v & BIT(5) ? 'X' : 'o',
		   v & BIT(4) ? 'X' : 'o',
		   v & BIT(3) ? 'X' : 'o',
		   v & BIT(2) ? 'X' : 'o',
		   v & BIT(1) ? 'X' : 'o',
		   v & BIT(0) ? 'X' : 'o');

	for (i = 0; i < 8; ++i) {
		if (v & BIT(i)) {
			high = i;
			break;
		}
	}

	for (i = 7; i >= 0; --i) {
		if (v & BIT(i)) {
			low = i;
			break;
		}
	}

	if (low == 255 || high == 255) {
		uint8_t val = fix8_to_int(avg2);
		return;
	}

	uint8_t mappedl = low * (255 / 7);
	uint8_t mappedh = high * (255 / 7);
	uint16_t mapped = ((uint16_t)(mappedl + mappedh)) / 2;

	fix8_t xf = fix8_from_int(mapped);
	avg2 = fix8_add(fix8_mul(alpha2, xf), fix8_mul(oneminusalpha2, avg2));
	uint8_t val = fix8_to_int(avg2);

	//lcd_printf(10, "B l %u h %u           ", low, high);
	//lcd_printf(11, "M l %u h %u   ", mappedl, mappedh);
	lcd_printf(9,  "IRSNS %u    ", val);
}

uint8_t irsens_get_direction() {
	return 255 - fix8_to_int(avg2);
}

