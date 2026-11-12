#pragma once
#include <string>

namespace ibag151 {

// Puerto de comunicacion con el drone: todo texto que se envia por aca "viaja" hacia el
// drone y este lo ejecuta. El modelo depende de esta abstraccion, no de la consola (DIP).
class ICanalDrone {
public:
    virtual ~ICanalDrone() = default;
    virtual void enviar(const std::string& orden) = 0;
};

}  // namespace ibag151
