#include <iostream>

#include "drone/controller/ControladorDeComandos.hpp"
#include "drone/model/Drone.hpp"
#include "drone/other/CanalConsola.hpp"
#include "drone/view/VistaConsola.hpp"

// Raiz de composicion: unico lugar donde se eligen las clases concretas y se las conecta.
// Por cout salen solo las ordenes hacia el drone; los avisos al operador van por cerr.
int main() {
    ibag151::CanalConsola canal;
    ibag151::Drone drone(canal);
    ibag151::ControladorDeComandos controlador(drone);
    ibag151::VistaConsola vista(controlador, std::cin, std::cerr);

    vista.ejecutar();
    return 0;
}
