#include <iostream>
#include <vector>
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
		data = list;
		for (int i = data.size() / 2 - 1; i >= 0; i--)
			heapifyElem(i);
		//for (int i : list)
			//insert(i);
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

	void partHeapify(int begin, int end)
	{
		int pos = begin;
		end = end - begin;
		while (leftChildOf(pos) < end) {
			int childPos = leftChildOf(pos);
			int rightChildPos = rightChildOf(pos);
			if (rightChildPos < end && data[rightChildPos] < data[childPos])
				childPos = rightChildPos;
			if (data[pos] > data[childPos]) {
				std::swap(data[pos], data[childPos]);
			} else {
				break;
			}
			pos = childPos;
		}
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
		//int pos = data.size() - 1;
		//int parentPos = parentOf(pos);
		//while (parentPos >= 0 && data[parentPos] > data[pos]) {
		//	std::swap(data[parentPos], data[pos]);
		//	pos = parentPos;
		//	parentPos = parentOf(pos);
		//}
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


	void heapifyElem(int i)
	{
		int min = i; // Initialize minimun as root 
		int l = 2 * i + 1; // left = 2*i + 1 
		int r = 2 * i + 2; // right = 2*i + 2 
		int n = data.size();

		if (l < n && data[l] < data[min])
			min = l;

		if (r < n && data[r] < data[min])
			min = r;

		// If largest is not root 
		if (min != i) {
			std::swap(data[i], data[min]);

			// Recursively heapify the affected sub-tree 
			heapifyElem(min);
		}
	}

	template <typename F>
	void forEach(F f)
	{
		for (auto& e : data)
			f(e);
	}
};

void heapifySubTree(int* data, int i, int n)
{
	int min = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && data[l] < data[min])
		min = l;

	if (r < n && data[r] < data[min])
		min = r;

	if (min != i) {
		std::swap(data[i], data[min]);

		heapifySubTree(data, min, n);
	}
}

void heapifyRange(int* data, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapifySubTree(data, i, n);
}

template <typename T>
void heapSort(T& range)
{
	MinHeap heap(range);
	int* data = heap.data.data() + 1;
	for (int i = 1; i < range.size(); i++) {
		heapifyRange(data + i, heap.data.size() - i - 1);
	}
	range = heap.data;
}

int main()
{
	//MinHeap heap{5, 14, 23, 32, 41, 78};
	//MinHeap heap{78, 41, 32, 23, 14, 5};
	MinHeap heap{11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	heapSort(heap.data);
	heap.forEach([](int i) {std::cout << i << ' '; });

	// 1 2 5 4 3 9 6 10 7 8
	/*
	       1 
	    2      5 
	  4   3  9   6 
	10 7 8
	*/
}