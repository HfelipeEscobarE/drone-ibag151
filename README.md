# Drone IBag151

Drone de reparto en C++17 con CMake y GTest, organizado en MVC. Cada texto que sale por la
salida estandar (cout) es una orden que "viaja" al drone.

## Estructura (MVC)

```
include/drone/ y src/
  model/        Drone, IDrone, ICanalDrone, OperacionInvalida     -> MODELO
  controller/   ControladorDeComandos                             -> CONTROLADOR
  view/         VistaConsola                                      -> VISTA
  other/        CanalConsola (adaptador hacia el drone)           -> otros
  main.cpp      raiz de composicion: crea los objetos y los conecta
tests/          DroneTests, ControladorTests, VistaTests, CanalFalso (doble de prueba)
```

Cada clase tiene su `.hpp` (declaracion, en `include/`) y su `.cpp` (implementacion, en `src/`).

## Que es la "vista" y que es el "canal del drone"

Hay dos usos de la consola que no hay que mezclar:

| | Quien lo usa | Por donde va | Clase |
|---|---|---|---|
| **Vista** | el operador (persona) | entrada: `cin`; avisos y ayuda: `cerr` | `VistaConsola` |
| **Canal del drone** | el drone | `cout` | `CanalConsola` (implementa `ICanalDrone`) |

La vista le habla a la persona. El canal le habla al drone. Por eso los rechazos
("[rechazado] ...") van por `cerr` y nunca llegan al drone: por `cout` solo salen ordenes
validas. Se puede ver cada canal por separado: `drone_app 2> avisos.txt` deja en pantalla solo
lo que recibe el drone.

## Flujo de una orden

```
operador escribe "despegar"
   -> VistaConsola      lee la linea (cin) y se la pasa al controlador
   -> ControladorDeComandos   normaliza el texto, busca el comando y llama a IDrone::despegar()
   -> Drone             valida el estado; si es valido cambia su estado y llama a ICanalDrone::enviar("DESPEGAR")
   -> CanalConsola      imprime "DESPEGAR" por cout  (= se envia al drone)
si el drone rechaza: lanza OperacionInvalida -> el controlador la atrapa y devuelve Resultado{exito=false}
   -> VistaConsola      muestra "[rechazado] ..." por cerr
```

## Compilar y probar (VSCode + CMake)

```
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure     # o ejecutar ./drone_tests
./drone_app                               # escribir: despegar, elevar, ... salir
```

En Windows con MSYS2/MinGW agregar el generador: `cmake -S . -B build -G "MinGW Makefiles"`.
Si GTest no esta instalado, CMake lo descarga solo (hace falta internet la primera vez).

## Ciclos TDD (un commit por paso)

Para cada ciclo: **Rojo** (escribir la prueba, ver que falla) -> **Verde** (codigo minimo que
pasa) -> **Refactor** (limpiar sin romper). Orden de los bloques en `DroneTests.cpp`:

1. Estado inicial  2. despegar  3. aterrizar  4. elevar / bajar  5. girar
6. acelerar / frenar  7. descargar paquete  8. foto de la recepcion
9. notificar entrega  10. recorrido completo

Luego `ControladorTests.cpp` (comandos de texto) y `VistaTests.cpp` (la vista con
`stringstream`, sin consola real).

## Conceptos aplicados

| Concepto | Donde |
|---|---|
| MVC | modelo `Drone`, vista `VistaConsola`, controlador `ControladorDeComandos` |
| Clean Architecture / Puertos y Adaptadores | `ICanalDrone` es el puerto; `CanalConsola` y `CanalFalso` son adaptadores; el modelo no conoce la consola |
| SRP | `Drone` (reglas y estado), `ControladorDeComandos` (traduce texto), `VistaConsola` (habla con el operador), `CanalConsola` (envia al drone) |
| OCP / LSP | Se agrega otro canal u otro drone sin tocar el resto |
| ISP | `IDrone` e `ICanalDrone` son interfaces chicas y especificas |
| DIP / Inyeccion de dependencias | `Drone` depende de `ICanalDrone`; el controlador, de `IDrone`; la vista recibe sus flujos; `main` elige las clases concretas |
| GRASP Controlador | `ControladorDeComandos` |
| GRASP Experto en informacion | `Drone` valida porque es quien tiene el estado |
| GRASP Bajo acoplamiento / Alta cohesion | Dependencias solo hacia interfaces |
| Clean Code | Nombres que dicen la intencion, constantes con nombre, funciones cortas |
| Refactoring | `exigirEnVuelo()` y `normalizar()` extraidos para no repetir codigo |

## Supuestos (confirmar con la catedra)

La consigna no fija las reglas, asi que se eligieron estas:

- El drone arranca en tierra, detenido y con el paquete.
- Despega a 5 m; cada orden sube/baja 5 m (maximo 120 m); acelera/frena de a 10 km/h (maximo 60).
- Para aterrizar tiene que estar frenado y a la altura de despegue (5 m) o menos.
- Frenar estando detenido se rechaza (igual que acelerar en la velocidad maxima).
- Para descargar tiene que estar detenido.
- La foto de la recepcion se saca despues de descargar; la notificacion, despues de la foto.
- Las ordenes de vuelo en tierra se rechazan con `OperacionInvalida`.

## Pendiente

- Diagrama de clases en DrawIO (`docs/`): se hace al final, cuando el diseno este cerrado.
