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

#ifndef MBB_TIMER_COMMON_H
#define MBB_TIMER_COMMON_H

#include "hsm.h"

#define MTMR_NROF_TIMERS(LAST_TIMER_EVENT) (LAST_TIMER_EVENT - MHSM_EVENT_CUSTOM + 1)

#define MTMR_ONE_MSEC	(1)
#define MTMR_ONE_SEC	(1000 * MTMR_ONE_MSEC)
#define MTMR_ONE_MIN	(60 * MTMR_ONE_SEC)
#define MTMR_ONE_HOUR	(60 * MTMR_ONE_MIN)

#endif /* MBB_TIMER_COMMON_H */
