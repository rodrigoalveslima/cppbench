// Copyright (C) 2022 Georgia Tech Center for Experimental Research in Computer
// Systems

#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Write log message `x` to stdout tagged with the current timestamp.
#define LOGMSG(x) \
  std::cout << "[" << current_timestamp() << "] " << x << std::endl

// Return a string with the current timestamp formatted as '%Y-%m-%d %H:%M:%S'.
inline std::string current_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_c);
  std::ostringstream now_stream;
  now_stream << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
  return now_stream.str();
}

// Execution log of the benchmarked function.
typedef struct ExecutionLog {
  double timestamp; /* time elapsed since benchmark beginning (in seconds) */
  double latency;   /* function execution time (in seconds) */
} ExecutionLog;

// Run `function` at most `max_executions` times (limited by `max_duration`
// seconds) and write the execution logs to `output_filepath` in CSV format.
// If specified, `set_up` and `tear_down` are run before and after each
// `function` execution, respectively.
template <typename T>
void run_benchmark(const std::function<T()>& function,
                   const std::function<void()>& set_up,
                   const std::function<void()>& tear_down,
                   const int max_executions, const int max_duration,
                   const std::string& output_filepath) {
  LOGMSG("Starting benchmark.");
  std::vector<ExecutionLog> logs;
  auto t0 = std::chrono::steady_clock::now();
  for (auto i = 1; i <= max_executions; i++) {
    if (set_up != nullptr) set_up();
    auto start_time = std::chrono::steady_clock::now();
    function();
    std::chrono::duration<double> latency =
        std::chrono::steady_clock::now() - start_time;
    if (tear_down != nullptr) tear_down();
    std::chrono::duration<double> timestamp = start_time - t0;
    logs.push_back(ExecutionLog{timestamp.count(), latency.count()});
    if (timestamp.count() > max_duration) {
      LOGMSG("Benchmark timed out after " + std::to_string(i) + " executions.");
      break;
    }
  }
  LOGMSG("Writing execution logs to the output file.");
  std::ofstream output_file;
  output_file.open(output_filepath);
  output_file << "timestamp,latency" << std::endl;
  for (const auto& log : logs)
    output_file << std::fixed << std::setprecision(9) << log.timestamp << ","
                << log.latency << std::endl;
  output_file.close();
}
