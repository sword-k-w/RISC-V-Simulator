#include "interpreter.h"

int32_t main() {
#ifdef LOCAL
  freopen("input.txt", "r", stdin);
  freopen("log_naive.txt", "w", stderr);
#endif

  sjtu::Interpreter interpreter;
  interpreter.Run();
  return 0;
}