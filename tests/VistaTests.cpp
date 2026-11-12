#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "CanalFalso.hpp"
#include "drone/controller/ControladorDeComandos.hpp"
#include "drone/model/Drone.hpp"
#include "drone/view/VistaConsola.hpp"

using ibag151::ControladorDeComandos;
using ibag151::Drone;
using ibag151::VistaConsola;

// La vista se prueba sin consola real: la entrada y los avisos son stringstream.
class VistaTest : public ::testing::Test {
protected:
    CanalFalso canal;
    Drone drone{canal};
    ControladorDeComandos controlador{drone};

    // Simula que el operador escribe 'texto' y devuelve lo que la vista le mostro.
    std::string operadorEscribe(const std::string& texto) {
        std::istringstream entrada(texto);
        std::ostringstream avisos;
        VistaConsola vista(controlador, entrada, avisos);
        vista.ejecutar();
        return avisos.str();
    }

    static bool contiene(const std::string& texto, const std::string& buscado) {
        return texto.find(buscado) != std::string::npos;
    }
};

TEST_F(VistaTest, AlIniciarMuestraLaAyudaConLosComandos) {
    const std::string avisos = operadorEscribe("salir\n");
    EXPECT_TRUE(contiene(avisos, "despegar"));
    EXPECT_TRUE(contiene(avisos, "salir"));
}

TEST_F(VistaTest, UnaOrdenValidaViajaAlDroneYNoSeMuestraComoAviso) {
    const std::string avisos = operadorEscribe("despegar\nsalir\n");
    ASSERT_EQ(canal.cantidad(), 1u);
    EXPECT_EQ(canal.ultima(), "DESPEGAR");
    EXPECT_FALSE(contiene(avisos, "[rechazado]"));
}

TEST_F(VistaTest, UnRechazoSeMuestraAlOperadorYNoLlegaAlDrone) {
    const std::string avisos = operadorEscribe("aterrizar\nsalir\n");
    EXPECT_TRUE(contiene(avisos, "[rechazado]"));
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(VistaTest, SalirTerminaYNoProcesaLoQueSigue) {
    operadorEscribe("salir\ndespegar\n");
    EXPECT_EQ(canal.cantidad(), 0u);
}

TEST_F(VistaTest, SiSeAcabaLaEntradaTambienTermina) {
    operadorEscribe("despegar\n");
    EXPECT_EQ(canal.cantidad(), 1u);
}

TEST_F(VistaTest, LasLineasEnBlancoSeIgnoranSinAvisos) {
    const std::string avisos = operadorEscribe("\n   \ndespegar\nsalir\n");
    EXPECT_FALSE(contiene(avisos, "[rechazado]"));
    EXPECT_EQ(canal.cantidad(), 1u);
}
