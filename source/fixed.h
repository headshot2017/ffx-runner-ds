#ifndef FIXED_H_INCLUDED
#define FIXED_H_INCLUDED

#define SCALE_VERTICES 32
#define DEGREES_IN_CIRCLE (1 << 15)

#define itof32(n)         ((n << 12)) ///< Convert int to f32
#define f32toi(n)         ((n >> 12)) ///< Convert f32 to int
#define ftof32(n)         ((int)((n) * (1 << 12))) ///< Convert float to f32
#define f32tof(n)         (((float)(n)) / (float)(1 << 12)) ///< Convert f32 to float

#ifdef __NDS__
#include <nds/arm9/math.h>
#include <nds/arm9/trig_lut.h>
#else
#include <math.h>
#include <stdint.h>
static inline int32_t mulf32(int32_t a, int32_t b)
{
    int64_t result = (int64_t)a * (int64_t)b;
    return (int32_t)(result >> 12);
}

static inline int16_t sinLerp(int16_t angle)
{
	float angleRad = (angle * (M_PI*2) / DEGREES_IN_CIRCLE);
	float result = sinf(angleRad);
	return (int16_t)(ftof32(result));
}

static inline int16_t cosLerp(int16_t angle)
{
	float angleRad = (angle * (M_PI*2) / DEGREES_IN_CIRCLE);
	float result = cosf(angleRad);
	return (int16_t)(ftof32(result));
}
#endif // __NDS__

#endif // FIXED_H_INCLUDED
