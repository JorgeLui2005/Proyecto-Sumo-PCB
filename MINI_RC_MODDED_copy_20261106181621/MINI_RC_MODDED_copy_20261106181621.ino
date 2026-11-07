/*
 * RobotESP32 - Firmware documentado
 * Versión de esta entrega: v1.0.0-doc.1
 *
 * Fuente original:
 * programacionESP32arduinoIDE.txt
 *
 * Entorno de programación utilizado: Arduino IDE.
 * Las versiones originales del IDE, core y biblioteca no están
 * especificadas en el archivo recibido.
 *
 * Dependencia externa:
 * PS4Controller.h
 *
 * Compatibilidad PWM:
 * Utiliza la API LEDC anterior a Arduino-ESP32 3.x.
 *
 * Cambios de esta entrega:
 * Comentarios y formato. Se conserva la lógica ejecutable original.
 */

#include <PS4Controller.h>

// ============================================================
// PINES DEL CONTROLADOR DE MOTORES
// ============================================================

// Motor izquierdo: dos entradas de dirección y una señal PWM.
#define MI1 15
#define MI2 4
#define MIPWM 5

// Motor derecho: dos entradas de dirección y una señal PWM.
#define MD1 16
#define MD2 17
#define MDPWM 18

// ============================================================
// INDICADORES DE CONEXIÓN
// ============================================================

#define LED1 22  // Indicador de mando conectado.
#define LED2 19  // Indicador de desconexión.

// Habilitación del controlador de motores:
// HIGH al conectar el mando; LOW al desconectarlo.
#define STDBYM 2

// ============================================================
// CONFIGURACIÓN PWM
// ============================================================

// Frecuencia PWM en Hz.
#define freq 2000

// Los números siguientes son canales LEDC, no números de GPIO.
#define MD1_PWM 2  // Motor derecho: salida MDPWM = GPIO18.
#define MI1_PWM 3  // Motor izquierdo: salida MIPWM = GPIO5.

// Valores PWM calculados para cada motor.
int Rpwm = 0, Lpwm = 0;

// Extremo superior del mapeo para avance y retroceso.
// Con resolución de 8 bits, el rango PWM es de 0 a 255.
int speed_rc = 255;

// Extremo superior del mapeo para los giros.
int speed_rc2 = 200;

// Extremo inferior del mapeo cuando se activa el movimiento.
// Estos valores no representan una velocidad física medida.
int minspeed_rc = 30;

// ============================================================
// INICIALIZACIÓN
// ============================================================

// Arduino ejecuta setup() una vez al encender o reiniciar.
void setup() {
    // Configurar los canales PWM a 2000 Hz y 8 bits.
    ledcSetup(MD1_PWM, freq, 8);
    ledcSetup(MI1_PWM, freq, 8);

    // Configurar las entradas de dirección como salidas del ESP32.
    pinMode(MD1, OUTPUT);
    pinMode(MD2, OUTPUT);
    pinMode(MI1, OUTPUT);
    pinMode(MI2, OUTPUT);

    // Configurar los indicadores.
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    // Configurar la señal de habilitación del controlador.
    // Se conserva el original: no hay una escritura digital
    // explícita del estado inicial de este pin en setup().
    pinMode(STDBYM, OUTPUT);

    // Asociar cada canal PWM a su GPIO correspondiente.
    ledcAttachPin(MDPWM, MD1_PWM);
    ledcAttachPin(MIPWM, MI1_PWM);

    // Inicializar las salidas de movimiento en parada.
    mstop(1);

    // Registrar las funciones que la biblioteca invocará
    // al conectar o desconectar el mando.
    PS4.attachOnConnect(onConnect);
    PS4.attachOnDisconnect(onDisConnect);

    // Iniciar la biblioteca PS4.
    // El mando debe tener configurada la asociación Bluetooth
    // correspondiente al anfitrión.
    // Esta llamada no configura por sí sola el emparejamiento.
    PS4.begin();

    // Mantener las salidas detenidas y esperar 80 ms.
    mstop(80);
}

// ============================================================
// BUCLE PRINCIPAL
// ============================================================

// Arduino ejecuta loop() continuamente.
//
// Se conserva el orden original de los if independientes:
// una orden posterior puede sobrescribir una anterior.
//
// El bucle original no comprueba PS4.isConnected().
void loop() {
    // --------------------------------------------------------
    // CONTROL MEDIANTE CRUCETA
    // --------------------------------------------------------

    // Avance con PWM fijo de 100 en ambos motores.
    if (PS4.Up() == 1) {
        forward(100, 100, 5);
    }

    // Retroceso con PWM fijo de 100.
    if (PS4.Down() == 1) {
        backward(100, 100, 5);
    }

    // Giro derecho con PWM fijo de 100.
    if (PS4.Right() == 1) {
        right(100, 100, 5);
    }

    // Giro izquierdo con PWM fijo de 100.
    if (PS4.Left() == 1) {
        left(100, 100, 5);
    }

    // --------------------------------------------------------
    // JOYSTICK IZQUIERDO: AVANCE Y RETROCESO
    // --------------------------------------------------------

    // Zona muerta: valores entre -9 y 9.
    //
    // La condición comienza en 10, pero map() comienza en 11.
    // Se conserva este desfase del original.
    // map() extrapola y no limita automáticamente el resultado.
    if (PS4.LStickY() >= 10) {
        Lpwm = map(
            PS4.LStickY(), 11, 127,
            minspeed_rc, speed_rc
        );

        Rpwm = map(
            PS4.LStickY(), 11, 127,
            minspeed_rc, speed_rc
        );

        forward(Rpwm, Lpwm, 2);
    }

    // Valores negativos del eje Y: retroceso proporcional.
    // También se conserva el desfase entre -10 y -11.
    if (PS4.LStickY() <= (-10)) {
        Lpwm = map(
            PS4.LStickY(), -11, -128,
            minspeed_rc, speed_rc
        );

        Rpwm = map(
            PS4.LStickY(), -11, -128,
            minspeed_rc, speed_rc
        );

        backward(Rpwm, Lpwm, 2);
    }

    // --------------------------------------------------------
    // JOYSTICK DERECHO: GIROS
    // --------------------------------------------------------

    // Estos bloques se ejecutan después de avance y retroceso.
    // Si ambos joysticks están activos, pueden sobrescribir
    // la orden de movimiento anterior.

    // Valores positivos del eje X: giro derecho.
    if (PS4.RStickX() >= 10) {
        Lpwm = map(
            PS4.RStickX(), 11, 127,
            minspeed_rc, speed_rc2
        );

        Rpwm = map(
            PS4.RStickX(), 11, 127,
            minspeed_rc, speed_rc2
        );

        right(Rpwm, Lpwm, 2);
    }

    // Valores negativos del eje X: giro izquierdo.
    if (PS4.RStickX() <= (-10)) {
        Lpwm = map(
            PS4.RStickX(), -11, -128,
            minspeed_rc, speed_rc2
        );

        Rpwm = map(
            PS4.RStickX(), -11, -128,
            minspeed_rc, speed_rc2
        );

        left(Rpwm, Lpwm, 2);
    }

    // --------------------------------------------------------
    // ESTADO NEUTRAL
    // --------------------------------------------------------

    // Si ambos ejes están centrados, ejecutar stop2().
    //
    // Esta condición no consulta la cruceta. Por ello puede
    // sobrescribir una orden de los botones direccionales
    // en la misma iteración.
    if (
        ((PS4.RStickX() < 10) && (PS4.RStickX() > -10)) &&
        ((PS4.LStickY() < 10) && (PS4.LStickY() > -10))
    ) {
        stop2(1);
    }
}

// ============================================================
// EVENTOS DE CONEXIÓN
// ============================================================

// La biblioteca invoca esta función al conectar el mando.
void onConnect() {
    // Solicitar color naranja para la luz del mando.
    PS4.setLed(255, 127, 0);
    PS4.sendToController();

    delay(5);

    // Actualizar los indicadores externos.
    digitalWrite(LED1, 1);
    digitalWrite(LED2, 0);

    // Habilitar el controlador de motores.
    digitalWrite(STDBYM, 1);
}

// La biblioteca invoca esta función al desconectar el mando.
void onDisConnect() {
    // Deshabilitar primero el controlador.
    digitalWrite(STDBYM, 0);

    // Anular PWM y colocar las entradas de dirección en LOW.
    mstop(1);

    // Actualizar los indicadores externos.
    digitalWrite(LED1, 0);
    digitalWrite(LED2, 1);
}

// ============================================================
// FUNCIONES DE MOVIMIENTO
// ============================================================

/*
 * Parámetros comunes:
 *
 * PWM1: ciclo de trabajo del motor derecho, de 0 a 255.
 * PWM2: ciclo de trabajo del motor izquierdo, de 0 a 255.
 * T: retardo en milisegundos después de escribir las salidas.
 *
 * El retardo T no detiene automáticamente los motores.
 * Las salidas conservan su estado hasta recibir otra orden.
 *
 * La dirección física depende del cableado y del montaje.
 */

// Avance lógico: ambas parejas de dirección en 0/1.
void forward(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);

    digitalWrite(MD1, 0);
    digitalWrite(MD2, 1);

    digitalWrite(MI1, 0);
    digitalWrite(MI2, 1);

    delay(T);
}

// Retroceso lógico: ambas parejas de dirección en 1/0.
void backward(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);

    digitalWrite(MD1, 1);
    digitalWrite(MD2, 0);

    digitalWrite(MI1, 1);
    digitalWrite(MI2, 0);

    delay(T);
}

// Giro derecho lógico: motores con sentidos opuestos.
void right(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);

    digitalWrite(MD1, 0);
    digitalWrite(MD2, 1);

    digitalWrite(MI1, 1);
    digitalWrite(MI2, 0);

    delay(T);
}

// Giro izquierdo lógico: patrón inverso al giro derecho.
void left(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);

    digitalWrite(MD1, 1);
    digitalWrite(MD2, 0);

    digitalWrite(MI1, 0);
    digitalWrite(MI2, 1);

    delay(T);
}

// ============================================================
// FUNCIONES DE PARADA
// ============================================================

// Parada con PWM cero y entradas de dirección en LOW.
// T representa un retardo en milisegundos.
void mstop(int T) {
    ledcWrite(MD1_PWM, 0);
    ledcWrite(MI1_PWM, 0);

    digitalWrite(MD1, 0);
    digitalWrite(MD2, 0);

    digitalWrite(MI1, 0);
    digitalWrite(MI2, 0);

    delay(T);
}

// Patrón alternativo de parada del programa original.
//
// Mantiene PWM=100 y las cuatro entradas de dirección en HIGH.
// Su efecto eléctrico depende del controlador de motores.
//
// No equivale a mstop() ni deshabilita STDBYM.
// T representa un retardo en milisegundos.
void stop2(int T) {
    ledcWrite(MD1_PWM, 100);
    ledcWrite(MI1_PWM, 100);

    digitalWrite(MD1, 1);
    digitalWrite(MD2, 1);

    digitalWrite(MI1, 1);
    digitalWrite(MI2, 1);

    delay(T);
}