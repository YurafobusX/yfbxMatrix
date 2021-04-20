#include "vld.h"
#include "matrix.hpp"
#include <iostream>
#include <vector>

int main() {
	/*yfbx::matrix<float> a(2, 2);
	a.fillUniform(1);
	a.setCell(1, 0, 2);
	std::cout << a.makeInverse() * a;*/

	yfbx::matrix<float> c(2, 2,
		{ 1, 1,
		1, 2 });
	yfbx::matrix<float> b(2, 2,
		{ -1, 0,
		1, 1 });
	std::cout << c.makeLowerTriangular();
	return 0;
}