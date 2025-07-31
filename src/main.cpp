#include "simulator.h"

int32_t main() {
#ifdef LOCAL
  freopen("input.txt", "r", stdin);
  freopen("log.txt", "w", stderr);
#endif

  sjtu::Simulator sim;
  sim.Init();
  while (true) {
    sim.Run();
  }
}