#include "drone/other/CanalConsola.hpp"

#include <iostream>

namespace ibag151 {

void CanalConsola::enviar(const std::string& orden) {
    std::cout << orden << std::endl;  // endl: cada orden sale de inmediato hacia el drone
}

}  // namespace ibag151
