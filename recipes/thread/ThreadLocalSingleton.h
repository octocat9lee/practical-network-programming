// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (giantchen at gmail dot com)

// more details: http://blog.csdn.net/jnu_simba/article/details/14037869

//每个线程都有一个单例对象T，即每个线程都有一个T*（__thread修饰，指针是POD类型数据），
//每个线程都有各自的特定数据（用TSD实现），t_value_ 即指向实际数据T的指针。

#ifndef MUDUO_BASE_THREADLOCALSINGLETON_H
#define MUDUO_BASE_THREADLOCALSINGLETON_H

#include <boost/noncopyable.hpp>

namespace muduo
{

template<typename T>
class ThreadLocalSingleton : boost::noncopyable
{
 public:

  static T& instance()
  {
    if (!t_value_)
    {
      t_value_ = new T();
    }
    return *t_value_;
  }

  // See muduo/base/ThreadLocalSingleton.h for how to delete it automatically.
  static void destroy()
  {
    delete t_value_;
    t_value_ = 0;
  }

 private:
  ThreadLocalSingleton();
  ~ThreadLocalSingleton();

  static __thread T* t_value_;
};

template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_ = 0;

}
#endif
