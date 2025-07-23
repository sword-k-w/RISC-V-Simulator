#include "interpreter.h"

int main() {
#ifdef LOCAL
  freopen("input.txt", "r", stdin);
#endif

  sjtu::Interpreter interpreter;
  interpreter.Run();
  return 0;
}