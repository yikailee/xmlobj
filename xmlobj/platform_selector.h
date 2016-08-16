/*
	Platform selector header.
	Currently only implement windows platform
*/
#pragma once

#include <math.h>
#include <cmath>
#include <limits>
#include <float.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if defined(_MSC_VER)

namespace std {
	inline int isnan(double x) {
		return _isnan(x);
	}
	inline int isinf(double x) {
		return _finite(x) == 0;
	}

#define _IS_NAN(arg) std::isnan(arg)
#define _IS_INF(arg) std::isinf(arg)
#define __PLATFORM_THROW(arg) throw (arg)
}

#endif // end defined(_MSC_VER)

