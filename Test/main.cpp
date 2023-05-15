#include "../dll/vecT.h"
#include "../static/vecT.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <ios>
#include <time.h>
#include <vector>
#pragma comment(lib, "dll.lib")
#pragma comment(lib, "static.lib")
#pragma comment(lib, "Winmm.lib")


template<typename T, size_t DIMENSION> inline
std::ostream& operator<<(std::ostream& stream, const Static::Vector<T, DIMENSION>& value)
{
	stream << "{";
	for (int i = 0; i < DIMENSION - 1; i++)
	{
		stream << value.v[i] << ",";
	}
	stream << value.v[DIMENSION - 1] << "}";
	return stream;
}
template<typename T, size_t DIMENSION> inline
std::ostream& operator<<(std::ostream& stream, const Dll::Vector<T, DIMENSION>& value)
{
	stream << "{";
	for (int i = 0; i < DIMENSION - 1; i++)
	{
		stream << value.v[i] << ",";
	}
	stream << value.v[DIMENSION - 1] << "}";
	return stream;
}



template<class TestClass>
auto Test(unsigned testNum)
{
	LONGLONG time = {};
	while (testNum > 0)
	{
		auto num = min(testNum, 1000);
		auto testData = new TestClass[num];
		for (unsigned i = 0; i < num; i++)
			for (auto& v : testData[i])
				v = ((rand()+1) % 10000) / 100.0f;

		LARGE_INTEGER start;
		LARGE_INTEGER end;
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < num; i++)
			testData[i].normalize();
		QueryPerformanceCounter(&end);

		delete[] testData;
		time += end.QuadPart - start.QuadPart;
		testNum -= num;
	}
	return time; 
}

class ResultTime
{
public:
	void add(int64_t result)
	{
		data.push_back(result);
		maxData = max(maxData, result);
		minData = min(minData, result);
	}

	int64_t average() const {
		int64_t all = 0;
		for (auto& v : data)
			all += v;
		return all / data.size();
	}

	std::vector<int64_t> data;
	int64_t maxData = INT64_MIN;
	int64_t minData = INT64_MAX;
};

int main()
{
	constexpr auto testCount = 10000000;
	constexpr auto tryCount = 20;
	timeBeginPeriod(1);
	srand((unsigned)time(nullptr));

	ResultTime staticResult;
	ResultTime dllResult;
	ResultTime dllNoExpResult;
	for (int i = 0; i < tryCount; i++)
	{
		std::cout << "Running test(" << (i+1) << "/" << tryCount << ")\n";
		std::cout << "Static..\n";
		staticResult.add(Test<Static::Vector<float, 3>>(testCount));
		std::cout << "Dll..\n";
		dllResult.add(Test<Dll::Vector<float, 3>>(testCount));
		std::cout << "Dll(NotExport)..\n";
		dllNoExpResult.add(Test<Dll_NoExport::Vector<float, 3>>(testCount));

	}
	std::cout << "done.\n\n";

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	auto toNsec = [countPerSec = freq.QuadPart](int64_t v)
	{
		return v * 1000000000 / countPerSec;
	};

	std::cout << "\nResult(Min)\n";
	std::cout << "LIB process time            : " << std::right << std::setw(10) <<toNsec(staticResult.minData) << " nsec\n";
	std::cout << "DLL process time            : " << std::right << std::setw(10) <<toNsec(dllResult.minData) << " nsec\n";
	std::cout << "DLL(NotExport) process time : " << std::right << std::setw(10) <<toNsec(dllNoExpResult.minData) << " nsec\n";

	std::cout << "\nResult(Max)\n";
	std::cout << "LIB process time            : " << std::right << std::setw(10) << toNsec(staticResult.maxData) << " nsec\n";
	std::cout << "DLL process time            : " << std::right << std::setw(10) << toNsec(dllResult.maxData) << " nsec\n";
	std::cout << "DLL(NotExport) process time : " << std::right << std::setw(10) << toNsec(dllNoExpResult.maxData) << " nsec\n";

	std::cout << "\nResult(Average)\n";
	std::cout << "LIB process time            : " << std::right << std::setw(10) << toNsec(staticResult.average()) << " nsec\n";
	std::cout << "DLL process time            : " << std::right << std::setw(10) << toNsec(dllResult.average()) << " nsec\n";
	std::cout << "DLL(NotExport) process time : " << std::right << std::setw(10) << toNsec(dllNoExpResult.average()) << " nsec\n";

	timeEndPeriod(1);

	
	return 0;
}