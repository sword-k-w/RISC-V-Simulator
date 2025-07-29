#include "simulator.h"

int main() {
#ifdef DEBUG
  freopen("input.txt", "r", stdin);
#endif

  sjtu::Simulator sim;
  sim.Init();
  while (true) {
    sim.Run();
  }
}