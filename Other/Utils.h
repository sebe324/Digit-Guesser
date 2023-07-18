#pragma once
#include <random>
#include <cmath>
#include <sstream>
#include <vector>
namespace Utils {
	int clamp(int min, int max, int value);
	double rand_double(double min, double max);
	std::vector<std::string> split(const std::string& s, char delim);
}