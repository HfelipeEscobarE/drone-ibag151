#pragma once
#include "drone/model/ICanalDrone.hpp"

namespace ibag151 {

// Adaptador del puerto ICanalDrone: lo que se imprime en la salida estandar (cout) es lo
// que recibe el drone. No es la "vista": la vista habla con el operador (ver VistaConsola).
class CanalConsola : public ICanalDrone {
public:
    void enviar(const std::string& orden) override;
};

}  // namespace ibag151
