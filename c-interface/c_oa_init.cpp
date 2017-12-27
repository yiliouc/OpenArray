
#include "../Array.hpp"
#include "../Init.hpp"

extern "C"{
  void c_init(int fcomm, int* procs_shape){
    Shape ps;
    ps[0] = procs_shape[0];
    ps[1] = procs_shape[1];
    ps[2] = procs_shape[2];
    oa::init(fcomm, ps);
  }
}
