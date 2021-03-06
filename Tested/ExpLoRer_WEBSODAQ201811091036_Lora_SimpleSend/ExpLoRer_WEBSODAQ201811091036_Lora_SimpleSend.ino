//MMO: https://support.sodaq.com/sodaq-one/lorawan/

#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

// USE YOUR OWN KEYS!
//MMO: const    declaracion deuna cosntante
//MMO: u        Sin signo
//MMO: int8     de 8bits
//MMO: _t       simplemente para decir que es de "type", type unsigned 8bits
const uint8_t devAddr[4] =
{
  0x00, 0x00, 0x00, 0x00
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Some complete random hex
uint8_t testPayload[] =
{
  0x53, 0x4F, 0x44, 0x41, 0x51
};

void setup()
{
  while ((!debugSerial) && (millis() < 10000));

  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  debugSerial.println(LoRaBee.getDefaultBaudRate());

  LoRaBee.setDiag(debugSerial); // optional

  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false)) //MMO: Activation by Personalization
  {
    debugSerial.println("Connection to the network was successful.");
  }
  else
  {
    debugSerial.println("Connection to the network failed!");
  }

 // Uncomment this line to for the RN2903 with the Actility Network
 //For OTAA update the DEFAULT_FSB in the library
 LoRaBee.setFsbChannels(1);

}

void loop()
{
  debugSerial.println("Sleeping for 5 seconds before starting sending out test packets.");
  for (uint8_t i = 5; i > 0; i--)
  {
    debugSerial.println(i);
    delay(1000);
  }

  // send 10 packets, with at least a 5 seconds delay after each transmission (more seconds if the device is busy)
  uint8_t i = 10;
  while (i > 0)
  {
    switch (LoRaBee.send(1, testPayload, 5))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      i--;
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe program will now .");
      while (1) {};
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
    delay(60000);
  }
}
