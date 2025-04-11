#include "Vector.h"
#include <iostream>
#include <algorithm>

Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
    : _size(size), _capacity(size), _multiplicativeCoef(coef) 
{
    _data = new ValueType[_capacity];
    std::copy(rawArray, rawArray + size, _data);
}

Vector::Vector(const Vector& other)
    : _size(other._size), _capacity(other._size),  
      _multiplicativeCoef(other._multiplicativeCoef)
{
    _data = new ValueType[_capacity];
    std::copy(other._data, other._data + _size, _data);
}
Vector& Vector::operator=(const Vector& other) 
{
    if (this != &other) 
    {
        ValueType* newData = new ValueType[other._capacity];
        std::copy(other._data, other._data + other._size, newData);
        delete[] _data;
        _data = newData;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;
    }
    return *this;
}

Vector::Vector(Vector&& other) noexcept
    : _data(other._data),
      _size(other._size),
      _capacity(other._capacity),
      _multiplicativeCoef(other._multiplicativeCoef) 
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) noexcept 
{
    if (this != &other) 
    {
        delete[] _data;
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _multiplicativeCoef = other._multiplicativeCoef;
        
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}

Vector::~Vector() 
{
    delete[] _data;
    _data = nullptr;
}

void Vector::pushBack(const ValueType& value)
{
    if (_capacity == _size)
    {
        size_t newCapacity;
        if (_size == 0)
        {
            newCapacity = _multiplicativeCoef; 
        }
        else
        {
            newCapacity = _size * _multiplicativeCoef;
        }
        reserve(newCapacity);
    }
    _data[_size] = value;
    _size++;
}

void Vector::pushFront(const ValueType& value)
{
    if (_capacity == _size)
    {
        size_t newCapacity;
        if (_size == 0)
        {
            newCapacity = _multiplicativeCoef;
        }
        else
        {
            newCapacity = _size * _multiplicativeCoef;
        }
        reserve(newCapacity);
    }
    
    if (_size > 0)
    {
        std::move_backward(_data, _data + _size, _data + _size + 1);
    }

    _data[0] = value;
    _size++;
}

void Vector::insert(const ValueType& value, size_t pos) 
{
    if (pos > _size) 
    {
        throw std::out_of_range("Insert position out of range");
    }
    
    if (_size >= _capacity) 
    {
        size_t newCapacity = std::max((size_t)(_capacity * _multiplicativeCoef), _capacity + 1);
        reserve(newCapacity);
    }
    
    if (pos < _size) 
    {
        std::move_backward(_data + pos, _data + _size, _data + _size + 1);
    }
    _data[pos] = value;
    _size++;
}

void Vector::insert(const ValueType* values, size_t size, size_t pos) 
{
    if (pos > _size) 
    {
        throw std::out_of_range("Insert position out of range");
    }
    
    if (size == 0) 
    {
        return;  
    }
    
    if (_size + size > _capacity) 
    {
        reserve(std::max((size_t)((_size + size) * _multiplicativeCoef), _size + size));
    }
    
    std::move_backward(_data + pos, _data + _size, _data + _size + size);
    std::copy(values, values + size, _data + pos);
    _size += size;
}

void Vector::insert(const Vector& vector, size_t pos) 
{
    if (pos > _size) 
    {
        throw std::out_of_range("Insert position out of range");
    }
    
    if (vector._size == 0) 
    {
        return;  
    }
    
    if (_size + vector._size > _capacity) 
    {
        reserve(std::max((size_t)((_size + vector._size) * _multiplicativeCoef), _size + vector._size));
    }
    
    std::move_backward(_data + pos, _data + _size, _data + _size + vector._size);
    
    std::copy(vector._data, vector._data + vector._size, _data + pos);
    
    _size += vector._size;
}

void Vector::popBack() 
{
    if (_size == 0) 
    {
        throw std::out_of_range("Cannot popBack from empty vector");
    }
    _size--;
}

void Vector::popFront()
{
    if (_size == 0)
    {
        throw std::out_of_range("Cannot popFront from empty vector");
    }
    
    if (_size > 1)
    {
        std::move(_data + 1, _data + _size, _data);
    }
    _size--;
}

void Vector::erase(size_t pos, size_t count) 
{
    if (pos >= _size) 
    {
        throw std::out_of_range("Erase position out of range");
    }
    
    if (count == 0) 
    {
        return;  
    }
    
    size_t actualCount = std::min(count, _size - pos);
    
    if (pos + actualCount < _size) 
    {
        std::move(_data + pos + actualCount, _data + _size, _data + pos);
    }
    _size -= actualCount;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) 
{
    if (beginPos >= endPos) 
    {
        throw std::invalid_argument("beginPos must be less than endPos");
    }
    if (beginPos >= _size) 
    {
        throw std::out_of_range("beginPos out of range");
    }
    if (endPos > _size) 
    {
        throw std::out_of_range("endPos out of range");
    }
    
    size_t count = endPos - beginPos;
    std::move(_data + endPos, _data + _size, _data + beginPos);
    _size -= count;
}

size_t Vector::size() const 
{
    return _size;
}

size_t Vector::capacity() const 
{
    return _capacity;
}

double Vector::loadFactor() const 
{
    if (_capacity == 0) return 0.0;
    return (double)_size / _capacity;
}

ValueType& Vector::operator[](size_t idx) 
{
    return _data[idx];
}

const ValueType& Vector::operator[](size_t idx) const 
{
    return _data[idx];
}

long long Vector::find(const ValueType& value) const 
{
    for (size_t i = 0; i < _size; ++i) {
        if (_data[i] == value) {
            return i;
        }
    }
    return -1;
}

void Vector::reserve(size_t capacity)
{
    if (capacity > _capacity)
    {
        ValueType* newData = new ValueType[capacity];
        std::copy(_data, _data + _size, newData);
        delete[] _data;
        _data = newData;
        _capacity = capacity;
    }
}

void Vector::shrinkToFit() 
{
    if (_size < _capacity) 
    {
        ValueType* newData = new ValueType[_size];
        std::copy(_data, _data + _size, newData);
        delete[] _data;
        _data = newData;
        _capacity = _size;
    }
}

Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr) {}

ValueType& Vector::Iterator::operator*() 
{
    return *_ptr;
}

const ValueType& Vector::Iterator::operator*() const 
{
    return *_ptr;
}

ValueType* Vector::Iterator::operator->() 
{
    return _ptr;
}

const ValueType* Vector::Iterator::operator->() const 
{
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++() 
{
    _ptr++;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) 
{
    Iterator tmp = *this;
    _ptr++;
    return tmp;
}

bool Vector::Iterator::operator==(const Iterator& other) const 
{
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator& other) const 
{
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() 
{
    return Iterator(_data);
}

Vector::Iterator Vector::end() 
{
    return Iterator(_data + _size);
}