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

#include <chrono>
#include <mutex>
using namespace std::chrono_literals;

namespace uteki
{

//! elapsed timer
//! @details  Timer starts when constructed and is always running.
//!     Timer can be restarted.
template< class ClockType = std::chrono::steady_clock >
class elapsed_timer
{
	static_assert( ClockType::is_steady, "must use steady clock type" );

public:
    using rep = typename ClockType::rep;
    using period = typename ClockType::period;
    using duration = typename ClockType::duration;
    using time_point = typename ClockType::time_point;

    //! constructor
    //! @details constructs and starts timer
    elapsed_timer( )
        : start_time_( ClockType::now() )
    {
    }

    //! copy constructor
    elapsed_timer( elapsed_timer<ClockType>& other ) noexcept = default;

    //! move constructor
    elapsed_timer( elapsed_timer<ClockType>&& other ) noexcept = default;

    elapsed_timer<ClockType>& operator=( elapsed_timer<ClockType>& rhs ) noexcept = default;

    elapsed_timer<ClockType>& operator=( elapsed_timer<ClockType>&& rhs ) noexcept = default;

    //! is timer running
    constexpr bool is_running( ) const
    {
        return true;
    }

    //! restart timer
    void restart( )
    {
        start_time_ = ClockType::now();
    }

    //! get duration of timer running
    //! @returns  duration of timer running
    template<typename T = duration>
    T value( )
    {
        return std::chrono::duration_cast<T>( ClockType::now() - start_time_ );
    }

private:
    time_point start_time_;
};

}

#endif
