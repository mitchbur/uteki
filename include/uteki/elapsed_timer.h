//
//  elapsed_timer.h
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

#ifndef elapsed_timer_h
#define elapsed_timer_h

#include <atomic>
#include <chrono>
#include <mutex>

namespace uteki
{

//! elapsed timer
//! @details  Timer is always running. Timer can be restarted.
template< class ClockType = std::chrono::steady_clock >
class elapsed_timer
{
	static_assert( ClockType::is_steady, "must use steady clock type" );

public:
    //! scalar type for duration tick count
    using rep = typename ClockType::rep;
    //! `std::ratio` type for duration tick period, in seconds
    using period = typename ClockType::period;
    //! `std::chrono::duration<rep, period>` type represents a duration
    using duration = typename ClockType::duration;
    //! reference clock type's `time_point` type, represents a point in time
    using time_point = typename ClockType::time_point;

    //! constructor
    elapsed_timer( )
        : start_time_( ClockType::now() )
    {
    }

    //! copy constructor
    explicit elapsed_timer( elapsed_timer& other ) noexcept
        : start_time_( other.start_time_.load() )
    {
    }

    //! move constructor
    explicit elapsed_timer( elapsed_timer&& other ) noexcept
        : start_time_( other.start_time_.load() )
    {
    }

    //! destructor
    ~elapsed_timer( ) = default;

    //! copy assignment
    elapsed_timer& operator=( elapsed_timer& rhs ) noexcept
    {
        start_time_ = rhs.start_time_.load();
        return *this;
    }

    //! move assignment
    elapsed_timer& operator=( elapsed_timer&& rhs ) noexcept
    {
        start_time_ = rhs.start_time_.load();
        return *this;
    }

    //! is timer running
    //! @returns  `true`
    constexpr bool is_running( ) const
    {
        return true;
    }

    //! restart timer
    //!
    //!  \snippet  test_elapsed_timer.cpp restart elapsed_timer example
    void restart( )
    {
        start_time_ = ClockType::now();
    }

    //! get timer value
    //! @returns  duration of timer running
    //!
    //!  \snippet test_elapsed_timer.cpp value elapsed_timer example
    template<typename T = duration>
    T value( )
    {
        duration elapsed = ClockType::now() - start_time_.load();
        return std::chrono::duration_cast<T>( elapsed );
    }

private:
    std::atomic<time_point> start_time_;
};

}

#endif
