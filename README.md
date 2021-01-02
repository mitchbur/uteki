# uteki
A small simple C++ template library currently containing:
1. elapsed_timer
2. stopwatch_timer 

The `elapsed_timer` class is a simple elapsed timer. The timer starts when created and is always running; there is no start/stop control. The timer's `value()` method when called returns a `std::chrono::duration` representation of the elapsed time. The `restart()` method resets the the timer's elapsed time to 0 but the timer continues to run.

The `stopwatch_timer` class behaves the same as the `elapsed_timer` but can also be stopped and started using the methods `stop()` and `start()` respectively. There is also a `reset()` method that stops the timer and resets the timer's start time.

Both the `elapsed_timer` and `stopwatch_timer` classes are thread-safe.
