/*
 * mpBuffer.h
 *
 *  Created on: May 27, 2012
 *      Author: cbg
 */

#ifndef MPBUFFER_H_
#define MPBUFFER_H_

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#  include "memory.h"
#else
#  include <memory>
#endif

#include <stdexcept>


/** Limit maximum number of elements a buffer might contain. */
static const ::size_t _max_size = (1<<25);

template <typename _Type, typename _Alloc = std::allocator<_Type> > class mpBuffer
{
public:

  typedef _Type& reference;
  typedef const _Type& const_reference;
  typedef _Type* pointer;
  typedef const _Type* const_pointer;

  /* currently, we don't need any iterators... */
  /*
  typedef int iterator;
  typedef const int const_iterator;
  typedef int reverse_iterator;
  typedef const int const_reverse_iterator;
  */

  typedef int size_type;
  typedef int difference_type;

  typedef _Type value_type;
  typedef _Alloc allocator_type;

protected:
  /* member variables */
  /** Current number of elements in use. */
  size_type _size;
  

  /** Allocator of current element type. */
  allocator_type _allocator;
  /** Pointer to all elements of buffer. */
  pointer _fields;

  /** Attribute deciding whether the current buffer implements a ringbuffer. */
  bool _is_ringbuffer;

  /**
    * In case of a ring buffer, this is the index to the first valid element.
    * Initial value is 0.
    */
  size_type _first;
  /**
    * In case of a ring buffer, this is the index to the last valid element.
    * Initial value is _size - 1.
    */
  size_type _last;

  /** Saves the current index of  */
  //size_type _current_idx;

  /** Saves the number of currently used elements for a ring buffer. */
  size_type _used_elements;

  /**
   * @brief Releases all elements by calling the destructor of the saved elements
   *        and deallocating all needed memory.
   */
  virtual void clean()
  {
    for (size_type i = 0; i < _size; ++i)
    {
      _allocator.destroy(&_fields[i]);
    }
    _allocator.deallocate(_fields, _size);
  }

public:

  /* ============ */
  /* constructors */
  /* ============ */

  explicit mpBuffer(const allocator_type& a = allocator_type() ) :
    _size(0),
    //_capacity(0),
    _allocator(a),
    _fields(0),
    _is_ringbuffer(false),
    _first(0),
    _last(-1),
    //_current_idx(0),
    _used_elements(0)
  {}

  explicit mpBuffer(size_type n,
                    bool rb = false,
                    const value_type& v = value_type(),
                    const allocator_type& a = allocator_type()) :
    _size(n),
    //_capacity(n + 10),
    _allocator(a),
#if defined _MSC_VER && _MSC_VER <= 1200
    _fields(_allocator.allocate(_size, 0)),
#else
    _fields(_allocator.allocate(_size)),
#endif
    _is_ringbuffer(rb),
    _first(0),
    _last(-1),
    //_current_idx(0),
    _used_elements(0)
  {
    // construct objects
    for (size_type i = 0; i < _size; ++i)
    {
      _allocator.construct(&_fields[i], v);
    }
  }

  /**
   * Copy constructor.
   */
  mpBuffer(const mpBuffer<value_type, allocator_type>& buf) :
    _size(buf._size),
    //_capacity(buf._capacity),
    _allocator(buf._allocator),
#if defined _MSC_VER && _MSC_VER <= 1200
    _fields(_allocator.allocate(_size, 0)),
#else
    _fields(_allocator.allocate(_size)),
#endif
    _is_ringbuffer(buf._is_ringbuffer),
    _first(buf._first),
    _last(buf._last),
    //_current_idx(buf._current_idx),
    _used_elements(buf._used_elements)
  {
    // copy all elements
    for (size_type i = 0; i < _size; ++i)
    {
      _allocator.construct(&(_fields[i]), buf._fields[i]);
    }

  }

  virtual mpBuffer& operator=(const mpBuffer<value_type, allocator_type>& buf)
  {
    clean();
    _size = buf._size;
    //_capacity = buf._capacity;
    _allocator = buf._allocator;
#if defined _MSC_VER && _MSC_VER <= 1200
    _fields = _allocator.allocate(_size, 0),
#else
    _fields = _allocator.allocate(_size);
#endif
    _is_ringbuffer = buf._is_ringbuffer;
    _first = buf._first;
    _last = buf._last;
    //_current_idx = buf._current_idx;
    _used_elements = buf._used_elements;
    // copy all elements
    for (size_type i = 0; i < _size; ++i)
    {
      _allocator.construct(&(_fields[i]), buf._fields[i]);
    }
    return *this;
  }

  /* ========== */
  /* destructor */
  /* ========== */
  virtual ~mpBuffer()
  {
    // just call clean to release all elements
    clean();
  }

  /* ======================= */
  /* public capacity methods */
  /* ======================= */

  inline virtual size_type size() const
  {
    return _size;
  }

  inline virtual size_type max_size() const
  {
    return _max_size;
  }

  void virtual resize(size_type sz, value_type c = value_type() )
  {
#if defined _MSC_VER && _MSC_VER <= 1200
    pointer newFields = _allocator.allocate(sz, 0);
#else
    pointer newFields = _allocator.allocate(sz);
#endif

    // if we do not have a ring buffer, just copy the first elements to
    // the newly allocated array
    if (!_is_ringbuffer)
    {
    	size_type i;
      // number of fields to copy is minimum of current size and new size
      size_type copy_fields = (sz > _size) ? _size : sz;
      // copy all existing fields
      for (i = 0; i < copy_fields; ++i)
      {
        _allocator.construct(&(newFields[i]), _fields[i]);
      }

      // initialize all new elements if any
      for (i = copy_fields; i < sz; ++i)
      {
        _allocator.construct(&(newFields[i]), c);
      }
      _last = sz - 1;
    }
    else
    // if we have a ring buffer, copy all elements from first to last
    {
    	size_type newElementPtr, oldElementPtr, copySize;
    	int elementsCopied;

    	if (sz < _used_elements)
    	{
    		// TODO: change exception type
    		throw std::out_of_range("Can not decrease size of ring buffer: too much elements in use!");
    	}

    	copySize = (sz > _size) ? _size : sz;
    	newElementPtr = _first % sz;
    	oldElementPtr = _first % _size;

    	for (elementsCopied = 0; elementsCopied < copySize; ++elementsCopied)
    	{
    		_allocator.construct(&(newFields[newElementPtr]), _fields[oldElementPtr]);
    		newElementPtr = (newElementPtr + sz - 1) % sz;
    		oldElementPtr = (oldElementPtr + _size - 1) % _size;
    	}

    	for (elementsCopied = _size; elementsCopied < sz; ++elementsCopied)
    	{
    		_allocator.construct(&(newFields[newElementPtr]), c);
    		newElementPtr = (newElementPtr + sz - 1) % sz;
    	}


    }

    // deallocate existing memory
    clean();

    // set new fields
    _fields = newFields;
    // set new size
    _size = sz;

  }

  inline virtual size_type capacity() const
  {
    //return _capacity;
    return _size;
  }

  inline virtual bool empty() const
  {
    return (_size == 0);
  }

  /* ======================= */
  /* public capacity methods */
  /* ======================= */

  inline virtual reference at(size_type n)
  {
    if ((n >= _size) || (n < 0))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    return _fields[n];
  }

  inline virtual const_reference at(size_type n) const
  {
    if ((n >= _size) || (n < 0))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    // as we cannot change the size of a constant buffer, we do
    // not need to track the size of actually accessed elements

    return _fields[n];
  }

  inline virtual reference operator[](size_type n)
  {
  	if (!_is_ringbuffer && (n >= _size || n < 0))
			throw std::out_of_range("mpBuffer has not been accessed properly");

		// save original index
		size_type idx = n;

		n = n % _size;
		n = (n < 0) ? (n + _size) : n;
		reference retVal = _fields[n];

		// save the lowest index of the current array access:
		// if the currently used buffer size is less than the
		// difference between current index and
		if ((_first - idx) > _used_elements)
		{
			// if we cannot increase the number of used elements, throw
			// an out of range exception
			if ((_first - idx) > _size)
				throw std::out_of_range("Ring buffer access to element which is currently in use");

			_used_elements = (_first - idx) + 1;

			_last = idx;
		}

		return retVal;
  }

  inline virtual const_reference operator[](size_type n) const
  {
  	if (!_is_ringbuffer && ((n >= _size) ||(n < 0)))
			throw std::out_of_range("mpBuffer has not been accessed properly");

		n = n % _size;
		n = (n < 0) ? (n + _size) : n;

		// as we cannot change the size of a constant buffer, we do
		// not need to track the size of actually accessed elements

		return _fields[n];
  }

  inline virtual reference front()
  {
    reference retVal = (_is_ringbuffer) ?
        _fields[_first % _size] : _fields[0];
    return retVal;
  }

  inline virtual const_reference front() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_first % _size] : _fields[0];

    return retVal;
  }

  inline virtual reference back()
  {
    reference retVal = (_is_ringbuffer) ?
        _fields[_last % _size] : _fields[_size];
    return retVal;
  }

  inline virtual const_reference back() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_last % _size] : _fields[_size];

    return retVal;
  }

  inline virtual allocator_type get_allocator() const
  {
    return _allocator;
  }

  /* ================================= */
  /* public extensions for ring buffer */
  /* ================================= */

  /**
   * @brief Sets the current index of the ring buffer to the given
   *        value and adapts internal pointers to the first and
   *        last element of the ring buffer.
   */
  inline virtual void setCurrentIdx(size_type n)
  {
  	// the last index is calculated based on the previous values of last and
		// current_idx.
  	_last = _last + (n - _first);

    _first = n;

  }

  inline virtual size_type getFirst() const
  {
  	return _first;
  }

  inline virtual size_type getLast() const
	{
		return _last;
	}

  /**
   * @brief Returns the number of accessed elements of the
   *        ring buffer which is equivalent to the actually
   *        used buffer size.
   */
  inline virtual size_type getUsedBufferSize() const
  {
    //return (_last == -1) ? 0 : ((_first - _last + 1 + _size) % _size);
  	return _used_elements;
  }


  /**
   * @brief Returns true if the current buffer implements a
   *        a ring buffer, otherwise false.
   */
  inline virtual bool isRingBuffer() const
  {
  	return _is_ringbuffer;
  }

};


#endif /* MPBUFFER_H_ */
