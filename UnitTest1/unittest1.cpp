#include "stdafx.h"
#include "CppUnitTest.h"

using namespace yfbx;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest1
{		
	TEST_CLASS(defMethods)
	{
	public:
		
		TEST_METHOD(main1)
		{
			matrix<float> a = (0, 0);
			try {
				a.getCell(1, 1);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}
			try {
				a.getCell(0, 1);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}
			try {
				a.getCell(0, 0);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}
			try {
				a.setCell(1, 1, 0);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}
			try {
				a.setCell(0, 1, 0);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}
			try {
				a.setCell(0, 0, 0);
			}
			catch (const std::exception& exc) {
				Assert::IsTrue(exc.what()[0] == '0');
			}

			a = matrix<float>(2, 2, { 0, 1, 2, 3 });
			Assert::IsTrue(a.getCell(0, 0) == 0);
			Assert::IsTrue(a.getCell(1, 0) == 1);
			Assert::IsTrue(a.getCell(0, 1) == 2);
			Assert::IsTrue(a.getCell(1, 1) == 3);

			auto b = matrix<float>(2, 2, { 0, 1, 1, 3 });
			Assert::IsTrue(b.getCell(0, 0) == 0);
			Assert::IsTrue(b.getCell(1, 0) == 1);
			Assert::IsTrue(b.getCell(0, 1) == 1);
			Assert::IsTrue(b.getCell(1, 1) == 3);

			b.setCell(0, 1, 2);
			Assert::IsTrue(b.getCell(0, 1) == 2);

			Assert::IsTrue(a == b);

			auto c = matrix<std::vector<float>>(2, 2);
			auto d = matrix<std::list<float>>(2, 2);
		}

	};

	TEST_CLASS(Tranformation) {

		TEST_METHOD(one) {

			matrix<float> a(4, 3, 
				{1, 2, 3, 4,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			matrix<float> b(4, 3,
				{ 1, 1, 1, 3,
				1, 2, 3, 4,
				1, 1, 1, 1 });
			a.doTranformation1(0, 1);
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(tho) {

			matrix<float> a(4, 3,
				{ 1, 2, 3, 4,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			matrix<float> b(4, 3,
				{ 2, 4, 6, 8,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			a.doTranformation2(0, 2);
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(three) {

			matrix<float> a(4, 3,
				{ 1, 2, 3, 4,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			matrix<float> b(4, 3,
				{ 0, 1, 2, 1,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			a.doTranformation3(0, 1, 1);
			Assert::IsTrue(a == b);
		}
	};

	TEST_CLASS(algo) {

		TEST_METHOD(makeTranspose) {
			matrix<float> a(4, 3,
				{ 1, 2, 3, 4,
				1, 1, 1, 3,
				1, 1, 1, 1 });
			matrix<float> b(3, 4,
				{ 1, 1, 1,
				2, 1, 1,
				3, 1, 1,
				4, 3, 1});
			Assert::IsTrue(a.makeTranspose() == b);
		}

		TEST_METHOD(makeUpperTriangular) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1});
			matrix<float> b(2, 2,
				{ 1, 2,
				0, -1});
			Assert::IsTrue(a.makeUpperTriangular() == b);
		}

		TEST_METHOD(makeLowerTriangular) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1 });
			matrix<float> b(2, 2,
				{ -1, 0,
				1, 1 });
			Assert::IsTrue(a.makeLowerTriangular() == b);
		}

		TEST_METHOD(LandU) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1 });
			matrix<float> b(2, 2,
				{ -1, 0,
				0, 1 });
			matrix<float> c(2, 2,
				{ 1, 0,
				0, -1 });
			Assert::IsTrue(a.makeLowerTriangular().makeUpperTriangular() == b);
			Assert::IsTrue(a.makeUpperTriangular().makeLowerTriangular() == c);
		}

		TEST_METHOD(Rang) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1 });
			matrix<float> b(2, 2,
				{ -1, 0,
				1, 1 });
			Assert::IsTrue(a.computeRang() == 2);
		}

		TEST_METHOD(solve) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1 });
			matrix<float> b(1, 2,
				{ 1, 1});
			matrix<float> c(1, 2,
				{ 1, 0 });
			Assert::IsTrue(a.solve(b) == c);
		}

		TEST_METHOD(makeInverse) {
			matrix<float> a(2, 2,
				{ 1, 2,
				1, 1 });
			matrix<float> b(2, 2,
				{ -1, 2,
				1, -1 });
			Assert::IsTrue(a.makeInverse() == b);
		}
	};
}