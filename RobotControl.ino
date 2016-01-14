//Jacques Janse Van Vuuren
//12092369

#include <Servo.h> 
Servo myservo;

//----------------------  Stepper Motor  ----------------------
//------------------------------------------------------------- 
#define step1 7  //top arm
#define dir1 6

#define step2 10
#define dir2 9
//------------------------------------------------------------- 


//---------------------  Bumper Switches  ---------------------
//------------------------------------------------------------- 
#define button1 12
#define button2 13


int bumper1 = 0; //top bumper
int bumper2 = 0;
//------------------------------------------------------------- 


//----------------------  Robot Control  ----------------------
//------------------------------------------------------------- 
#define solenoid 2
#define electro 4

int stepInitSpeed = 7000;  //Initialize speed set
int stepSpeed = 7000;      //Move to speed set
int accConst = 210;

int arm1Dist =0;
int arm2Dist =0;

int diffArm1 =0;
int diffArm2 =0;

int topDist =0;
int bottomDist =0;


int countTop = 0;
int countBottom = 0;


//    7 --- 8 --- 9    //Point Location
//    4 --- 5 --- 6
//    1 --- 2 --- 3

int point1[2] = {268, 268};      //---Point Definition
int point2[2] = {14, 562};      // {topdist, botdist}
int point3[2] = {47, 545};

int point4[2] = {32, 635};
int point5[2] = {52, 605};
int point6[2] = {78, 578};

int point7[2] = {90, 680};
int point8[2] = {95, 640};
int point9[2] = {110, 602};

int partGreen[2] ={145, 145};
int partRed[2] ={190, 270};
int partBlue[2] ={187, 215};

int partCollector[2] ={50, 400};
//------------------------------------------------------------- 


//----------------------      Misc       ----------------------
//------------------------------------------------------------- 
boolean LinkEst = false;
boolean toggleOn = false;
boolean initializeToggle = false;
boolean solenoidOn = false;
boolean electroOn=false;
//------------------------------------------------------------- 










//----------------------      Setup      ----------------------
//------------------------------------------------------------- 
void setup() {
  pinMode(solenoid, OUTPUT);
  pinMode(electro,OUTPUT);
  
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  
  Serial.begin(9600);
  
  myservo.writeMicroseconds(1500); //set initial servo position if desired
  myservo.attach(11);
 
  }
//------------------------------------------------------------- 












//----------------------      Loop       ----------------------
//------------------------------------------------------------- 
void loop() {
  bumper1 = digitalRead(button1);  //---  right arm top
  bumper2 = digitalRead(button2);  //---  right arm bottom

  if(bumper1 == HIGH){digitalWrite(step1, LOW);}  //---   stop on bumper limit
  if(bumper2 == HIGH){digitalWrite(step1, LOW);}
  
  if (LinkEst ==false){                        //Initial link confirmation
     Serial.println("*** Robot Control v0.07 ***");    
     Serial.println("0 - Initialize");               //Serial commands
     Serial.println("1-9 - Move to point");
     Serial.println("d - Diamond Pattern");
     Serial.println("s - Square Pattern");
     Serial.println("q - Pick up"); 
     Serial.println("w - Drop off");
     Serial.println("c - ElectroMagent On");
     Serial.println("v - ElectroMagent Off");
     Serial.println("r - Part RED"); 
     Serial.println("g - Part GREEN");
     Serial.println("b - Part BLUE");
     Serial.println("y - Sliders UP");
     Serial.println("u - Sliders DOWN");
     Serial.println(" "); 
  LinkEst=true;}
  
  byte rByte = Serial.read();

  //********************     Servo Control     ******************
   if (rByte == 'l'){                            //Calibrate Servos
    Serial.print("Calibrating Servos... ");
    calServos();
    Serial.print("Complete!");}
    
   if (rByte == 'u'){                            //Servos DOWN
    Serial.print("Servos down... ");
    for(int x=20; x<130; x=x+10){
      delay(70);
      myservo.write(x);}
    delay(70);}
  
   if (rByte == 'y'){                          //Servos UP
    Serial.print("Servos up... ");
    for(int x=120; x>0; x=x-10){
      delay(70);
      myservo.write(x);}
    delay(70);}
  //*************************************************************


  //******************** Part Holder Positions ******************
  if (rByte == 'r'){                          //Red Part holder
    Serial.print("Moving RED... ");
    topDist = partRed[0];
    bottomDist =partRed[1];
    moveTo();}
    
  if (rByte == 'g'){                          //Green Part holder
    Serial.print("Moving GREEN... ");
    topDist = partGreen[0];
    bottomDist =partGreen[1];
    moveTo();}
    
  if (rByte == 'b'){                          //Blue Part holder
    Serial.print("Moving BLUE... ");
    topDist = partBlue[0];
    bottomDist =partBlue[1];
    moveTo();}
  //*************************************************************
  
  
  //******************** Electromagnet Control ******************
  if (rByte == 'c'){ 
    Serial.println("ElectroMagnet ON... ");
    electroOn =true;
    digitalWrite(electro, HIGH);
    delay(100);}
  if (rByte == 'v'){ 
    Serial.println("ElectroMagnet OFF... ");
    electroOn =false;
    digitalWrite(electro, LOW);
    delay(100);}
    
    
   if (rByte == 'q'){ 
    Serial.println("Pick up... ");
    pickUp();}
    
   if (rByte == 'w'){ 
    Serial.println("Drop Off... ");
    dropOff();}
    
   if (solenoidOn == true){digitalWrite(solenoid, HIGH);}
   if (solenoidOn == false){digitalWrite(solenoid, LOW);}
   
   if (electroOn == true){digitalWrite(electro, HIGH);}
   if (electroOn == false){digitalWrite(electro, LOW);}
  //*************************************************************
  
  
  //********************   SCARA Arm Movement  ******************
  if (rByte == 'h'){ 
    Serial.print("Homing... ");
    topDist = 50;
    bottomDist = 80;
    moveTo();}
  
  if (rByte == '0'){ 
    Serial.print("Initializing... ");
    initialize();}
  
  if (rByte == '1'){
    Serial.print("Moving Point 1... ");
    topDist = point1[0];
    bottomDist =point1[1];
    moveTo();}
  
  if (rByte == '2'){
    Serial.print("Moving Point 2... ");
    topDist = point2[0];
    bottomDist =point2[1];
    moveTo();}
  
  if (rByte == '3'){
    Serial.print("Moving Point 3... ");
    topDist = point3[0];
    bottomDist =point3[1];
    moveTo();}
  
  if (rByte == '4'){
    Serial.print("Moving Point 4... ");
    topDist = point4[0];
    bottomDist =point4[1];
    moveTo();}
  
  if (rByte == '5'){
    Serial.print("Moving Point 5... ");
    topDist = point5[0];
    bottomDist =point5[1];
    moveTo();}  
    
  if (rByte == '6'){
    Serial.print("Moving Point 6... ");
    topDist = point6[0];
    bottomDist =point6[1];
    moveTo();}  
    
  if (rByte == '7'){
    Serial.print("Moving Point 7... ");
    topDist = point7[0];
    bottomDist =point7[1];
    moveTo();}  
    
  if (rByte == '8'){
    Serial.print("Moving Point 8... ");
    topDist = point8[0];
    bottomDist =point8[1];
    moveTo();}  
    
  if (rByte == '9'){
    Serial.print("Moving Point 9... ");
    topDist = point9[0];
    bottomDist =point9[1];
    moveTo();}  
  //*************************************************************
    
    
  //******************   Pre-Programmed Patterns  *************** 
  if (rByte == 'd'){diamondPattern();}
  if (rByte == 's'){squarePattern();}
  //*************************************************************
  
}
//-------------------------------------------------------------










//--------------     Print Current Position      --------------
//------------------------------------------------------------- 
void printCurrentPos(){
  Serial.println(" ");
  Serial.println(" ");
  Serial.print("Arm1 Pos - ");
  Serial.println(countTop);
  Serial.print("Arm2 Pos - ");
  Serial.print(countBottom);
  Serial.println(" ");
  Serial.println(" ");}
//------------------------------------------------------------- 


//---------------      pickUp and dropOff       ---------------
//------------------------------------------------------------- 
void pickUp(){
    delay(1200);
    digitalWrite(solenoid, HIGH);
    electroOn=true;
    digitalWrite(electro, HIGH);
    delay(1200);
    digitalWrite(solenoid, LOW);
    delay(1200);}
    
void dropOff(){
    delay(1200);
    digitalWrite(solenoid, HIGH);
    delay(1200);
    electroOn=false;
    digitalWrite(electro, LOW);
    digitalWrite(solenoid, LOW);
    delay(1200);}
//-------------------------------------------------------------


//-------------------      Initialize       -------------------
//------------------------------------------------------------- 
void initialize(){
  long x;
  long runSpeed;
  x = 0;
  runSpeed = 50000;
  
  while (bumper1 == LOW) {          //Top arm calibrate
    bumper1 = digitalRead(button1);
    digitalWrite(dir1,HIGH);
    
    digitalWrite(step1, HIGH);
    delayMicroseconds(5);
    digitalWrite(step1, LOW);
    
    if(runSpeed > stepInitSpeed){
      runSpeed = 15000 - (accConst*x);}
    else{
      runSpeed = stepInitSpeed;}
    delayMicroseconds(runSpeed);
    x++;}
    countTop =0;
    Serial.print("| Top arm connection | ");
    
  x = 0;
  runSpeed = 50000;
  while (bumper2 == LOW) {          //Bottom plate calibrate
    bumper2 = digitalRead(button2);
    digitalWrite(dir2,HIGH);
    
    digitalWrite(step2, HIGH);
    delayMicroseconds(5);
    digitalWrite(step2, LOW);
    
    if(runSpeed > stepInitSpeed){
      runSpeed = 15000 - (accConst*x);}
    else{
      runSpeed = stepInitSpeed;}
      
    delayMicroseconds(runSpeed);
    x++;} 
    countBottom =0; 
    Serial.print("Bottom arm connection | ");
  
  
  Serial.println("Initialize Complete!");
  Serial.println(" ");
  initializeToggle = true;

  delay(1000);
  topDist = 50;
  bottomDist = 80;
  moveTo();
  delay(1200);
    
  printCurrentPos();}
//------------------------------------------------------------- 
  
  
  
  
//-----------------      Move to Points       -----------------
//------------------------------------------------------------- 
  void moveTo(){
    if(initializeToggle == true){
    Serial.print("Moving to - ");
    Serial.print(topDist);
    Serial.print(" , ");
    Serial.print(bottomDist);
    
    boolean a1Clockwise =false;
    boolean a2Clockwise =false;
    
    if(countTop > topDist){
      diffArm1 = countTop -topDist;
      a1Clockwise = true;
      digitalWrite(dir1,HIGH);}
    if(countTop < topDist){
      diffArm1 = topDist -countTop;
      a1Clockwise = false;
      digitalWrite(dir1,LOW);}
    
    if(countBottom > bottomDist){
      diffArm2 = countBottom -bottomDist;
      a2Clockwise = true;
      digitalWrite(dir2,HIGH);}
    if(countBottom < bottomDist){
      diffArm2 = bottomDist -countBottom;
      a2Clockwise = false;
      digitalWrite(dir2,LOW);}
    
    int x =0;
    long runSpeed =50000;
    int upAcc =0;
    long halfwayBottom =abs(bottomDist - countBottom)/2;
    Serial.print(" ---- ");
    Serial.println(halfwayBottom);
    
    while(countTop != topDist || countBottom != bottomDist){ 
      
        if(countTop != topDist){
          digitalWrite(step1, HIGH);
          if(a1Clockwise == true){countTop--;}
          if(a1Clockwise == false){countTop++;}}
    
        if(countBottom != bottomDist){
          digitalWrite(step2, HIGH);
          if(a2Clockwise == true){countBottom--;}
          if(a2Clockwise == false){countBottom++;}}
 
        delayMicroseconds(5);
        digitalWrite(step1, LOW);
        digitalWrite(step2, LOW);
        
        
        if(runSpeed > stepInitSpeed){
          runSpeed = 15000 - (accConst*upAcc);
          upAcc++;
          Serial.println(x);}
        else{runSpeed = stepSpeed;}
        
        if((countBottom+x) > countBottom+halfwayBottom){
         Serial.println(x);}
          
        delayMicroseconds(runSpeed);
        x++;}
    
    printCurrentPos();
    delay(500);}
    
    if(initializeToggle == false){Serial.println("*** Please Initialize First ***");}
  
}
 
//------------------------------------------------------------- 




//-----------------      Square Pattern       -----------------
//------------------------------------------------------------- 
  void squarePattern(){
    Serial.println("Square Pattern");
    if(initializeToggle == true){

    topDist = point1[0];
    bottomDist =point1[1];
    moveTo();

    topDist = point2[0];
    bottomDist =point2[1];
    moveTo();

    topDist = point3[0];
    bottomDist =point3[1];
    moveTo();

    topDist = point6[0];
    bottomDist =point6[1];
    moveTo();

    topDist = point9[0];
    bottomDist =point9[1];
    moveTo();

    topDist = point8[0];
    bottomDist =point8[1];
    moveTo();

    topDist = point7[0];
    bottomDist =point7[1];
    moveTo();

    topDist = point4[0];
    bottomDist =point4[1];
    moveTo();}

  
    if(initializeToggle == false){Serial.println("*** Please Initialize First ***");}}
//------------------------------------------------------------- 




//-----------------      Diamond Pattern       ----------------
//------------------------------------------------------------- 
  void diamondPattern(){
    Serial.println("Diamond Pattern");
    if(initializeToggle == true){
    topDist = point2[0];
    bottomDist =point2[1];
    moveTo();

    topDist = point4[0];
    bottomDist =point4[1];
    moveTo();

    topDist = point8[0];
    bottomDist =point8[1];
    moveTo();

    topDist = point6[0];
    bottomDist =point6[1];
    moveTo();}

    
    if(initializeToggle == false){Serial.println("*** Please Initialize First ***");}}
//------------------------------------------------------------- 


void calServos(){
   for(int x=90; x<130; x=x+10){
      delay(70);
      myservo.write(x);}
    delay(70);
    
   for(int x=110; x>0; x=x-10){
      delay(70);
      myservo.write(x);}
   delay(70);}




