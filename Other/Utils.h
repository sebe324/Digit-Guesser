#pragma once
#include <random>
#include <cmath>
namespace Utils {
	int clamp(int min, int max, int value);
	double sigmoid(double n);
	double rand_double(double min, double max);
}