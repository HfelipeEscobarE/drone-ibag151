#include "drone/view/VistaConsola.hpp"

#include <iostream>
#include <string>

namespace ibag151 {

VistaConsola::VistaConsola(ControladorDeComandos& controlador, std::istream& entrada,
                           std::ostream& avisos)
    : controlador_(controlador), entrada_(entrada), avisos_(avisos) {}

void VistaConsola::mostrarAyuda() const {
    avisos_ << "Drone IBag151. Comandos:";
    for (const auto& comando : controlador_.comandosDisponibles()) {
        avisos_ << ' ' << comando;
    }
    avisos_ << '\n';
}

void VistaConsola::ejecutar() {
    mostrarAyuda();

    std::string linea;
    while (std::getline(entrada_, linea)) {
        const Resultado resultado = controlador_.ejecutar(linea);
        if (!resultado.exito) {
            avisos_ << "[rechazado] " << resultado.mensaje << '\n';
        }
        if (resultado.terminar) {
            break;
        }
    }
}

}  // namespace ibag151
