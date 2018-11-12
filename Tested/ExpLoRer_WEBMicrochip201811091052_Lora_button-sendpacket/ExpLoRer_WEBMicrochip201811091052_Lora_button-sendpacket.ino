/*
* MMO:https://www.microchip.com/forums/m998378.aspx
* RN2903 sodaq_hit_button_send_packet.ino
*/


/********************************
* sodaq_hit_button_send_packet.ino rev 1
* Pete Wilson P.E. 2017-06-09
*
* This program connects the Sodag Explorer board with MCHP RN2903 to the Loriot cloud using a MultiTech gateway.
* It's pretty simple. Join the network, hit BTN2 and it sends a packet containing the hitCount to Loriot's server.
* There is a console output and flashing LEDS just for fun. You should see packets showup in Loriot SampleApp,
* but you may have to refresh the webpage occationally.
*
* This program is a mix of example sketches from from support.sodaq.com
* Also check out http://support.sodaq.com/explorer/ for environment setup, hardware and pin defintion details.
* Additionally note the modified Sodaq_RN2483.h file #define DEFAULT_FSB 1 for the US frequencies.
* This sketch binds in ABP mode. There is no handshaking. Similar to UDP, the sender does not recieve ACK.
* Radio transmission can be "successful" without a gateway. The packets just fall on the floor.
* The keys are assigned by Loriot and may be different for different user accounts.
* To see key values login to Loriot.io go to..
* Dashboard|Applcaiions|SampleApp and enroll device. You may have to run get_hwEUI.ino to get the DevEUI (the last 16 bytes of the hwEUI)
* then go to..
* Dashboard|Applcaiions|SampleApp|Devices and click on the DeviceEUI hypertest. Look for LoRaWAN AES128 Keys.
*
* Also note this is an ABP connection so there is no handshaking or ACK.
* Similar to UDP packets. The transmission can be successful without a gateway.
*/


#include <Sodaq_RN2483.h>
#include <Sodaq_RN2483_internal.h>
#include <Utils.h>
#include <Sodaq_RN2483.h>
//#include <Sodaq_RN2903.h>
#define debugSerial SerialUSB
#define loraSerial Serial2
enum txStates_t {TX_STATE_READY,TX_STATE_SEND,TX_STATE_CRASH};
enum txStates_t txState;
enum passFail_t {PASS,FAIL};

// USE YOUR OWN KEYS!
const uint8_t devAddr[4] ={
  //0x00, 0x00, 0x00, 0x00
  //00-7E-21-BC
  0x00, 0x7e, 0x21, 0xbc};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] ={
  //F6-92-CD-D5-59-FB-6F-8F-83-A2-71-58-47-2B-04-F3
  0xf6, 0x92, 0xcd, 0xd5, 0x59, 0xfb, 0x6f, 0x8f, 0x83, 0xa2, 0x71, 0x58, 0x47, 0x2b, 0x04, 0xf3 //appSKey assigned by Loriot
  };

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] ={
  //C9-D3-49-66-F4-1D-5B-A5-5C-82-29-56-12-8B-4B-9B
  0xc9, 0xd3, 0x49, 0x66, 0xf4, 0x1d, 0x5b, 0xa5, 0x5c, 0x82, 0x29, 0x56, 0x12, 0x8b, 0x4b, 0x9b //nwkSKey assigned by Loriot
  };

// Some complete random hex
uint8_t testPayload[] ={
  0x00, 0x00, 0x00, 0x00, 0x00 //Make up your own payload, the hitCount.
  };
int hitCount;


void setup(){
  while ((!debugSerial) && (millis() < 10000));

  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  LoRaBee.setDiag(debugSerial); // optional
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false)){
    debugSerial.println("Connection to the network was successful.");}
  else{debugSerial.println("Connection to the network failed!");}

  // Configure the button as an input and enable the internal pull-up resistor
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  txState = TX_STATE_READY;
  hitCount = 0;}

void loop(){
  if (txState == TX_STATE_READY){
    // Read the button value into a variable
    int sensorVal = digitalRead(BUTTON);
    // Turn on the LED when the Button is pushed
    // LOW = hit
  if (sensorVal == HIGH) {
  extinguishLEDs();}
  else {
    hitCount++;
    lightGreenLED();
    txState = TX_STATE_SEND;}}

  switch (txState){
    case TX_STATE_READY:
      debugSerial.println("TX_STATE_READY");
      extinguishLEDs();
      lightBlueLED();
      delay(500);
      extinguishLEDs();
      delay(500);
    break;
    case TX_STATE_SEND:
      debugSerial.println("TX_STATE_SEND");
      testPayload[0] = hitCount;
      debugSerial.print("hitCount = ");
      debugSerial.print(hitCount);
      debugSerial.println(); //new line
      if (sendPacket() == PASS){
        txState = TX_STATE_READY;
        extinguishLEDs();}
        else{txState = TX_STATE_CRASH;}
      break;
    case TX_STATE_CRASH:
      debugSerial.println("TX_STATE_CRASH!");
      extinguishLEDs();
      lightRedLED();
      trap();
      break;
    default:{
      txState = TX_STATE_CRASH; //Program should never get here.
      break;}}}

void lightGreenLED(){digitalWrite(LED_GREEN, LOW);} //light LED

void lightRedLED(){digitalWrite(LED_RED, LOW);}//light LED

void lightBlueLED(){digitalWrite(LED_BLUE, LOW);}//light LED

void extinguishLEDs(){
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);}

void trap(){
  while (1){
  //__asm__("nop");
  }}

passFail_t sendPacket(){
  enum passFail_t result;
  result = FAIL; //assume fail
  // debugSerial.println("Sleeping for 5 seconds before starting sending out test packets.");
  // for (uint8_t i = 5; i > 0; i--)
  debugSerial.println("Countdown...");
  for (uint8_t i = 3; i > 0; i--){
    debugSerial.println(i);
    delay(1000);}

  switch (LoRaBee.send(1, testPayload, 5)){
    case NoError:
      debugSerial.println("Successful transmission.");
      result = PASS; //packet TX success!
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
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe program will now halt.");
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
      break;}
  return result;}
