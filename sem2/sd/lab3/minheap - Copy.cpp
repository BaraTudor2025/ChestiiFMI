#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class MinHeap {

	int leftChildOf(int i) { return i * 2 + 1; }
	int rightChildOf(int i) { return i * 2 + 2; }
	bool hasLeftChild(int i) { return leftChildOf(i) < data.size(); }
	bool hasRightChild(int i) { return rightChildOf(i) < data.size(); }
	int parentOf(int i) { return (i - 1) / 2; }

public:
	std::vector<int> data;

public:

	MinHeap() = default;

	MinHeap(std::initializer_list<int> list)
	{
		for (int i : list)
			insert(i);
	}

	template <typename T>
	MinHeap(const T& range)
	{
		for (int i : range)
			insert(i);
	}

	int extractMin()
	{
		int min = data[0];
		data[0] = data.back();
		data.pop_back();
		heapifyDown();
		return min;
	}

	void heapifyDown(int begin=0)
	{
		int pos = begin;
		while (hasLeftChild(pos)) {
			int childPos = leftChildOf(pos);
			int rightChildPos = rightChildOf(pos);
			if (rightChildPos < data.size() && data[rightChildPos] < data[childPos])
				childPos = rightChildPos;
			if (data[pos] > data[childPos]) {
				std::swap(data[pos], data[childPos]);
			} else {
				break;
			}
			pos = childPos;
		}
	}

	void insert(int x)
	{
		data.push_back(x);
		heapifyUp(data.size() - 1);
	}

	void heapifyUp(int end)
	{
		int pos = end;
		int parentPos = parentOf(pos);
		while (parentPos >= 0 && data[parentPos] > data[pos]) {
			std::swap(data[parentPos], data[pos]);
			pos = parentPos;
			parentPos = parentOf(pos);
		}
	}

	template <typename F>
	void forEach(F f)
	{
		for (auto& e : data)
			f(e);
	}
};

template <typename T>
void heapSort(T& range)
{
	MinHeap heap(range);
	for (int& i : range)
		i = heap.extractMin();
}

void countSort(std::vector<int>& vec)
{
	int max = *std::max_element(vec.begin(), vec.end());
	std::vector<int> count(max + 1, 0);
	std::vector<int> out(vec.size());

	for (int i = 0; i < vec.size(); i++)
		count[vec[i]] += 1;

	for (int i = 1; i <= max; i++)
		count[i] += count[i - 1];

	for (int i = vec.size() - 1; i >= 0; i--) {
		count[vec[i]] -= 1;
		out[count[vec[i]]] = vec[i];
	}
	vec = out;
}

int trans(int i, int mask)
{
	// pentru cifra
	// return (i % (int)std::powl(10, r)) / std::powl(10, r-1);
	return i & mask;
}

void radixSortImpl(std::vector<int>& vec, int mask)
{
	int max = *std::max_element(vec.begin(), vec.end());
	std::vector<int> count(max + 1, 0);
	std::vector<int> out(vec.size());

	for (int i = 0; i < vec.size(); i++)
		count[trans(vec[i], mask)] += 1;

	for (int i = 1; i <= max; i++)
		count[i] += count[i - 1];

	for (int i = vec.size() - 1; i >= 0; i--) {
		count[trans(vec[i], mask)] -= 1;
		out[count[trans(vec[i], mask)]] = vec[i];
	}
	vec = out;
}

void radixSort(std::vector<int>& vec)
{
	/*
	// sort cu ultima cifra
	int cif=0;
	for (int num = max; num != 0; num /= 10)
		cif++;
	for (int i = 1; i <= cif; i++)
		radixSortImpl(vec, i);
	*/
	int max = *std::max_element(vec.begin(), vec.end());
	int pow2 = 1;
	for(; max != 0; max = max >> 4)
		pow2 = pow2 << 4;
	// daca vreau pe 8 biti, schimb 4 in 8 si i=255
	for(int mask=15; mask < pow2; mask = mask << 4)
		radixSortImpl(vec, mask);
}

int main()
{
	MinHeap heap{11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	heap.forEach([](int i) {std::cout << i << ' '; });
	std::cout << '\n';
	std::cout << "min:" << heap.extractMin();
	std::cout << '\n';
	heapSort(heap.data);
	heap.forEach([](int i) {std::cout << i << ' '; });

	std::cout << '\n';
	std::vector<int> vec{11, 111, 103, 989, 74, 7, 5, 5, 2, 1};
	radixSort(vec);
	for (int i : vec)
		std::printf("%d, ", i);


	// 1 2 5 4 3 9 6 10 7 8
	/*
	       1 
	    2      5 
	  4   3  9   6 
	10 7 8
	*/
}