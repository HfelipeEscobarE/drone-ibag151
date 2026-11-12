#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "CanalFalso.hpp"
#include "drone/model/Drone.hpp"
#include "drone/model/IDrone.hpp"
#include "drone/model/OperacionInvalida.hpp"

using ibag151::Drone;
using ibag151::OperacionInvalida;

// Cada bloque es un ciclo TDD: primero se escribe la prueba (rojo), despues el
// codigo minimo que la hace pasar (verde) y por ultimo se limpia (refactor).
class DroneTest : public ::testing::Test {
protected:
    CanalFalso canal;
    Drone drone{canal};

    void enElAire() { drone.despegar(); }
};

// ---- Ciclo 1: estado inicial ---------------------------------------------
TEST_F(DroneTest, IniciaEnTierraYDetenido) {
    EXPECT_FALSE(drone.estaEnVuelo());
    EXPECT_EQ(drone.altitud(), 0);
    EXPECT_EQ(drone.velocidad(), 0);
}

TEST_F(DroneTest, IniciaLlevandoElPaquete) {
    EXPECT_TRUE(drone.tienePaquete());
    EXPECT_FALSE(drone.fotoTomada());
    EXPECT_FALSE(drone.entregaNotificada());
}

TEST_F(DroneTest, SePuedeUsarATravesDeLaInterfaz) {
    ibag151::IDrone& puerto = drone;
    puerto.despegar();
    EXPECT_TRUE(drone.estaEnVuelo());
}

// ---- Ciclo 2: despegar ----------------------------------------------------
TEST_F(DroneTest, DespegarPoneAlDroneEnVueloALaAlturaInicial) {
    drone.despegar();
    EXPECT_TRUE(drone.estaEnVuelo());
    EXPECT_EQ(drone.altitud(), Drone::ALTURA_DESPEGUE);
}

TEST_F(DroneTest, DespegarEnviaElComando) {
    drone.despegar();
    ASSERT_EQ(canal.cantidad(), 1u);
    EXPECT_EQ(canal.ultima(), "DESPEGAR");
}

TEST_F(DroneTest, DespegarEstandoEnVueloLanzaErrorYNoEnviaNada) {
    enElAire();
    EXPECT_THROW(drone.despegar(), OperacionInvalida);
    EXPECT_EQ(canal.cantidad(), 1u);
}

// ---- Ciclo 3: aterrizar ---------------------------------------------------
TEST_F(DroneTest, AterrizarDejaAlDroneEnTierra) {
    enElAire();
    drone.aterrizar();
    EXPECT_FALSE(drone.estaEnVuelo());
    EXPECT_EQ(drone.altitud(), 0);
    EXPECT_EQ(canal.ultima(), "ATERRIZAR");
}

TEST_F(DroneTest, AterrizarEnTierraLanzaError) {
    EXPECT_THROW(drone.aterrizar(), OperacionInvalida);
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(DroneTest, AterrizarConVelocidadLanzaError) {
    enElAire();
    drone.acelerar();
    EXPECT_THROW(drone.aterrizar(), OperacionInvalida);
    EXPECT_TRUE(drone.estaEnVuelo());
}

TEST_F(DroneTest, AterrizarDesdeUnaAlturaMayorLanzaError) {
    enElAire();
    drone.elevar();
    EXPECT_THROW(drone.aterrizar(), OperacionInvalida);
    EXPECT_TRUE(drone.estaEnVuelo());
}

TEST_F(DroneTest, BajandoALaAlturaDeAterrizajeSePuedeAterrizar) {
    enElAire();
    drone.elevar();
    drone.bajar();
    drone.aterrizar();
    EXPECT_FALSE(drone.estaEnVuelo());
}

// ---- Ciclo 4: elevar y bajar ----------------------------------------------
TEST_F(DroneTest, ElevarSubeUnPaso) {
    enElAire();
    drone.elevar();
    EXPECT_EQ(drone.altitud(), Drone::ALTURA_DESPEGUE + Drone::PASO_ALTURA);
    EXPECT_EQ(canal.ultima(), "ELEVAR");
}

TEST_F(DroneTest, ElevarMasAllaDeLaAlturaMaximaLanzaError) {
    enElAire();
    while (drone.altitud() + Drone::PASO_ALTURA <= Drone::ALTURA_MAXIMA) {
        drone.elevar();
    }
    EXPECT_EQ(drone.altitud(), Drone::ALTURA_MAXIMA);
    EXPECT_THROW(drone.elevar(), OperacionInvalida);
}

TEST_F(DroneTest, ElevarEnTierraLanzaError) {
    EXPECT_THROW(drone.elevar(), OperacionInvalida);
}

TEST_F(DroneTest, BajarBajaUnPaso) {
    enElAire();
    drone.elevar();
    drone.bajar();
    EXPECT_EQ(drone.altitud(), Drone::ALTURA_DESPEGUE);
    EXPECT_EQ(canal.ultima(), "BAJAR");
}

TEST_F(DroneTest, BajarPorDebajoDeLaAlturaMinimaLanzaError) {
    enElAire();
    EXPECT_THROW(drone.bajar(), OperacionInvalida);
    EXPECT_EQ(drone.altitud(), Drone::ALTURA_DESPEGUE);
}

TEST_F(DroneTest, BajarEnTierraLanzaError) {
    EXPECT_THROW(drone.bajar(), OperacionInvalida);
}

// ---- Ciclo 5: girar -------------------------------------------------------
TEST_F(DroneTest, GirarDerechaSumaNoventaGrados) {
    enElAire();
    drone.girarDerecha();
    EXPECT_EQ(drone.rumbo(), 90);
    EXPECT_EQ(canal.ultima(), "GIRAR_DERECHA");
}

TEST_F(DroneTest, GirarIzquierdaDesdeElRumboInicialDaDosCientosSetenta) {
    enElAire();
    drone.girarIzquierda();
    EXPECT_EQ(drone.rumbo(), 270);
    EXPECT_EQ(canal.ultima(), "GIRAR_IZQUIERDA");
}

TEST_F(DroneTest, CuatroGirosALaDerechaVuelvenAlRumboInicial) {
    enElAire();
    for (int i = 0; i < 4; ++i) {
        drone.girarDerecha();
    }
    EXPECT_EQ(drone.rumbo(), 0);
}

TEST_F(DroneTest, GirarEnTierraLanzaError) {
    EXPECT_THROW(drone.girarDerecha(), OperacionInvalida);
    EXPECT_THROW(drone.girarIzquierda(), OperacionInvalida);
}

// ---- Ciclo 6: acelerar y frenar ------------------------------------------
TEST_F(DroneTest, AcelerarSumaUnPasoDeVelocidad) {
    enElAire();
    drone.acelerar();
    EXPECT_EQ(drone.velocidad(), Drone::PASO_VELOCIDAD);
    EXPECT_EQ(canal.ultima(), "ACELERAR");
}

TEST_F(DroneTest, AcelerarMasAllaDeLaVelocidadMaximaLanzaError) {
    enElAire();
    while (drone.velocidad() + Drone::PASO_VELOCIDAD <= Drone::VELOCIDAD_MAXIMA) {
        drone.acelerar();
    }
    EXPECT_EQ(drone.velocidad(), Drone::VELOCIDAD_MAXIMA);
    EXPECT_THROW(drone.acelerar(), OperacionInvalida);
}

TEST_F(DroneTest, AcelerarEnTierraLanzaError) {
    EXPECT_THROW(drone.acelerar(), OperacionInvalida);
}

TEST_F(DroneTest, FrenarRestaUnPasoDeVelocidad) {
    enElAire();
    drone.acelerar();
    drone.acelerar();
    drone.frenar();
    EXPECT_EQ(drone.velocidad(), Drone::PASO_VELOCIDAD);
    EXPECT_EQ(canal.ultima(), "FRENAR");
}

TEST_F(DroneTest, FrenarDetenidoLanzaErrorYNoEnviaNada) {
    enElAire();
    EXPECT_THROW(drone.frenar(), OperacionInvalida);
    EXPECT_EQ(drone.velocidad(), 0);
    EXPECT_EQ(canal.cantidad(), 1u);  // solo el despegue
}

TEST_F(DroneTest, FrenarEnTierraLanzaError) {
    EXPECT_THROW(drone.frenar(), OperacionInvalida);
}

// ---- Ciclo 7: descargar el paquete ---------------------------------------
TEST_F(DroneTest, DescargarPaqueteDetenidoLoEntrega) {
    drone.descargarPaquete();
    EXPECT_FALSE(drone.tienePaquete());
    EXPECT_EQ(canal.ultima(), "DESCARGAR_PAQUETE");
}

TEST_F(DroneTest, DescargarDosVecesLanzaError) {
    drone.descargarPaquete();
    EXPECT_THROW(drone.descargarPaquete(), OperacionInvalida);
}

TEST_F(DroneTest, DescargarEnMovimientoLanzaError) {
    enElAire();
    drone.acelerar();
    EXPECT_THROW(drone.descargarPaquete(), OperacionInvalida);
    EXPECT_TRUE(drone.tienePaquete());
}

// ---- Ciclo 8: foto de la recepcion ---------------------------------------
TEST_F(DroneTest, NoSePuedeSacarLaFotoSinDescargarElPaquete) {
    EXPECT_THROW(drone.sacarFotoRecepcion(), OperacionInvalida);
    EXPECT_FALSE(drone.fotoTomada());
}

TEST_F(DroneTest, DespuesDeDescargarSePuedeSacarLaFoto) {
    drone.descargarPaquete();
    drone.sacarFotoRecepcion();
    EXPECT_TRUE(drone.fotoTomada());
    EXPECT_EQ(canal.ultima(), "SACAR_FOTO_RECEPCION");
}

// ---- Ciclo 9: notificar la entrega ---------------------------------------
TEST_F(DroneTest, NoSePuedeNotificarSinLaFoto) {
    drone.descargarPaquete();
    EXPECT_THROW(drone.notificarEntrega(), OperacionInvalida);
    EXPECT_FALSE(drone.entregaNotificada());
}

TEST_F(DroneTest, ConLaFotoSePuedeNotificarLaEntrega) {
    drone.descargarPaquete();
    drone.sacarFotoRecepcion();
    drone.notificarEntrega();
    EXPECT_TRUE(drone.entregaNotificada());
    EXPECT_EQ(canal.ultima(), "NOTIFICAR_ENTREGA");
}

TEST_F(DroneTest, NotificarDosVecesLanzaError) {
    drone.descargarPaquete();
    drone.sacarFotoRecepcion();
    drone.notificarEntrega();
    EXPECT_THROW(drone.notificarEntrega(), OperacionInvalida);
}

// ---- Ciclo 10: recorrido completo de una entrega -------------------------
TEST_F(DroneTest, RecorridoCompletoDeUnaEntrega) {
    drone.despegar();
    drone.elevar();
    drone.acelerar();
    drone.girarDerecha();
    drone.frenar();
    drone.descargarPaquete();
    drone.sacarFotoRecepcion();
    drone.notificarEntrega();
    drone.bajar();
    drone.aterrizar();

    const std::vector<std::string> esperado = {
        "DESPEGAR", "ELEVAR", "ACELERAR", "GIRAR_DERECHA", "FRENAR",
        "DESCARGAR_PAQUETE", "SACAR_FOTO_RECEPCION", "NOTIFICAR_ENTREGA",
        "BAJAR", "ATERRIZAR"};
    EXPECT_TRUE(canal.todas() == esperado);
    EXPECT_FALSE(drone.estaEnVuelo());
}
