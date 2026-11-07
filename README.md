# Proyecto Sumo PCB
# Firmware de robot con ESP32 y mando PS4

## 1. Identificación y alcance

- Fuente original: programacionESP32arduinoIDE.txt.
- Entorno de programación utilizado: Arduino IDE.
- Lenguaje: C++ con el framework Arduino para ESP32.
- Versión asignada a esta entrega: v1.0.0-doc.1.
- Código comentado: RobotESP32/RobotESP32.ino.

Esta versión identifica la entrega documental. El archivo original no
incluye un número de versión ni un historial de cambios.

Se agregaron comentarios y se corrigieron las descripciones de los
canales PWM, conservando las instrucciones ejecutables, los pines,
los parámetros y la lógica del programa original.

El firmware controla un robot de dos motores mediante un mando PS4
conectado por Bluetooth. Permite avance, retroceso y giros mediante
la cruceta y los joysticks. También controla indicadores LED y una
señal de habilitación del controlador de motores.

El modelo exacto de placa ESP32 y del controlador de motores no
está especificado en el archivo original.

## 2. Archivos del proyecto

| Archivo | Descripción |
| --- | --- |
| README.readme | Documentación, dependencias, compilación, carga y uso. |
| RobotESP32/RobotESP32.ino | Código fuente completo comentado. |
| programacionESP32arduinoIDE.txt | Archivo original que debe conservarse como referencia. |

La carpeta del sketch debe llamarse RobotESP32 y contener el archivo
RobotESP32.ino.

El README utiliza formato Markdown y puede abrirse con un editor
de texto.

## 3. Dependencias y versiones

El archivo original no registra las versiones instaladas. Por tanto,
no es posible afirmar cuáles se utilizaron realmente.

Las versiones de referencia siguientes deben distinguirse de las
versiones originales del proyecto.

| Componente | Versión original | Referencia o requisito |
| --- | --- | --- |
| Arduino IDE | No especificada | Registrar la versión exacta desde Ayuda > Acerca de Arduino IDE. |
| Paquete esp32 de Espressif Systems | No especificada | El código utiliza la API LEDC anterior a 3.x. El autor de la biblioteca PS4 cita 1.0.3 como referencia histórica, sin validación de esta entrega. |
| Biblioteca PS4Controller.h | No especificada | El repositorio de referencia aed3/PS4-esp32 declara la versión 2.1 en library.properties. Confirmar si corresponde a la biblioteca realmente utilizada. |
| Funciones Arduino: pinMode, digitalWrite, delay, map y byte | Incluidas en el core | No requieren instalar bibliotecas adicionales. |
| LEDC y pila Bluetooth | Incluidas o vinculadas al core ESP32 | Su compatibilidad depende del paquete de placas y de la biblioteca PS4. |
| Controlador USB de la placa | No identificado | Depende del conversor USB y del sistema operativo utilizados. |

Repositorio de referencia de la biblioteca PS4:

https://github.com/aed3/PS4-esp32

La inclusión de PS4Controller.h no permite identificar por sí sola
el repositorio, fork o versión utilizada originalmente.

El repositorio de referencia está archivado y advierte que sus
instrucciones pueden requerir ajustes. Un fork puede tener requisitos
distintos.

### Compatibilidad de PWM

El programa utiliza:

- ledcSetup()
- ledcAttachPin()
- ledcWrite() con números de canal.

Arduino-ESP32 3.x eliminó ledcSetup() y ledcAttachPin(), y modificó
el uso de canales y pines en la API LEDC.

Por tanto, este código requiere un entorno con la API anterior,
o una adaptación antes de utilizar Arduino-ESP32 3.x.

La compatibilidad de LEDC no garantiza por sí sola que la biblioteca
PS4 funcione con cualquier versión 1.x o 2.x del core.

La versión de Arduino IDE y la versión del paquete de placas esp32
son componentes distintos.

### Registro del entorno reproducible

Completar con los datos del equipo donde se compile y pruebe:

| Dato | Valor |
| --- | --- |
| Arduino IDE, versión exacta | Pendiente de confirmar |
| Paquete esp32, versión exacta | Pendiente de confirmar |
| Biblioteca PS4, repositorio o fork | Pendiente de confirmar |
| Biblioteca PS4, versión y commit o hash del ZIP | Pendiente de confirmar |
| Modelo físico de placa ESP32 | Pendiente de confirmar |
| Placa seleccionada en Arduino IDE | Pendiente de confirmar |
| Sistema operativo | Pendiente de confirmar |
| Controlador USB, si corresponde | Pendiente de confirmar |
| Opciones de flash, partición y carga | Pendiente de confirmar |
| Resultado de compilación | Pendiente de verificar |
| Resultado de pruebas físicas | Pendiente de verificar |

## 4. Hardware y conexiones

El código contempla:

- Una placa ESP32 compatible con la biblioteca y con Bluetooth clásico.
- Un mando PS4 compatible.
- Dos motores.
- Un controlador de motores con entradas de dirección y PWM.
- Una señal de habilitación o standby.
- Dos indicadores LED.
- Alimentación apropiada para los componentes.

No debe suponerse que cualquier variante ESP32 es compatible con
esta biblioteca.

### Asignación de pines

| Señal | GPIO | Función |
| --- | ---: | --- |
| MI1 | 15 | Dirección 1 del motor izquierdo |
| MI2 | 4 | Dirección 2 del motor izquierdo |
| MIPWM | 5 | PWM del motor izquierdo; canal LEDC 3 |
| MD1 | 16 | Dirección 1 del motor derecho |
| MD2 | 17 | Dirección 2 del motor derecho |
| MDPWM | 18 | PWM del motor derecho; canal LEDC 2 |
| LED1 | 22 | Indicador de conexión |
| LED2 | 19 | Indicador de desconexión |
| STDBYM | 2 | Habilitación del controlador de motores |

Los motores se conectan al controlador de potencia. Los GPIO del
ESP32 proporcionan las señales de control.

El controlador y el ESP32 deben compartir una referencia de tierra.
La alimentación de motores debe corresponder a los componentes reales.

Confirmar la disponibilidad de los pines en la placa utilizada.
El sentido físico de movimiento depende del montaje y del cableado.

## 5. Preparación en Arduino IDE

1. Abrir Arduino IDE y registrar su versión exacta.

2. En Archivo > Preferencias, agregar la siguiente dirección en
   el campo de URLs adicionales del Gestor de placas, si todavía
   no está configurada:

   https://dl.espressif.com/dl/package_esp32_index.json

3. Abrir el Gestor de placas y buscar esp32 de Espressif Systems.

4. Instalar la versión utilizada originalmente, si se conoce.
   Si no existe ese registro, la versión 1.0.3 citada por el autor
   de la biblioteca constituye una referencia histórica sujeta
   a verificación. No seleccionar 3.x para este código sin adaptarlo.

5. Recuperar preferentemente la misma biblioteca PS4 utilizada
   originalmente. Si no está disponible, consultar:

   https://github.com/aed3/PS4-esp32

6. Para instalar un ZIP de la biblioteca, utilizar:

   Programa/Sketch > Incluir biblioteca > Añadir biblioteca .ZIP.

7. Revisar library.properties y registrar versión, repositorio
   y revisión exacta. Evitar instalaciones duplicadas que aporten
   diferentes copias de PS4Controller.h.

8. Guardar el código en:

   RobotESP32/RobotESP32.ino

9. Abrir RobotESP32.ino con Arduino IDE.

10. En Herramientas > Placa, seleccionar la placa física utilizada.
    Elegir ESP32 Dev Module solamente si corresponde al hardware.

11. Seleccionar el puerto y registrar las opciones de compilación
    y carga.

Los nombres de los menús pueden variar según el idioma y la versión
del IDE.

## 6. Compilación y carga

1. Conectar la placa al ordenador mediante un cable USB de datos.

2. Confirmar placa, puerto, versión del paquete esp32 y biblioteca PS4.

3. Pulsar Verificar para compilar el sketch y sus dependencias.

4. Revisar los mensajes del IDE. El resultado esperado es una
   compilación sin errores.

5. Si la compilación termina correctamente, pulsar Subir.

6. Esperar la confirmación de carga.

7. Si la placa requiere utilizar BOOT o EN para entrar en modo
   de programación, seguir las instrucciones de su fabricante.

8. Tras cargar el programa, el ESP32 ejecuta el firmware
   automáticamente.

No se necesita ejecutar un programa adicional en el ordenador.

El código original no inicializa Serial ni imprime mensajes.
Por tanto, no se esperan mensajes en el Monitor Serie.

La compilación y carga no se realizaron durante esta documentación.

## 7. Emparejamiento y ejecución

El código utiliza:

PS4.begin();

No incluye una dirección MAC explícita ni una rutina que configure
el emparejamiento del mando.

La biblioteca de referencia requiere que la dirección Bluetooth del
anfitrión corresponda a la dirección almacenada en el mando.

### Procedimiento

1. Configurar previamente la asociación del mando con el anfitrión
   ESP32 según la documentación de la biblioteca utilizada.

2. El autor documenta como alternativa configurar una dirección
   de anfitrión mediante:

   PS4.begin("MAC_DEL_ANFITRION");

   Debe utilizarse una dirección válida y correspondiente a la
   almacenada en el mando. No es la dirección MAC propia del mando.

   Este cambio no se aplicó al código de esta entrega.

3. Encender el robot.

4. Activar la conexión del mando mediante el botón PS, una vez
   configurada su asociación.

5. Comprobar los indicadores de conexión.

Para la primera prueba, mantener las ruedas levantadas y verificar
dirección, parada y desconexión.

### Indicadores

Al conectarse el mando:

- LED1 se enciende.
- LED2 se apaga.
- STDBYM pasa a nivel alto.
- Se solicita color naranja para la luz del mando.

Al desconectarse:

- STDBYM pasa a nivel bajo.
- Se ejecuta mstop().
- LED1 se apaga.
- LED2 se enciende.

### Controles

| Entrada | Orden programada | PWM |
| --- | --- | --- |
| Cruceta arriba | Avance | 100 |
| Cruceta abajo | Retroceso | 100 |
| Cruceta derecha | Giro derecho | 100 |
| Cruceta izquierda | Giro izquierdo | 100 |
| Joystick izquierdo, eje Y positivo | Avance proporcional | Nominalmente 30 a 255 |
| Joystick izquierdo, eje Y negativo | Retroceso proporcional | Nominalmente 30 a 255 |
| Joystick derecho, eje X positivo | Giro derecho proporcional | Nominalmente 30 a 200 |
| Joystick derecho, eje X negativo | Giro izquierdo proporcional | Nominalmente 30 a 200 |
| Ambos ejes entre -9 y 9 | stop2() | 100 y entradas de dirección en HIGH |

Limitación de la cruceta:

Cuando ambos joysticks están centrados, la llamada final a stop2()
sobrescribe las órdenes de la cruceta en la misma iteración.

Los if son independientes. Una orden posterior puede sobrescribir
otra anterior. El giro con joystick se evalúa después del avance.

## 8. Organización del firmware

| Función | Responsabilidad |
| --- | --- |
| setup() | Configurar PWM, pines, parada inicial y eventos de conexión. |
| loop() | Consultar controles y ejecutar las órdenes. |
| onConnect() | Actualizar indicadores y habilitar motores al conectar. |
| onDisConnect() | Deshabilitar el controlador y detener salidas al desconectar. |
| forward() | Aplicar el patrón de avance. |
| backward() | Aplicar el patrón de retroceso. |
| right() | Aplicar el patrón de giro derecho. |
| left() | Aplicar el patrón de giro izquierdo. |
| mstop() | Escribir PWM cero y entradas de dirección en LOW. |
| stop2() | Escribir PWM 100 y entradas de dirección en HIGH. |

### Parámetros principales

| Parámetro | Valor | Significado |
| --- | ---: | --- |
| freq | 2000 | Frecuencia PWM en Hz |
| Resolución PWM | 8 bits | Rango de 0 a 255 |
| speed_rc | 255 | Extremo superior del mapeo para avance y retroceso |
| speed_rc2 | 200 | Extremo superior del mapeo para giros |
| minspeed_rc | 30 | Extremo inferior del mapeo |

Estos valores representan ciclo de trabajo PWM, no velocidad medida.

En las funciones de movimiento:

- PWM1 corresponde al motor derecho.
- PWM2 corresponde al motor izquierdo.
- T representa un retardo en milisegundos.

El parámetro T no detiene automáticamente el movimiento al finalizar.
Las salidas conservan su estado hasta que otra orden las cambie.

## 9. Observaciones del código original

1. La zona muerta de los joysticks comprende los valores de -9 a 9.

2. Las condiciones activan movimiento desde ±10, pero map() utiliza
   ±11 como inicio. En ese umbral se produce una extrapolación
   ligera. No se utiliza constrain().

3. loop() no comprueba PS4.isConnected(). La deshabilitación al
   desconectar depende del evento de desconexión y de STDBYM.

4. stop2() no establece PWM cero ni deshabilita STDBYM. Su efecto
   eléctrico depende de la tabla de verdad del controlador real.

5. Durante setup() no se escribe explícitamente un estado inicial
   de STDBYM mediante digitalWrite(), ni se establece LED2 como
   indicador de desconexión inicial.

6. No existen sensores, realimentación de velocidad, telemetría
   ni comandos por puerto serie.

Estas observaciones se documentan sin modificar funcionalmente
el programa recibido.

## 10. Versionado y registro de cambios

| Versión | Descripción |
| --- | --- |
| Original sin versión declarada | Código recibido en programacionESP32arduinoIDE.txt. |
| v1.0.0-doc.1 | Comentarios, documentación e instrucciones; misma lógica ejecutable. |

Para iniciar un historial real con Git, guardar el README, el sketch
y el archivo original dentro de una carpeta de proyecto.

Con Git instalado y configurado, ejecutar desde esa carpeta:

```bash
git init
git add README.readme RobotESP32/RobotESP32.ino programacionESP32arduinoIDE.txt
git commit -m "Documenta firmware ESP32 y conserva logica original"
git tag -a v1.0.0-doc.1 -m "Entrega inicial documentada"
```

Estos comandos son instrucciones. No se creó un repositorio ni una
etiqueta Git durante esta entrega.

Para versiones futuras, registrar:

- Cambios funcionales.
- Modificaciones de pines o parámetros.
- Versiones exactas de dependencias.
- Resultados de compilación.
- Pruebas físicas realizadas.

## 11. Verificación y resolución de problemas

Se realizó una revisión estática y se comprobó que el código
comentado conserva las instrucciones ejecutables del original,
ignorando comentarios y espacios.

No se realizó una compilación con Arduino IDE ni una prueba
con hardware.

| Problema | Comprobación |
| --- | --- |
| PS4Controller.h no encontrado | Instalar la biblioteca correcta y revisar duplicados. |
| ledcSetup o ledcAttachPin no declarados | Revisar la versión del paquete esp32; el fuente usa la API anterior a 3.x. |
| Errores de Bluetooth o de la biblioteca PS4 | Verificar compatibilidad del core, fork y revisión exacta. |
| El mando no conecta | Revisar asociación, MAC del anfitrión y compatibilidad del ESP32. |
| La cruceta no mantiene el movimiento | Revisar la llamada final a stop2() con los joysticks centrados. |
| El robot se mueve en sentido contrario | Comprobar polaridad, montaje y entradas del controlador. |
| El Monitor Serie no muestra mensajes | El código no contiene salida por Serial. |

### Pruebas físicas pendientes

- Compilación y carga.
- Conexión del mando.
- Avance y retroceso.
- Giros a ambos lados.
- Estado neutral.
- Comportamiento de la cruceta.
- Detención y deshabilitación al perder conexión.

Registrar los resultados reales y las versiones exactas antes de
declarar el firmware validado.

## 12. Referencias

Fuente principal:

programacionESP32arduinoIDE.txt, proporcionado por el usuario.

Biblioteca PS4, instalación y emparejamiento:

https://github.com/aed3/PS4-esp32

Versión declarada por la biblioteca de referencia:

https://github.com/aed3/PS4-esp32/blob/master/library.properties

Migración de Arduino-ESP32 y cambios en LEDC:

https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html

SHA-256 del archivo original recibido:

11b42967c9b6dea69e8557cd6478f2227ce270e2d015338eb54137feb7383e99
