#include <gtest/gtest.h>

#include <algorithm>

#include "CanalFalso.hpp"
#include "drone/controller/ControladorDeComandos.hpp"
#include "drone/model/Drone.hpp"

using ibag151::ControladorDeComandos;
using ibag151::Drone;

class ControladorTest : public ::testing::Test {
protected:
    CanalFalso canal;
    Drone drone{canal};
    ControladorDeComandos controlador{drone};
};

TEST_F(ControladorTest, UnComandoValidoLlegaAlDrone) {
    const auto resultado = controlador.ejecutar("despegar");
    EXPECT_TRUE(resultado.exito);
    EXPECT_FALSE(resultado.terminar);
    EXPECT_TRUE(drone.estaEnVuelo());
    EXPECT_EQ(canal.ultima(), "DESPEGAR");
}

TEST_F(ControladorTest, NoDistingueMayusculasDeMinusculas) {
    EXPECT_TRUE(controlador.ejecutar("DESPEGAR").exito);
    EXPECT_TRUE(controlador.ejecutar("Elevar").exito);
}

TEST_F(ControladorTest, IgnoraLosEspaciosDeLosExtremos) {
    EXPECT_TRUE(controlador.ejecutar("  despegar \t").exito);
    EXPECT_TRUE(drone.estaEnVuelo());
}

TEST_F(ControladorTest, UnComandoDesconocidoSeRechazaSinTocarAlDrone) {
    const auto resultado = controlador.ejecutar("volar");
    EXPECT_FALSE(resultado.exito);
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(ControladorTest, UnaOperacionInvalidaSeInformaEnElResultado) {
    const auto resultado = controlador.ejecutar("aterrizar");
    EXPECT_FALSE(resultado.exito);
    EXPECT_FALSE(resultado.mensaje.empty());
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(ControladorTest, SalirPideTerminarSinTocarAlDrone) {
    const auto resultado = controlador.ejecutar("SALIR");
    EXPECT_TRUE(resultado.exito);
    EXPECT_TRUE(resultado.terminar);
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(ControladorTest, UnaLineaVaciaNoHaceNadaNiSeRechaza) {
    const auto resultado = controlador.ejecutar("   ");
    EXPECT_TRUE(resultado.exito);
    EXPECT_FALSE(resultado.terminar);
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(ControladorTest, InformaLosOnceComandosDelDroneMasSalir) {
    const auto comandos = controlador.comandosDisponibles();
    EXPECT_EQ(comandos.size(), 12u);
    EXPECT_TRUE(std::find(comandos.begin(), comandos.end(), "salir") != comandos.end());
    EXPECT_TRUE(std::find(comandos.begin(), comandos.end(), "despegar") != comandos.end());
}

TEST_F(ControladorTest, TodosLosComandosDeLaEntregaFuncionPorTexto) {
    const char* guion[] = {"despegar", "elevar", "bajar", "acelerar", "derecha",
                           "izquierda", "frenar", "descargar", "foto", "notificar",
                           "aterrizar"};
    for (const char* comando : guion) {
        EXPECT_TRUE(controlador.ejecutar(comando).exito);
    }
    EXPECT_EQ(canal.cantidad(), 11u);
}
