/*
 *  QEMU model of the Goldfish framebuffer: drawing templates.
 *
 *  Copyright (c) 2014 Linaro Limited
 *
 * Based heavily on the milkymist drawing templates, which are:
 *
 *  Copyright (c) 2010 Michael Walle <michael@walle.cc>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#if BITS == 8
#define COPY_PIXEL(to, r, g, b)                    \
    do {                                           \
        *to = rgb_to_pixel8(r, g, b);              \
    } while (0)
#elif BITS == 15
#define COPY_PIXEL(to, r, g, b)                    \
    do {                                           \
        *(uint16_t *)to = rgb_to_pixel15(r, g, b); \
    } while (0)
#elif BITS == 16
#define COPY_PIXEL(to, r, g, b)                    \
    do {                                           \
        *(uint16_t *)to = rgb_to_pixel16(r, g, b); \
    } while (0)
#elif BITS == 24
#define COPY_PIXEL(to, r, g, b)                    \
    do {                                           \
        uint32_t tmp = rgb_to_pixel24(r, g, b);    \
        to[0] =         tmp & 0xff;              \
        to[1] =  (tmp >> 8) & 0xff;              \
        to[2] = (tmp >> 16) & 0xff;              \
    } while (0)
#elif BITS == 32
#define COPY_PIXEL(to, r, g, b)                    \
    do {                                           \
        *(uint32_t *)to = rgb_to_pixel32(r, g, b); \
    } while (0)
#else
#error unknown bit depth
#endif

static void glue(draw_line_, BITS)(void *opaque, uint8_t *d, const uint8_t *s,
        int width, int deststep)
{
    uint16_t rgb565;
    uint8_t r, g, b;

    while (width--) {
        rgb565 = lduw_le_p(s);
        r = ((rgb565 >> 11) & 0x1f) << 3;
        g = ((rgb565 >>  5) & 0x3f) << 2;
        b = ((rgb565 >>  0) & 0x1f) << 3;
        COPY_PIXEL(d, r, g, b);
        d += deststep;
        s += 2;
    }
}

#undef BITS
#undef COPY_PIXEL
