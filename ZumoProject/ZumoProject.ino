#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4IMU gyro;

const int forwardTime = 500;

// motor speed when turning
uint16_t turnSpeed = 150;

uint16_t leftForwardSpeed = 150;
uint16_t rightForwardSpeed = 150;

//bool pause = false;

#include "proxSensors.h"

#include "types.h"

//current position of zumo
POSITION currPosition;

#include "movement.h"

// sets mode to start
MODE currMode = start;

const int BUFFER_SIZE = 50;
char buf[BUFFER_SIZE];

// holds position of people found
POSITION found[10];

// records positions on robots route
POSITION route[20];

void setup() {
  
  Serial1.begin(9600);

  // sets up line sensors and gyro
  movementSetup();

  // sets up proximity sensors
  proxSetup();

  // sets current position of robot to start.
  currPosition = START;

  Serial1.println("Setup Complete!");

}

// reads from Serial1 and returns string
String readSerial(){

  String data;

  if(Serial1.available()) // Check for availablity of data at Serial Port
  {
    int e = Serial1.readBytes(buf, BUFFER_SIZE); // reading number of bytes available and passing chars into array.

    for(int t = 0; t < e;t++)
    {
      char d = buf[t];
      data += d;
    }
    return data;// Printing the Serial data
  }
  return data;
}

// Manual control of robot using w,a,s,d to move,
// e to stop and v to scan a room
bool manualControl()
{
  bool end = false;
  if (Serial1.available() > 0){
    char inByte = Serial1.read(); // reads from controller

    Serial1.print(inByte); //prints controller response

    switch(inByte){
      case 'w':
        // moves forward
        //leftForwardSpeed = 100;
        //rightForwardSpeed = 100;
        motors.setSpeeds(straightSpeed, straightSpeed);
        break;
      case 'a':
        // turns left
        leftForwardSpeed = -leftForwardSpeed;
        rightForwardSpeed = 150;

        motors.setSpeeds(leftForwardSpeed, rightForwardSpeed);
        turnSensorUpdate();
        break;
      case 's':
        // stops motors
        stop();
        break;
      case 'd':
        // turns right
        rightForwardSpeed = -rightForwardSpeed;
        leftForwardSpeed = 150;

        motors.setSpeeds(leftForwardSpeed, rightForwardSpeed);
        turnSensorUpdate();
        break;
      case 'e':
        // stops motors and ends mode
        stop();
        end = true;
        break;
      case 'v':
        Serial1.println("Is this a Left Room or a Right Room? (L/R)");
        bool dir = waitForInstruction();
        if(dir)
        {
          manualScanLeft();
        }
        else
        {
          manualScanRight();
        }
        break;
      default:

        break;
    }
    turnSensorUpdate();
  }
  turnSensorUpdate();
  return end;
}

void manualScanLeft()
{
  bool searched = false;
  bool found = false;

  currPosition = L_ROOM;

  while(!searched)
  {
    int ta  = returnAngle(turnAngle);
    int a = 0;  

    turn(turnAngle90 + turnAngle45);

    // turns right and scans
    motors.setSpeeds(turnSpeed,-turnSpeed);

    while(ta > a)
    {
      turnSensorUpdate();
      //Serial1.println(ta);
      ta  = returnAngle(turnAngle);

      proxRead();

      if(objectSeen)
      {
        stop();

        personFound();

        motors.setSpeeds(turnSpeed,-turnSpeed);

        break;

      }

    }

    while(ta > a)
    {
      turnSensorUpdate();
      //Serial1.println(ta);
      ta  = returnAngle(turnAngle);
    }

    stop();

    wallFront = atIntersection();

    if(wallFront)
    {
      wallFront = false;
      searched = true;
    } 

    if(objectSeen)
    {
      searched = true;
      found = true;
      
    }
    if(!objectSeen){

      Serial1.println("Nothing found!");

      searched = true;

    }
  }

  Serial1.println("Room Searched!");

  proxRead();

  if(objectSeen)
  {
    // nav round object
    stop();

  }
}

void manualScanRight()
{
  bool searched = false;
  bool found = false;

  currPosition = R_ROOM;
  while(!searched)
  {
    int ta  = returnAngle(turnAngle);
    int a = 0;
       
    // turning left to scan
    motors.setSpeeds(-turnSpeed,turnSpeed);

    while(ta < a)
    {
      turnSensorUpdate();
      ta  = returnAngle(turnAngle);

      proxRead();

      if(objectSeen)
      {
        stop();
        
        personFound();

        motors.setSpeeds(-turnSpeed,turnSpeed);

        break;
      }
    }
    while(ta < a)
    {
      turnSensorUpdate();
      ta  = returnAngle(turnAngle);
    }

    stop();

    wallFront = atIntersection();

    if(wallFront)
    {
      //targetValueReset();
      wallFront = false;
      searched = true;
    } 

    if(objectSeen)
    {
      // ends search because object has been found
      searched = true;
      found = true;
    }
    if(!objectSeen){
      // ends search when whole room has been scanned
      // and nothing has been found.
      Serial1.println("Nothing found!");
      searched = true;
    }
  }

  Serial1.println("Room Searched!");

  proxRead();

  if(objectSeen)
  {
    // nav round object
    stop();

  }
}

// run manual control mode until mode changed
void rControlMode(){

  Serial1.println("Remote Control Ready!");
  while(currMode == mode1){

    checkMode();

    proxRead();
  
    manualControl();
  }
}

// check for mode selected
void checkMode(){

  String mode = readSerial();

  if(currMode == start)
  {
    Serial1.println("Waiting for mode to be selected ...");
  }

  while (currMode == start)
  {
    if(mode != NULL)
    {
      if(matchMode(mode) == m_end)
      {
        Serial1.print(mode);
        Serial1.println("Checking again.."); 
        delay(1000);
      }
      else
      {
        if(currMode == start){

          currMode = matchMode(mode);
          Serial1.print(mode);
          Serial1.println(" has been selected!");
        }
        break;
      }    
    }
    mode = readSerial();
  }

  while(currMode != start){

    if(matchMode(mode) == currMode)
    {
      break;
    }
    else if(mode == "mode1" || mode == "mode2" || mode == "mode3" || mode == "start")
    {
      currMode = matchMode(mode);
      Serial1.print("Changing mode to: ");
      Serial1.println(mode);
      break;
    }
    else
    {
      break;
    }       
  }
  delay(1000);
}


// Searchs surrounding area to determind where the robot is
void Search()
{
  turn(targetValue);
  forwardUntil(forwardTime);
  checkForOpening();
}

// Searches right rooms for objects
void scanRoomRight()
{
  bool searched = false;

  bool found = false;

  int wallCount = 0;

  int forwardCount = 0;

  while(!searched)
  {
    int ta  = returnAngle(turnAngle);
    int a = 0;
    if(forwardCount != 0)
    {
      // if not object found then robot moves further into room
      turn(-turnAngle90);
      int32_t sAngle = targetValue;
      targetValueReset();
      forwardUntil(timeToWall);
      targetValue = sAngle;
      forwardCount++;
    }
    else
    {
      forwardCount++;
    }    
    // turning left to scan
    motors.setSpeeds(-turnSpeed,turnSpeed);

    while(ta < a)
    {
      turnSensorUpdate();
      //Serial1.println(ta);
      ta  = returnAngle(turnAngle);

      proxRead();

      if(objectSeen)
      {
        stop();
        
        personFound();

        motors.setSpeeds(-turnSpeed,turnSpeed);

        break;
      }
    }
    while(ta < a)
    {
      turnSensorUpdate();
      ta  = returnAngle(turnAngle);
    }

    stop();

    wallFront = atIntersection();

    if(wallFront)
    {
      //targetValueReset();
      wallFront = false;
      searched = true;
    } 

    if(objectSeen)
    {
      // ends search
      searched = true;
      found = true;
      //Serial.println("Object Found!");
    }
    if(!objectSeen && forwardCount > 5){
      // ends search when whole room has been scanned
      // when nothing has been found.
      Serial1.println("Nothing found!");
      searched = true;

    }
  }

  Serial1.println("Room Searched!");

  proxRead();

  if(objectSeen)
  {
    // nav round object
    stop();

  }
}

// records where person has been found
void personFound()
{
  int count = 0;

  // gets size of found array
  for(int i=0; i<10; i++)
  {
    if(found[i] != '\0')
    {
      count++;
    }
  }

  // saves current position as persons position
  found[count] = currPosition;

  String numEnd;
  String n = (String)(count + 1);

  // determinds ending
  if(n.endsWith("1"))
  {
    numEnd = "st";
  }
  else if(n.endsWith("2"))
  {
    numEnd = "nd";
  }
  else if(n.endsWith("3"))
  {
    numEnd = "rd";
  }
  else
  {
    numEnd = "th";
  }

  String room;
  switch(found[count]){
    case L_ROOM:
      room = "Room on the Left";
      break;
    case R_ROOM:
      room = "Room on the Right";
      break;
  }

  delay(2000);

  encoderReset();

  moveToPerson();

  //sends location information to gui
  Serial1.print("Person found in position:  ");
  Serial1.print(n);
  Serial1.print(numEnd);
  Serial1.print(" ");
  Serial1.print(room);
  Serial1.print("\n");

  int16_t averageCount = getEncoderCount();

  delay(2000);

  reverseUntil(averageCount);
}

// scans left room 
void scanRoomLeft()
{
  bool searched = false;

  bool found = false;

  int forwardCount = 0;

  int wallCount = 0;

  while(!searched)
  {
    int ta  = returnAngle(turnAngle);
    int a = 0;
    if(forwardCount != 0)
    {
      //       
      turn(turnAngle90);
      int32_t sAngle = targetValue;
      targetValueReset();
      forwardUntil(timeToWall);
      targetValue = sAngle;
      forwardCount++;
    }
    else
    {
      forwardCount++;
    }    

    turn(turnAngle90 + turnAngle45);

    // turns right and scans
    motors.setSpeeds(turnSpeed,-turnSpeed);

    while(ta > a)
    {
      turnSensorUpdate();
      //Serial1.println(ta);
      ta  = returnAngle(turnAngle);

      proxRead();

      if(objectSeen)
      {
        stop();

        personFound();

        motors.setSpeeds(turnSpeed,-turnSpeed);

        break;

      }

    }

    while(ta > a)
    {
      turnSensorUpdate();
      //Serial1.println(ta);
      ta  = returnAngle(turnAngle);
    }

    stop();


    if(wallFront)
    {

      targetValueReset();
      wallFront = false;
      wallCount++;
    } 

    if(objectSeen)
    {
      searched = true;
      found = true;
      
    }
    if(!objectSeen && forwardCount > 5){

      Serial1.println("Nothing found!");

      searched = true;

    }
  }

  Serial1.println("Room Searched!");

  proxRead();

  if(objectSeen)
  {
    // nav round object
    stop();

  }

}

// waits for specific instructions from Serial1
// and returns bool based on response
bool waitForInstruction()
{
  Serial1.println("Waiting for response...");
  bool received = false;
  bool turn = false;
  String inByte;
  while(received == false){
    if (Serial1.available() > 0){
      
      inByte = readSerial();

      INSTRUCTION i = matchInstruct(inByte);

      //Serial1.println((String)inByte + " " + i);

      switch(i){
        case yes:
          Serial1.println("YES");        
          turn = true;  
          received = true; 
          break;
        case no:
          turn = false;
          received = true;
          break;
        case left:
          turn = true;
          received = true;
          break;
        case right:
          turn = false;
          received = true;
          break;
      }
    }
  }

  return turn;
}

// enters room and scans based and direction passed in
void enterRoom(char dir){

  if(currMode == mode2)
  {
    while(!waitForInstruction()){
      ;
    }
  }
  
  int32_t sAngle = targetValue;
  targetValueReset();

  if(dir == 'l')
  {
    Serial1.println("Turning Left..");

    targetValueReset();
    forwardUntil(timeToWall * 1.6);

    turn(turnAngle90);

    targetValueReset();
    forwardUntil(timeToWall * 1.2);

    if(wallFront)
    {
      reverseUntil();
      turn(sAngle);
      targetValueReset();
      forwardUntil(timeToWall/2);
      turn(turnAngle90);
      targetValueReset();
      forwardUntil(timeToWall * 1.2);      
    }

    // check where left hand wall is
    int a = returnAngle(turnAngle45 * 4);
    Serial1.println(a);

    turn(turnAngle45 * 4);

    reverseUntil(400);

    turn(turnAngle90);

    scanRoomLeft();
    
  }
  if(dir == 'r')
  {
    Serial1.println("Turning Right..");

    targetValueReset();
    forwardUntil(timeToWall * 1.5);

    turn(-turnAngle90);

    targetValueReset();
    forwardUntil(timeToWall * 1.2);

    if(wallFront)
    {
      reverseUntil();
      turn(sAngle);
      targetValueReset();
      forwardUntil(timeToWall);
      turn(-turnAngle90);
      targetValueReset();      
      forwardUntil(timeToWall * 1.2);  
      //targetValue = sAngle;    
    }

    // check where right hand wall is

    turn(-turnAngle90 * 2);

    driveToLine();

    reverseUntil();

    turn(-turnAngle90);

    scanRoomRight();  
  }

  targetValue = sAngle;

  forwardSpeed = forwardSpeedMax; 
}

// exits room based on direction passed in
void exitRoom(char dir)
{
  Serial1.println("Exiting Room");

  driveToLine();

  reverseUntil();
  
  if(dir == 'r')
  {
    turn(turnAngle90);
  }
  if(dir == 'l')
  {
    turn(-turnAngle90);
  }

  driveToLine();

  reverseUntil();

  fixAngle(dir);

  turn(targetValue);
}

// records current position of robot for return journey
void recordPosition(POSITION pos)
{
  int count = 0;

  for(int i=0; i<50; i++)
  {
    if(route[i] != '\0')
    {
      count++;
    }
  }
  route[count] = pos;
}

// used to find turn 
void findTurn(char dir)
{
  int32_t saveAngle = targetValue;
  bool isturn = false;

  uint16_t values[numSensors];

  uint8_t index1 = leftIndex;
  uint8_t index2 = rightIndex;

  int32_t angle;

  if(dir == 'l')
  {
    angle = turnAngle90;

  }
  if(dir == 'r')
  {
    angle = -turnAngle90;
    index1 = rightIndex;
    index2 = leftIndex;
  }
  
  forwardUntil(forwardTime);

  while(!isturn){

    turn(angle);

    targetValueReset();

    forwardUntil(timeToWall);

    lineSensors.readCalibrated(values);

    // if dir = left then index1 = leftIndex index2 = rightIndex
    // if dir = right then index1 = rightIndex index2 = leftIndex
    if(values[index1] < 5 && values[index2] > 45)
    {
      reverseUntil();
      turn(saveAngle);
      reverseFor(timeToWall/2);
    }
    if(values[index1] > 45 && values[index2] < 5)
    {
      reverseUntil();
      turn(saveAngle);
      targetValue = saveAngle;
      forwardUntil(timeToWall/2);
    }
    if(values[index1] > 45 && values[index2] > 45)
    {
      reverseUntil();
      turn(saveAngle);
      reverseFor(timeToWall/1.5);
    }
    if(values[index1] < 45 && values[index2] < 45)
    {
      if(values[index1] < 5 && values[index2] < 5)
      {
        Serial1.print(dir);
        Serial1.println(" Corner Found");
        isturn = true;
      }
      else if(values[index1] < 5 && values[index2] > 5)
      {
        reverseUntil();
        turn(saveAngle);
        reverseFor(timeToWall/2.5);
      }
      else if (values[index1] > 5 && values[index2] < 5)
      {
        reverseUntil();
        turn(saveAngle);
        forwardUntil(timeToWall/2.5);
      }
    }
  }
  targetValue = saveAngle;
}


// check if already reached junction on route
bool checkJunction()
{
  bool junction = false;

  for(int i=0; i<20; i++)
  {
    if(route[i] == L_R_CORNER)
    {
      Serial1.println("Already passed a junction");
      junction = true;
    }
  }
  return junction;
}

// runs manualControl mode until stopped
void activateRC()
{
  Serial1.println("Manual Control Activated!");
  bool stop = false;
  while(!stop)
  {
    manualControl();

    stop = manualControl();
    
  }
}

// uses recorded positions to return to start
void returnToStart()
{
  int count = 0;
  for(int i=0; i<50; i++)
  {
    if(route[i] != '\0')
    {
      count++;
    }
  }

  for(int i = count-1 ; i >= 0; i--)
  {
    printPosition(route[i]);
    switch(route[i]) {
      case START:
      {
        //Search();
        break;
      }
      case CORRIDOOR:
      {
        forwardUntil(forwardTime);
        break;
      }
      case L_CORNER:
      { // manual navigation
        //Serial1.println("TURNING RIGHT");
        //activateRC();
        turn(-turnAngle90);
        turnSensorReset();
        targetValueReset();
        Search();
        break;
      }
      case R_CORNER:
      { //manual navigation
        //Serial1.println("TURNING LEFT");
        //activateRC();
        turn(turnAngle90);
        turnSensorReset();
        targetValueReset();
        Search();     
        break;
      }
      case L_ROOM:
      {
        forwardUntil(forwardTime);
        // keeps going forward until left room is passed
        while(currPosition != CORRIDOOR)
        {
          Search();
          currPosition = checkPosition();
        }
        break;
      }
      case R_ROOM:
      {
        forwardUntil(forwardTime);
        while(currPosition != CORRIDOOR)
        {
          Search();
          currPosition = checkPosition();
        }
        break;
      }
      case END:
      {
        break;
      }
      case L_R_CORNER:
      {// correct & add return
        //Serial1.println("TURNING LEFT AT JUNCTION");
        bool dir = true;
        turn(turnAngle90);
        
        turnSensorReset();
        targetValueReset();
        if(dir)
        {
          turn(-turnAngle90);
        }
        else
        {
          turn(turnAngle90);
        }
        //turn(turnAngle90);        
        fixAngle(dir);
        Search();
        break;
      }
      default:
      {
        //Serial1.println("No position found");
        break;
      }
    }
  }
}

// prints current position to Serial1
void printPosition(POSITION curr)
{
  switch(curr)
  {
    case START:
    {
      Serial1.println("");
      break;
    }
    case CORRIDOOR:
    {
      Serial1.println("CORRIDOOR");
      break;
    }
    case L_CORNER:
      Serial1.println("LEFT CORNER");
      break;
    case R_CORNER:
    {
      Serial1.println("RIGHT CORNER");
      break;
    }
    case R_ROOM:
    {
      Serial1.println("ROOM ON RIGHT");
      break;
    }
    case L_ROOM:
    {
      Serial1.println("ROOM ON LEFT");
      break;
    }
    case L_R_CORNER:
    {
      Serial1.println("JUNCTION");
      break;
    }
    case END:
    {
      Serial1.println("END");
      break;
    }
    default:
    {
      break;
    }
  }  
}

// Semi Autonomous Mode
void sAutoMode(){

  bool end = false;

  bool passedJunction = false;

  while(currMode == mode2){

    checkMode();

    char dir;

    // get current position based on walls found
    currPosition = checkPosition();

    // records position for return
    recordPosition(currPosition);

    // prints position
    printPosition(currPosition);

    switch(currPosition) {
      case START:
      {
        Search();
        break;
      }
      case CORRIDOOR:
      {
        Search();
        break;
      }
      case L_CORNER:
      { // manual navigation
        activateRC();
        turn(turnAngle90);
        turnSensorReset();
        targetValueReset();
        turn(-turnAngle90);
        dir = 'l';
        fixAngle(dir);
        Search();
        break;
      }
      case R_CORNER:
      { //manual navigation
        activateRC();
        turn(-turnAngle90);
        turnSensorReset();
        targetValueReset();
        turn(turnAngle90);
        dir = 'r';
        fixAngle(dir);
        // turnSensorReset();
        // targetValueReset();
        Search();       
        break;
      }
      case L_ROOM:
      {
        // if robot hasnt reached an end yet search room
        if(!end)
        {
          Serial1.println("Enter Room? (Y/N)");
          dir = 'l';
          enterRoom(dir);
          //exitRoom(dir); replace with return to door
          activateRC();
          turn(-turnAngle90);
          fixAngle(dir);
          Search();
        }
        else
        {
          // otherwise drive past room
          targetValueReset();
          forwardUntil(forwardTime);
          Search();
        }
        break;
      }
      case R_ROOM:
      {
        // if robot hasnt reached an end yet search room
        if(!end)
        {
          Serial1.println("Enter Room? (Y/N)");
          dir ='r';
          enterRoom(dir);
          //exitRoom(dir);
          activateRC();
          turn(turnAngle90);
          fixAngle(dir);
          Search();
        }
        else
        {
          // otherwise drive past room
          targetValueReset();
          forwardUntil(forwardTime * 3);
          Search();          
        }
        break;
      }
      case END:
      {  
        // reached end of corridor
        Serial1.println("Turn Around? Y/N");
        // wait for instruction to turn round
        bool res = waitForInstruction();
        end = true;
        // if response is true then turn around
        if(res)
        {
          dir = 'l';
          turn(turnAngle90);
          fixAngle(dir);
          Search();
        }
        else
        {
          // ask if robot can return to the start
          Serial1.println("Return To Start? Y/N");
          res = waitForInstruction();
          if(res)
          {
            Serial1.println("Returning to Start..");
            returnToStart();
          }
        }
        break;
      }
      case L_R_CORNER:
      {// correct & add return
        turn(targetValue);
        Serial1.println("Left or Right?");
        bool in = waitForInstruction();
        passedJunction = true;
        // depending on response turn left or right
        if(in)
        {
          turn(turnAngle90);
        }
        else
        {
          turn(-turnAngle90);
        }
        // reset current gyro angle to 0
        turnSensorReset();
        targetValueReset();
        if(in)
        {
          turn(-turnAngle90);
          dir = 'l';
        }
        else
        {
          turn(turnAngle90);
          dir = 'r';
        }
        //turn(turnAngle90);        
        fixAngle(dir);
        Search();
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

void autoMode(){

  while(currMode == mode3){

    //checkMode();

    char dir;  

    bool end = false;

    bool passedJunction = false;

    currPosition = checkPosition();

    recordPosition(currPosition);

    printPosition(currPosition);
    switch(currPosition)
    {
      case START:
        Search();
        break;
      case CORRIDOOR:
        Search();
        break;
      case L_CORNER: 
        dir = 'l'; 
        if(!wallFront)
        {
          findTurn(dir);
        }
        else{
          fixAngle(dir);
        }
        turnSensorReset();
        targetValueReset();
        Search();
        break;
      case R_CORNER: //correct
        //Serial1.println(checkJunction());
        dir = 'r';
        
        if(!checkJunction()){
          if(!wallFront)
          {
            findTurn(dir);
          }
          else
          {
            fixAngle(dir);
          }
          turnSensorReset();
          targetValueReset();
        }
        Search();       
        break;
      case L_ROOM:
        dir = 'l';
        enterRoom(dir);
        exitRoom(dir); 
        Search();
        break;
      case R_ROOM:
        dir ='r';
        enterRoom(dir);
        exitRoom(dir);
        Search();
        break;
      case L_R_CORNER:
        dir = 'l';
        fixAngle(dir);
        //turn(turnAngle90);
        turnSensorReset();
        targetValueReset();
        Search(); 
        break;
      case END:
        // return back to start if second end
        turn(turnAngle90);
        dir = 'l';
        fixAngle(dir);
        Search();
        break;
      default:

        break;
    }
  }

}

void loop() {
  // checks if mode has been selected
  checkMode();
  
  switch(currMode)
  {
    case mode1:
      // Remote control mode set UI up
      // then start mode 1
      rControlMode();
      break;
    case mode2:
      //Semi-auto mode set UI up
      sAutoMode();
      break;
    case mode3:
      //Autonomous mode set UI up
      // Serial1.write(currMode)
      // wait for start from UI
      // then start mode 3
        autoMode();
      break;

  }

}

// returns robots current position based on walls found.
POSITION checkPosition(){

  // START
  if(wallLeft == false && wallRight == false && wallFront == false){

    return START;

  }
  // CORRIDOOR
  if(wallLeft == true && wallRight == true && wallFront == false){

    return CORRIDOOR;

  }

  // R_CORNER
  if(wallLeft == true && wallRight == false && wallFront == true){
    // for normal right corners with wall in front
    return R_CORNER;

  }

  if(wallLeft == true && wallRight == false && wallFront == false && halfWallRight == true)
  {
    //for right corners off of corridors with no wall in front
    return R_CORNER;
  }

  // L_CORNER
  if(wallLeft == false && wallRight == true && wallFront == true){
    //for normal left corners with wall in front
    return L_CORNER;    
  }

  if(wallLeft == false && wallRight == true && wallFront == false && halfWallLeft == true){
    // for left corners off corridors with no wall in front
    return L_CORNER;    
  }

  // L_R_CORNER
  if(wallLeft == false && wallRight == false && wallFront == true)
  {
    // junctions where you can turn left or right
    return L_R_CORNER;
    
  }

  // L_ROOM
  if(wallLeft == false && wallRight == true && wallFront == false){
    // room on the left
    return L_ROOM;
        
  }

  // R_ROOM  
  if(wallLeft == true && wallRight == false && wallFront == false){
    // room on the right
    return R_ROOM;
    
  } 

  // END
  if(wallLeft == true && wallRight == true && wallFront == true){

    return END;

  }

}
