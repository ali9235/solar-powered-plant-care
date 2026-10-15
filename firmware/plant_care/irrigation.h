#pragma once
#include <stdint.h>

namespace plant {
enum class Mode { Moisture, Timer, TimerAndMoisture };
struct Config {
  Mode mode = Mode::TimerAndMoisture;
  int startPercent = 35, stopPercent = 65;
  uint32_t intervalMs = 21600000UL, soakMs = 60000UL, maxRunMs = 5000UL;
  bool valid() const {
    return startPercent >= 0 && stopPercent <= 100 && startPercent < stopPercent &&
      intervalMs > 0 && intervalMs < 0x80000000UL && soakMs < 0x80000000UL &&
      maxRunMs > 0 && maxRunMs <= 60000UL;
  }
};
inline int moisturePercent(int raw, int dry, int wet) {
  if (raw <= 0 || raw >= 1023 || dry < 0 || dry > 1023 || wet < 0 || wet > 1023 || dry == wet)
    return -1;
  long value = (static_cast<long>(raw) - dry) * 100L / (wet - dry);
  return value < 0 ? 0 : value > 100 ? 100 : static_cast<int>(value);
}
class Controller {
 public:
  explicit Controller(Config config = Config()) : config_(config) {}
  void begin(uint32_t now) { boot_ = true; armed_ = running_ = fault_ = false;
    healthy_ = false; lastStop_ = lastCycle_ = now; }
  bool arm() { if (!healthy_ || fault_ || !config_.valid()) return false; armed_ = true; return true; }
  void disarm(uint32_t now) { stop(now); armed_ = false; }
  bool resetFault() { if (armed_ || !healthy_ || !config_.valid()) return false; fault_ = false; return true; }
  void tick(uint32_t now, int moisture) {
    if (!boot_) begin(now);
    healthy_ = moisture >= 0 && moisture <= 100;
    if (!config_.valid() || !healthy_) {
      stop(now); fault_ = true; armed_ = false; return;
    }
    if (!armed_ || fault_) { stop(now); return; }
    if (running_) {
      if (moisture >= config_.stopPercent || elapsed(now, started_) >= config_.maxRunMs)
        stop(now);
      return;
    }
    if (elapsed(now, lastStop_) < config_.soakMs || moisture >= config_.stopPercent) return;
    const bool dry = moisture <= config_.startPercent;
    const bool due = elapsed(now, lastCycle_) >= config_.intervalMs;
    const bool request = config_.mode == Mode::Moisture ? dry :
      config_.mode == Mode::Timer ? due : due && dry;
    if (request) { running_ = true; started_ = lastCycle_ = now; }
  }
  bool pumpRequested() const { return running_ && armed_ && !fault_; }
  bool armed() const { return armed_; }
  bool fault() const { return fault_; }
 private:
  static uint32_t elapsed(uint32_t now, uint32_t before) { return uint32_t(now - before); }
  void stop(uint32_t now) { if (running_) lastStop_ = now; running_ = false; }
  Config config_;
  bool boot_ = false, healthy_ = false, armed_ = false, running_ = false, fault_ = false;
  uint32_t started_ = 0, lastStop_ = 0, lastCycle_ = 0;
};
}  // namespace plant
