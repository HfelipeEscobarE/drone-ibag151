#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "drone/model/IDrone.hpp"

namespace ibag151 {

// Respuesta del controlador a una linea de texto.
struct Resultado {
    bool exito;
    std::string mensaje;
    bool terminar;  // true cuando el operador pidio salir
};

// Controlador (MVC y GRASP): traduce el texto que escribe el operador en una llamada al
// drone. No imprime ni lee nada: devuelve un Resultado y la vista decide como mostrarlo.
class ControladorDeComandos {
public:
    explicit ControladorDeComandos(IDrone& drone);

    Resultado ejecutar(const std::string& comando) const;

    // Comandos que entiende (incluye "salir"), en orden alfabetico.
    std::vector<std::string> comandosDisponibles() const;

private:
    std::map<std::string, std::function<void()>> comandos_;
};

}  // namespace ibag151
