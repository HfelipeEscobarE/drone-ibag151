#pragma once

namespace ibag151 {

// Contrato de lo que sabe hacer un drone de reparto.
// Quien lo use (ej. el controlador) depende de esta interfaz, no de una clase concreta.
class IDrone {
public:
    virtual ~IDrone() = default;

    virtual void despegar() = 0;
    virtual void aterrizar() = 0;
    virtual void elevar() = 0;
    virtual void bajar() = 0;
    virtual void girarDerecha() = 0;
    virtual void girarIzquierda() = 0;
    virtual void acelerar() = 0;
    virtual void frenar() = 0;
    virtual void descargarPaquete() = 0;
    virtual void sacarFotoRecepcion() = 0;
    virtual void notificarEntrega() = 0;
};

}  // namespace ibag151
