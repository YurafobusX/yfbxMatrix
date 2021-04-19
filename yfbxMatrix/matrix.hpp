#pragma once
#include <utility>
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <memory>

/*list of exceptions (std::runtime_error):
non sync size in +
non sync size in *
rang is null in obr
differ heights in solve
*/

namespace yfb {

	template<typename T>
	class matrix {
	private:
		std::unique_ptr<T[]> data_;
		T determinant_;
		bool isDeterminantCorrect_ = false;
		size_t height_, width_;

	public:
		
		matrix(size_t in1 = 0, size_t in2 = 0, T t = 0);
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

		template<typename D>
		friend std::ostream& operator<<(std::ostream &, const matrix<D> &);
		template<typename D>
		friend std::istream& operator>>(std::istream &, const matrix<D> &);

		bool operator==(const matrix<T>&) const;
		const matrix<T>& operator=(matrix<T>);
		matrix<T> operator+(const matrix<T>&) const;
		matrix<T> operator*(const matrix<T>&) const;
		matrix<T> operator*(const T&) const;

		size_t computeRang() const;
		matrix<T> makeInverse() const;
		matrix<T> solve(const matrix<T>&) const;

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

namespace YFB {

	template<typename T>
	matrix<T>::matrix(size_t new_x, size_t new_y, T t) {
		data = std::unique_ptr <T[]>(new T[new_x * new_y]);
		x = new_x;
		y = new_y;
		fill(T);
	}

	template<typename T>
	matrix<T>::matrix(const matrix<T> & copping) : matrix(copping.x, copping.y) {
		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				set(i, j, copping.get(i, j));
			}
		}

		determinant = copping.determinant;
		isDeterminantCorrect = copping.isDeterminantCorrect;
	}

	template<typename T>
	matrix<T>::~matrix() {
	}

	template<typename T>
	void matrix<T>::fillNS(std::function<T(size_t, size_t)> func) {
		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				set(i, j, func(i,j));
			}
		}

		isDeterminantCorrect = false;
	}

	template<typename T>
	void matrix<T>::fill(T element) {
		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				set(i, j, element);
			}
		}

		isDeterminantCorrect = false;
	}

	template<typename T>
	void matrix<T>::swap(matrix<T>& swapping) noexcept {
		std::swap(data, swapping.data);
		std::swap(x, swapping.x);
		std::swap(y, swapping.y);
		std::swap(determinant, swapping.determinant);
		std::swap(isDeterminantCorrect, swapping.isDeterminantCorrect);
	}

	template<typename T>
	T& matrix<T>::get(size_t X, size_t Y) const{
		return data[X + x * Y];
	}

	template<typename T>
	void matrix<T>::set(size_t X, size_t Y, const T& t) {
		data[X + x * Y] = t;
		if (isDeterminantCorrect)
			isDeterminantCorrect = false;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & out, const matrix<T>& matrix) {
		for (size_t i = 0; i < matrix.y; i++) {
			for (size_t j = 0; j < matrix.x; j++) {
				out << matrix.get(j,i) << '\t';
			}
			out << std::endl;
		}
		return out;
	}

	template<typename T>
	const matrix<T>& matrix<T>::operator=(matrix<T> in) {
		swap(in);
		return *this;
	}

	template<typename T>
	bool matrix<T>::operator==(const matrix<T>& in) const {
		if (x != in.x)
			return false;

		if (y != in.y)
			return false;

		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				if (get(i, j) != in.get(i, j))
					return false;
			}
		}
		return true;
	}

	template<typename T>
	matrix<T> matrix<T>::operator+(const matrix<T>& in) const {
		
		if ((x != in.x) || (y != in.y))
			throw std::runtime_error("non sync size in +");

		matrix<T> out(x, y);
		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				out.set(i, j, (get(i, j) + in.get(i, j)));
			}
		}

		return out;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const matrix<T>& in) const {

		if (x != in.y)
			throw std::runtime_error("non sync size in *");

		matrix<T> out(in.x, y);
		for (size_t i = 0; i < in.x; i++) {
			for (size_t j = 0; j < y; j++) {
				T sum = T(0);
				for (size_t k = 0; k < x; k++) {
					sum += (get(k, j) * in.get(i, k));
				}
				out.set(i, j, sum);
			}
		}
		 
		return out;
	}

	template<typename T>
	matrix<T> matrix<T>::operator*(const T &) const {
		auto out = matrix<T>(this);
		/*for (size_t i = 0; i < in.x; i++) {
			for (size_t j = 0; j < y; j++) {
				T sum = T(0);
				for (size_t k = 0; k < x; k++) {
					sum += (get(k, j) * in.get(i, k));
				}
				out.set(i, j, sum);
			}
		}*/
		out.isDeterminantCorrect = false;
		return out;	
	}

	//“ранспорирует матрицу
	template<typename T>
	matrix<T> matrix<T>::tran() const {
		matrix<T> out(y, x);

		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++) {
				out.set(j, i, get(i, j));
			}
		}

		return out;
	}

	//ƒетерминант (определитель)
	template<typename T>
	T matrix<T>::det() {
		if (x != y)
			throw std::runtime_error("matrix isn't square");

		if (isDeterminantCorrect)
			return determinant;

		determinant = 1;
		auto stupen = vtrie();
		for (size_t i = 0; i < std::min(x, y); i++) {
			determinant *= stupen.get(i, i);
		}
		isDeterminantCorrect == true;
		return determinant;
	}

	//ѕриведение к верхнетруегольнольному (ступенчатому виду) без изменени€ детерминанта
	template<typename T>
	matrix<T> matrix<T>::vtrie() const{
		auto res = *this;
		for (size_t i = 0; i < std::min(x,y); i++) {
			size_t not_null = i;
			for (size_t j = i; j < y; j++) {
				if (res.get(i, j) != 0) {
					not_null = j;
					break;
				}
			}
			res.trans1(i, not_null);
			auto& norm = get(i, i);
			if (norm == 0) {
				continue;
			}
			norm = 1 / norm;
			for (size_t j = i + 1; j < y; j++) {
				res.trans3(j, i, -(get(i, j) * norm));
			}
		}
		return res;
	}

	//ѕриведение к нижнетруегольнольному (ступенчатому виду) без изменени€ детерминанта
	template<typename T>
	matrix<T> matrix<T>::ntrie() const {
		auto res = *this;
		for (size_t i = 0; i < std::min(x, y); i++) {
			size_t not_null = i;
			auto& norm = get(i, i);
			if (norm == 0)
				for (size_t j = 0; j < i; j++) {
					if (res.get(i, j) != 0) {
						not_null = j;
						break;
					}
				}
			res.trans1(i, not_null);
			if (norm == 0) {
				continue;
			}
			norm = 1 / norm;
			for (size_t j = 0; j < i; j++) {
				res.trans3(j, i, -(get(i, j) * norm));
			}
		}
		return res;
	}

	//перестановка строк
	template<typename T>
	void matrix<T>::trans1(size_t first, size_t second) {
		for (size_t i = 0; i < x; i++) {
			auto c = get(i, first);
			set(i, first, get(i, second));
			set(i, second, c);
		}
	}

	//”множение на константу
	template<typename T>
	void matrix<T>::trans2(size_t line, T con) {
		isDeterminantCorrect = false;
		for (size_t i = 0; i < x; i++) {
			set(i, line, get(i, line) * con);
		}
	}

	//ѕрибавление строки умноженной на константу
	template<typename T>
	void matrix<T>::trans3(size_t changing, size_t second, T con) {
		for (size_t i = 0; i < x; i++) {
			set(i, changing, get(i, changing) + get(i, second) * con);
		}
	}

	//–анг
	template<typename T>
	size_t matrix<T>::rang() const {
		auto stupen = vtrie();
		size_t res = 0;
		for (size_t i = 0; i < std::min(x, y); i++) {
			if (stupen.get(i, i) != 0) {
				res++;
			};
		}
		return res;
	}

	//ќбратна€
	template<typename T>
	matrix<T> matrix<T>::obr() const {
		matrix<T> temp(x, y);
		temp.fillNS([](int X, int Y) {return (X == Y) ? 1 : 0; });
		return solve(temp);
	}

	template<typename T>
	matrix<T> matrix<T>::solve(const matrix<T>& _in_) const {
		if (y != _in_.y)
			throw std::runtime_error("differ heights in solve");
		matrix<T> temp(x + _in_.x, y);
		for (size_t i = 0; i < y; i++) {
			for (size_t j = 0; j < x + _in_.x; j++)
				temp.set(j, i, (j >= x) ? (_in_.get(j - x, i)) : (get(j, i)));
		}

		temp = temp.ntrie().vtrie();

		matrix<T> res(_in_.x, y);

		for (size_t i = 0; i < _in_.x; i++) {
			for (size_t j = 0; j < y; j++) {
				res.set(i, j, temp.get(i + x, j));
			}
		}

		return res;
	}
	
}