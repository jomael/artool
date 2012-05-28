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
  /** Limit maximum number of elements a buffer might contain. */
  static const size_type _max_size = (1<<25);

  /** Allocator of current element type. */
  allocator_type _allocator;
  /** Pointer to all elements of buffer. */
  pointer _fields;

  /** Attribute deciding whether the current buffer implements a ringbuffer. */
  bool _is_ringbuffer;

  /** In case of a ring buffer, this is the index to the first valid element. */
  size_type _first;
  /** In case of a ring buffer, this is the index to the last valid element. */
  size_type _last;

  /**  */
  size_type _current_idx;

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
    _last(0),
    _current_idx(0)
  {}

  explicit mpBuffer(size_type n,
                    bool rb = false,
                    const value_type& v = value_type(),
                    const allocator_type& a = allocator_type()) :
    _size(n),
    //_capacity(n + 10),
    _allocator(a),
    _fields(_allocator.allocate(_size)),
    _is_ringbuffer(rb),
    _first(0),
    _last(0),
    _current_idx(0)
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
    _fields(_allocator.allocate(_size)),
    _is_ringbuffer(buf._is_ringbuffer),
    _first(buf._first),
    _last(buf._last),
    _current_idx(buf._current_idx)
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
    _fields = _allocator.allocate(_size);
    _is_ringbuffer = buf._is_ringbuffer;
    _first = buf._first;
    _last = buf._last;
    _current_idx = buf._current_idx;
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
    pointer newFields = _allocator.allocate(sz);

    // if we do not have a ring buffer, just copy the first elements to
    // the newly allocated array
    if (!_is_ringbuffer)
    {
      // number of fields to copy is minimum of current size and new size
      size_type copy_fields = (sz > _size) ? _size : sz;
      // copy all existing fields
      for (size_type i = 0; i < copy_fields; ++i)
      {
        _allocator.construct(&(newFields[i]), _fields[i]);
      }

      // initialize all new elements if any
      for (size_type i = copy_fields; i < sz; ++i)
      {
        _allocator.construct(&(newFields[i]), c);
      }
      _last = _size - 1;
    }
    else
    // if we have a ring buffer, copy all elements from first to last
    {
      // calculate number of elements to copy
      size_type copy_fields = (sz > _size) ?
          ((_first - _last + 1 + _size) % _size) : (sz);
      size_type bufPointer = (sz > _size) ? _last : ((_first - sz + _size) % _size);
      for (size_type i = 0; i < copy_fields; ++i)
      {
        _allocator.construct(&(newFields[i]), _fields[bufPointer]);
        bufPointer = (bufPointer + 1) % _size;
      }

      // initialize all new elements if any
      for (size_type i = copy_fields; i < sz; ++i)
      {
        _allocator.construct(&(newFields[i]), c);
      }

      // set new element pointers for first and last
      _first = (sz > _size) ? (copy_fields) : (sz);
      _last = (_last > sz) ? (sz - 1) : 0;

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
    if (!_is_ringbuffer && n >= _size)
      throw std::out_of_range("mpBuffer has not been accessed properly");

    reference retVal = (_is_ringbuffer) ?
        _fields[n % _size] : _fields[n];

    // save the lowest index of the current array access:
    // if the currently used buffer size is less than the
    // difference between current index and
    if (((_first - _last + _size + 1) % _size) > (_current_idx - n))
    {
      // if we cannot increase the number of used elements, throw
      // an out of range exception
      if ((_current_idx - n) > _size)
        throw std::out_of_range("Ring buffer access to element which is currently in use");

      _last = n % _size;
    }

    return retVal;
  }

  inline virtual const_reference at(size_type n) const
  {
    if (!_is_ringbuffer && n >= _size)
      throw std::out_of_range("mpBuffer has not been accessed properly");

    const_reference retVal = (_is_ringbuffer) ?
        _fields[n % _size] : _fields[n];

    // as we cannot change the size of a constant buffer, we do
    // not need to track the size of actually accessed elements

    return retVal;
  }

  inline virtual reference operator[](size_type n)
  {
    return at(n);
  }

  inline virtual const_reference operator[](size_type n) const
  {
    return at(n);
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
    // the first element points to the given index modulo available buffer size
    _first = n % _size;
    // the last index is calculated based on the previous values of last and
    // current_idx.
    _last = (_last + (n - _current_idx) + _size) % _size;
    // sets the current index to the given index.
    _current_idx = n;
  }

  /**
   * @brief Returns the number of accessed elements of the
   *        ring buffer which is equivalent to the actually
   *        used buffer size.
   */
  inline virtual size_type getUsedBufferSize(size_type n)
  {
    return ((_first - _last + 1 + _size) % _size);
  }



};


#endif /* MPBUFFER_H_ */
