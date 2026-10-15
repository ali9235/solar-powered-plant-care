#include "../firmware/plant_care/irrigation.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
static int checks = 0;
#define CHECK(test) do { ++checks; if (!(test)) { std::cerr << "FAIL line " << __LINE__ << "\n"; std::exit(1); } } while (0)
plant::Config shortConfig() {
  plant::Config c; c.intervalMs = 100; c.soakMs = 10; c.maxRunMs = 5; return c;
}

int main() {
  CHECK(plant::moisturePercent(600, 900, 300) == 50);
  CHECK(plant::moisturePercent(600, 300, 900) == 50);
  CHECK(plant::moisturePercent(1, 900, 300) == 100);
  CHECK(plant::moisturePercent(1022, 900, 300) == 0);
  CHECK(plant::moisturePercent(0, 900, 300) == -1);
  CHECK(plant::moisturePercent(1023, 900, 300) == -1);
  CHECK(plant::moisturePercent(500, 500, 500) == -1);
  CHECK(plant::moisturePercent(-1, 900, 300) == -1);
  plant::Controller p(shortConfig()); p.begin(0);
  CHECK(!p.pumpRequested()); CHECK(!p.arm());
  p.tick(0, 20); CHECK(p.arm()); p.tick(99, 20); CHECK(!p.pumpRequested());
  p.tick(100, 20); CHECK(p.pumpRequested()); p.tick(104, 50); CHECK(p.pumpRequested());
  p.tick(105, 50); CHECK(!p.pumpRequested());
  p.tick(110, 20); CHECK(!p.pumpRequested()); p.tick(199, 20); CHECK(!p.pumpRequested());
  p.tick(200, 20); CHECK(p.pumpRequested()); p.tick(201, 65); CHECK(!p.pumpRequested());
  p.tick(300, 20); CHECK(p.pumpRequested()); p.tick(301, -1);
  CHECK(p.fault()); CHECK(!p.armed()); CHECK(!p.pumpRequested()); CHECK(!p.resetFault());
  p.tick(302, 20); CHECK(!p.pumpRequested()); CHECK(!p.arm()); CHECK(p.resetFault()); CHECK(p.arm());
  p.disarm(303); CHECK(!p.pumpRequested()); CHECK(!p.armed());
  plant::Config wetConfig = shortConfig(); plant::Controller wet(wetConfig); wet.begin(0);
  wet.tick(0, 80); CHECK(wet.arm()); wet.tick(100, 80); CHECK(!wet.pumpRequested());
  wet.tick(101, 50); CHECK(!wet.pumpRequested()); wet.tick(102, 35); CHECK(wet.pumpRequested());
  plant::Config m = shortConfig(); m.mode = plant::Mode::Moisture;
  plant::Controller moisture(m); moisture.begin(0); moisture.tick(0, 20); CHECK(moisture.arm());
  moisture.tick(9, 20); CHECK(!moisture.pumpRequested()); moisture.tick(10, 20); CHECK(moisture.pumpRequested());
  plant::Config t = shortConfig(); t.mode = plant::Mode::Timer;
  plant::Controller timer(t); timer.begin(0); timer.tick(0, 50); CHECK(timer.arm());
  timer.tick(100, 50); CHECK(timer.pumpRequested());
  plant::Config bad = shortConfig(); bad.startPercent = bad.stopPercent;
  CHECK(!bad.valid()); plant::Controller invalid(bad); invalid.tick(0, 20); CHECK(invalid.fault()); CHECK(!invalid.arm());
  const uint32_t start = 0xfffffff0UL;
  plant::Controller wrap(shortConfig()); wrap.begin(start); wrap.tick(start, 20); CHECK(wrap.arm());
  wrap.tick(uint32_t(start + 100UL), 20); CHECK(wrap.pumpRequested());
  wrap.tick(uint32_t(start + 105UL), 20); CHECK(!wrap.pumpRequested());

  std::cout << "PASS: " << checks << " core checks; no physical hardware tested.\n";
}
