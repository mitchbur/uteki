//
//  stopwatch_timer.h
//
//  Created by Mitch Burghart on 2021-01-01.
//  Copyright © 2021 Mitchell Burghart. All rights reserved.
//

#ifndef stopwatch_timer_h
#define stopwatch_timer_h

#include <chrono>
#include <mutex>
using namespace std::chrono_literals;

namespace uteki
{

//! stopwatch  timer class
//! @details  Enhanced elapsed_timer that can be started, stopped and reset.
template< class ClockType = std::chrono::steady_clock >
class stopwatch_timer
{
	static_assert( ClockType::is_steady, "must use steady clock type" );

public:
	using rep = typename ClockType::rep;
	using period = typename ClockType::period;
	using duration = typename ClockType::duration;
	using time_point = typename ClockType::time_point;

	//! constructor
	//! @details constructs and starts timer
	stopwatch_timer( )
	: lock_( )
	, running_( true )
	, start_time_( ClockType::now() )
	, accumulated_( duration::zero() )
	{}

	//! copy constructor
	stopwatch_timer( stopwatch_timer<ClockType>& other ) noexcept
	: lock_( )
	{
		other.lock_.lock();
		running_ = other.running_ ;
		start_time_ = other.start_time_;
		accumulated_ = other.accumulated_;
		other.lock_.unlock();
	}

	//! move constructor
	stopwatch_timer( stopwatch_timer<ClockType>&& other ) noexcept
	: lock_( )
	, running_( false )
	, start_time_( time_point::min() )
	, accumulated_( duration::zero() )
	{
		other.lock_.lock();
		running_ = std::move( other.running_ );
		start_time_ = std::move( other.start_time_ );
		accumulated_ = std::move( other.accumulated_ );
		other.lock_.unlock();
	}

	stopwatch_timer<ClockType>& operator=( stopwatch_timer<ClockType>& rhs ) noexcept
	{
		rhs.lock_.lock();
		auto rhs_running = rhs.running_;
		auto rhs_start_time = rhs.start_time_;
		auto rhs_accumulated = rhs.accumulated_;
		rhs.lock_.unlock();

		lock_.lock();
		running_ = rhs_running;
		start_time_ = rhs_start_time;
		accumulated_ = rhs_accumulated;
		lock_.unlock();

		return *this;
	}

	stopwatch_timer<ClockType>& operator=( stopwatch_timer<ClockType>&& rhs ) noexcept
	{
		rhs.lock_.lock();
		auto rhs_running = rhs.running_;
		auto rhs_start_time = rhs.start_time_;
		auto rhs_accumulated = rhs.accumulated_;
		rhs.lock_.unlock();

		lock_.lock();
		running_ = rhs_running;
		start_time_ = rhs_start_time;
		accumulated_ = rhs_accumulated;
		lock_.unlock();

		return *this;
	}

	//! is timer running
	bool is_running( ) const
	{
		return running_;
	}

	//! restart timer
	void restart( )
	{
		lock_.lock();
		running_ = true;
		start_time_ = ClockType::now();
		accumulated_ = duration::zero();
		lock_.unlock();
	}

	//! reset timer
	void reset( )
	{
		lock_.lock();
		running_ = false;
		start_time_ = time_point::min();
		accumulated_ = duration::zero();
		lock_.unlock();
	}

	//! start timer
	void start( )
	{
		lock_.lock();
		if ( ! running_ )
		{
			start_time_ = ClockType::now();
			running_ = true;
		}
		lock_.unlock();
	}

	//! stop timer
	void stop( )
	{
		lock_.lock();
		if( running_ )
		{
			auto stop_time = ClockType::now();
			running_ = false;
			accumulated_ += ( stop_time - start_time_ );
		}
		lock_.unlock();
	}

	//! get duration of timer running
	//! @returns  duration of timer running
	template<typename T = duration>
	T value( )
	{
		T result;
		lock_.lock();
		if ( running_ )
		{
			result = std::chrono::duration_cast<T>( ( ClockType::now() - start_time_ ) + accumulated_ );
		}
		else
		{
			result = std::chrono::duration_cast<T>( accumulated_ );
		}
		lock_.unlock();
		return result;
	}
	
private:
	std::mutex lock_;
	bool running_;
	time_point start_time_;
	duration accumulated_;
};

}

#endif
