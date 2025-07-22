#include "interpreter.h"

int main() {
  freopen("input.txt", "r", stdin);

  sjtu::Interpreter interpreter;
  interpreter.Run();
  return 0;
}