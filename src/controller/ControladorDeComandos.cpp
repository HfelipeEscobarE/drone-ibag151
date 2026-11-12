#include "drone/controller/ControladorDeComandos.hpp"

#include <algorithm>
#include <cctype>

#include "drone/model/OperacionInvalida.hpp"

namespace ibag151 {

namespace {

const char* const COMANDO_SALIR = "salir";

// Quita espacios de los extremos y pasa a minusculas: "  DESPEGAR " == "despegar".
std::string normalizar(const std::string& texto) {
    const auto esEspacio = [](unsigned char c) { return std::isspace(c) != 0; };
    const auto inicio = std::find_if_not(texto.begin(), texto.end(), esEspacio);
    const auto fin = std::find_if_not(texto.rbegin(), texto.rend(), esEspacio).base();
    std::string limpio = (inicio < fin) ? std::string(inicio, fin) : std::string();
    std::transform(limpio.begin(), limpio.end(), limpio.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return limpio;
}

}  // namespace

ControladorDeComandos::ControladorDeComandos(IDrone& drone) {
    comandos_ = {
        {"despegar", [&drone] { drone.despegar(); }},
        {"aterrizar", [&drone] { drone.aterrizar(); }},
        {"elevar", [&drone] { drone.elevar(); }},
        {"bajar", [&drone] { drone.bajar(); }},
        {"derecha", [&drone] { drone.girarDerecha(); }},
        {"izquierda", [&drone] { drone.girarIzquierda(); }},
        {"acelerar", [&drone] { drone.acelerar(); }},
        {"frenar", [&drone] { drone.frenar(); }},
        {"descargar", [&drone] { drone.descargarPaquete(); }},
        {"foto", [&drone] { drone.sacarFotoRecepcion(); }},
        {"notificar", [&drone] { drone.notificarEntrega(); }},
    };
}

Resultado ControladorDeComandos::ejecutar(const std::string& comando) const {
    const std::string clave = normalizar(comando);
    if (clave.empty()) {
        return {true, "Linea vacia", false};
    }
    if (clave == COMANDO_SALIR) {
        return {true, "Hasta luego", true};
    }
    const auto it = comandos_.find(clave);
    if (it == comandos_.end()) {
        return {false, "Comando desconocido: " + comando, false};
    }
    try {
        it->second();
    } catch (const OperacionInvalida& error) {
        return {false, error.what(), false};
    }
    return {true, "OK", false};
}

std::vector<std::string> ControladorDeComandos::comandosDisponibles() const {
    std::vector<std::string> nombres;
    for (const auto& par : comandos_) {
        nombres.push_back(par.first);
    }
    nombres.push_back(COMANDO_SALIR);
    return nombres;
}

}  // namespace ibag151
