//#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;


class MyTimer
{
public:
	MyTimer() {}
	~MyTimer() {}
	void startTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	double stopTimer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		double result = duration.count();
		return result;
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
};


void task_1();
void task_2();


int main()
{
	srand(time(NULL));
	
	task_1();
	task_2();

	system("pause");
	return 0;
}
bool IsLessThan(int i) {
	return i <= 12;
}


void task_1()
{
	constexpr int m = 8, n = 12;
	vector<vector<int>> matrix(m, vector<int>(n));
	vector<int> paralelleResult(m), monoThreadedResult(m);
	int maxInLessThanNums = 0;
	string text;
	MyTimer timer;

	for (int i = 0; i < m; i++)
		generate(matrix[i].begin(), matrix[i].end(), []() { return rand() % 100; });


	/////////////////////////////////////////////////////////////////////////////////////////////PARALLEL SECTION
	#pragma omp parallel num_threads(m) shared(matrix, paralelleResult) private(maxInLessThanNums, text)
	{
		#pragma omp single 
		{
			text = "\nThread " + to_string(omp_get_thread_num()) + "(Sasha Yanchuk, var-25)\n";
			cout << text.data();
		}
	}

	timer.startTimer();
	#pragma omp parallel num_threads(m) shared(matrix, paralelleResult) private(maxInLessThanNums, text)	
	{
		//#pragma omp for schedule(guided, 6)
		#pragma omp for schedule(dynamic, 4)
		for (int i = 0; i < m; i++)
		{
			std::vector<int> results;
			std::copy_if(matrix[i].begin(), matrix[i].end(), std::back_inserter(results), [](int a) { return a < 12; });
			vector<int>::iterator it = max_element(results.begin(), results.end());

			if (it != results.end()) {
				maxInLessThanNums = *it;
			}
			else
			{
				maxInLessThanNums = -1;
			}

			paralelleResult[i] = maxInLessThanNums;

			cout << "Max element in row " << i << " is " << maxInLessThanNums;

			text = "\nThread: " + to_string(omp_get_thread_num()) + " has worked with row #" + to_string(i) + ". Result:" + to_string(paralelleResult[i]);
			cout << text.data();
		}
	}
	cout << "\nParallel time: " << timer.stopTimer();



	///////////////////////////////////////////////////////////////////////////////////////////////////NOT PARALLEL SECTION
	timer.startTimer();
	cout << "\n\nMain thread: result: ";
	for (int i = 0; i < m; i++)
	{
		std::vector<int> results;
		std::copy_if(matrix[i].begin(), matrix[i].end(), std::back_inserter(results), [](int a) { return a < 12; });
		vector<int>::iterator it = max_element(results.begin(), results.end());

		if (it != results.end()) {
			maxInLessThanNums = *it;
		}
		else
		{
			maxInLessThanNums = -1;
		}
	
		monoThreadedResult[i] = maxInLessThanNums;

		cout << "Max element in row " << i << " is " << maxInLessThanNums;

		cout << monoThreadedResult[i] << " ";
	}
	cout << "\nMono-thread time: " << timer.stopTimer() << endl;
}



int quadratic_func(int x, int y)
{ 
	return x * x;
}


void task_2()
{
	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				constexpr int sizeOfArr = 60;
				vector<int> arrC(sizeOfArr);

				generate(arrC.begin(), arrC.end(), []() { return rand() % 100 - 50; });
				cout << "\nArray: ";
				copy(arrC.begin(), arrC.end(), ostream_iterator<int>(cout, " "));

				std::vector<int> results;
				std::copy_if(arrC.begin(), arrC.end(), std::back_inserter(results), [](int a) { return a % 7 == 0; });
				int sum = accumulate(results.begin(), results.end(), 0);
				cout << "\nSum of elements that match condition is  = " << sum << endl;
			}
			#pragma omp section
			{
				constexpr int sizeOfArr = 50;
				vector<int> arr(sizeOfArr);

				generate(arr.begin(), arr.end(), []() { return rand() % 100 - 50; });
				cout << "\nArray: ";
				copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " "));

				std::vector<int> results;
				std::copy_if(arr.begin(), arr.end(), std::back_inserter(results), [](int a) { return a % 2 == 0; });
				int sum = accumulate(results.begin(), results.end(), 0, quadratic_func);
				cout << "\nSum of elements that match condition is  = " << sum << endl;
			}
		}
	}
}
