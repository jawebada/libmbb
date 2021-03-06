/* * Copyright (C) 2015 Jan Weil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MBB_TYPES_H
#define MBB_TYPES_H

/* 
 * This file is included to define the following types:
 * * fixed-width integer types uint8_t, int8_t, uint16_t, ...
 * * bool
 * * size_t
 *
 * If your system does not have <inttypes.h>, <stdbool.h>, or <stddef.h>, you
 * should define MBB_SYSTEM_TYPES_HEADER before including mbb/types.h which is
 * included below and expected to provide the type definitions listed above.
 */

#ifdef MBB_SYSTEM_TYPES_HEADER
# include MBB_SYSTEM_TYPES_HEADER
#else
# include <inttypes.h>
# include <stdbool.h>
# include <stddef.h>
#endif

#endif /* MBB_TYPES_H */
