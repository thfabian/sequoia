//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/MicroBenchmark.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/StringSwitch.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef SEQUOIA_ON_UNIX
#define SEQUOIA_PRINT_NON_ASCII
#include <unistd.h>
#endif

#ifdef SEQUOIA_ON_WIN32
#include <intrin.h>
#endif

namespace sequoia {

namespace core {

namespace {

class Accumulator {
  std::uint64_t n_;
  double sum_, sum2_;

public:
  Accumulator() : n_(0), sum_(0.0), sum2_(0.0) {}

  void operator<<(double value) {
    ++n_;
    sum_ += value;
    sum2_ += value * value;
  }

  void clear() {
    n_ = 0;
    sum_ = 0.0;
    sum2_ = 0.0;
  }

  double mean() const { return sum_ / n_; }

  double dev() const {
    double sc0 = double(n_) - 1.0;
    return std::sqrt(sum2_ / sc0 - sum_ * sum_ / sc0 / n_);
  }

  double std() const { return dev() / std::sqrt(n_); }

  std::uint64_t count() const { return n_; }

  double sum() const { return sum_; }
};

struct rdtsc_struct {
#ifdef SEQUOIA_ON_WIN32
  unsigned __int64 ticks;
  double cycle() const { return double(ticks); }
#else
  std::uint32_t hi;
  std::uint32_t lo;
  double cycle() const { return (std::uint64_t(hi) << 32) + lo; }
#endif
};

double operator-(const rdtsc_struct& end, const rdtsc_struct& begin) {
  return end.cycle() - begin.cycle();
}

class Timer {
public:
  using chrono_clock = std::chrono::high_resolution_clock;

  void start() {
    chronoStart_ = chrono_clock::now();
#ifdef SEQUOIA_ON_WIN32
    cycStart_.ticks = __rdtsc();
#else
    asm volatile("cpuid" : : "a"(0) : "bx", "cx", "dx"); // CPUID exec-barrier
    asm volatile("rdtsc" : "=a"((cycStart_).lo), "=d"((cycStart_).hi)); // RDTSC
#endif
  }

  void stop() {
#ifdef SEQUOIA_ON_WIN32
    cycStop_.ticks = __rdtsc();
#else
    asm volatile("rdtsc" : "=a"((cycStop_).lo), "=d"((cycStop_).hi)); // RDTSC
    asm volatile("cpuid" : : "a"(0) : "bx", "cx", "dx");              // CPUID exec-barrier
#endif
    chronoStop_ = chrono_clock::now();
  }

  double cycles() const { return cycStop_ - cycStart_; }

  double nsec() const {
    using usec_dur = std::chrono::duration<double, std::ratio<1, 1000000000>>;
    auto res = std::chrono::duration_cast<usec_dur>(chronoStop_ - chronoStart_);
    return res.count();
  }

private:
  rdtsc_struct cycStart_, cycStop_;
  chrono_clock::time_point chronoStart_, chronoStop_;
};

} // anonymous namespace

class MicroBenchmarker::MicroBenchmarkerImpl {
public:
  enum TimerModeKind { TK_Cycle = 0, TK_Nsec = 1 };

  struct Measurement {
    double Cycle = 0.0;
    double Nsec = 0.0;
  };

  struct Node {
    std::string Name = "";
    Node* Parent = nullptr;
    Measurement CurrentRun;
    std::set<Node*> Children;
    Accumulator Acc[2];
  };

  MicroBenchmarkerImpl() {
    sentinel_.Name = "__root__";
    stack_.push_back(&sentinel_);
  }

  void startBenchmark(const std::string& name) { processStart(name, getMeasurementOverhead()); }

  void nextBenchmark(const std::string& name) {
    processStop(stack_.back()->Name, getMeasurementOverhead());
    processStart(name, Measurement());
  }

  void stopBenchmark(const std::string& name) { processStop(name, getMeasurementOverhead()); }

  void startTimer() { timer_.start(); }

  void stopTimer() { timer_.stop(); }

  void setTimerMode(const char* mode) {
    mode_ = StringSwitch<TimerModeKind>(mode).Cases("Nsec", "nsec", TK_Nsec).Default(TK_Cycle);
  }

  void print() {
    const int width = 78;

    auto prec = outStream_.precision();
    outStream_ << std::string(width, '=') << "\n";

    std::vector<bool> marker;
    std::string timerModeStr = mode_ == TK_Cycle ? "  cycle_mean" : "   nsec_mean";

#ifdef SEQUOIA_PRINT_NON_ASCII
    outStream_ << "┬ (% of parent) name";
#else
    outStream_ << "  (% of parent) name ";
#endif
    outStream_ << std::string(width - 22 - 33, ' ') << timerModeStr << std::string(3, ' ')
               << "spread" << std::string(4, ' ') << "run_cnt"
               << "\n";

    std::size_t numChildren = sentinel_.Children.size();
    for(const auto& child : sentinel_.Children) {
      --numChildren;
      printTreeHelper(child, numChildren == 0, marker,
                      mode_ == TK_Cycle ? sentinel_.CurrentRun.Cycle : sentinel_.CurrentRun.Nsec,
                      child->Acc[mode_].mean() * child->Acc[mode_].count(), width);
    }

    outStream_ << std::string(width, '-') << "\n";
    outStream_.precision(prec);
    outStream_.flush();
  }

private:
  Measurement getMeasurementOverhead() {
    double cycle = timer_.cycles();
    double nsec = timer_.nsec();
    timer_.start();
    timer_.stop();
    cycle -= timer_.cycles();
    return Measurement{cycle, nsec};
  }

  void processStart(const std::string& name, const Measurement& measurement) {
    static bool firstStart = true;
    if(firstStart)
      firstStart = false;
    else {
      for(Node* node : stack_) {
        node->CurrentRun.Cycle += measurement.Cycle;
        node->CurrentRun.Nsec += measurement.Nsec;
      }
    }

    tree_[name].Name = name;
    tree_[name].Parent = stack_.back();
    stack_.push_back(&tree_[name]);
    tree_[name].Parent->Children.insert(&tree_[name]);
  }

  void processStop(const std::string& name, const Measurement& measurement) {
    if(name != stack_.back()->Name)
      SEQUOIA_THROW(Exception, "MicroBenchmaker: stopped '%s' instead of '%s'", name,
                    stack_.back()->Name);

    for(Node* node : stack_) {
      node->CurrentRun.Cycle += measurement.Cycle;
      node->CurrentRun.Nsec += measurement.Nsec;
    }

    Node* node = stack_.back();
    node->Acc[TK_Cycle] << node->CurrentRun.Cycle;
    node->Acc[TK_Nsec] << node->CurrentRun.Nsec;
    node->CurrentRun = Measurement();
    stack_.pop_back();
  }

  void printTreeHelper(const Node* parent, const bool lastchild, std::vector<bool>& marker,
                       const double& parent_time, const double& self_time, int width) const
      noexcept {
    for(auto m : marker) {
      if(!m)
#ifdef SEQUOIA_PRINT_NON_ASCII
        outStream_ << "│" << std::string(4 - 1, ' ');
#else
        outStream_ << " " << std::string(4 - 1, ' ');
#endif
      else
        outStream_ << std::string(4, ' ');
    }

    int percentage = std::lround(100 * self_time / parent_time);
    double spread = 100 * (parent->Acc[mode_].std() / parent->Acc[mode_].mean());

    int whitespace =
        width - 34 - 4 * (int)marker.size() - (int)parent->Name.size() - 6 - (percentage == 100);

    if(whitespace < 0)
      whitespace = 0;

#ifdef SEQUOIA_PRINT_NON_ASCII
    outStream_ << (lastchild ? "└" : "├") << "─";
#else
    outStream_ << " - ";
#endif

    // Percentage / Name
    outStream_ << (percentage < 10 ? " " : "") << percentage << "% " << parent->Name;

    // Mean
    outStream_ << std::string(whitespace, ' ') << "[" << std::fixed
               << std::setprecision(parent->Acc[mode_].mean() < 10 ? 1 : 0) << std::right
               << std::setw(10) << parent->Acc[mode_].mean();

    // Spread
    outStream_ << std::right << std::setw(8);
    if(std::isnan(spread))
      outStream_ << "- " << std::right << std::setw(12);
    else
      outStream_ << spread << "%" << std::right << std::setw(11);

    // Run count
    outStream_ << parent->Acc[mode_].count() << " ]" << std::endl;

    std::size_t numChildren = parent->Children.size();
    marker.push_back(lastchild);
    for(auto const& child : parent->Children) {
      --numChildren;
      printTreeHelper(child, numChildren == 0, marker, self_time,
                      double(child->Acc[mode_].mean() * child->Acc[mode_].count()), width);
    }

    marker.pop_back();
  }

private:
  Timer timer_;
  TimerModeKind mode_ = TK_Cycle;
  std::vector<Node*> stack_;
  std::unordered_map<std::string, Node> tree_;
  std::ostream& outStream_ = std::cout;

  Node sentinel_;
};

MicroBenchmarker* MicroBenchmarkerGlobal = nullptr;

void MicroBenchmarker::init() {
  assert(!MicroBenchmarkerGlobal && "MicroBenchmark already initialized!");
  MicroBenchmarkerGlobal = new MicroBenchmarker();
}

MicroBenchmarker::MicroBenchmarker() { impl_ = std::make_unique<MicroBenchmarkerImpl>(); }

void MicroBenchmarker::startBenchmark(const char* name) { impl_->startBenchmark(name); }

void MicroBenchmarker::stopBenchmark(const char* name) { impl_->stopBenchmark(name); }

void MicroBenchmarker::nextBenchmark(const char* name) { impl_->nextBenchmark(name); }

void MicroBenchmarker::startTimer() { impl_->startTimer(); }

void MicroBenchmarker::stopTimer() { impl_->stopTimer(); }

void MicroBenchmarker::setTimerMode(const char* mode) { impl_->setTimerMode(mode); }

void MicroBenchmarker::print() { impl_->print(); }

} // namespace core

} // namespace sequoia
