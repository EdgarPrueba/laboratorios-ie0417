# Universidad de Costa Rica  
**Escuela de Ingeniería Eléctrica**  
**IE-0417 – Diseño de Software para Ingeniería**  
**Laboratorio 8: Software Testing**  
**Integrantes:**  
- Diego Alfaro (C20259)  
- Edgar Alvarado (C10351)  
- Jean Zuñiga (C18767)  

---

## Concepto general

El presente laboratorio introduce al estudiante al **software testing** mediante la práctica con pruebas unitarias utilizando la biblioteca **Google Test**. Se enfatiza la importancia del testing en el ciclo de vida del desarrollo de software, mostrando cómo validar módulos de código de manera automática y continua.

Se implementan pruebas sobre dos clases:  
- `Calculator`: una clase sencilla para realizar operaciones aritméticas.  
- `Authentication`: una clase que gestiona el inicio de sesión de usuarios, utilizando un objeto `Database`.

Además, se automatiza el proceso de compilación y testing mediante **GitHub Actions**, como ejemplo de un flujo de **CI/CD (Integración Continua y Entrega Continua)**.

---

## Estructura del proyecto
```plaintext
.github/workflows/main.yml # CI con GitHub Actions
laboratorio8/
├── calculator.h # Lógica de operaciones aritméticas
├── authentication.h # Clase Authentication (login y registro)
├── database.h # Interfaz de base de datos
├── main_random.cpp
├── mock_database.h # Mock para pruebas de Authentication
├── random_utils.h
├── test_calculator.cpp # Pruebas unitarias para Calculator
├── test_authentication.cpp # Pruebas unitarias para Authentication
├── CMakeLists.txt # Configuración del proyecto con CMake
├──build/ # Archivos generados tras compilar
└── test_seed.cpp

``` 

---

## Framework de pruebas: Google Test

El proyecto utiliza [Google Test](https://github.com/google/googletest) como framework para escribir y ejecutar pruebas unitarias. Este se descarga automáticamente mediante `FetchContent` dentro del `CMakeLists.txt`.

## Pruebas con generación aleatoria controlada (semilla)

En software testing, una **semilla** es un valor inicial utilizado por generadores de números aleatorios. Establecer una semilla garantiza la **reproducibilidad** de las pruebas, ya que permite obtener la misma secuencia de números aleatorios en cada ejecución. Esto es útil para detectar errores de forma consistente y para facilitar el debugging.

### Código fuente (main_random.cpp)

El siguiente programa genera una secuencia de números aleatorios utilizando una semilla fija:

```cpp
#include <iostream>
#include <random>

int main() {
    unsigned int seed = 12345;
    std::mt19937 generator(seed);

    for (int i = 0; i < 5; ++i) {
        std::cout << generator() << std::endl;
    }

    return 0;
}
```

### Pruebas de `SeedTest`
Para validar que se genera la secuencia esperada de números, se implementó el siguiente test con Google Test:

```cpp
TEST(SeedTest, GeneratesExpectedValues) {
    unsigned int seed = 12345;
    std::vector<uint32_t> expected = {
        3992670690, 3823185381, 1358822685, 561383553, 789925284
    };

    auto result = generateRandomSequence(seed, 5);

    EXPECT_EQ(result, expected);
}
```

### Pruebas de `Calculator`

En este ejemplo se prueba la clase `Calculator`, que implementa operaciones aritméticas básicas. Se utiliza una **Test Fixture** (`CalculatorTest`) para encapsular la configuración común de los tests.

```cpp
// Prueba para el método Add usando TestFixture
TEST_F(CalculatorTest, AddTest) {
    EXPECT_EQ(calc.Add(1, 1), 2);
    EXPECT_EQ(calc.Add(-1, 1), 0);
    EXPECT_EQ(calc.Add(-1, -1), -2);
}

// Prueba para el método Subtract usando TestFixture
TEST_F(CalculatorTest, SubtractTest) {
    EXPECT_EQ(calc.Subtract(2, 1), 1);
    EXPECT_EQ(calc.Subtract(-1, 1), -2);
    EXPECT_EQ(calc.Subtract(-1, -1), 0);
}
```
#### `AddTest`
Prueba el método `Add` para distintos casos:
- Sumar dos números positivos: `1 + 1 = 2`
- Sumar un número negativo y uno positivo: `-1 + 1 = 0`
- Sumar dos números negativos: `-1 + -1 = -2`

**Se espera que el método devuelva el resultado correcto de la suma en cada caso.**
#### `SubtractTest`
Prueba el método `Subtract` con diferentes combinaciones:
- Restar dos positivos: `2 - 1 = 1`
- Restar un número negativo con uno positivo: `-1 - 1 = -2`
- Restar dos negativos: `-1 - (-1) = 0`

**Se espera que la resta se realice correctamente y el resultado sea el esperado.**

---

### Pruebas de `Authentication` con `MockDatabase`

Se prueba la clase `Authentication`, la cual depende de una interfaz `Database`. Se utiliza un objeto `MockDatabase` con Google Mock para simular el comportamiento de una base de datos real.

```cpp

// Prueba para el inicio de sesión exitoso
TEST(AuthenticationTest, LoginSuccess) {
    MockDatabase db;
    Authentication auth(&db);

    EXPECT_CALL(db, userExists("Alice"))
        .WillOnce(Return(true));
    EXPECT_CALL(db, getPassword("Alice"))
        .WillOnce(Return("password"));

    EXPECT_TRUE(auth.login("Alice", "password"));
}

// Prueba para el inicio de sesión fallido
TEST(AuthenticationTest, LoginFailure) {
    MockDatabase db;
    Authentication auth(&db);

    EXPECT_CALL(db, userExists("Alice"))
        .WillOnce(Return(true));
    EXPECT_CALL(db, getPassword("Alice"))
        .WillOnce(Return("password"));

    EXPECT_FALSE(auth.login("Alice", "wrongpassword"));

    EXPECT_CALL(db, userExists("Bob"))
        .WillOnce(Return(false));

    EXPECT_FALSE(auth.login("Bob", "password"));
}

// Prueba para el registro de un nuevo usuario
TEST(AuthenticationTest, RegisterUser) {
    MockDatabase db;
    Authentication auth(&db);

    EXPECT_CALL(db, userExists("Charlie"))
        .WillOnce(Return(false));
    EXPECT_CALL(db, addUser("Charlie", "password"))
        .Times(1);

    auth.registerUser("Charlie", "password");
}

```
#### `LoginSuccess`
- Simula que el usuario "Alice" existe y su contraseña es correcta.
- **Se espera que el método `login("Alice", "password")` devuelva `true`**, indicando una autenticación exitosa.

#### `LoginFailure`
- Simula dos escenarios:
  1. El usuario "Alice" existe, pero la contraseña ingresada es incorrecta.
  2. El usuario "Bob" no existe en la base de datos.
- **Se espera que el método `login` devuelva `false`** en ambos casos, indicando que la autenticación ha fallado.

#### `RegisterUser`
- Simula que el usuario "Charlie" no existe.
- **Se espera que se invoque el método `addUser("Charlie", "password")` una vez**, indicando que el registro fue exitoso.
---


## Compilación y ejecución de pruebas
Requisitos:
- cmake

- g++ (o cualquier compilador compatible con C++17)

- make

### Instrucciones
```bash
mkdir build
cd build
cmake ..
make
```
Esto creará un directorio `build` donde se generarán los archivos de compilación y ejecutables de las pruebas.

### Ejecutar pruebas
```bash
./test_calculator
./test_authentication
./test_seed
```
---

## Resultados esperados
Al ejecutar las pruebas correctamente compiladas, se espera que todas pasen sin errores. En caso contrario, se mostrará un mensaje detallando la falla.
## Salida esperada – Seed
```bash
./test_seed 
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from SeedTest
[ RUN      ] SeedTest.GeneratesExpectedValues
[       OK ] SeedTest.GeneratesExpectedValues (0 ms)
[----------] 1 test from SeedTest (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```
## Salida esperada – Calculator
```bash
./test_calculator 
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from CalculatorTest
[ RUN      ] CalculatorTest.AddTest
[       OK ] CalculatorTest.AddTest (0 ms)
[ RUN      ] CalculatorTest.SubtractTest
[       OK ] CalculatorTest.SubtractTest (0 ms)
[----------] 2 tests from CalculatorTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

Lo mismo aplica para las pruebas de `Authentication`, donde se valida el inicio de sesión y el registro de usuarios.
### Salida esperada – Authentication

```bash
./test_authentication 
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from AuthenticationTest
[ RUN      ] AuthenticationTest.LoginSuccess
[       OK ] AuthenticationTest.LoginSuccess (0 ms)
[ RUN      ] AuthenticationTest.LoginFailure
[       OK ] AuthenticationTest.LoginFailure (0 ms)
[ RUN      ] AuthenticationTest.RegisterUser
[       OK ] AuthenticationTest.RegisterUser (0 ms)
[----------] 3 tests from AuthenticationTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 3 tests.
```
---

## Integración continua con GitHub Actions

[![CI/CD Pipeline](https://github.com/EdgarPrueba/laboratorios-ie0417/actions/workflows/main.yml/badge.svg)](https://github.com/EdgarPrueba/laboratorios-ie0417/actions/workflows/main.yml)

Este proyecto implementa una pipeline de integración y entrega continua (CI/CD) utilizando **GitHub Actions**. El badge anterior muestra el estado más reciente de la ejecución del workflow.

La acción se ejecuta automáticamente cada vez que se realiza un `push` o se abre un `pull request` hacia la rama `main`.

### Archivo de configuración (`.github/workflows/ci.yml`)

```yaml
name: CI/CD Pipeline

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Install dependencies (CMake, Make, GCC, etc.)
      run: |
        sudo apt-get update
        sudo apt-get install -y build-essential cmake

    - name: Build and run tests
      run: |
        cd laboratorio8
        mkdir -p build
        cd build
        cmake ..
        make
        ./test_calculator
        ./test_authentication

  deploy:
    needs: build
    runs-on: ubuntu-latest
    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Deploy to production
      run: echo "Deploying to production server"
      # Aquí se podría agregar un script real de despliegue
```
### Beneficios del pipeline
* Verificación automática de que las pruebas unitarias pasan antes de permitir cambios en main.

* Posibilidad de automatizar el despliegue a producción si se integra con un entorno real.

* Mejora la calidad y confiabilidad del código al detectar errores rápidamente.

Este enfoque asegura que todo cambio en el repositorio sea probado y validado automáticamente antes de integrarse en la rama principal o ser desplegado.

## Conclusiones

Este laboratorio demuestra la importancia de escribir pruebas automáticas desde etapas tempranas en el desarrollo de software. Además, refuerza el uso de herramientas modernas como CMake y GitHub Actions para mantener una base de código robusta, verificable y lista para producción de manera automatizada.