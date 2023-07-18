#include "Utils.h"

int Utils::clamp(int min, int max, int value)
{
	if (value > max) return max;
	else if (value < min) return min;
	else return value;
}

double Utils::rand_double(double min, double max)
{
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}
std::vector<std::string> Utils::split(const std::string& s, char delim)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}