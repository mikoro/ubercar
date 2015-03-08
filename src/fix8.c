#include "fix8.h"

ufix8_t ufix8_add(ufix8_t u, ufix8_t v)
{
	uint32_t sum = (uint32_t)u + v;
	
	if (sum > UFIX8_MAX)
		return UFIX8_MAX;

	return sum;
}

ufix8_t ufix8_sub(ufix8_t u, ufix8_t v)
{
	if (v > u)
		return UFIX8_MIN;

	return u - v;
}

ufix8_t ufix8_mul(ufix8_t u, ufix8_t v)
{
	uint32_t product = ((uint32_t)u * v) >> 8;
	
	if (product > UFIX8_MAX)
		return UFIX8_MAX;

	return product;
}

ufix8_t ufix8_from_int(uint8_t u)
{
	return u * UFIX8_ONE;
}

uint8_t ufix8_to_int(ufix8_t u)
{
	return (u >> 8);
}

fix8_t fix8_add(fix8_t u, fix8_t v)
{
	int32_t sum = (int32_t)u + v;

	if (sum > FIX8_MAX)
		return FIX8_MAX;

	if (sum < FIX8_MIN)
		return FIX8_MIN;

	return sum;
}

fix8_t fix8_sub(fix8_t u, fix8_t v)
{
	int32_t diff = (int32_t)u - v;

	if (diff > FIX8_MAX)
		return FIX8_MAX;

	if (diff < FIX8_MIN)
		return FIX8_MIN;

	return diff;
}

fix8_t fix8_mul(fix8_t u, fix8_t v)
{
	int32_t product = ((int32_t)u * v) >> 8;

	if (product > FIX8_MAX)
		return FIX8_MAX;

	if (product < FIX8_MIN)
		return FIX8_MIN;

	return product;
}

fix8_t fix8_from_int(int8_t u)
{
	return u * FIX8_ONE;
}

int8_t fix8_to_int(fix8_t u)
{
	return (u >> 8);
}
