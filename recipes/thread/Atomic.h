// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (giantchen at gmail dot com)

// https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html

#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace muduo
{

namespace detail
{
template<typename T>
class AtomicIntegerT : boost::noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  // uncomment if you need copying and assignment
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  T get() const
  {
      //bool __sync_bool_compare_and_swap(type *ptr, type oldval type newval, ...)
      //    type __sync_val_compare_and_swap(type *ptr, type oldval type newval, ...)
      //    These builtins perform an atomic compare and swap.That is, if the current value of *ptr is oldval, then write newval into *ptr.
    return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);
  }

  T getAndAdd(T x)
  {
    return __sync_fetch_and_add(&value_, x);
  }

  T addAndGet(T x)
  {
    return getAndAdd(x) + x;
  }

  T incrementAndGet()
  {
    return addAndGet(1);
  }

  void add(T x)
  {
    getAndAdd(x);
  }

  void increment()
  {
    incrementAndGet();
  }

  void decrement()
  {
    getAndAdd(-1);
  }

  T getAndSet(T newValue)
  {
    //This builtin, as described by Intel, is not a traditional test-and-set operation,
    //but rather an atomic exchange operation.It writes value into *ptr, and returns the previous contents of *ptr.
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  volatile T value_;
};
}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // MUDUO_BASE_ATOMIC_H
