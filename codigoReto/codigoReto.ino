//joystick https://arduinogetstarted.com/tutorials/arduino-joystick
//driver https://www.diarioelectronicohoy.com/blog/descripcion-del-driver-a4988
//stepper https://components101.com/sites/default/files/component_datasheet/NEMA17.pdf

#include <ezButton.h>

//define pins
int x_pin=A0; // Arduino pin connected to VRX pin
int button_pin=2;  // Arduino pin connected to SW  pin
int sol_pin=3;
int step_pin=8;
int dir_pin=9;
int switch_der_pin=A1;
int switch_izq_pin=A2;

//initial values
int step_delay=300;
int xValue = 0; // To store value of the X axis
int bValue = 0; // To store value of the button

//define joystick button
ezButton button(button_pin);

void setup() {
  Serial.begin(9600) ;
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  pinMode(sol_pin,OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
}

void loop() {
  
  button.loop(); // MUST call the loop() function first
  
  // read analog X values from joystick
  xValue = analogRead(x_pin);
  // Read the button value
  bValue = button.getState();

  //if button is pressed SHOOT solenoid
  if(bValue==0){
    //Serial.println("shoot");
    digitalWrite(sol_pin,HIGH);
  }

  //if button not pressed CUT solenoid
  if(bValue==1){
    //Serial.println("stop shoot");
    digitalWrite(sol_pin,LOW);
  }

  //move right
  if(xValue>520 && analogRead(switch_der_pin)!=1023){
    digitalWrite(dir_pin,HIGH);
    while(xValue>600){
      if(xValue<600 || analogRead(switch_der_pin)==1023){
        break;
      }
      xValue = analogRead(x_pin);
      digitalWrite(step_pin, HIGH);
      delayMicroseconds(step_delay);
      digitalWrite(step_pin, LOW);
      delayMicroseconds(step_delay);
    }
  }else{
    //move left
    if(xValue<500 && analogRead(switch_izq_pin)!=1023){
      digitalWrite(dir_pin,LOW);
      while(xValue<450){
        if(xValue>450 || analogRead(switch_izq_pin)==1023){
          break;
        }
        xValue = analogRead(x_pin);
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(step_delay);
        digitalWrite(step_pin, LOW);
        delayMicroseconds(step_delay);
      }
    }else{
      digitalWrite(step_pin, LOW);
    }
  }
  
  // print joystick values
  Serial.print("x = ");
  Serial.print(xValue);
  Serial.print(" : button = ");
  Serial.println(bValue);  
}
