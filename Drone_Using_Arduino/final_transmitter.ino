#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);

//We create variables for the time width values of each PWM input signal
unsigned long counter_1, current_count;

//We create 4 variables to stopre the previous value of the input signal (if LOW or HIGH)
byte last_CH1_state;

//To store the 1000us to 2000us value we create variables and store each channel
int Ch1;      //In my case channel 1 of the receiver and pin D8 of arduino

const byte address[6] = "00001";
// Arduino pin numbers 
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int SW_pin1 = 3; // digital pin connected to switch output
const int right_pin = 2; // analog pin connected to X output
const int left_pin = 3; // analog pin connected to Y output


void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);
  PCICR |= (1 << PCIE0);    //enable PCMSK0 scan                                                 
  PCMSK0 |= (1 << PCINT0);  //Set pin D8 trigger an interrupt on state change.
  Serial.begin(115200);  
}

void loop() {
  Serial.print("Ch1: ");
  Serial.print(Ch1);
  Serial.println("    ");
  const char text[] = "nrftest";
  radio.write(&text, sizeof(text));
  delay(500);
  Serial.print("Switch: ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin));
  Serial.print("Switch: ");
  Serial.print(digitalRead(SW_pin1));
  Serial.print("right-axis: ");
  Serial.print(analogRead(right_pin));
  Serial.print("left-axis: ");
  Serial.println(analogRead(left_pin));
  delay(500);
}

ISR(PCINT0_vect){
//First we take the current count value in micro seconds using the micros() function
  
  current_count = micros();
  if(PINB & B00000001){                              //We make an AND with the pin state register, We verify if pin 8 is HIGH???
    if(last_CH1_state == 0){                         //If the last state was 0, then we have a state change...
      last_CH1_state = 1;                            //Store the current state into the last state for the next loop
      counter_1 = current_count;                     //Set counter_1 to current value.
    }
  }
  else if(last_CH1_state == 1){                      //If pin 8 is LOW and the last state was HIGH then we have a state change      
    last_CH1_state = 0;                              //Store the current state into the last state for the next loop
    Ch1 = current_count - counter_1;   //We make the time difference. Channel 1 is current_time - timer_1.
  }
 
}
 
