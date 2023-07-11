#include "Utils.h"

int Utils::clamp(int min, int max, int value)
{
	if (value > max) return max;
	else if (value < min) return min;
	else return value;
}

double Utils::sigmoid(double n)
{
	return (n / (1 + abs(n))+1)/2;
}

double Utils::rand_double(double min, double max)
{
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}