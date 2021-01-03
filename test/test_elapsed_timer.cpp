//
//  test elapsed_timer C++ code
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

#include "uteki/elapsed_timer.h"
#include <iostream>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;


class Test_elapsed_timer : public ::testing::Test
{
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
    template<typename CT>
    uteki::elapsed_timer<CT> echo( uteki::elapsed_timer<CT> a )
    {
        return a;
    }

};

TEST_F( Test_elapsed_timer, default_constructor )
{
    using timer_type_dc = uteki::elapsed_timer<>;

    std::chrono::duration<double> sleep1_duration = 300ms;
    std::chrono::duration<double> sleep2_duration = 600ms;

    timer_type_dc my_timer;

    EXPECT_TRUE( my_timer.is_running() );

	std::this_thread::sleep_for( sleep1_duration );
	decltype(sleep1_duration) elapsed_1 = my_timer.value();
	std::this_thread::sleep_for( sleep2_duration );
	decltype(sleep1_duration) elapsed_1_2 = my_timer.value();

	auto expected_elapsed_1 = sleep1_duration;
	EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
	auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, copy_constructor )
{
    using timer_type_cc = uteki::elapsed_timer<>;

    {
        std::atomic< timer_type_cc::duration > my_duration;
        bool duration_is_lock_free = my_duration.is_lock_free();

        std::cout << "elapsed_timer<>::duration lock free: " <<
            ( duration_is_lock_free ? "true" : "false" ) << "\n";
    }

    std::chrono::duration<double> sleep1_duration = 150ms;
    std::chrono::duration<double> sleep2_duration = 300ms;

    timer_type_cc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep1_duration );
    decltype(sleep1_duration) elapsed_1 = my_timer.value();

    timer_type_cc other_timer( my_timer );
    EXPECT_TRUE( my_timer.is_running() );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep2_duration );
    decltype(sleep1_duration) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep1_duration;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
    auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, move_constructor )
{
    using timer_type_cc = uteki::elapsed_timer<>;

    std::chrono::duration<double> sleep1_duration = 150ms;
    std::chrono::duration<double> sleep2_duration = 300ms;

    timer_type_cc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep1_duration );
    decltype(sleep1_duration) elapsed_1 = my_timer.value();

    timer_type_cc other_timer( std::move( my_timer ) );
    EXPECT_TRUE( my_timer.is_running() );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep2_duration );
    decltype(sleep1_duration) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep1_duration;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
    auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, copy_assignment )
{
    using timer_type_ca = uteki::elapsed_timer<>;

    std::chrono::duration<double> sleep1_duration = 150ms;
    std::chrono::duration<double> sleep2_duration = 300ms;

    timer_type_ca my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep1_duration );
    decltype(sleep1_duration) elapsed_1 = my_timer.value();

    timer_type_ca other_timer;
    other_timer = my_timer;
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep2_duration );
    decltype(sleep1_duration) elapsed_1_2 = my_timer.value();
    decltype(sleep1_duration) elapsed_1_2_2nd = other_timer.value();

    auto expected_elapsed_1 = sleep1_duration;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
    auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
    auto expected_elapsed_1_2_2nd = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2_2nd.count(), expected_elapsed_1_2_2nd.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, move_assignment )
{
    using timer_type_ma = uteki::elapsed_timer<>;

    std::chrono::duration<double> sleep1_duration = 270ms;
    std::chrono::duration<double> sleep2_duration = 130ms;

    timer_type_ma my_timer;
    std::this_thread::sleep_for( sleep1_duration );
    decltype(sleep1_duration) elapsed_1 = my_timer.value();

    timer_type_ma other_timer;
    other_timer = std::move( my_timer );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep2_duration );
    decltype(sleep1_duration) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep1_duration;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
    auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, construction_highres )
{
    using timer_type_c_hr = uteki::elapsed_timer< std::chrono::high_resolution_clock >;

	std::chrono::duration<double> sleep1_duration = 320ms;
	std::chrono::duration<double> sleep2_duration = 240ms;

    timer_type_c_hr my_timer;
    EXPECT_TRUE( my_timer.is_running() );

	std::this_thread::sleep_for( sleep1_duration );
	decltype(sleep1_duration) elapsed_1 = my_timer.value();
	std::this_thread::sleep_for( sleep2_duration );
	decltype(sleep1_duration) elapsed_1_2 = my_timer.value();

	auto expected_elapsed_1 = sleep1_duration;
	EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
	auto expected_elapsed_1_2 = sleep1_duration + sleep2_duration;
	EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 12E-3 );
}

TEST_F( Test_elapsed_timer, restart )
{
    using timer_type_dc = uteki::elapsed_timer<>;

    std::chrono::duration<double> sleep1_duration = 230ms;
    std::chrono::duration<double> sleep2_duration = 120ms;

    timer_type_dc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep1_duration );
    decltype(sleep1_duration) elapsed_1 = my_timer.value();

    my_timer.restart();
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep2_duration );
    decltype(sleep1_duration) elapsed_2 = my_timer.value();

    auto expected_elapsed_1 = sleep1_duration;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), 6E-3 );
    auto expected_elapsed_2 = sleep2_duration;
    EXPECT_NEAR( elapsed_2.count(), expected_elapsed_2.count(), 6E-3 );
}

TEST_F( Test_elapsed_timer, timing )
{
    using timer_type_tm = uteki::elapsed_timer<>;
	{
        timer_type_tm::period clk_period;
		std::cout << "clock period: " << clk_period.num << "/" << clk_period.den << "\n";
	}

    timer_type_tm my_timer;
    EXPECT_TRUE( my_timer.is_running() );

	std::chrono::duration<double> step_interval = 125ms;

    auto prev_val = timer_type_tm::duration::zero();
    for ( int k = 0; k < 5; ++k )
	{
        std::this_thread::sleep_for( step_interval );
        auto curr_val = my_timer.value();
        EXPECT_TRUE( my_timer.is_running() );
        auto val_diff = std::chrono::duration_cast< std::chrono::duration<double> >( curr_val - prev_val );
        prev_val = curr_val;
        EXPECT_NEAR( val_diff.count(), step_interval.count(), 6E-3 );
	}
}
