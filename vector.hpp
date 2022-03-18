#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include "./iterators/random_access_iterator.hpp"
#include "./utils/utils.hpp"

namespace ft {
template < class T, class Alloc = std::allocator<T> >
class vector {
 private:
	T	*arr;
	size_t	_size;
	size_t	_cap;
	Alloc	_alloc;

 public:
	typedef T										value_type;
	typedef Alloc										allocator_type;
	typedef size_t										size_type;
	typedef ptrdiff_t									difference_type;
	typedef value_type&									reference;
	typedef const value_type&								const_reference;
	typedef typename allocator_type::pointer						pointer;
	typedef typename allocator_type::const_pointer						const_pointer;
	typedef random_access_iterator< value_type, difference_type, pointer, reference>	iterator;
	typedef random_access_iterator< value_type, difference_type, pointer, reference, true>	const_iterator;
	// typedef reverse_iterator<iterator>									reverse_iterator;
	// typedef reverse_iterator<const_iterator>							const_reverse_iterator;

//========================================================================================================
// constructor/destructor
//========================================================================================================

	explicit vector(const allocator_type& alloc = allocator_type()): arr(0), _size(0), _cap(0), _alloc(alloc) {}

	explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()): _size(n), _cap(n), _alloc(alloc) {

		if (n == 0) { arr = 0; return;}

		this->arr = _alloc.allocate(n);
		size_type i = 0;
		try {
			for(; i < n; ++i) {
				_alloc.construct(&this->arr[i], val);
			}
		} catch(...) {
			for (size_t j = 0; j < i; ++j) {
				_alloc.destroy(&arr[j]);
			}
			_alloc.deallocate(arr, _cap);
			this->_size = 0;
			this->_cap = 0;
			throw;
		}
	}

	template<class InputIterator>
			vector(InputIterator first, InputIterator last,  const allocator_type& alloc = allocator_type()): _alloc(alloc) {
		// check iterator valid!!!
		_size = iterator_dist(first, last);
		_cap = _size;
		this->arr = _alloc.allocate(_size);
		size_type i = 0;
		try {
			for (; i < _size; ++i) {
				_alloc.construct(&arr[i], *first);
				first++;
			}
		} catch(...) {
			for (size_type j = 0; j < i; ++j) {
				_alloc.destroy(&arr[j]);
			}
			_alloc.deallocate(arr, _cap);
			this->_size = 0;
			this->_cap = 0;
			throw;
		}
	}

	vector(const vector &other): _size(other._size), _cap(other._cap), _alloc(other._alloc) {
		this->arr = _alloc.allocate(_cap);
		size_type i = 0;
		try {
			for (; i < _size; ++i) {
				_alloc.construct(&arr[i], other.arr[i]);
			}
		} catch (...) {
			for (size_type j = 0; j < i; ++j) {
				_alloc.destroy(&arr[j]);
			}
			_alloc.deallocate(arr, _cap);
			_size = 0;
			_cap = 0;
			throw;
		}
	}


	~vector() {
		for (size_type j = 0; j < _size; ++j) {
			_alloc.destroy(&arr[j]);
		}
		_alloc.deallocate(arr, _cap);
	}

	vector &operator=(const vector &other) {

		if (this == &other) { return *this; }

		for (size_type j = 0; j < _size; j++) {
			_alloc.destroy(&arr[j]);
		}
		_alloc.deallocate(arr, _cap);
		_size = 0;
		_cap = 0;
		if (other._size == 0)
			return *this;
		_size = other._size;
		_alloc = other._alloc;
		_cap = other._cap;
		arr = _alloc.allocate(_cap);
		size_type i = 0;
		try {
			for (; i < _size; i++) {
				_alloc.construct(&arr[i], other[i]);
			}
		} catch (...) {
			for (size_type j = 0; j < i; j++)
				_alloc.destroy(&arr[j]);
			_alloc.deallocate(arr, _cap);
			_size = 0;
			_cap = 0;
			throw;
		}
	}

//========================================================================================================
// Element access
//========================================================================================================

	reference operator[] (size_type n) { return (*(arr + n)); }

	const_reference operator[](size_type n) const { return (*(arr + n)); }

	reference at(size_type n) {
		if (n >= _size)
			throw std::out_of_range("out_of_range error: item number (" + to_str(n)
				+ ") is greater than available items (" + to_str(_size - 1) + ")");
		return (*(arr + n));
	}

	const_reference at(size_type n) const {
		if (n >= _size)
			throw std::out_of_range("out_of_range error: item number (" + to_str(n)
				+ ") is greater than available items (" + to_str(_size - 1) + ")");
		return (*(arr + n));
	}

	reference front() { return (*arr); }

	const_reference front () const { return (*arr); }

	reference back() { return  (*(arr + _size - 1)); }

	const_reference back() const { return (*(arr + _size - 1)); }


//========================================================================================================
// Iterators
//========================================================================================================

	iterator begin() { iterator b(arr); return (b); }

	const_iterator begin() const { iterator b(arr); return (b); }

	iterator end() { iterator b(arr + _size); return (b); }

	const_iterator end() const { iterator e(arr + _size); return (e); }

//========================================================================================================
// Capacity
//========================================================================================================

	size_t size() const {
		return (this->_size);
	}

	size_type max_size() const {
		return (_alloc.max_size());
	}

	void resize(size_type n, value_type val = value_type()) {
		if (n < _size) {
			for (; _size != n; _size--)
				_alloc.destroy(&arr[_size]);
		} else if (n > _size) {
			if (n > _cap)
				reserve(n);
			size_type i = _size;
			try {
				for (; i < n; i++)
					_alloc.construct(&arr[i], val);
				_size = i;
			} catch (...) {
				for (size_type j = _size; j < i; ++j)
					_alloc.destroy(&arr[j]);
				throw;
			}
		}
	}

	size_type capacity() const { return _cap; }

	bool empty() const { return (_size == 0); }

	void reserve(size_type n) {
		if (n <= _cap) return;

		pointer new_arr = _alloc.allocate(n);
		size_type i = 0;

		try {
			for (; i < _size; i++)
				_alloc.construct(&new_arr[i], arr[i]);
			for (size_type j = 0; j < _size; j++)
				_alloc.destroy(&arr[j]);
			_alloc.deallocate(arr, _cap);
			_cap = n;
			arr = new_arr;
		} catch (...) {
			for (size_type j = 0; j < i; j++)
				_alloc.destroy(&new_arr[j]);
			_alloc.deallocate(new_arr, n);
			throw;
		}
	}

//========================================================================================================
// Modifers
//========================================================================================================

	void swap(vector& x) {
		if (this == &x) { return; }

		size_type	tmp_size = _size;
		size_type	tmp_cap = _cap;
		pointer		tmp_arr = arr;
		allocator_type	tmp_alloc = _alloc;

		_size = x._size;
		_cap = x._cap;
		_alloc = x._alloc;
		arr = x.arr;

		x._size = tmp_size;
		x._cap = tmp_cap;
		x._alloc = tmp_alloc;
		x.arr = tmp_arr;
	}



//========================================================================================================
// Non-member funcition (friend)
//========================================================================================================

}; // class vector
//========================================================================================================
// Non-member funcition (friend)
//========================================================================================================


} // namespace ft

#endif // VECTOR_HPP_
