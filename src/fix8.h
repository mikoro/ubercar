#ifndef CAR_FIX8_H
#define CAR_FIX8_H

#include <stdint.h>

typedef uint16_t ufix8_t;
typedef int16_t fix8_t;

#define UFIX8_ONE (uint16_t)0x0100
#define UFIX8_MAX (uint16_t)0xffff
#define UFIX8_MIN (uint16_t)0x0000
#define UF8(x) ((ufix8_t)((x) * 256.0 + 0.5))

#define FIX8_ONE (int16_t)0x0100
#define FIX8_MAX (int16_t)0x7fff
#define FIX8_MIN (int16_t)0x8000
#define F8(x) ((fix8_t)(((x) >= 0) ? ((x) * 256.0 + 0.5) : ((x) * 256.0 - 0.5)))

ufix8_t ufix8_add(ufix8_t u, ufix8_t v);
ufix8_t ufix8_sub(ufix8_t u, ufix8_t v);
ufix8_t ufix8_mul(ufix8_t u, ufix8_t v);
ufix8_t ufix8_from_int(uint8_t u);
uint8_t ufix8_to_int(ufix8_t u);

fix8_t fix8_add(fix8_t u, fix8_t v);
fix8_t fix8_sub(fix8_t u, fix8_t v);
fix8_t fix8_mul(fix8_t u, fix8_t v);
fix8_t fix8_from_int(int8_t u);
int8_t fix8_to_int(fix8_t u);

#endif
