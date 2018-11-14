/*
Fracmento copiado de "C:\Users\Ruth\Documents\Arduino\libraries\Sodaq_RN2483\Readme.md"
Caracteristicas:
- Intenta espablecer comunicacion con gateway con valores dados
- tiene monitor que espera, al menos 10 segundo para conocer resultados de la conexión

Prueba fallida
El monotor me genera "Connection to the network failed!"
este código intenta establecer el enlace con la infomación que le doy, habria que crear un código con los valores establecidos por el gateway
Probablemente falte la inicializacion del LoRa

Secuencia del código:
-

*/

#include "Sodaq_RN2483.h" //MMO: asegurar "" o <>

#define loraSerial Serial2 //MMO: asegurar sii Serial2 es el pueto del lora
#define debugSerial SerialUSB

// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
	0x00, 0x00, 0x02, 0x03
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
	0x0D, 0x0E, 0x0A, 0x0D,
	0x0B, 0x0E, 0x0E, 0x0F,
	0x0C, 0x0A, 0x0F, 0x0E,
	0x0B, 0x0A, 0x0B, 0x0E,
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
	0x0D, 0x0E, 0x0A, 0x0D,
	0x0B, 0x0E, 0x0E, 0x0F,
	0x0C, 0x0A, 0x0F, 0x0E,
	0x0B, 0x0A, 0x0B, 0x0E,
};

const uint8_t testPayload[] =
{
	0x30, 0x31, 0xFF, 0xDE, 0xAD
};

void setup()
{
	while ((!debugSerial) && (millis() < 10000));//MMO: obligo a que me epere a  abrir el monitor, almennos por 10 segundos

	int MMObaudrateLora=LoRaBee.getDefaultBaudRate();
	loraSerial.begin(MMObaudrateLora);
	debugSerial.println(MMObaudrateLora);

	if (!LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, true))
	{
		debugSerial.println("Connection to the network failed!");
		return;
	}

	if (LoRaBee.sendReqAck(1, testPayload, 5, 3) != NoError)
	{
		debugSerial.println("Failed to send the packet!");
	}
}

void loop()
{
}
