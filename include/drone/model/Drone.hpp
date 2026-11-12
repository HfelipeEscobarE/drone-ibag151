#pragma once
#include "drone/model/ICanalDrone.hpp"
#include "drone/model/IDrone.hpp"

namespace ibag151 {

// Drone "IBag151" (Modelo). Mantiene su estado, valida las reglas y, si la operacion es
// valida, envia la orden por el canal.
class Drone : public IDrone {
public:
    static constexpr int ALTURA_DESPEGUE = 5;      // metros
    static constexpr int ALTURA_ATERRIZAJE = 5;    // metros: altura maxima para poder aterrizar
    static constexpr int ALTURA_MAXIMA = 120;      // metros
    static constexpr int PASO_ALTURA = 5;          // metros por orden
    static constexpr int VELOCIDAD_MAXIMA = 60;    // km/h
    static constexpr int PASO_VELOCIDAD = 10;      // km/h por orden
    static constexpr int GRADOS_POR_GIRO = 90;
    static constexpr int GRADOS_VUELTA_COMPLETA = 360;
    explicit Drone(ICanalDrone& canal);

    void despegar() override;
    void aterrizar() override;
    void elevar() override;
    void bajar() override;
    void girarDerecha() override;
    void girarIzquierda() override;
    void acelerar() override;
    void frenar() override;
    void descargarPaquete() override;
    void sacarFotoRecepcion() override;
    void notificarEntrega() override;

    // Consultas de estado (no modifican nada)
    bool estaEnVuelo() const { return enVuelo_; }
    int altitud() const { return altitud_; }
    int velocidad() const { return velocidad_; }
    int rumbo() const { return rumbo_; }  // 0..359 grados, 0 = rumbo inicial
    bool tienePaquete() const { return tienePaquete_; }
    bool fotoTomada() const { return fotoTomada_; }
    bool entregaNotificada() const { return entregaNotificada_; }

private:
    void exigirEnVuelo() const;

    ICanalDrone& canal_;
    bool enVuelo_ = false;
    int altitud_ = 0;
    int velocidad_ = 0;
    int rumbo_ = 0;
    bool tienePaquete_ = true;
    bool fotoTomada_ = false;
    bool entregaNotificada_ = false;
};

}  // namespace ibag151
