#include "simulator.h"

int32_t main(int32_t argc, char **argv) {
#ifdef LOCAL
  freopen("input.txt", "r", stdin);
  // freopen("log.txt", "w", stderr);
#endif

  sjtu::Simulator sim;
  sim.Init(argc, argv);
  while (true) {
    sim.Run();
  }
}