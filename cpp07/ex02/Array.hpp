#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdexcept>
#include <cstddef>

template <typename T>
class Array
{
public:
	Array() : _data(new T[0]()), _size(0) {}

	Array(unsigned int n) : _data(new T[n]()), _size(n) {}

	Array(Array const& other) : _data(new T[other._size]()), _size(other._size)
	{
		for (unsigned int i = 0; i < _size; i++)
			_data[i] = other._data[i];
	}

	Array& operator=(Array const& other)
	{
		if (this != &other)
		{
			delete[] _data;
			_size = other._size;
			_data = new T[_size]();
			for (unsigned int i = 0; i < _size; i++)
				_data[i] = other._data[i];
		}
		return *this;
	}

	~Array() { delete[] _data; }

	T& operator[](unsigned int idx)
	{
		if (idx >= _size)
			throw std::out_of_range("index out of bounds");
		return _data[idx];
	}

	T const& operator[](unsigned int idx) const
	{
		if (idx >= _size)
			throw std::out_of_range("index out of bounds");
		return _data[idx];
	}

	unsigned int size() const { return _size; }

private:
	T*           _data;
	unsigned int _size;
};

#endif
