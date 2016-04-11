/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

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

/**
 * @brief A generic buffer which may be either used as a simple vector or ring buffer.
 * @details The implementation of this class is based on the STL vector and behaves
 *          exactly like it in case it is used as an array. Only the methods referenced by
 *          the muParser have been implemented - some missing parts like iterators are not
 *          available but can still be implemented.
 */
template <typename _Type, typename _Alloc = std::allocator<_Type> > class mpBuffer
{
public:

  /** Reference type of the data being stored in the vector. */
  typedef _Type& reference;
  /** Constant reference type of the data being stored in the vector. */
  typedef const _Type& const_reference;
  /** Pointer type of the data being stored in the vector. */
  typedef _Type* pointer;
  /** Constant pointer type of the data being stored in the vector. */
  typedef const _Type* const_pointer;

  /* currently, we don't need any iterators... */
  /*
  typedef int iterator;
  typedef const int const_iterator;
  typedef int reverse_iterator;
  typedef const int const_reverse_iterator;
  */

  /** The standard size type is int to allow negative indices for access. */
  typedef int size_type;

  /** Data type of the data being stored. */
  typedef _Type value_type;
  /** Allocator type for the used data type. */
  typedef _Alloc allocator_type;

protected:

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
   * Initial value is -1.
   */
  size_type _first;
  /**
   * In case of a ring buffer, this is the index to the last valid element.
   * Initial value is -1.
   */
  mutable size_type _last;

  /** Saves the number of currently used elements for a ring buffer. */
  mutable size_type _used_elements;

  /**
   * @brief Releases all elements by calling the destructor of the saved
   *        elements and deallocating all needed memory.
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

  /**
   * @brief Creates a new buffer object with no elements. The default buffer
   *        type is array.
   * @param[in] a Defines the allocator type for the class which shall be saved
   *            in the buffer. Defaults to the standard allocator.
   */
  explicit mpBuffer(const allocator_type& a = allocator_type() ) :
    _size(0),
    //_capacity(0),
    _allocator(a),
    _fields(0),
    _is_ringbuffer(false),
    _first(-1),
    _last(-1),
    //_current_idx(0),
    _used_elements(0)
  {}

  /**
   * @brief Creates a new buffer object with the specified number of elements
   *        and the specified type.
   * @param[in] n Initial size of the array.
   * @param[in] rb Defines the type of the array: If set to true, the buffer is
   *            a ring buffer, otherwise, it will be an array.
   * @param[in] v Defines the class type which shall be saved in the buffer.
   * @param[in] a Defines the allocator type for the class which shall be saved
   *            in the buffer. Defaults to the standard allocator.
   */
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
    _first(-1),
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
   * @brief Creates a new buffer by copying all elements of the specified
   *        buffer object.
   * @param[in] buf The buffer to copy.
   * @warning Take care when copying a buffer containing pointers to elements -
   *          only the pointers will be copied, but not the elements themselves.
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

  /**
   * @brief Assignment operator - deallocates all currently saved object in the
   *        buffer and creates a copy of the specified buffer.
   * @param[in] buf The buffer to copy.
   * @warning Take care when copying a buffer containing pointers to elements -
   *          only the pointers will be copied, but not the elements themselves.
   */
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


  /**
   * @brief The destructor of the mpBuffer class.
   */
  virtual ~mpBuffer()
  {
    // just call clean to release all elements
    clean();
  }


  /**
   * @brief Returns the current size of the buffer.
   */
  inline virtual size_type size() const
  {
    return _size;
  }

  /**
   * @brief Returns the maximum number of elements which can be saved in a the
   *        buffer class.
   */
  inline virtual size_type max_size() const
  {
    return _max_size;
  }

  /**
   * @brief Resizes the current buffer to the given size.
   * @param[in] sz The size of the new buffer.
   * @param[in] c The class type of the elements to save in the buffer.
   * @exception std::length_error If the new size of a ring buffer will be
   *            less than the current number of elements in use.
   */
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
          throw std::length_error("Can not decrease size of ring buffer: too many elements in use!");
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

  /**
   * @brief Returns the capacity of the current buffer which is equal to the
   *        size.
   */
  inline virtual size_type capacity() const
  {
    //return _capacity;
    return _size;
  }

  /**
   * @brief Returns true if the current amount of saved elements is zero.
   */
  inline virtual bool empty() const
  {
    return (_size == 0);
  }


  /**
   * @brief Returns the element currently saved at the specified position. This
   *        function behaves the same for the array and ring buffer
   *        implementation and does not change the _used_elements field.
   * @param[in] n The position of the element saved in the buffer.
   * @exception std::out_of_range If the specified position is not within
   *            0 <= n < size.
   */
  inline virtual reference at(size_type n)
  {
    if ((n >= _size) || (n < 0))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    return _fields[n];
  }

  /**
   * @brief Returns the element currently saved at the specified position. This
   *        function behaves the same for the array and ring buffer
   *        implementation and does not change the _used_elements field.
   * @param[in] n The index of the element saved in the buffer.
   * @exception std::out_of_range If the specified position is not within
   *            0 <= n < size.
   */
  inline virtual const_reference at(size_type n) const
  {
    if ((n >= _size) || (n < 0))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    // as we cannot change the size of a constant buffer, we do
    // not need to track the size of actually accessed elements

    return _fields[n];
  }

  /**
   * @brief Returns the element currently saved at the specified position, but
   *        uses a modulo operation in case of a ring buffer.
   * @param[in] n The index of the element saved in the buffer.
   * @exception std::out_of_range If the current buffer is an array and the
   *            index is not within 0 <= n < size or the number of used elements
   *            in the ring buffer is greater than the current size.
   */
  inline virtual reference operator[](size_type n)
  {
    if (!_is_ringbuffer && (n >= _size || n < 0))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    // save original index
    size_type idx = n;

    n = n % _size;
    n = (n < 0) ? (n + _size) : n;
    reference retVal = _fields[n];

//    std::cout << "operator[]: _first = " << _first << ", idx = " << idx
//        << ", _used_elements = " << _used_elements
//        << ", _size = " << _size << std::endl;

    // save the lowest index of the current array access:
    // if the currently used buffer size is less than the
    // difference between current index and the first element in use,
    // we have to increase the number of used elements
    if ((_first - idx) >= _used_elements)
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

  /**
   * @brief Returns the element currently saved at the specified position, but
   *        uses a modulo operation in case of a ring buffer.
   * @param[in] n The index of the element saved in the buffer.
   * @exception std::out_of_range If the current buffer is an array and the
   *            index is not within 0 <= n < size or the number of used elements
   *            in the ring buffer is greater than the current size.
   */
  inline virtual const_reference operator[](size_type n) const
  {
    if (!_is_ringbuffer && ((n >= _size) ||(n < 0)))
      throw std::out_of_range("mpBuffer has not been accessed properly");

    size_type idx = n;

    n = n % _size;
    n = (n < 0) ? (n + _size) : n;

    // save the lowest index of the current array access:
    // if the currently used buffer size is less than the
    // difference between current index and the first element in use,
    // we have to increase the number of used elements
    if ((_first - idx) >= _used_elements)
    {
      // if we cannot increase the number of used elements, throw
      // an out of range exception
      if ((_first - idx) > _size)
        throw std::out_of_range("Ring buffer access to element which is currently in use");

      _used_elements = (_first - idx) + 1;

      _last = idx;
    }

    return _fields[n];
  }

  /**
   * @brief Returns the first element of the current buffer.
   */
  inline virtual reference front()
  {
    reference retVal = (_is_ringbuffer) ?
        _fields[_first % _size] : _fields[0];
    return retVal;
  }

  /**
   * @brief Returns the first element of the current buffer.
   */
  inline virtual const_reference front() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_first % _size] : _fields[0];

    return retVal;
  }

  /**
   * @brief Returns the last element of the current buffer.
   */
  inline virtual reference back()
  {
    reference retVal = (_is_ringbuffer) ?
        _fields[_last % _size] : _fields[_size];
    return retVal;
  }

  /**
   * @brief Returns the last element of the current buffer.
   */
  inline virtual const_reference back() const
  {
    const_reference retVal = (_is_ringbuffer) ?
        _fields[_last % _size] : _fields[_size];

    return retVal;
  }

  /**
   * @brief Returns the allocator type of the current buffer.
   */
  inline virtual allocator_type get_allocator() const
  {
    return _allocator;
  }


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

//    std::cout << "Setting current idx to " << n << ", _last = "
//    		<< _last << ", _first = " << _first << std::endl;

  }

  /**
   * @brief Returns the first element saved in the current buffer.
   */
  inline virtual size_type getFirst() const
  {
  	return _first;
  }

  /**
   * @brief Returns the last element saved in the current buffer.
   */
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
   * @brief Returns true if the current buffer behaves like a a ring buffer,
   *        false otherwise.
   */
  inline virtual bool isRingBuffer() const
  {
  	return _is_ringbuffer;
  }

  /**
   * @brief Sets the current buffer to behave like a ring buffer.
   */
  inline virtual void setRingBuffer()
  {
	  _is_ringbuffer = true;
  }

  /**
   * @brief Sets the current buffer to behave like a vector.
   */
  inline virtual void setVector()
  {
	  _is_ringbuffer = false;
  }

};


#endif /* MPBUFFER_H_ */
