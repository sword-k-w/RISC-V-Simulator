#include "Tomasulo_module/register_file.h"
#include <cstring>

namespace sjtu {

RegisterFile::RegisterFile() {
  memset(dependence_, -1, sizeof(dependence_));
}


}