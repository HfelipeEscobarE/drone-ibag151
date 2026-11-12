#pragma once
#include <iosfwd>

#include "drone/controller/ControladorDeComandos.hpp"

namespace ibag151 {

// Vista (MVC): es lo que ve y escribe el OPERADOR. Lee las ordenes, se las pasa al
// controlador y muestra los avisos (ayuda y rechazos).
// Los flujos se inyectan: en la app real son cin y cerr; en las pruebas, stringstream.
// Los avisos NO van por el canal del drone, asi al drone solo le llegan ordenes validas.
class VistaConsola {
public:
    VistaConsola(ControladorDeComandos& controlador, std::istream& entrada,
                 std::ostream& avisos);

    void ejecutar();

private:
    void mostrarAyuda() const;

    ControladorDeComandos& controlador_;
    std::istream& entrada_;
    std::ostream& avisos_;
};

}  // namespace ibag151
