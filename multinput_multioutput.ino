//pin constants
//referenced from "Analog input and the serial monitor": https://www.youtube.com/watch?v=zZS4XYam_Sw&ab_channel=ScottFitzgerald
const int ledPin = 2;  
const int potPin = A7;  

//setup //referenced from "Analog input and the serial monitor": https://www.youtube.com/watch?v=zZS4XYam_Sw&ab_channel=ScottFitzgerald
void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);

  while (!Serial) {
    ;;
  }
}

void loop() {
  
  //The game begins and Player 1 is prompted to press the enter key to start the game
  Serial.println("Welcome to who can turn the knob the fastest!");
  Serial.println("Player 1, please press enter to begin your turn.");

  //Waits for the user to press the enter key before proceeding with the game / used Chat GPT for understanding this functionality & correct syntax
  //Referenced Arduino Library While: https://www.arduino.cc/reference/en/language/structure/control-structure/while/
  //Referenced Arduino Library Serial.available: https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
  //Referenced Arduino Library Serial.read: https://www.arduino.cc/reference/cs/language/functions/communication/serial/read/
  while (Serial.available() <= 0);
  while (Serial.available() > 0) {
    Serial.read(); 
  }

  //Player 1's turn begins and stores the recorded speed information in potVal1 for the result to be displayed at the end of the game
  //Player 1 is asked to reset the knob by calling the knob reset function
  //Turn off LED light and wait a bit before starting Player 2's Turn
  int potVal1 = recordSpeed();
  Serial.println("Player 1, please reset the knob back to its original position.");
  knobReset();
  analogWrite(ledPin, 0);
  delay(500); 

  //Player 2 is now prompted to press the enter key to start the game (same reference as previously from Player 1)
  Serial.println("Press Enter to start Player 2's turn.");
  while (Serial.available() <= 0);
  while (Serial.available() > 0) {
    Serial.read(); 
  }

  //Player 2's turn begins and stores the recorded speed information in potVal2 for the result to be displayed at the end of the game
  int potVal2 = recordSpeed();

  //End of game results in a series of if else statements comparing potVal1 and potVal2 to determine if it is a win, loss, or tie for the players
  Serial.println("Game over!");
  if (potVal1 < potVal2) {
    Serial.println("Player 1 wins!");
    Serial.print("Player 1 speed: ");
    Serial.println(potVal1);
    Serial.print("Player 2 speed: ");
    Serial.println(potVal2);
  } else if (potVal1 > potVal2) {
    Serial.println("Player 2 wins!");
    Serial.print("Player 1 speed: ");
    Serial.println(potVal1);
    Serial.print("Player 2 speed: ");
    Serial.println(potVal2);
  } else {
    Serial.println("It's a tie!");
    Serial.print("Both players had a speed of: ");
    Serial.println(potVal1);
  }

  //extended wait time as the game resets
  delay(5000);
}

//recordSpeed is the speed recorded and assigned as an integer value for potVal to display at the end
int recordSpeed() {
  //Begins to record the start time using milliseconds using unsigned long that don't store negative numbers since it is tracking time
  //Arduino Library reference: https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
  unsigned long startTime = millis();

  //Potentiometer value is read and stored
  //Referenced from "Analog input and the serial monitor": https://www.youtube.com/watch?v=zZS4XYam_Sw&ab_channel=ScottFitzgerald
  int initialPotVal = analogRead(potPin);
  int currentPotVal = initialPotVal;

  //Tracks speed of the knob until it reaches 1023 which is the maximum value
  //LED light's brightness is updated based on the potentiometer's knob position
  while (currentPotVal < 1023) {
    int ledVal = map(currentPotVal, 0, 1023, 0, 255);
    analogWrite(ledPin, ledVal);
    currentPotVal = analogRead(potPin);
  }

  //LED light is turned off after the maximum value has been reached as a visual indicator
  analogWrite(ledPin, 0);

  //End time is recorded the same way beginTime is
  unsigned long endTime = millis();

  //Speed is finally calculated in turns per second and returned as such
  unsigned long elapsedTime = endTime - startTime;
  int speed = 1023 / (elapsedTime / 1000); 
  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.println(" turns per second");

  return speed;
}

//knobReset is called as a void as there is no integer value being returned
void knobReset() {
  
  //Potentiometer value is read and stored similarly to recordSpeed
  int initialPotVal = analogRead(potPin);
  int currentPotVal = initialPotVal;

  //Tracks the potentiometer's knob turning in a counterclockwise direction within a threshold of 5 as there is some flickering occuring with the potentiometer / used chat gpt to iron out this detail
  //LED light's brightness is updated based on the potentiometer's knob position
  while (currentPotVal < 1023 && currentPotVal <= initialPotVal + 5) {
    int ledVal = map(currentPotVal, 0, 1023, 0, 255);
    analogWrite(ledPin, ledVal);
    currentPotVal = analogRead(potPin);
  }

  //LED light is turned off after the maximum value has been reached as a visual indicator
  analogWrite(ledPin, 0);
}

