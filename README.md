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

| testcase \ predictor | local | global | two_level adaptive |  tournament  |
|:--------------------:|:-----:|:------:|:------------------:|:------------:|
|     array_test1      |   %   |   85   |         92         |              |
|     array_test2      |  88   |   95   |         89         |              |
|      basicopt1       |  92   |   88   |         94         |              |
