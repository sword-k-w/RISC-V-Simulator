#include "simulator.h"

int main() {
#ifdef DEBUG
  freopen("input.txt", "r", stdin);
  freopen("log.txt", "w", stderr);
#endif

  sjtu::Simulator sim;
  sim.Init();
  while (true) {
    sim.Run();
  }
}