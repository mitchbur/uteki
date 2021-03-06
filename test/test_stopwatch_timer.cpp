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
    static constexpr std::chrono::duration<double> duration_tolerance = 6ms;

    static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_medium = 48 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_large = 77 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_xl = 120 * duration_tolerance;

    template<typename CT>
    uteki::stopwatch_timer<CT> echo( uteki::stopwatch_timer<CT> a )
    {
        return a;
    }

};

constexpr std::chrono::duration<double> Test_stopwatch_timer::duration_tolerance;
constexpr std::chrono::duration<double> Test_stopwatch_timer::sleep_duration_xs;
constexpr std::chrono::duration<double> Test_stopwatch_timer::sleep_duration_small;
constexpr std::chrono::duration<double> Test_stopwatch_timer::sleep_duration_medium;
constexpr std::chrono::duration<double> Test_stopwatch_timer::sleep_duration_large;
constexpr std::chrono::duration<double> Test_stopwatch_timer::sleep_duration_xl;

TEST_F( Test_stopwatch_timer, default_constructor )
{
    using timer_type_dc = uteki::stopwatch_timer<>;

    timer_type_dc my_timer;

    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_1 = my_timer.value();
    std::this_thread::sleep_for( sleep_duration_medium );
    decltype(sleep_duration_large) elapsed_1_2 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_small + sleep_duration_medium;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, construct_stopped )
{
    using timer_type_cs = uteki::stopwatch_timer<>;

    timer_type_cs my_timer( false );  // start=false
    EXPECT_FALSE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_medium );
    decltype(sleep_duration_medium) elapsed_1 = my_timer.value();

    my_timer.start();
    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_medium) elapsed_1_2 = my_timer.value();

    auto expected_elapsed_1 = std::chrono::duration<double>::zero();
    EXPECT_EQ( elapsed_1.count(), expected_elapsed_1.count() );
    auto expected_elapsed_1_2 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, copy_constructor )
{
    using timer_type_cc = uteki::stopwatch_timer<>;

    {
        std::atomic< timer_type_cc::duration > my_duration;
        bool duration_is_lock_free = my_duration.is_lock_free();

        std::cout << "stopwatch_timer<>::duration lock free: " <<
            ( duration_is_lock_free ? "true" : "false" ) << "\n";
    }

    timer_type_cc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_xs) elapsed_1 = my_timer.value();

    timer_type_cc other_timer( my_timer );
    EXPECT_TRUE( my_timer.is_running() );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_xs) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep_duration_xs;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_xs + sleep_duration_small;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, move_constructor )
{
    using timer_type_cc = uteki::stopwatch_timer<>;

    timer_type_cc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_xs) elapsed_1 = my_timer.value();

    timer_type_cc other_timer( std::move( my_timer ) );
    EXPECT_TRUE( my_timer.is_running() );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_xs) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep_duration_xs;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_xs + sleep_duration_small;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, copy_assignment )
{
    using timer_type_ca = uteki::stopwatch_timer<>;

    timer_type_ca my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_1 = my_timer.value();

    timer_type_ca other_timer;
    other_timer = my_timer;
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_small) elapsed_1_2 = my_timer.value();
    decltype(sleep_duration_small) elapsed_1_2_2nd = other_timer.value();

    auto expected_elapsed_1 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_small + sleep_duration_xs;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
    auto expected_elapsed_1_2_2nd = sleep_duration_small + sleep_duration_xs;
    EXPECT_NEAR( elapsed_1_2_2nd.count(), expected_elapsed_1_2_2nd.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, move_assignment )
{
    using timer_type_ma = uteki::stopwatch_timer<>;

    timer_type_ma my_timer;
    std::this_thread::sleep_for( sleep_duration_medium );
    decltype(sleep_duration_medium) elapsed_1 = my_timer.value();

    timer_type_ma other_timer;
    other_timer = std::move( my_timer );
    EXPECT_TRUE( other_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_medium) elapsed_1_2 = other_timer.value();

    auto expected_elapsed_1 = sleep_duration_medium;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_medium + sleep_duration_small;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, construction_highres )
{
    using timer_type_c_hr = uteki::stopwatch_timer< std::chrono::high_resolution_clock >;

    timer_type_c_hr my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_1 = my_timer.value();
    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_small) elapsed_1_2 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_small + sleep_duration_xs;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, restart )
{
    //! [restart stopwatch_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include "uteki/stopwatch_timer.h"
    // #include <thread>
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_large = 77 * duration_tolerance;

    using timer_type_dc = uteki::stopwatch_timer<>;

    timer_type_dc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_large );
    decltype(sleep_duration_large) elapsed_1 = my_timer.value();

    my_timer.restart();
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_large) elapsed_2 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_large;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_2 = sleep_duration_small;
    EXPECT_NEAR( elapsed_2.count(), expected_elapsed_2.count(), duration_tolerance.count() );

    //! [restart stopwatch_timer example]
}

TEST_F( Test_stopwatch_timer, timing )
{
    //! [value stopwatch_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include <thread>
    // #include "uteki/stopwatch_timer.h"
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;
    using timer_type_tm = uteki::stopwatch_timer<>;

    timer_type_tm my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    auto prev_val = timer_type_tm::duration::zero();
    for ( int k = 0; k < 5; ++k )
    {
        std::this_thread::sleep_for( sleep_duration_xs );
        auto curr_val = my_timer.value();
        EXPECT_TRUE( my_timer.is_running() );
        auto val_diff = std::chrono::duration_cast< std::chrono::duration<double> >( curr_val - prev_val );
        prev_val = curr_val;
        EXPECT_NEAR( val_diff.count(), sleep_duration_xs.count(), duration_tolerance.count() );
    }

    //! [value stopwatch_timer example]
    {
        timer_type_tm::period clk_period;
        std::cout << "clock period: " << clk_period.num << "/" << clk_period.den << "\n";
    }
}

TEST_F( Test_stopwatch_timer, stop_start )
{
    using timer_type_dc = uteki::stopwatch_timer<>;

    timer_type_dc my_timer( false );  // start=false
    EXPECT_FALSE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_xs) elapsed_0 = my_timer.value();

    my_timer.start();
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_after_start = my_timer.value();

    my_timer.start();
    EXPECT_TRUE( my_timer.is_running() );
    std::this_thread::sleep_for( sleep_duration_medium );
    decltype(sleep_duration_small) elapsed_after_start2 = my_timer.value();

    auto expected_elapsed_0 = decltype( elapsed_0 )::zero();
    EXPECT_EQ( elapsed_0.count(), expected_elapsed_0.count() );

    auto expected_elapsed_after_start = sleep_duration_small;
    EXPECT_NEAR( elapsed_after_start.count(), expected_elapsed_after_start.count(),
                duration_tolerance.count() );

    auto expected_elapsed_after_start2 = sleep_duration_small + sleep_duration_medium;
    EXPECT_NEAR( elapsed_after_start2.count(), expected_elapsed_after_start2.count(),
                2*duration_tolerance.count() );
}

TEST_F( Test_stopwatch_timer, start_stop )
{
    //! [start_stop stopwatch_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include <thread>
    // #include "uteki/stopwatch_timer.h"
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_medium = 48 * duration_tolerance;
    using timer_type_dc = uteki::stopwatch_timer<>;

    timer_type_dc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_1 = my_timer.value();

    my_timer.stop();
    EXPECT_FALSE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_medium );
    EXPECT_FALSE( my_timer.is_running() );
    decltype(sleep_duration_small) elapsed_after_stop = my_timer.value();

    my_timer.stop();
    EXPECT_FALSE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_xs );
    EXPECT_FALSE( my_timer.is_running() );
    decltype(sleep_duration_xs) elapsed_after_stop2 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );

    auto expected_elapsed_after_stop = sleep_duration_small;
    EXPECT_NEAR( elapsed_after_stop.count(), expected_elapsed_after_stop.count(),
                duration_tolerance.count() );
    EXPECT_NEAR( elapsed_1.count(), elapsed_after_stop.count(), duration_tolerance.count() );

    EXPECT_EQ( elapsed_after_stop.count(), elapsed_after_stop2.count() );

    //! [start_stop stopwatch_timer example]
}

TEST_F( Test_stopwatch_timer, reset )
{
    //! [reset_start stopwatch_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include <thread>
    // #include "uteki/stopwatch_timer.h"
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_medium = 48 * duration_tolerance;
    using timer_type_dc = uteki::stopwatch_timer<>;

    timer_type_dc my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_medium );
    decltype(sleep_duration_medium) elapsed_1 = my_timer.value();

    my_timer.reset();
    EXPECT_FALSE( my_timer.is_running() );
    decltype(sleep_duration_small) elapsed_2 = my_timer.value();

    std::this_thread::sleep_for( sleep_duration_small );
    EXPECT_FALSE( my_timer.is_running() );
    decltype(sleep_duration_small) elapsed_3 = my_timer.value();

    my_timer.start();
    EXPECT_TRUE( my_timer.is_running() );
    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_xs) elapsed_4 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_medium;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_zero = decltype(sleep_duration_small)::zero();
    EXPECT_EQ( elapsed_2.count(), expected_elapsed_zero.count() );
    EXPECT_EQ( elapsed_3.count(), expected_elapsed_zero.count() );

    auto expected_elapsed_4 = sleep_duration_xs;
    EXPECT_NEAR( elapsed_4.count(), expected_elapsed_4.count(), duration_tolerance.count() );

    //! [reset_start stopwatch_timer example]
}

TEST_F( Test_stopwatch_timer, comparison )
{
    //! [comparison stopwatch_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include "uteki/elapsed_timer.h"
    // #include <thread>
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;

    using timer_type_cmp = uteki::stopwatch_timer<>;

    timer_type_cmp timer_a;
    EXPECT_TRUE( timer_a.is_running() );

    timer_type_cmp timer_b( timer_a );
    EXPECT_TRUE( timer_b.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );

    timer_type_cmp timer_c;
    EXPECT_TRUE( timer_c.is_running() );

    //  timer_a == timer_b > timer_c
    EXPECT_TRUE( timer_a == timer_b );
    EXPECT_TRUE( timer_a != timer_c );
    EXPECT_TRUE( timer_b > timer_c );
    EXPECT_TRUE( timer_c < timer_b );
    EXPECT_TRUE( timer_b >= timer_c );
    EXPECT_TRUE( timer_c <= timer_b );

    //! [comparison stopwatch_timer example]

    EXPECT_TRUE( timer_a == timer_a );
    EXPECT_TRUE( timer_b == timer_a );
    EXPECT_TRUE( timer_b == timer_b );
    EXPECT_TRUE( timer_c == timer_c );
    EXPECT_TRUE( timer_a != timer_c );
    EXPECT_TRUE( timer_b != timer_c );
    EXPECT_TRUE( timer_c != timer_a );
    EXPECT_TRUE( timer_c != timer_b );
    EXPECT_TRUE( timer_c < timer_a );
    EXPECT_TRUE( timer_a <= timer_a );
    EXPECT_TRUE( timer_a <= timer_b );
    EXPECT_TRUE( timer_b <= timer_a );
    EXPECT_TRUE( timer_b <= timer_b );
    EXPECT_TRUE( timer_c <= timer_a );
    EXPECT_TRUE( timer_c <= timer_c );
    EXPECT_TRUE( timer_a > timer_c );
    EXPECT_TRUE( timer_a >= timer_a );
    EXPECT_TRUE( timer_a >= timer_b );
    EXPECT_TRUE( timer_a >= timer_c );
    EXPECT_TRUE( timer_b >= timer_a );
    EXPECT_TRUE( timer_b >= timer_b );
    EXPECT_TRUE( timer_c >= timer_c );
}
