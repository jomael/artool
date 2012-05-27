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
  /** Save number of actually reserved memory. Must be greater or equal to _size. */
  size_type _capacity;

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

  /**
   * @brief Releases all elements by calling the destructor and deallocating
   *        all needed memory.
   */
  virtual void clean()
  {
    for (size_type i = 0; i < _capacity; ++i)
    {
      _allocator.destroy(&_fields[i]);
    }
    _allocator.deallocate(_fields, _capacity);
  }

public:

  /* ============ */
  /* constructors */
  /* ============ */

  explicit mpBuffer(const allocator_type& a = allocator_type() ) :
    _size(0),
    _capacity(0),
    _allocator(a),
    _fields(0),
    _is_ringbuffer(false),
    _first(0),
    _last(0)
  {}

  explicit mpBuffer(size_type n,
                    const value_type& v = value_type(),
                    bool rb = false,
                    const allocator_type& a = allocator_type()) :
    _size(n),
    _capacity(n + 10),
    _allocator(a),
    _fields(_allocator.allocate(_capacity)),
    _is_ringbuffer(rb),
    _first(0),
    _last(n)
  {
    // construct objects
    for (size_type i = 0; i < _capacity; ++i)
    {
      _allocator.construct(&_fields[i], v);
    }
  }

  /**
   * Copy constructor.
   */
  mpBuffer(const mpBuffer<value_type, allocator_type>& buf) :
    _size(buf._size),
    _capacity(buf._capacity),
    _allocator(buf._allocator),
    _fields(_allocator.allocate(_capacity)),
    _is_ringbuffer(buf._is_ringbuffer),
    _first(buf._first),
    _last(buf._last)
  {
    // copy all elements
    for (size_type i = 0; i < _capacity; ++i)
    {
      _allocator.construct(&(_fields[i]), buf._fields[i]);
    }

  }

  virtual mpBuffer& operator=(const mpBuffer<value_type, allocator_type>& buf)
  {
    clean();
    _size = buf._size;
    _capacity = buf._capacity;
    _allocator = buf._allocator;
    _fields = _allocator.allocate(_capacity);
    _is_ringbuffer = buf._is_ringbuffer;
    _first = buf._first;
    _last = buf._last;
    // copy all elements
    for (size_type i = 0; i < _capacity; ++i)
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

  void virtual resize(size_type sz, _Type c = _Type() )
  {
    // TODO
  }

  inline virtual size_type capacity() const
  {
    return _capacity;
  }

  inline virtual bool empty() const
  {
    return (_size == 0);
  }

  virtual void reserve(size_type n)
  {
    // TODO
  }

  /* ======================= */
  /* public capacity methods */
  /* ======================= */

  inline virtual reference at(size_type n)
  {
    if (!_is_ringbuffer && n >= _size)
      throw std::out_of_range("mpBuffer has not been accessed properly");

    reference retVal = (_is_ringbuffer) ?
        _fields[n % _capacity] : _fields[n];

    return retVal;
  }

  inline virtual const_reference at(size_type n) const
  {
    if (!_is_ringbuffer && n >= _size)
      throw std::out_of_range("mpBuffer has not been accessed properly");

    const_reference retVal = (_is_ringbuffer) ?
        _fields[n % _capacity] : _fields[n];

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
        _fields[_first % _capacity] : _fields[0];
    return retVal;
  }

  inline virtual const_reference front() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_first % _capacity] : _fields[0];

    return retVal;
  }

  inline virtual reference back()
  {
    reference retVal = (_is_ringbuffer) ?
        _fields[_last % _capacity] : _fields[_size];
    return retVal;
  }

  inline virtual const_reference back() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_last % _capacity] : _fields[_size];

    return retVal;
  }

  inline virtual allocator_type get_allocator() const
  {
    return _allocator;
  }



};


#endif /* MPBUFFER_H_ */
