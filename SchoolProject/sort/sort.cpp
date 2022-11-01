#include <iostream>
#include <chrono>
#include <random>

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
			tmpData[idx++] = std::move(data[lidx]);
			lidx++;
		}
		else
		{
			tmpData[idx++] = std::move(data[ridx]);
			ridx++;
		}

		if (lidx == left_size)
		{
			std::copy(std::make_move_iterator(data + ridx), std::make_move_iterator(data + size), tmpData.get() + idx);
			break;
		}
		if (ridx == size)
		{
			std::copy(std::make_move_iterator(data + lidx), std::make_move_iterator(data + left_size), tmpData.get() + idx);
			break;
		}
	}

	std::copy(std::make_move_iterator(tmpData.get()), std::make_move_iterator(tmpData.get() + size), data);

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

using Item = int;

template<typename T>
struct StorageItem
{
	std::string name;
	SortFunction<T> sortFunction;
	std::chrono::high_resolution_clock::duration duration{};
};

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
		StorageItem<Item>{"Quick(std)",& quickSortStd},
	};

	const int size = 100000;
	std::unique_ptr<Item[]> data(new Item[size]);
	
	std::unique_ptr<Item[]> dataTmp(new Item[size]);

	for (int i = 0; i < size; ++i)
		data[i] = i;
//	print(data, size);
	std::random_device rd;
	std::mt19937 g(rd());

	const int attempts = 1;

	for (int i = 0; i < attempts; ++i)
	{
		std::shuffle(data.get(), data.get() + size, g);
		for (auto& storageItem : storage)
		{
			std::copy(data.get(), data.get() + size, dataTmp.get());
			//print(data, size);

			const auto start_time = std::chrono::high_resolution_clock::now();
			storageItem.sortFunction(dataTmp.get(), size);
			const auto end_time = std::chrono::high_resolution_clock::now();
			storageItem.duration += end_time - start_time;

		}
	}
	
	std::cout << "size: " << size << std::endl;
	for (const auto& storageItem : storage)
		std::cout << storageItem.name << ": " << std::chrono::duration_cast<std::chrono::microseconds>(storageItem.duration).count() / 1000.0 / attempts << " ms" << std::endl;
	return 0;
}
