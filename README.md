# Ejemplos de Uso para Placa Personalizada

Este repositorio contiene ejemplos de código para interactuar con diferentes componentes integrados en una placa de desarrollo personalizada. A continuación, se describen brevemente cada uno de los ejemplos y los componentes que utilizan.

## Contenido

1.  **ADC ADS7830:** Este ejemplo (aunque el código proporcionado utiliza la librería del acelerómetro LIS3DH) ilustraría cómo leer valores analógicos utilizando el convertidor analógico a digital ADS7830. El componente principal utilizado sería el **ADC ADS7830**.

2.  **Acelerómetro LIS3DH:** Este ejemplo muestra cómo inicializar y leer los datos de aceleración en los tres ejes (X, Y, Z) utilizando el acelerómetro LIS3DH. El componente principal utilizado es el **Acelerómetro LIS3DH**.

3.  **Módulo LoRa Ra-SH01:** Este ejemplo detalla la configuración e inicialización del módulo LoRa Ra-SH01 para la comunicación LoRaWAN. Incluye la configuración de pines, credenciales de red y un ejemplo básico de envío de datos. El componente principal utilizado es el **Módulo LoRa Ra-SH01**.

4.  **Módulo NRF24L01 (Transmisor):** Este ejemplo muestra cómo configurar el módulo NRF24L01 para transmitir datos de forma inalámbrica. Se enfoca en la inicialización del radio y la configuración de la dirección de escritura. El componente principal utilizado es el **Módulo NRF24L01**.

5.  **Módulo NRF24L01 (Receptor):** Este ejemplo ilustra cómo configurar el módulo NRF24L01 para recibir datos de forma inalámbrica. Se centra en la inicialización del radio, la apertura de un canal de lectura y la recepción de mensajes. El componente principal utilizado es el **Módulo NRF24L01**.

6.  **Módulo NRF24L01 (Transmisor - Ejemplo 2):** Similar al ejemplo 4, este muestra una configuración de transmisor del NRF24L01 con un ejemplo de envío periódico de un mensaje de texto. El componente principal utilizado es el **Módulo NRF24L01**.

7.  **Expansor de E/S PCF8574:** Este ejemplo explica cómo utilizar el expansor de entradas/salidas PCF8574 a través del protocolo I2C para controlar pines digitales, en este caso, encendiendo y apagando LEDs de forma secuencial. El componente principal utilizado es el **Expansor de E/S PCF8574**.

8.  **RTC DS3231:** Este ejemplo muestra cómo interactuar con el reloj de tiempo real DS3231 utilizando el protocolo I2C para obtener la fecha y hora actual, y también cómo detectar y configurar la hora en caso de pérdida de energía. El componente principal utilizado es el **RTC DS3231**.

Este README proporciona una visión general de los ejemplos de uso incluidos en este repositorio y los componentes de hardware asociados a cada uno. Para ver el código de cada ejemplo, por favor, consulta los archivos individuales en el repositorio.
