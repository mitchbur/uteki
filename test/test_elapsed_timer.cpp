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
    static constexpr std::chrono::duration<double> duration_tolerance = 6ms;

    static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_medium = 48 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_large = 77 * duration_tolerance;
    static constexpr std::chrono::duration<double> sleep_duration_xl = 120 * duration_tolerance;

    template<typename CT>
    uteki::elapsed_timer<CT> echo( uteki::elapsed_timer<CT> a )
    {
        return a;
    }

};

constexpr std::chrono::duration<double> Test_elapsed_timer::duration_tolerance;
constexpr std::chrono::duration<double> Test_elapsed_timer::sleep_duration_xs;
constexpr std::chrono::duration<double> Test_elapsed_timer::sleep_duration_small;
constexpr std::chrono::duration<double> Test_elapsed_timer::sleep_duration_medium;
constexpr std::chrono::duration<double> Test_elapsed_timer::sleep_duration_large;
constexpr std::chrono::duration<double> Test_elapsed_timer::sleep_duration_xl;

TEST_F( Test_elapsed_timer, default_constructor )
{
    using timer_type_dc = uteki::elapsed_timer<>;

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

TEST_F( Test_elapsed_timer, copy_constructor )
{
    using timer_type_cc = uteki::elapsed_timer<>;

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

TEST_F( Test_elapsed_timer, move_constructor )
{
    using timer_type_cc = uteki::elapsed_timer<>;

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

TEST_F( Test_elapsed_timer, copy_assignment )
{
    using timer_type_ca = uteki::elapsed_timer<>;

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

TEST_F( Test_elapsed_timer, move_assignment )
{
    using timer_type_ma = uteki::elapsed_timer<>;

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

TEST_F( Test_elapsed_timer, construction_highres )
{
    using timer_type_c_hr = uteki::elapsed_timer< std::chrono::high_resolution_clock >;

    timer_type_c_hr my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_1 = my_timer.value();
    std::this_thread::sleep_for( sleep_duration_xs );
    decltype(sleep_duration_small) elapsed_1_2 = my_timer.value();

    auto expected_elapsed_1 = sleep_duration_small;
    EXPECT_NEAR( elapsed_1.count(), expected_elapsed_1.count(), duration_tolerance.count() );
    auto expected_elapsed_1_2 = sleep_duration_small + sleep_duration_xs;
    EXPECT_NEAR( elapsed_1_2.count(), expected_elapsed_1_2.count(), 2*duration_tolerance.count() );}

TEST_F( Test_elapsed_timer, restart )
{
    //! [restart elapsed_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include "uteki/elapsed_timer.h"
    // #include <thread>
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;
    // static constexpr std::chrono::duration<double> sleep_duration_large = 77 * duration_tolerance;

    uteki::elapsed_timer<> my_timer;
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_large );
    decltype(sleep_duration_large) elapsed_1_observed = my_timer.value();

    my_timer.restart();
    EXPECT_TRUE( my_timer.is_running() );

    std::this_thread::sleep_for( sleep_duration_small );
    decltype(sleep_duration_small) elapsed_2_observed = my_timer.value();

    auto elapsed_1_expected = sleep_duration_large;
    EXPECT_NEAR( elapsed_1_observed.count(), elapsed_1_expected.count(),
                duration_tolerance.count() );
    auto elapsed_2_expected = sleep_duration_small;
    EXPECT_NEAR( elapsed_2_observed.count(), elapsed_2_expected.count(),
                duration_tolerance.count() );

    //! [restart elapsed_timer example]
}

TEST_F( Test_elapsed_timer, timing )
{
    //! [value elapsed_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include "uteki/elapsed_timer.h"
    // #include <thread>
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_xs = 20 * duration_tolerance;

    using timer_type_tm = uteki::elapsed_timer<>;

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

    //! [value elapsed_timer example]
}

TEST_F( Test_elapsed_timer, comparison )
{
    //! [comparison elapsed_timer example]

    // #include <chrono>
    // using namespace std::chrono_literals;
    // #include "uteki/elapsed_timer.h"
    // #include <thread>
    //
    // static constexpr std::chrono::duration<double> duration_tolerance = 6ms;
    // static constexpr std::chrono::duration<double> sleep_duration_small = 32 * duration_tolerance;

    using timer_type_cmp = uteki::elapsed_timer<>;

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

    //! [comparison elapsed_timer example]

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
