import processing.serial.*;
import g4p_controls.*;
import processing.event.MouseEvent;

Serial myPort;

String ports[];

long startTime;

String txt;

int count = 0;

int foundCount = 0;

boolean mode1 = false;

boolean mode2 = false;

boolean mode3 = false;

boolean startMenu = true;

public void setup(){
  size(740, 380, JAVA2D);
  noStroke();
  createGUI();
  customGUI();
  
  fwdButton.fireAllEvents(true);
  backButton.fireAllEvents(true); 
  leftButton.fireAllEvents(true); 
  rightButton.fireAllEvents(true); 
  
  foundList.setEnabled(false);
  ZumoComs.setEnabled(false);
  
  String portName = "COM6";
  
  ports = Serial.list();
  
  println(ports.length);
  
  for(int i = 0 ;i < ports.length ; i++ ){
    
  
    if(portName.equals(ports[i])){
      
      println("Found Port");
    
      myPort = new Serial(this,portName,9600);
      
    }
    
  }
  
  if(myPort == null){
    
    println("Nothing found on " + portName);
  }
   
  // Place your setup code here
  
    //ZumoComs.setEnabled(false);
    
    startTime = System.nanoTime();
  
}

public void draw(){
  background(230);
  
  ArrayList<String> msg = new ArrayList<String>();
  
  if(myPort != null){
    if(myPort.available() > 0)
    {
      delay(200);
      msg.add(myPort.readString());
    
      if (msg.get(msg.size()-1) != null)
      {
        //println(msg.get(msg.size()-1));
       // Boolean m = msg.get(msg.size()-1).equals("clear");
       // println(m);
        if(msg.get(msg.size()-1).equals("clear"))
        {
          println("clearing textArea");
        
          ZumoComs.setText("");
          msg.clear();
        }
        else if(msg.get(msg.size()-1).contains("Person found in position:  "))
        {
          String t = msg.get(msg.size()-1);
          int s = t.indexOf(":");
          t = t.substring(s + 1,t.length()-1);
          foundCount++;
          foundNum.setText("" + foundCount);
          foundList.appendText("\n"+ foundCount + ": " + t);
        }
        else if(msg.size() > 1)
        {
          if(msg.get(msg.size()-1) != msg.get(msg.size()-2))
          {
            ZumoComs.appendText("\n"+ msg.get(msg.size()-1));
          }
        }
        else if(msg.get(msg.size()-1).contains("Manual Control Activated!"))
        {
          ZumoComs.appendText("\n"+ msg.get(msg.size()-1));
          ZumoComs.appendText("\n Use Buttons or keys (w,a,s,d) to move and use the SCAN button to search a room, Press START when done.");
          input.setFocus(false);
          enableControlButtons();
        }
        else
        {
          if(msg.get(msg.size()-1).contains("*"))
          {
            println(msg.get(msg.size()-1));
          }
          else
          {
            ZumoComs.appendText("\n"+ msg.get(msg.size()-1));
          }
        }
      }
        
      
        //println(ZumoComs.getCaretPos()[0]);
    
    }
  }
  
  //println(txt);
  
  if(txt != null && count == 0)
  {
    //println(txt);
      myPort.write(txt);
      println("Sent Direction :" + txt);
      count++;
  }
  
  customGUI();
  
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){
  if(startMenu)
  {
    rControl.setEnabled(true);
    rControl.setOpaque(true);
    sAuto.setEnabled(true);
    sAuto.setOpaque(true);
    auto.setEnabled(true);
    auto.setOpaque(true);
    sAuto.setLocalColorScheme(GCScheme.CYAN_SCHEME);
    auto.setLocalColorScheme(GCScheme.GOLD_SCHEME);
    rControl.setLocalColorScheme(GCScheme.ORANGE_SCHEME);
    disableControlButtons();
  }
  if(mode1)
  {
    //println("Mode1 Enabled");
    rControl.setEnabled(true);
    rControl.setOpaque(true);
    sAuto.setEnabled(false);
    sAuto.setOpaque(false);
    sAuto.setLocalColorScheme(8);
    auto.setEnabled(false);
    auto.setOpaque(false);
    auto.setLocalColorScheme(8);
    enableControlButtons();
  }
  if(mode2)
  {
    sAuto.setEnabled(true);
    sAuto.setOpaque(true);
    auto.setEnabled(false);
    auto.setOpaque(false);
    rControl.setEnabled(false);
    rControl.setOpaque(false);
    rControl.setLocalColorScheme(8);
    auto.setLocalColorScheme(8);
    disableControlButtons();
  }
  if(mode3)
  {
    sAuto.setEnabled(false);
    sAuto.setOpaque(false);
    auto.setEnabled(true);
    auto.setOpaque(true);
    rControl.setEnabled(false);
    rControl.setOpaque(false);
    sAuto.setLocalColorScheme(8);
    rControl.setLocalColorScheme(8);
    disableControlButtons();
  }
}

void keyPressed(){
  
  if(key == 'w'){
    // button press
    fwdButton_click1(fwdButton,GEvent.CLICKED);
  }

  if(key == 'a'){
    // button press
    leftButton_click1(leftButton,GEvent.CLICKED);
  }
  
  if(key == 's'){
    // button press
    backButton_click1(backButton,GEvent.CLICKED);
  }
  
  if(key == 'd'){
    // button press
    rightButton_click1(rightButton,GEvent.CLICKED);
  }
  
}

void disableControlButtons()
{
  fwdButton.setEnabled(false);
  backButton.setEnabled(false);
  leftButton.setEnabled(false);
  rightButton.setEnabled(false);
  
  fwdButton.setLocalColorScheme(8);
  backButton.setLocalColorScheme(8);
  leftButton.setLocalColorScheme(8);
  rightButton.setLocalColorScheme(8);
  
}

void enableControlButtons()
{
  fwdButton.setEnabled(true);
  backButton.setEnabled(true);
  leftButton.setEnabled(true);
  rightButton.setEnabled(true);
  
  fwdButton.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  backButton.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  leftButton.setLocalColorScheme(GCScheme.BLUE_SCHEME);
  rightButton.setLocalColorScheme(GCScheme.BLUE_SCHEME);
}
