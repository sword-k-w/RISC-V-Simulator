#include "Tomasulo_module/load_store_buffer.h"

namespace sjtu {

void LoadStoreBuffer::Run() {
  if (predict_failed_) {
    head_ = 0;
    tail_ = 0;
    predict_failed_ = false;
  }
}

}