#include <initializer_list>
#include <math.h>
#include <iostream>
#include <assert.h>
namespace VEC_NS
{
	template<typename T, size_t DIMENSION>
	class Vector
	{
	public:
		constexpr Vector() : v{} {}

		constexpr Vector(std::initializer_list<T> init)
		{
			assert(init.size() == DIMENSION);
			auto it = init.begin();
			auto dst = &v[0];
			while (it != init.end())
				*dst++ = *it++;
		}

		template<typename T2=T>
		double length()
		{
			double sqr = 0;
			for (auto& dv : v)
				sqr += dv * dv;
			return sqrt(sqr);
		}

		Vector<T, DIMENSION> normalize()
		{
			auto len = length();

			Vector<T, DIMENSION> ret;
			for (int i = 0; i < DIMENSION; i++)
				ret.v[i] = (T)(v[i] / len);

			return ret;
		}

		T get(int i) { return v[i]; }
		T* begin() { return &v[0]; }
		T* end() { return &v[DIMENSION-1]; }

		T v[DIMENSION];
	};

}