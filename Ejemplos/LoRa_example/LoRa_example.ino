#include <Arduino.h>
#include <LoRaWan-Arduino.h> // Incluye la librería LoRaWAN
#include <SPI.h> // Incluye la librería SPI para la comunicación con el módulo LoRa

// --- Definiciones de configuración ---
#define LORAWAN_APP_DATA_BUFF_SIZE 64 // Tamaño del buffer de datos a transmitir
#define LORAWAN_APP_TX_DUTYCYCLE 60000 // Intervalo de tiempo entre transmisiones (60 segundos)
#define JOINREQ_NBTRIALS 3 // Número de intentos de unión a la red LoRaWAN

// --- Configuración de pines para ESP32 y módulo LoRa ---
int PIN_LORA_RESET = 14;  // Pin de reset del módulo LoRa
int PIN_LORA_NSS = 15;    // Pin CS (Chip Select) para SPI
int PIN_LORA_SCLK = 18;   // Pin SCK (Clock) para SPI
int PIN_LORA_MISO = 19;   // Pin MISO (Master In Slave Out) para SPI
int PIN_LORA_DIO_1 = 21;  // Pin DIO1 para interrupciones
int PIN_LORA_BUSY = 22;   // Pin BUSY para detectar estado del módulo
int PIN_LORA_MOSI = 23;   // Pin MOSI (Master Out Slave In) para SPI
int RADIO_TXEN = -1;      // Pin para habilitar la transmisión (opcional)
int RADIO_RXEN = -1;      // Pin para habilitar la recepción (opcional)

// --- Variables para LoRaWAN ---
hw_config hwConfig; // Configuración del hardware LoRa
TimerEvent_t appTimer; // Timer para controlar las transmisiones periódicas
static uint8_t m_lora_app_data_buffer[LORAWAN_APP_DATA_BUFF_SIZE]; // Buffer de datos para LoRaWAN
static lmh_app_data_t m_lora_app_data = {m_lora_app_data_buffer, 0, 0, 0, 0}; // Estructura de datos para LoRaWAN

// --- Parámetros de configuración LoRaWAN ---
static lmh_param_t lora_param_init = {
    LORAWAN_ADR_ON, // Adaptative Data Rate (ADR) activado
    LORAWAN_DEFAULT_DATARATE, // Data Rate por defecto
    LORAWAN_PUBLIC_NETWORK, // Red pública LoRaWAN
    JOINREQ_NBTRIALS, // Número de intentos de unión
    LORAWAN_DEFAULT_TX_POWER, // Potencia de transmisión por defecto
    LORAWAN_DUTYCYCLE_OFF // Duty Cycle desactivado
};

// --- Callbacks para eventos LoRaWAN ---
static lmh_callback_t lora_callbacks = {
    BoardGetBatteryLevel, // Función para obtener el nivel de batería
    BoardGetUniqueId, // Función para obtener el ID único del dispositivo
    BoardGetRandomSeed, // Función para obtener una semilla aleatoria
    lorawan_rx_handler, // Handler para datos recibidos
    lorawan_has_joined_handler, // Handler para unión exitosa
    lorawan_confirm_class_handler, // Handler para confirmación de clase
    lorawan_join_failed_handler // Handler para fallo de unión
};

// --- Credenciales LoRaWAN (reemplaza con tus propias credenciales) ---
uint8_t nodeDeviceEUI[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22}; // Device EUI
uint8_t nodeAppEUI[8] = {0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00}; // Application EUI
uint8_t nodeAppKey[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}; // Application Key

void setup() {
    Serial.begin(115200); // Inicializa la comunicación serial
    Serial.println("Iniciando LoRaWAN con ESP32...");

    // --- Configuración del hardware LoRa ---
    hwConfig.CHIP_TYPE = SX1262_CHIP; // Tipo de chip LoRa (SX1262)
    hwConfig.PIN_LORA_RESET = PIN_LORA_RESET;
    hwConfig.PIN_LORA_NSS = PIN_LORA_NSS;
    hwConfig.PIN_LORA_SCLK = PIN_LORA_SCLK;
    hwConfig.PIN_LORA_MISO = PIN_LORA_MISO;
    hwConfig.PIN_LORA_DIO_1 = PIN_LORA_DIO_1;
    hwConfig.PIN_LORA_BUSY = PIN_LORA_BUSY;
    hwConfig.PIN_LORA_MOSI = PIN_LORA_MOSI;
    hwConfig.RADIO_TXEN = RADIO_TXEN;
    hwConfig.RADIO_RXEN = RADIO_RXEN;
    hwConfig.USE_DIO2_ANT_SWITCH = true; // Usar DIO2 para control de antena
    hwConfig.USE_DIO3_TCXO = true; // Usar DIO3 para control de TCXO
    hwConfig.USE_DIO3_ANT_SWITCH = false;

    // --- Inicialización del hardware LoRa ---
    uint32_t err_code = lora_hardware_init(hwConfig);
    if (err_code != 0) {
        Serial.printf("Error al inicializar LoRa: %d\n", err_code);
    }

    // --- Inicialización del timer ---
    err_code = timers_init();
    if (err_code != 0) {
        Serial.printf("Error al inicializar el timer: %d\n", err_code);
    }

    // --- Configuración de credenciales LoRaWAN ---
    lmh_setDevEui(nodeDeviceEUI);
    lmh_setAppEui(nodeAppEUI);
    lmh_setAppKey(nodeAppKey);

    // --- Inicialización de LoRaWAN ---
    err_code = lmh_init(&lora_callbacks, lora_param_init, true, CLASS_A, LORAMAC_REGION_EU868); // Configura la región (EU868 en este ejemplo)
    if (err_code != 0) {
        Serial.printf("Error al inicializar LoRaWAN: %d\n", err_code);
    }

    // --- Unirse a la red LoRaWAN ---
    lmh_join();
}

void loop() {
    // El manejo de eventos LoRaWAN se realiza automáticamente en segundo plano
    delay(100); // Pequeña pausa para permitir que otros procesos se ejecuten
}

// --- Handlers de eventos LoRaWAN ---
static void lorawan_join_failed_handler(void) {
    Serial.println("Fallo al unirse a la red LoRaWAN!");
}

static void lorawan_has_joined_handler(void) {
    Serial.println("Unión a la red LoRaWAN exitosa!");
    lmh_class_request(CLASS_A); // Solicita la clase A
    TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE); // Configura el timer para la transmisión periódica
    TimerStart(&appTimer); // Inicia el timer
}

static void lorawan_rx_handler(lmh_app_data_t *app_data) {
    Serial.printf("Paquete LoRa recibido en puerto %d, tamaño: %d, RSSI: %d, SNR: %d\n",
                  app_data->port, app_data->buffsize, app_data->rssi, app_data->snr);
}

static void lorawan_confirm_class_handler(DeviceClass_t Class) {
    Serial.printf("Cambio a clase %c completado\n", "ABC"[Class]);
}

// --- Función para enviar datos LoRaWAN ---
static void send_lora_frame(void) {
    if (lmh_join_status_get() != LMH_SET) {
        Serial.println("No unido a la red, saltando envío de frame");
return;
    }

    uint32_t i = 0; // Contador para el buffer de datos
    m_lora_app_data.port = LORAWAN_APP_PORT; // Puerto de aplicación LoRaWAN
    m_lora_app_data.buffer[i++] = 'D'; // Datos a enviar
    m_lora_app_data.buffer[i++] = 'a';
    m_lora_app_data.buffer[i++] = 't';
    m_lora_app_data.buffer[i++] = 'o';
    m_lora_app_data.buffer[i++] = 's';
    m_lora_app_data.buffsize = i; // Tamaño del buffer de datos

    lmh_error_status error = lmh_send(&m_lora_app_data, LMH_UNCONFIRMED_MSG); // Envía los datos
    Serial.printf("Resultado del envío LoRa: %d\n", error); // Imprime el resultado del envío
}

// --- Handler para el timer de transmisión periódica ---
static void tx_lora_periodic_handler(void) {
    TimerSetValue(&appTimer, LORAWAN_APP_TX_DUTYCYCLE); // Reconfigura el timer
    TimerStart(&appTimer); // Reinicia el timer
    Serial.println("Enviando datos LoRa...");
    send_lora_frame(); // Envía el frame LoRa
}

// --- Inicialización del timer ---
static uint32_t timers_init(void) {
    appTimer.timerNum = 1; // Número de timer (puedes cambiarlo si usas otros timers)
    TimerInit(&appTimer, tx_lora_periodic_handler); // Inicializa el timer con el handler
    return 0; // Indica inicialización exitosa
}