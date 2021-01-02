//
//  test elapsed_timer C++ code
//
//  Created by Mitch Burghart on 2021-01-01
//  Copyright © 2021 Mitchell Burghart. All rights reserved.
//

#include "uteki/elapsed_timer.h"
#include <iostream>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;


class Test_elapsed_timer : public ::testing::Test
{
public:
	using timer_type = uteki::elapsed_timer<>;

protected:

	Test_elapsed_timer()
	{
	 // common set-up work for each test
	}

	~Test_elapsed_timer() override
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
		for ( unsigned long k = 0; k < 1E9; ++k )
		{
			if ( my_timer.value() >= loop_time )
			{
				loops = k;
				break;
			}
		}
		return loops;
	}
};

// std::this_thread::sleep_for(2000ms);

TEST_F( Test_elapsed_timer, construction )
{
	std::chrono::duration<double> sleep1_duration = 300ms;
	std::chrono::duration<double> sleep2_duration = 600ms;

	uteki::elapsed_timer<> my_timer;
	std::this_thread::sleep_for( sleep1_duration );
	decltype(sleep1_duration) elapsed_1 = my_timer.value();
	std::this_thread::sleep_for( sleep2_duration );
	decltype(sleep1_duration) elapsed_1_2 = my_timer.value();

	auto expected_elapsed_1 = sleep1_duration;
	EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
	auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
	EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, construction_b )
{
	std::chrono::duration<double> sleep1_duration = 300ms;
	std::chrono::duration<double> sleep2_duration = 600ms;

	uteki::elapsed_timer< std::chrono::high_resolution_clock > my_timer;
	std::this_thread::sleep_for( sleep1_duration );
	decltype(sleep1_duration) elapsed_1 = my_timer.value();
	std::this_thread::sleep_for( sleep2_duration );
	decltype(sleep1_duration) elapsed_1_2 = my_timer.value();

	auto expected_elapsed_1 = sleep1_duration;
	EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
	auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
	EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, BasicTiming )
{
	{
		std::chrono::steady_clock::period clk_period;
		std::cout << "clock period: " << clk_period.num << "/" << clk_period.den << "\n";
	}

	std::chrono::duration<double> step_interval = 666666666.6666667ns;

	std::cout << "tribble( ~ns, ~loops";
	for ( std::chrono::duration<double> test_time = 1s;
		 test_time <= 3s; test_time += step_interval )
	{
		auto loop_count = test_loop_count( test_time );

		std::cout << ",\n" <<
		std::chrono::duration_cast<std::chrono::nanoseconds>(test_time).count() <<
		", " << loop_count;
	}
	std::cout << " )" << "\n";

	// EXPECT_NEAR( outside_elapsed.count(), accum_duration.count(), 1E-3 );
}
