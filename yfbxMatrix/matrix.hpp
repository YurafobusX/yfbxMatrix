#pragma once
#include <utility>
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <list>

/*list of exceptions (std::runtime_error):
0. Out of range
1. Not the same size
2. Not the same size
3. Matrix is not square
4. Heights does not match
*/

namespace yfbx {

	template<typename T>
	class matrix {
	private:
		std::unique_ptr<T[]> data_;
		T determinant_;
		bool isDeterminantCorrect_ = false;
		size_t height_, width_;

	public:
		
		matrix(size_t width = 0, size_t heidth = 0, std::list<T> list = {});
		matrix(const matrix<T>&);
		~matrix();

		void fillWithFunction(std::function<T(size_t, size_t)>);
		void fillUniform(T);

		void swap(matrix<T>&) noexcept;

		T& getCell(size_t, size_t) const;
		void setCell(size_t, size_t, const T&);

		matrix makeTranspose() const;
		T computeDeterminant();
		matrix makeUpperTriangular() const;
		matrix makeLowerTriangular() const;
		size_t computeRang() const;
		matrix<T> makeInverse() const;
		matrix<T> solve(const matrix<T>&) const;

		template<typename D>
		friend std::ostream& operator<<(std::ostream&, const matrix<D>&);
		template<typename D>
		friend std::istream& operator>>(std::istream&, const matrix<D>&);

		bool operator==(const matrix<T>&) const;
		const matrix<T>& operator=(matrix<T>);
		matrix<T> operator+(const matrix<T>&) const;
		matrix<T> operator*(const matrix<T>&) const;
		matrix<T> operator*(const T&) const;

		// Ѕлок преобразований. 
		// »спользуем следующие их обозначени€
		// 1 тип: перестановка строк
		// 2 тип: ”множение на константу
		// 3 тип: ѕрибавление строки умноженной на константу

		void doTranformation1(size_t, size_t);
		void doTranformation2(size_t, T);
		void doTranformation3(size_t, size_t, T);
	};
}

namespace yfbx {

	template<typename T>
	matrix<T>::matrix(size_t wight, size_t height, std::list<T> list) {
		data_ = std::unique_ptr <T[]>(new T[wight * height]);
		height_ = height;
		width_ = wight;
		auto iter = list.begin();
		for (size_t i = 0; i < height_; i++)
			for (size_t j = 0; j < width_; j++)
				if (iter == list.end()) {
					setCell(j, i, {});
				}
				else {
					setCell(j, i, *iter);
					iter++;
				}
	}
	
	template<typename T>
	matrix<T>::matrix(const matrix<T>& copping) : matrix(copping.width_, copping.height_) {
		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				setCell(i, j, copping.getCell(i, j));
			}
		}

		determinant_ = copping.determinant_;
		isDeterminantCorrect_ = copping.isDeterminantCorrect_;
	}

	template<typename T>
	matrix<T>::~matrix() {
	}

	template<typename T>
	void matrix<T>::fillWithFunction(std::function<T(size_t, size_t)> func) {
		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				setCell(i, j, func(i,j));
			}
		}

		isDeterminantCorrect_ = false;
	}

	template<typename T>
	void matrix<T>::fillUniform(T element) {
		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				setCell(i, j, element);
			}
		}

		isDeterminantCorrect_ = false;
	}

	template<typename T>
	void matrix<T>::swap(matrix<T>& swapping) noexcept {
		std::swap(data_, swapping.data_);
		std::swap(width_, swapping.width_);
		std::swap(height_, swapping.height_);
		std::swap(determinant_, swapping.determinant_);
		std::swap(isDeterminantCorrect_, swapping.isDeterminantCorrect_);
	}

	template<typename T>
	T& matrix<T>::getCell(size_t x, size_t y) const{
		if (x >= width_ || y >= height_)
			throw std::runtime_error(std::string("0. Out of range in ") + __func__ + " " + __FILE__);
		return data_[x + width_ * y];
	}

	template<typename T>
	void matrix<T>::setCell(size_t x, size_t y, const T& t) {
		if (x >= width_ || y >= height_)
			throw std::runtime_error(std::string("0. Out of range in ") + __func__ + " " + __FILE__);
		data_[x + width_ * y] = t;
		
		isDeterminantCorrect_ = false;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream& ostream, const matrix<T>& in) {
		for (size_t i = 0; i < in.height_; i++) {
			for (size_t j = 0; j < in.width_; j++) {
				ostream << in.getCell(j,i) << '\t';
			}
			ostream << '\n';
		}
		return ostream;
	}

	template<typename T>
	const matrix<T>& matrix<T>::operator=(matrix<T> in) {
		swap(in);
		return *this;
	}

	template<typename T>
	bool matrix<T>::operator==(const matrix<T>& in) const {
		if (height_ != in.height_)
			return false;

		if (width_ != in.width_)
			return false;

		for (size_t i = 0; i < height_; i++) {
			for (size_t j = 0; j < width_; j++) {
				if (getCell(j, i) != in.getCell(j, i))
					return false;
			}
		}
		return true;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(const matrix<T>& in) const {
		
		if ((height_ != in.height_) || (width_ != in.width_))
			throw std::runtime_error(std::string("1. Not the same size ") + __func__ + " " + __FILE__);

		matrix<T> res(width_, height_);
		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				res.set(i, j, (getCell(i, j) + in.getCell(i, j)));
			}
		}

		return res;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const matrix<T>& in) const {

		if (width_ != in.height_)
			throw std::runtime_error(std::string("2. Not the same size ") + __func__ + " " + __FILE__);

		matrix<T> out((in.width_), height_);
		for (size_t i = 0; i < in.width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				T sum = T(0);
				for (size_t k = 0; k < width_; k++) {
					sum += (getCell(k, j) * in.getCell(i, k));
				}
				out.setCell(i, j, sum);
			}
		}
		 
		return out;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const T& t) const {
		auto res = *this;
		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				res.setCell(i, j, getCell(i, j) * t);
			}
		}
		return res;	
	}
	
	//“ранспорирует матрицу
	template<typename T>
	matrix<T> matrix<T>::makeTranspose() const {
		matrix<T> res(height_, width_);

		for (size_t i = 0; i < width_; i++) {
			for (size_t j = 0; j < height_; j++) {
				res.setCell(j, i, getCell(i, j));
			}
		}

		return res;
	}
	
	//ƒетерминант (определитель)
	template<typename T>
	T matrix<T>::computeDeterminant() {
		if (width_ != height_)
			throw std::runtime_error("3. Matrix is not square " + __func__ + " " + __FILE__);

		if (isDeterminantCorrect_)
			return determinant_;

		determinant_ = 1;
		auto triangular = makeUpperTriangular();
		for (size_t i = 0; i < width_; i++) {
			determinant_ *= triangular.getCell(i, i);
		}
		isDeterminantCorrect_ == true;
		return determinant_;
	}
	
	//ѕриведение к верхнетруегольнольному (ступенчатому виду) без изменени€ детерминанта
	template<typename T>
	matrix<T> matrix<T>::makeUpperTriangular() const{
		auto res = *this;
		for (size_t i = 0; i < std::min(width_,height_); i++) {
			size_t not_null = i;
			for (size_t j = i; j < height_; j++) {
				if (res.getCell(i, j) != 0) {
					not_null = j;
					break;
				}
			}
			res.doTranformation1(i, not_null);
			auto& norm = getCell(i, i);
			if (norm == 0) {
				continue;
			}
			norm = 1 / norm;
			for (size_t j = i + 1; j < height_; j++) {
				res.doTranformation3(j, i, -(getCell(i, j) * norm));
			}
		}
		return res;
	}
	
	//ѕриведение к нижнетруегольнольному (ступенчатому виду) без изменени€ детерминанта
	template<typename T>
	matrix<T> matrix<T>::makeLowerTriangular() const {
		auto res = *this;
		for (size_t i = std::min(width_, height_); i != 0;) {
			--i;
			size_t not_null = i;
			for (size_t j = i + 1; j != 0;) {
				--j;
				if (res.getCell(i, j) != 0) {
					not_null = j;
					break;
				}
			}
			res.doTranformation1(i, not_null);
			auto& norm = getCell(i, i);
			if (norm == 0) {
				continue;
			}
			std::cout << res << std::endl;
			norm = 1 / norm;
			for (size_t j = 0; j < i; j++) {
				res.doTranformation3(j, i, -(getCell(i, j) * norm));
			}
		}
		return res;
	}
	
	//перестановка строк
	template<typename T>
	void matrix<T>::doTranformation1(size_t first, size_t second) {
		for (size_t i = 0; i < width_; i++) {
			auto c = getCell(i, first);
			setCell(i, first, getCell(i, second));
			setCell(i, second, c);
		}
	}

	//”множение на константу
	template<typename T>
	void matrix<T>::doTranformation2(size_t line, T t) {
		isDeterminantCorrect_ = false;
		for (size_t i = 0; i < width_; i++) {
			setCell(i, line, getCell(i, line) * t);
		}
	}

	//ѕрибавление строки умноженной на константу
	template<typename T>
	void matrix<T>::doTranformation3(size_t changing, size_t second, T t) {
		for (size_t i = 0; i < width_; i++) {
			setCell(i, changing, getCell(i, changing) + getCell(i, second) * t);
		}
	}
	
	//–анг
	template<typename T>
	size_t matrix<T>::computeRang() const {
		auto triangular = makeUpperTriangular();
		size_t res = 0;
		for (size_t i = 0; i < std::min(width_, height_); i++) {
			if (triangular.getCell(i, i) != 0) {
				res++;
			};
		}
		return res;
	}

	//ќбратна€
	template<typename T>
	matrix<T> matrix<T>::makeInverse() const {
		matrix<T> one(width_, height_);
		one.fillWithFunction([](size_t x, size_t y) -> T {return (x == y) ? T(1) : T(0); });
		return solve(one);
	}

	//–ешает систему уравнений, где изначальна€ матрица - блок с переменными (сводит еЄ к единичной)
	template<typename T>
	matrix<T> matrix<T>::solve(const matrix<T>& in) const {
		if (height_ != in.height_)
			throw std::runtime_error(std::string("4. Heights does not match ") + __func__ + " " + __FILE__);

		matrix<T> temp(width_ + in.width_, height_);
		for (size_t i = 0; i < height_; i++) {
			for (size_t j = 0; j < width_ + in.width_; j++)
				temp.setCell(j, i, (j >= width_) ? (in.getCell(j - width_, i)) : (getCell(j, i)));
		}

		temp = temp.makeLowerTriangular().makeUpperTriangular();

		matrix<T> res(in.width_, in.height_);

		for (size_t i = 0; i < in.width_; i++) {
			for (size_t j = 0; j < in.height_; j++) {
				res.setCell(i, j, temp.getCell(i + width_, j) / temp.getCell(i, i));
			}
		}

		return res;
	} 	
}