#include "drone/model/Drone.hpp"

#include "drone/model/OperacionInvalida.hpp"

namespace ibag151 {

Drone::Drone(ICanalDrone& canal) : canal_(canal) {}

void Drone::exigirEnVuelo() const {
    if (!enVuelo_) {
        throw OperacionInvalida("El drone esta en tierra: primero debe despegar");
    }
}

void Drone::despegar() {
    if (enVuelo_) {
        throw OperacionInvalida("El drone ya esta en vuelo");
    }
    enVuelo_ = true;
    altitud_ = ALTURA_DESPEGUE;
    canal_.enviar("DESPEGAR");
}

void Drone::aterrizar() {
    exigirEnVuelo();
    if (velocidad_ != 0) {
        throw OperacionInvalida("Hay que frenar por completo antes de aterrizar");
    }
    if (altitud_ > ALTURA_ATERRIZAJE) {
        throw OperacionInvalida("Hay que bajar hasta la altura de aterrizaje antes de aterrizar");
    }

    enVuelo_ = false;
    altitud_ = 0;
    canal_.enviar("ATERRIZAR");
}

void Drone::elevar() {
    exigirEnVuelo();
    if (altitud_ + PASO_ALTURA > ALTURA_MAXIMA) {
        throw OperacionInvalida("Altura maxima alcanzada");
    }
    altitud_ += PASO_ALTURA;
    canal_.enviar("ELEVAR");
}

void Drone::bajar() {
    exigirEnVuelo();
    if (altitud_ - PASO_ALTURA < 1) {
        throw OperacionInvalida("Altura minima de vuelo: para llegar al suelo use aterrizar");
    }
    altitud_ -= PASO_ALTURA;
    canal_.enviar("BAJAR");
}

void Drone::girarDerecha() {
    exigirEnVuelo();
    rumbo_ = (rumbo_ + GRADOS_POR_GIRO) % 360;
    canal_.enviar("GIRAR_DERECHA");
}

void Drone::girarIzquierda() {
    exigirEnVuelo();
    rumbo_ = (rumbo_ + 360 - GRADOS_POR_GIRO) % 360;
    canal_.enviar("GIRAR_IZQUIERDA");
}

void Drone::acelerar() {
    exigirEnVuelo();
    if (velocidad_ + PASO_VELOCIDAD > VELOCIDAD_MAXIMA) {
        throw OperacionInvalida("Velocidad maxima alcanzada");
    }
    velocidad_ += PASO_VELOCIDAD;
    canal_.enviar("ACELERAR");
}

void Drone::frenar() {
    exigirEnVuelo();
    if (velocidad_ == 0) {
        throw OperacionInvalida("El drone ya esta detenido");
    }
    velocidad_ -= PASO_VELOCIDAD;
    canal_.enviar("FRENAR");
}

void Drone::descargarPaquete() {
    if (!tienePaquete_) {
        throw OperacionInvalida("No hay paquete para descargar");
    }
    if (velocidad_ != 0) {
        throw OperacionInvalida("El drone debe estar detenido para descargar");
    }
    tienePaquete_ = false;
    canal_.enviar("DESCARGAR_PAQUETE");
}

void Drone::sacarFotoRecepcion() {
    if (tienePaquete_) {
        throw OperacionInvalida("Primero hay que descargar el paquete");
    }
    fotoTomada_ = true;
    canal_.enviar("SACAR_FOTO_RECEPCION");
}

void Drone::notificarEntrega() {
    if (!fotoTomada_) {
        throw OperacionInvalida("Falta la foto de la recepcion");
    }
    if (entregaNotificada_) {
        throw OperacionInvalida("La entrega ya fue notificada");
    }
    entregaNotificada_ = true;
    canal_.enviar("NOTIFICAR_ENTREGA");
}

}  // namespace ibag151
