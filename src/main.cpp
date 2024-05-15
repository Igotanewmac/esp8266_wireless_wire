#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <espnow.h>



#define PIN_LED 2




// decide which board we want to turn on.
#define MULTIBOARD

// this is the target to send to, so should be the opposite of what is plugged in.
// i.e.
// if board A is plugged in, uncomment board B
// if board B is plugged in, uncomment board A

// board A mac address
uint8_t mac_partner[] = { 0x30 , 0x83 , 0x98 , 0xB0 , 0xFD , 0x73 };

// board B mac address
// uint8_t mac_partner[] = { 0x30 , 0x83 , 0x98 , 0xB0 , 0xE7 , 0xB9 };


// declare our io pins
#define PIN_RX 3
#define PIN_TX 1




#ifdef MULTIBOARD


void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);



void setup() {

  // set up our pins

  // set up receive input on rx
  pinMode( PIN_RX , INPUT );
  
  // set up transmit output on tx
  pinMode( PIN_TX , OUTPUT );
  digitalWrite( PIN_TX , LOW );
  
  // configure the onboard led
  pinMode( PIN_LED , OUTPUT );
  digitalWrite( PIN_LED , HIGH );


  
  // set up the espnow link
  
  // enable wifi mode as a station
  WiFi.mode(WIFI_STA);

  // initialise espnow network
  if ( esp_now_init() != 0 ) {
    while (1) {
      // if we cannot initialise, blink the led continously.
      digitalWrite( PIN_LED , !digitalRead( PIN_LED ) );
      delay(100);
    }
  }

  // set our role as both sender and receiver
  esp_now_set_self_role( ESP_NOW_ROLE_COMBO );
  
  // add our peer asboth sender and receiver
  esp_now_add_peer( mac_partner , ESP_NOW_ROLE_COMBO , 1 , NULL , 0 );

  // register our callback for when we receive data
  esp_now_register_recv_cb(OnDataRecv);


  
}


// state tracking for rx pin
uint8_t pin_previous_state = 0;
uint8_t pin_current_state = 0;

void loop() {

  // fetch and check the current state of the RX pin
  pin_current_state = digitalRead( PIN_RX );

  if ( pin_previous_state == pin_current_state ) {
    // nothing to change, return early
    return;
  }

  // something has changed!

  // update our previous state
  pin_previous_state = pin_current_state;

  // broadcast the change
  esp_now_send( mac_partner , &pin_current_state , 0x01 );

  // update the led
  digitalWrite( PIN_LED , pin_current_state ? LOW : HIGH );

}






// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  // we have received an update!
  // just write it out to the tx pin.
  digitalWrite( PIN_TX , *incomingData );
}







#endif

























