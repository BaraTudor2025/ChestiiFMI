// bct141_tema1.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <fstream>

class vector {

private:
	int size;
	int* data;

public:
	vector() : size(0), data(nullptr) { }

	vector(int val, int size) :size(size), data(new int[size])
	{
		for (int i = 0; i < size; i++)
			data[i] = val;
	}

	vector(const vector& v) : size(v.size), data(new int[size])
	{
		for (int i = 0; i < size; i++)
			data[i] = v.data[i];
	}

	vector& operator=(const vector& v)
	{
		if (size < v.size){
			delete[] data;
			data = new int[v.size];
		}
		size = v.size;
		for (int i = 0; i < size; i++)
			data[i] = v.data[i];
		return *this;
	}	
	
	~vector()
	{
		if (data)
			delete[] data;
		data = nullptr;
	}

	void resize_fill(int val, int newSize) 
	{
		if (size < newSize){
			delete[] data;
			data = new int[newSize];
		}
		size = newSize;
		for (int i = 0; i < newSize; i++)
			data[i] = val;
	}

	std::pair<int, int> max()
	{
		int max = data[0];
		int pos = 0;
		for (int i = 0; i < size; i++){
			if (data[i] > max){
				max = data[i];
				pos = i;
			}
		}
		return{ max, pos };
	}

	void sort()
	{
		std::sort(data, data + size);
	}

	int suma()
	{
		int sum = 0;
		for (int i = 0; i < size; i++)
			sum += data[i];
		return sum;
	}

	friend int operator*(const vector& v1, const vector& v2)
	{
		if (v1.size != v2.size)
			return 0;
		int prod = 0;
		for (int i = 0; i < v1.size; i++)
			prod += v1.data[i] * v2.data[i];
		return prod;
	}

	friend std::istream& operator>>(std::istream& in, vector& v)
	{
		int size, val;
		//std::cout << "size: ";
		in >> size;

		//std::cout << "vals: ";

		v.resize_fill(0, size);

		for (int i = 0; i < v.size; i++){
			in >> val;
			v.data[i] = val;
		}
		return in;
	}

	friend std::ostream& operator<<(std::ostream& os, const vector& v)
	{
		for (int i = 0; i < v.size; i++)
			os << v.data[i] << ' ';
		return os;
	}

	friend class vector_de_vectori;
};

class vector_de_vectori {

private:
	int dim;
	vector* v;

public:
	vector_de_vectori() : dim(0), v(nullptr) { }

	vector_de_vectori(int val, int size) : dim(size), v(new vector[size]) 
	{
		for (int i = 0; i < size; i++)
			v[i].resize_fill(val, size);
	}

	//vector_de_vectori(std::initializer_list<std::initializer_list<int>> list):dim(list.size()) {}

	vector_de_vectori(const vector_de_vectori& other) : dim(other.dim), v(new vector[dim])
	{
		for (int i = 0; i < dim; i++)
			v[i] = other.v[i];
	}

	~vector_de_vectori()
	{
		if (v)
			delete[] v;
		v = nullptr;
	}

	void fill(int colSize = -1)
	{
		int maxSize = colSize;
		if (colSize == -1){
			for (int i = 0; i < dim; i++){
				if (maxSize < v[i].size)
					maxSize = v[i].size;
			}
		}
		for (int i = 0; i < dim; i++)
			if (v[i].size < maxSize)
				this->resize_vec(i, maxSize);
	}

	vector_de_vectori operator+(const vector_de_vectori& m)
	{
		int col1 = this->maxColSize();
		int col2 = m.maxColSize();
		int maxCol = col1 > col2 ? col1 : col2;
		vector_de_vectori sum = *this, t2 = m;
		sum.fill(maxCol);
		t2.fill(maxCol);

		for (int i = 0; i < sum.dim; i++)
			for (int j = 0; j < maxCol; j++)
				sum.v[i].data[j] += t2.v[i].data[j];
		return sum; 
	}

	friend std::istream& operator>>(std::istream& is, vector_de_vectori& m) 
	{
		int dim;
		std::cout << "dim: ";
		is >> dim;
		m.v = new vector[dim];
		m.dim = dim;
		for (int i = 0; i < m.dim; i++){
			is >> m.v[i];
		}
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const vector_de_vectori& m) 
	{
		os << '\n';
		for (int i = 0; i < m.dim; i++)
			os << m.v[i] << '\n';
		return os;
	}

private:
	void resize_vec(int i, int newSize)
	{
		int* newData = new int[newSize];
		vector& vec = v[i];
		for (int i = 0; i < vec.size; i++)
			newData[i] = vec.data[i];
		for (int i = vec.size; i < newSize; i++)
			newData[i] = 0;
		delete[] vec.data;
		vec.data = newData;
		vec.size = newSize;
	}

	int maxColSize() const
	{
		int maxSize = 0;
		for (int i = 0; i < dim; i++)
			if (maxSize < v[i].size)
				maxSize = v[i].size;
		return maxSize;
	}
};

int main()
{
	std::ifstream fin("date.txt");
	vector_de_vectori v1, v2;
	fin >> v1;
	std::cout << v1;
	return 0;
}

