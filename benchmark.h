#include <iostream>
#include <chrono>
// Simple benchmark library to time the execution of a function
// To get time: executionTime = report_time(functionName (str), function, arguments e.g group)
// Can also just use benchmark but report time will output time to cl
using namespace std;
template <typename Function, typename... Args>
auto benchmark(Function fn, Args &&...args)
{
    // start time
    auto start = chrono::high_resolution_clock::now();

    // unpack arguments and forward to function to run
    fn(forward<Args>(args)...);

    // end time
    auto end = chrono::high_resolution_clock::now();

    // calculate result
    return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}

template <typename Function, typename... Args>
auto report_time(const string &fn_name, Function fn, Args &&...args)
{
    // Run benchmark using passed function
    auto duration = benchmark(fn, forward<Args>(args)...);
    // Output time
    cout << "Time taken by " << fn_name << ": " << duration << " nanosecs\n";
    return duration;
}