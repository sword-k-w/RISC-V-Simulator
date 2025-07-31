# RISC-V Simulator in C++!

## Tomasulo

### idea

``` c++
class module1 {
private:
  type info1;
  ...  // infos from other module(last cycle)
  
  type storage;
  ...  // stuffs it actually stores
  
  module2 *ptr;
  ...  // point to other modules and pass info to next cycle
public:
  void Run(); // simulate single cycle
  void Copy(const module1 &); // copy all infos except ptrs
} m1[2];

class module2 {
  friend class module1;
} m2[2];

m1[0].ptr = &m2[1];

```

### Predictor

| testcase \ predictor | local  | global | two_level adaptive | tournament |
|:--------------------:|:------:|:------:|:------------------:|:----------:|
|     array_test1      | 54.55% | 54.55% |       59.09%       |   54.55%   |
|     array_test2      | 53.85% | 53.85% |       57.69%       |   53.85%   |
|      basicopt1       | 82.40% | 59.11% |       93.47%       |   80.42%   |
|      bulgarian       | 94.43% | 58.14% |       94.47%       |   93.96%   |
|         expr         | 83.78% | 57.66% |       81.08%       |   82.88%   |
|         gcd          | 66.67% | 56.67% |       69.17%       |   65.83%   |
|        hanoi         | 61.09% | 50.02% |       86.53%       |   61.08%   |
|       lvalue2        | 33.33% | 33.33% |       33.33%       |   33.33%   |
|        magic         | 78.41% | 32.95% |       83.45%       |   77.14%   |
|     manyargument     | 20.00% | 60.00% |       20.00%       |   20.00%   |
|      multiarray      | 85.19% | 80.86% |       84.57%       |   85.19%   |
|        naive         |   -    |   -    |         -          |     -      |
|          pi          | 82.40% | 59.73% |       84.37%       |   80.97%   |
|        qsort         | 87.42% | 73.48% |       91.86%       |   86.03%   |
|        queens        | 73.36% | 57.56% |       73.64%       |   72.58%   |
|    statement_test    | 56.44% | 55.94% |       57.43%       |   50.99%   |
|      superloop       | 93.82% | 86.20% |       95.15%       |   93.24%   |
|         tak          | 73.80% | 73.81% |       76.88%       |   68.64%   |
