#include "simulator.h"

int main() {
  sjtu::Simulator sim;
  sim.Init();
  while (true) {
    sim.Run();
  }
}