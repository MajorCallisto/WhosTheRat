#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define ISMEGA true
#else
#define ISMEGA false
#endif


//Multiplexer
int r0 = 0;      //value of select pin at the 4051 (s0)
int r1 = 0;      //value of select pin at the 4051 (s1)
int r2 = 0;      //value of select pin at the 4051 (s2)
int count = 0;   //which y pin we are selecting
int potValues[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//Array of 12 values - 2 pots per player

//LEDs are plugged into 22-28

int multi_s0 = 2;//Multiplexer pin to S2
int multi_s1 = 3;//Multiplexer pin to S1
int multi_s2 = 4;//Multiplexer pin to S0

int numPlayers = 6;
int playerScoreHealth[6] = {4, 4, 4, 4, 4};
int playerScoreRange[6] = {0, 0, 0, 0, 0};
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
 
byte leds = 0;
byte leds2 = 0;
byte ledGraph[6] = {0,0,0,0,0,0};

int scoreHealth =4;
int scoreRange = 4;

const boolean TESTMODE = false;

void setup() 
{
  Serial.begin(9600);
  //Set the pinmode for the pins used with the
  //shift register
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  //Update the scores
  //based on the initial playerScoreHealth
  //and playerScoreRange
  updateScoreBar();
   
  //Init multiplexer
  //using pins 2, 3 and 4 set to output
  pinMode(multi_s0, OUTPUT);    // s0
  pinMode(multi_s1, OUTPUT);    // s1
  pinMode(multi_s2, OUTPUT);    // s2
  
  //LED player kill lights
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  
  //These are the pins we read
  //to get the multiplexer values
  pinMode(A2, INPUT);    // A2 Multiplexer 1
  pinMode(A3, INPUT);    // A3 Multiplexer 2
}

void loop() 
{
  
  //Update the Pot values from the multiplexers
  updatePotValues();
  
  //Update the Player Scores using the new Pot Values
  updatePlayerScores();
  
  if (TESTMODE == true){
    useTestValues();
  }
  
  //Update the LEDs from the new Player Scores
  updateScoreBar();
  delay(30);
}

//If TESTMODE = false, this function will run
void useTestValues(){
  
    for (int count=0; count<12; count++) {
      potValues[count] = random(1024);
    }
    
    updatePlayerScores();
    static int h = 0;
    static int hI = 1;
    static int r = 2;
    static int rI = 1;
    playerScoreHealth[0] = h;
    playerScoreRange[0] = r;

    if (r%3 ==0){
      h+= hI;
    }
    if (h > 4 || h < 0){hI = hI *-1;h+=hI;}
    r+= rI;
    if (r > 4 || r < 0){rI = rI *-1;r+=rI;} 
}

//Loop through each of the scores and adjust them using
//The pot values
void updatePlayerScores(){
  for (int i=0; i<numPlayers; i ++){
    //Make the health pot turn opposite from the range pot
    playerScoreHealth[i]  = float(1024.00-potValues[i*2])/1024.0*5.0;
    playerScoreRange[i]  = float(potValues[i*2+1])/1024.0*5.0;

    //If this is on a mega, use pins 22-27 for the 
    //LED indicator
    if (ISMEGA == true){
      if (playerScoreHealth[i] <= 1){digitalWrite(22+i, HIGH);}else if (playerScoreHealth[i] > 1){digitalWrite(22+i, LOW);};
    }
  }
}

//Reset all the byte values used
//with the shift register to be off
//and update the shift register
void ledsOff(){
  for (int i=0; i<numPlayers; i ++){
    ledGraph[i] = 0;
  }
  updateShiftRegister();
}

//
void updateScoreBar(){
  ledsOff();//Turn everything off first
  for (int i=0; i<numPlayers; i ++){
   setHealth(i);
   setRange(i);
  }
  updateShiftRegister();
}
//Left Side Health
void setHealth(int player){
  int val = playerScoreHealth[player];
    for (int i = 0; i <val; i++){
      bitSet(ledGraph[player], 7-i);
    }
}  
//Right Side Range
void setRange(int player){
    int val = playerScoreRange[player];
    for (int i = 0; i <val; i++){
      bitSet(ledGraph[player], i);
    }
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
    for (int i=0; i<numPlayers; i ++){
      shiftOut(dataPin, clockPin, LSBFIRST, ledGraph[numPlayers-i-1]);
    }
   digitalWrite(latchPin, HIGH);
}

void updatePotValues(){
  for (count=0; count<=7; count++) {
    r0 = bitRead(count,0);    // use this with arduino 0013 (and newer versions)     
    r1 = bitRead(count,1);    // use this with arduino 0013 (and newer versions)     
    r2 = bitRead(count,2);    // use this with arduino 0013 (and newer versions)     
    digitalWrite(multi_s0, r0);
    digitalWrite(multi_s1, r1);
    digitalWrite(multi_s2, r2);
    
    potValues[count] = analogRead(A2);
    if (count <4){
      potValues[count+8] = analogRead(A3);
    }
  }
  
//  for (count=0; count<=11; count++) {
//    Serial.print ( potValues[count]);
//    Serial.print(" ");
//  }
//  Serial.println("");
}
