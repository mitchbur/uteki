//
//  elapsed_timer.h
//
//  Created by Mitch Burghart on 2020-12-31.
//  Copyright © 2021 Mitchell Burghart. All rights reserved.
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
