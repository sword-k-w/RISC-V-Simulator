#include "interpreter.h"

int main() {
#ifdef LOCAL
  freopen("input.txt", "r", stdin);
  freopen("log_naive.txt", "w", stderr);
#endif

  sjtu::Interpreter interpreter;
  interpreter.Run();
  return 0;
}