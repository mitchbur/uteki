//
//  test stopwatch_timer C++ code
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

#include "uteki/stopwatch_timer.h"
#include <iostream>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;


class Test_stopwatch_timer : public ::testing::Test
{
public:
	using timer_type = uteki::stopwatch_timer<>;

protected:

	Test_stopwatch_timer()
	{
	 // common set-up work for each test
	}

	~Test_stopwatch_timer() override
	{
		// common clean-up work that doesn't throw exceptions
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:


	void SetUp() override
	{
		// Code here will be called immediately after the constructor (right
		// before each test).

	}

	void TearDown() override
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

public:

	static unsigned long test_loop_count( std::chrono::duration<double>& loop_time )
	{
		timer_type my_timer;
		unsigned long loops = 0;
		my_timer.reset();
		my_timer.start();
		for ( unsigned long k = 0; k < 1E9; ++k )
		{
			if ( my_timer.value() >= loop_time )
			{
				loops = k;
				break;
			}
		}
		my_timer.stop();
		return loops;
	}
};

TEST_F( Test_stopwatch_timer, BasicTiming )
{
	{
		std::chrono::steady_clock::period clk_period;
		std::cout << "clock period: " << clk_period.num << "/" << clk_period.den << "\n";
	}

	std::chrono::duration<double> step_interval = 666666666.6666667ns;

	timer_type o1_timer;
	o1_timer.reset();   // to stop and reset timer
	timer_type outside_timer( std::move( o1_timer ) );
	bool toggle_start = false;
	std::chrono::duration<double> accum_duration;
	std::cout << "tribble( ~ns, ~loops";
	for ( std::chrono::duration<double> test_time = 1s;
		 test_time <= 3s; test_time += step_interval )
	{
		// alternate start-stop of outside timer for each case
		if ( toggle_start )
		{
			outside_timer.start();
			accum_duration += test_time;
		}
		toggle_start ^= true;
		auto loop_count = test_loop_count( test_time );
		outside_timer.stop();

		std::cout << ",\n" <<
		std::chrono::duration_cast<std::chrono::nanoseconds>(test_time).count() <<
		", " << loop_count;
	}
	std::cout << " )" << "\n";

	std::chrono::duration<double> outside_elapsed = outside_timer.value();
	std::cout << "outside time: " << outside_elapsed.count()  << " s" << "\n";

	EXPECT_NEAR( outside_elapsed.count(), accum_duration.count(), 1E-3 );
}
