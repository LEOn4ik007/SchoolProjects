#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

template <typename T>
void print(const T * data, int size)
{
	for (int i = 0; i < size; ++i)
		std::cout << data[i];
	std::cout << std::endl;
}

template <typename T>
void bubbleSort(T * data, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (data[j] > data[j + 1])
			{
				std::swap(data[j], data[j + 1]);
			}
		}
	}
}

template <typename T>
void shakerSort(T * data, int size) 
{
	int control = size - 1;
	int left = 0;
	int right = size - 1;
	do 
	{
		for (int i = left; i < right; i++) 
		{
			if (data[i] > data[i + 1]) 
			{
				std::swap(data[i], data[i + 1]);
				control = i;
			}
		}
		right = control;
		for (int i = right; i > left; i--) 
		{
			if (data[i] < data[i - 1]) 
			{
				std::swap(data[i], data[i - 1]);
				control = i;
			}
		}
		left = control;
	} 
	while (left < right);
}

template <typename T>
void selectionSort(T * data, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		int min_index = i;
		for (int j = i + 1; j < size; j++)
		{
			if (data[j] < data[min_index])
			{
				min_index = j;
			}
		}
		if (min_index != i)
		{
			std::swap(data[i], data[min_index]);
		}
	}
}

template <typename T>
void insertionSort(T * data, int size)
{
	for (int j = 1; j < size; j++)
	{
	 	const T key = data[j];
		int i = j - 1;

		for ( ; i >= 0 && data[i] > key; i = i - 1)
			data[i + 1] = data[i];
		
		data[i + 1] = key;
	}
}

template<typename T>
void mergeSort(T * data, int size) noexcept
{
	if (size <= 1)
		return;

	int const left_size = size / 2;
	int const right_size = size - left_size;

	mergeSort(data, left_size);
	mergeSort(data + left_size, right_size);

	int lidx = 0, ridx = left_size, idx = 0;
	std::unique_ptr<T[]> tmpData(new T[size]);

	while (lidx < left_size || ridx < size)
	{
		if (data[lidx] < data[ridx])
		{
			tmpData[idx++] = data[lidx];
			lidx++;
		}
		else
		{
			tmpData[idx++] = data[ridx];
			ridx++;
		}

		if (lidx == left_size)
		{
			std::copy(data + ridx, data + size, tmpData.get() + idx);
			break;
		}
		if (ridx == size)
		{
			std::copy(data + lidx, data + left_size, tmpData.get() + idx);
			break;
		}
	}

	std::copy(tmpData.get(), tmpData.get() + size, data);
}

template<typename T>
void quickSort(T * data, int size)
{
	int i = 0;
	int j = size - 1;

	T mid = data[size / 2];

	do
	{
		
		while (data[i] < mid)
			i++;
		
		while (data[j] > mid)
			j--;
		
		if (i <= j)
		{
			std::swap(data[i], data[j]);

			i++;
			j--;
		}
	} 
	while (i <= j);

	if (j > 0)
		quickSort(data, j + 1);

	if (i < size)
		quickSort(&data[i], size - i);
}

template<typename T>
void quickSortStd(T* data, int size)
{
	std::sort(data, data + size);
}

template<typename T>
using SortFunction = void(*)(T*, int);

struct SelfCounter
{
	static size_t assignments;
	static size_t comparisons;
	
	int data;
	SelfCounter(int value = 0)
		: data(value)
	{}

	SelfCounter(const SelfCounter & rhs) noexcept
		: data(rhs.data)
	{
		++assignments;
	}

	SelfCounter(SelfCounter && rhs) noexcept
		: data(rhs.data)
	{
		++assignments;
	}

	SelfCounter & operator=(const SelfCounter & rhs) noexcept
	{
		if (this == &rhs)
			return *this;
		++assignments;
		data = rhs.data;
		return *this;
	}

	SelfCounter & operator=(SelfCounter && rhs) noexcept
	{
		if (this == &rhs)
			return *this;
		++assignments;
		data = rhs.data;
		return *this;
	}

	bool operator<(const SelfCounter & rhs) const noexcept
	{
		++comparisons;
		return data < rhs.data;
	}
	bool operator>(const SelfCounter& rhs) const noexcept
	{
		++comparisons;
		return data > rhs.data;
	}

};

size_t SelfCounter::assignments = 0;
size_t SelfCounter::comparisons = 0;

using Item = SelfCounter;

template<typename T>
struct StorageItem
{
	std::string name;
	SortFunction<T> sortFunction;
	size_t assignments{0};
	size_t comparisons{0};
	std::chrono::high_resolution_clock::duration duration{};
	std::unique_ptr<std::ofstream> stream;
};

std::random_device rd;
std::mt19937 g(rd());

int main()
{
	StorageItem<Item> storage[]
	{
		StorageItem<Item>{"Bubble", & bubbleSort},
		StorageItem<Item>{"Shaker", & shakerSort},
		StorageItem<Item>{"Selection", & selectionSort},
		StorageItem<Item>{"Insertion", & insertionSort},
		StorageItem<Item>{"Merge", & mergeSort},
		StorageItem<Item>{"Quick", & quickSort},
		StorageItem<Item>{"Quick(std)", & quickSortStd},
	};
	
	for (auto & storageItem : storage)
	{
		storageItem.stream = std::make_unique<std::ofstream>(storageItem.name + ".tsv");
		storageItem.stream->imbue(std::locale("ru_RU"));
		(*storageItem.stream) << "size\ttime\tcomparisons\tassignments\n";
		(*storageItem.stream) << storageItem.name << "\t" << storageItem.name << "\t" << storageItem.name << "\t" << storageItem.name << std::endl;
	}
	
	for (int size = 10, step = 5; size <= 100000; size += step)
	{
		if (size >= 10000)
			step = 5000;
		else if (size >= 1000)
			step = 500;
		else if (size >= 100)
			step = 50;

		std::unique_ptr<Item[]> data(new Item[size]);

		std::unique_ptr<Item[]> dataTmp(new Item[size]);

		for (int i = 0; i < size; ++i)
			data[i] = i;
		//	print(data, size);
		
		for (auto & storageItem : storage)
		{
			storageItem.assignments = 0;
			storageItem.comparisons = 0;
			storageItem.duration = {};
		}
		
		const int attempts = 10;
		for (int i = 0; i < attempts; ++i)
		{
			std::shuffle(data.get(), data.get() + size, g);
			for (auto & storageItem : storage)
			{
				std::copy(data.get(), data.get() + size, dataTmp.get());
				//print(data, size);

				SelfCounter::comparisons = 0;
				SelfCounter::assignments = 0;
				const auto start_time = std::chrono::high_resolution_clock::now();
				storageItem.sortFunction(dataTmp.get(), size);
				const auto end_time = std::chrono::high_resolution_clock::now();
				storageItem.duration += end_time - start_time;
				storageItem.comparisons += SelfCounter::comparisons;
				storageItem.assignments += SelfCounter::assignments;
			}
		}

		std::cout << "size: " << size << std::endl;
		for (const auto & storageItem : storage)
		{
			const auto time = std::chrono::duration_cast<std::chrono::microseconds>(storageItem.duration).count() / 1000.0 / attempts;
			std::cout << storageItem.name << ": " << time << " ms" << std::endl;
			(*storageItem.stream) << size << "\t" << time << "\t" << storageItem.comparisons / attempts << "\t" << storageItem.assignments / attempts << std::endl;
		}
	}
	return 0;
}
