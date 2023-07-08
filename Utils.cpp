#include "Utils.h"

template<class T>
T Utils::clamp(T min, T max, T value)
{
	if (value > max) return max;
	else if (value < min) return min;
	else return value;
}
