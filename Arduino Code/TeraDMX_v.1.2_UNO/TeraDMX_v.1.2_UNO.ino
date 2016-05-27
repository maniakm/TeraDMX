#include <Conceptinetics.h>

#define DMX_SLAVE_ADDRESS    1 //DMX address
#define DMX_SLAVE_CHANNELS   4 //DMX channes

// Configure a DMX slave controller
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

//PIN configuration
const int StatusLed = 13;
const int PWM_R = 11;
const int PWM_G = 10;
const int PWM_B = 9;

unsigned long       lastFrameReceivedTime;
const unsigned long dmxTimeoutMillis = 10000UL;

// the setup routine runs once when you press reset:
void setup() {             

  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();  
  
  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (DMX_SLAVE_ADDRESS);

  //
  // Register on frame complete event to determine signal timeout
  //
  dmx_slave.onReceiveComplete ( OnFrameReceiveComplete );
  
  // Set led pin as output pin
  pinMode ( PWM_R, OUTPUT );
  pinMode ( PWM_G, OUTPUT );
  pinMode ( PWM_B, OUTPUT );
  pinMode ( StatusLed, OUTPUT );
}

// the loop routine runs over and over again forever:
void loop() 
{
  
  float M = dmx_slave.getChannelValue (1) / 255.0f;
  int R = dmx_slave.getChannelValue (2) * M;
  int G = dmx_slave.getChannelValue (3) * M;
  int B = dmx_slave.getChannelValue (4) * M;

  analogWrite(PWM_R,R);
  analogWrite(PWM_G,G);
  analogWrite(PWM_B,B);

  if(millis() - lastFrameReceivedTime > 3000)
  {
    digitalWrite(StatusLed,HIGH);
    analogWrite(PWM_R,255);
    analogWrite(PWM_G,0);
    analogWrite(PWM_B,0);
  }
}

bool sta = false;
void OnFrameReceiveComplete (unsigned short channelsReceived)
{
  if ( channelsReceived == DMX_SLAVE_CHANNELS)
  {
    // All slave channels have been received
    //digitalWrite(StatusLed,LOW);    
  }
  else
  {
    // We have received a frame but not all channels we where 
    // waiting for, master might have transmitted less
    // channels
  }
  
  digitalWrite(StatusLed,sta);
  sta = !sta;
  // Update receive time to determine signal timeout
  lastFrameReceivedTime = millis ();
}
