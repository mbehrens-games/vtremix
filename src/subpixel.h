/*******************************************************************************
** subpixel.h (subpixel fixed point constants)
*******************************************************************************/

#ifndef SUBPIXEL_H
#define SUBPIXEL_H

#define SUBPIXEL_MANTISSA_NUM_BITS 4

/* the full mantissa value is (1 << MANTISSA_NUM_BITS) */
#define SUBPIXEL_MANTISSA_FULL 16
#define SUBPIXEL_MANTISSA_HALF (SUBPIXEL_MANTISSA_FULL / 2)

#endif
