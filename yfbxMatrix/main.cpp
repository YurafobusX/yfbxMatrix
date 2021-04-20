#include "vld.h"
#include "matrix.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

int main() {
	yfbx::matrix<float> a(2, 2);
	a.fillUniform(1);
	a.setCell(1, 0, 2);
	std::cout << a.makeInverse() * a;
	
	return 0;
}