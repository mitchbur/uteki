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
        duration elapsed = calculate_elapsed( ClockType::now() );
        return std::chrono::duration_cast<T>( elapsed );
    }

    template< class U >
    friend bool operator==( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );
    template< class U >
    friend bool operator!=( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );
    template< class U >
    friend bool operator<( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );
    template< class U >
    friend bool operator<=( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );
    template< class U >
    friend bool operator>( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );
    template< class U >
    friend bool operator>=( const elapsed_timer<U>& lhs,
                              const elapsed_timer<U>& rhs );

private:
    std::atomic<time_point> start_time_;

    inline duration calculate_elapsed( const time_point& reftime ) const
    {
        return reftime - start_time_.load();
    }
};

//! compare if `lhs` elapsed time is equal to `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time eqauls `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator==( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) == rhs.calculate_elapsed( time_now );
}

//! compare if `lhs` elapsed time is not equal to `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time does not eqaul `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator!=( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) != rhs.calculate_elapsed( time_now );
}

//! compare if `lhs` elapsed time is less than `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time is less than `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator<( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) < rhs.calculate_elapsed( time_now );
}

//! compare if `lhs` elapsed time is less than or equal to `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time is less than or equal to `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator<=( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) <= rhs.calculate_elapsed( time_now );
}

//! compare if `lhs` elapsed time is greater than `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time is greater than `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator>( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) > rhs.calculate_elapsed( time_now );
}

//! compare if `lhs` elapsed time is greater than or equal to `rhs` elapsed time
//! @param lhs lef-hand side argument
//! @param rhs right-hand side argument
//! @returns  `true` only if `lhs` elapsed time is greater than or equal to  `rhs` elapsed time
//!
//!  \snippet test_elapsed_timer.cpp comparison elapsed_timer example
template < class ClockType >
bool operator>=( const elapsed_timer<ClockType>& lhs,
                           const elapsed_timer<ClockType>& rhs )
{
    auto time_now = ClockType::now( );
    return lhs.calculate_elapsed( time_now ) >= rhs.calculate_elapsed( time_now );
}

}

#endif
