#include <Sodaq_RN2483.h>
#include <Sodaq_RN2483_internal.h>
#include <Utils.h>

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
- parámetro iniciales para el initOTA()
		devAddr[4]
		appSKey[16]
		nwkSKey[16]
		testPayload[]

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
	//while ((!debugSerial) && (millis() < 10000));//MMO: obligo a que me epere a  abrir el monitor, almennos por 10 segundos
	while (!debugSerial); //MMO: sino se ejecuta el monitor no se ejecuta el código
	//y lo coloco al principio para que no se ejecute nada hasta que habra el Monitor

	int MMObaudrateLora = LoRaBee.getDefaultBaudRate();
	loraSerial.begin(MMObaudrateLora);
	debugSerial.println(MMObaudrateLora);

	LoRaBee.setDiag(debugSerial); // MMO: Supuetamente

	if (!LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, true))
	{
		debugSerial.println("Connection to the network failed!");
		return;
	}

	if (LoRaBee.sendReqAck(1, testPayload, 5, 3) != NoError)
	{
		debugSerial.println("Failed to send the packet!");
	}




	// Uncomment this line to for the RN2903 with the Actility Network
	//For OTAA update the DEFAULT_FSB in the library
	int MMO_setFsbChannels=LoRaBee.setFsbChannels(0);
	// Enables all the channels that belong to the given Frequency Sub-Band (FSB)
	// and disables the rest.
	// fsb is [1, 8] or 0 to enable all channels.
	// Returns true if all channels were set successfully.
	//bool setFsbChannels(uint8_t fsb);
	/*
	delay(1100);
	if (MMO_setFsbChannels)
	{
		debugSerial.println("True el setFsbChannels");
	}
	else
	{
		debugSerial.println("False el setFsbChannels");
	}
	*/
}

void loop()
{
	//debugSerial.println(LoRaBee.STR_CMD_SET_CHANNEL_STATUS);
}
