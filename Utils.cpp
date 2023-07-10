#include "Utils.h"

int Utils::clamp(int min, int max, int value)
{
	if (value > max) return max;
	else if (value < min) return min;
	else return value;
}
