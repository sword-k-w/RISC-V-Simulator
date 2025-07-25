# RISC-V Simulator in C++!

## single-stage pipeline! void me!

## Tomasulo

### idea

```
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
  void Copy(); // copy all infos except ptrs
} m1[2];

class module2 {
  friend module1;
} m2[2];

m1[0].ptr = &m2[1];
m1[0].ptr = &m2[0];
```
