//
//  stopwatch_timer.h
//
//  Copyright © 2021 Mitchell Burghart.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef stopwatch_timer_h
#define stopwatch_timer_h

#include <chrono>
#include <mutex>
using namespace std::chrono_literals;

namespace uteki
{

//! stopwatch  timer class
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
    //! @details constructs and starts timer. For the default constructor the
    //! timer is started to be consistent with the `elapsed_timer`.
    stopwatch_timer( )
        : lock_( )
        , running_( true )
        , start_time_( ClockType::now() )
        , accumulated_( duration::zero() )
    {}

    //! constructor
    //!  @param  start    initial running state
    stopwatch_timer( bool start )
        :lock_( )
        , running_( start )
        , start_time_( ClockType::now() )
        , accumulated_( duration::zero() )
    {}

    //! copy constructor
    explicit stopwatch_timer( stopwatch_timer& other ) noexcept
        : lock_( )
    {
        other.lock_.lock();
        running_ = other.running_ ;
        start_time_ = other.start_time_;
        accumulated_ = other.accumulated_;
        other.lock_.unlock();
    }

	//! move constructor
    explicit stopwatch_timer( stopwatch_timer&& other ) noexcept
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

    ~stopwatch_timer( ) = default;

    //! copy assignment
    stopwatch_timer& operator=( stopwatch_timer& rhs ) noexcept
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

    //! move assignment
    stopwatch_timer& operator=( stopwatch_timer&& rhs ) noexcept
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
        start_time_ = ClockType::now();
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
        if ( running_ )
        {
            auto stop_time = ClockType::now();
            running_ = false;
            accumulated_ += ( stop_time - start_time_ );
        }
        lock_.unlock();
    }

    //! get elapsed time
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
