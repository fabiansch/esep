// Name        : channel.cpp
// Author      : Stephan Pareigis
// Copyright   : CC0 licence

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <cstddef>
#include <condition_variable>
#include <chrono>
#include "Signals.h"

using namespace std;

namespace logicLayer {

constexpr int DEADLOCK_TIMEOUT_250_ms = 250;

// the semaphore class should be in its own .h file. See the semaphore post on this site
class Semaphore {
public:
    Semaphore(const int count = 0)
        : release(false)
		, count_(count)
        , condition_()
        , mtx_()   {}

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore(Semaphore&&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;

    inline void post(void) {
        std::lock_guard<std::mutex> lock(mtx_);
        count_++;
        condition_.notify_one();    }

    inline void wait(void) {
        std::unique_lock<std::mutex> lock(mtx_);
		condition_.wait(lock, [&] { return get_condition();});
        count_--;
    }

    inline bool wait_for(void) {
    	bool no_timeout = true;
		std::unique_lock<std::mutex> lock(mtx_);
		no_timeout = condition_.wait_for(lock, chrono::milliseconds(DEADLOCK_TIMEOUT_250_ms), [&]{ return get_condition();});
		if(no_timeout) {
			count_--;
		}
		return count_;
	}

    inline bool try_wait(void) {
        std::unique_lock<std::mutex> lock(mtx_);
        if(count_ > 0) {count_--; return true; }
        return false;
    }

    inline size_t get_value() const {    	return count_;    }

    void destroy() {
    	release = true;
    	condition_.notify_all();
    }
private:
    bool get_condition() { return count_ > 0 or release; }

    bool release;
    int count_;
    std::condition_variable condition_;
    std::mutex mtx_;
};

template <typename Type_>
class Channel {
  public:
      explicit Channel(const int max_size)
          : sem_free_spaces_{max_size + 1}
          , sem_size_{}
          , queue_{}
          , mtx_{}
          , destroyed(false)
      {
    	  static int ID = 0;
    	  id = ID++;
      }

      int size(void) const {
          return sem_size_.get_value();
      }

      int max_size(void) const {
          return sem_free_spaces_.get_value() + sem_size_.get_value();
      }

      inline Type_ dequeue(void) {
          sem_size_.wait();
          Type_ return__{};
          mtx_.lock();
          if(queue_.size() > 0 and not destroyed) {
			  return__ = queue_.front();
			  queue_.pop();
          }
          mtx_.unlock();
          sem_free_spaces_.post();
          return return__;
      }

      inline void enqueue(const Type_ element) {
    	  bool no_timeout;
    	  no_timeout = sem_free_spaces_.wait_for();
    	  if (no_timeout) {
			  mtx_.lock();
			  queue_.push(element);
			  mtx_.unlock();
			  sem_size_.post();
    	  } else {
    		  throw std::range_error("timeout happened during enqueing!");
    	  }
      }

      void operator<<(const Type_ element) {
    	  try {
    		  enqueue(element);
    	  } catch (std::range_error& e) {
    		  throw e;
    	  }
      }

      Type_ friend operator<<(Type_& target, Channel<Type_>& source) {
          return target = source.dequeue();
      }

      void friend operator<<(Channel<Type_>& target,
    		  Channel<Type_>& source) {
          target.enqueue(source.dequeue());
      }

      int getId() { return id; }

      void destroy() {
    	  destroyed = true;
    	  sem_free_spaces_.destroy();
    	  sem_size_.destroy();
      }
  private:
      Semaphore sem_free_spaces_, sem_size_;
      std::queue<Type_> queue_;
      std::mutex mtx_;
      int id;
      bool destroyed;
};

} /* namespace logicLayer */

#endif /* CHANNEL_H_ */
