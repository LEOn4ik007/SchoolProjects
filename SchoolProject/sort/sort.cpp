#include <iostream>
#include <chrono>
#include <random>

void print(const int * data, int size)
{
	for (int i = 0; i < size; ++i)
		std::cout << data[i];
	std::cout << std::endl;
}

void bubbleSort(int* data, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (data[j] > data[j + 1])
			{
				int temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
			}
		}
	}
}

int main()
{
	const int size = 10000;
	int * data = new int[size];
	
	for (int i = 0; i < size; ++i)
		data[i] = i;
//	print(data, size);
	std::random_device rd;
	std::mt19937 g(rd());

	const int attempts = 100;
	std::chrono::system_clock::duration duration;

	for (int i = 0; i < attempts; ++i)
	{
		std::shuffle(data, data + size, g);
		//	print(data, size);

		const auto start_time = std::chrono::system_clock::now();
		bubbleSort(data, size);
		const auto end_time = std::chrono::system_clock::now();
		duration += end_time - start_time;
	}
	
//	print(data, size);
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1000.0 / attempts << " ms" << std::endl;
	return 0;
}
