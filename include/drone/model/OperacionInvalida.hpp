#pragma once
#include <stdexcept>
#include <string>

namespace ibag151 {

// Se lanza cuando se pide una operacion que el estado actual del drone no permite.
class OperacionInvalida : public std::logic_error {
public:
    explicit OperacionInvalida(const std::string& motivo) : std::logic_error(motivo) {}
};

}  // namespace ibag151
