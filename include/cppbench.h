// Copyright (C) 2020 Georgia Tech Center for Experimental Research in Computer
// Systems

#include <ctime>

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// Logging and debugging utilities.
#define LOGMSG(x) std::cout << timestamp() << " " << x << std::endl
// #define DEBUG 1
#ifdef DEBUG
  #define DEBUGMSG(x) LOGMSG(x)
#else
  #define DEBUGMSG(x)
#endif
inline std::string timestamp() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_c);
  std::ostringstream now_stream;
  now_stream << "[" << std::put_time(&now_tm, "%c") << "]";
  return now_stream.str();
}


// Execution log of the benchmarked function.
//
// Attributes:
// - timestamp_milli: timestamp (since the benchmark started) in milliseconds.
// - exec_time_milli: execution time in milliseconds.
typedef struct BenchmarkExecutionLog {
  double timestamp_milli;
  double exec_time_milli;
} BenchmarkExecutionLog;


// Execute the specified function at most 'max_executions' times (limited by
// 'max_duration' seconds) and write the execution logs in CSV format to the
// specified output file.
//
// Parameters:
// - function: function to be benchmarked.
// - set_up: function to be executed before the benchmarked function.
// - tear_down: function to be executed after the benchmarked function.
// - max_executions: max number of executions.
// - max_duration: max duration in seconds.
// - output_filepath: path to the output file.
void run_benchmark(const std::function<void()>& function,
    const std::function<void()>& set_up, const std::function<void()>& tear_down,
    const int& max_executions, const int& max_duration,
    const std::string& output_filepath) {
  // Log benchmark configuration.
  LOGMSG("Starting benchmark...\n"
      "  Max duration in seconds: " + std::to_string(max_duration) + "\n"
      "  Max number of executions: " + std::to_string(max_executions) + "\n"
      "  Output filepath: " + output_filepath);
  // Execute benchmarked function.
  LOGMSG("Executing benchmarked function...");
  std::vector<BenchmarkExecutionLog> logs;
  auto t0 = std::chrono::steady_clock::now();
  for (auto i = 1; i <= max_executions; i++) {
    if (set_up != nullptr)
      set_up();
    auto start_time = std::chrono::steady_clock::now();
    function();
    std::chrono::duration<double, std::milli> exec_time_milli = \
        std::chrono::steady_clock::now() - start_time;
    if (tear_down != nullptr)
      tear_down();
    std::chrono::duration<double, std::milli> timestamp_milli = start_time - t0;
    logs.push_back(BenchmarkExecutionLog{timestamp_milli.count(),
        exec_time_milli.count()});
    if (timestamp_milli.count() > max_duration * 1000) {
      LOGMSG("Benchmark timed out.");
      break;
    }
  }
  // Write execution logs to the output file in CSV format.
  LOGMSG("Writing logs to the output file...");
  std::ofstream output_file;
  output_file.open(output_filepath);
  output_file << "timestamp_milli,exec_time_milli" << std::endl;
  for (const auto& log : logs)
    output_file << std::fixed << std::setprecision(3) <<
        log.timestamp_milli << "," << log.exec_time_milli << std::endl;
  output_file.close();
}
