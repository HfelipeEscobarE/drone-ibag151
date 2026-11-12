#pragma once
#include <string>
#include <vector>

#include "drone/model/ICanalDrone.hpp"

// Doble de prueba: en vez de imprimir, guarda lo que el drone "recibiria".
class CanalFalso : public ibag151::ICanalDrone {
public:
    void enviar(const std::string& orden) override { ordenes_.push_back(orden); }

    std::size_t cantidad() const { return ordenes_.size(); }
    const std::string& ultima() const { return ordenes_.back(); }
    const std::vector<std::string>& todas() const { return ordenes_; }

private:
    std::vector<std::string> ordenes_;
};
