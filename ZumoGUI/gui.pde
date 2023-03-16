/* =========================================================
 * ====                   WARNING                        ===
 * =========================================================
 * The code in this tab has been generated from the GUI form
 * designer and care should be taken when editing this file.
 * Only add/edit code inside the event handlers i.e. only
 * use lines between the matching comment tags. e.g.

 void myBtnEvents(GButton button) { //_CODE_:button1:12356:
     // It is safe to enter your event code here  
 } //_CODE_:button1:12356:
 
 * Do not rename this tab!
 * =========================================================
 */

public void backButton_click1(GButton source, GEvent event) { //_CODE_:backButton:510967:
  println("backButton - GButton >> GEvent." + event + " @ " + millis());
  
  // writes to Xbee button pressed s
  if(myPort != null){
   myPort.write('s');
  }
  
} //_CODE_:backButton:510967:

public void leftButton_click1(GButton source, GEvent event) { //_CODE_:leftButton:712119:
  println("leftButton - GButton >> GEvent." + event + " @ " + millis());
  
  //writes to Xbee that a has been pressed
  if(myPort != null){
  myPort.write('a');
  }
  
} //_CODE_:leftButton:712119:

public void rightButton_click1(GButton source, GEvent event) { //_CODE_:rightButton:878779:
  println("rightButton - GButton >> GEvent." + event + " @ " + millis());
  // D
  if(myPort != null){
  myPort.write('d');
  }
  
} //_CODE_:rightButton:878779:

public void fwdButton_click1(GButton source, GEvent event) { //_CODE_:fwdButton:404561:
  println("fwdButton - GButton >> GEvent." + event + " @ " + millis());
  
  if(myPort != null){
  myPort.write('w');
  }
  
} //_CODE_:fwdButton:404561:

public void inputChange(GTextField source, GEvent event) { //_CODE_:input:409776:
  println("textfield1 - GTextField >> GEvent." + event + " @ " + millis());
  
  if(event == GEvent.ENTERED){
    txt = source.getText();
    //myPort.write(txt);
    count = 0;
    source.setText("");
  }
  
} //_CODE_:input:409776:

public void ZumoComs_change1(GTextArea source, GEvent event) { //_CODE_:ZumoComs:429445:
  println("ZumoComs - GTextArea >> GEvent." + event + " @ " + millis());
} //_CODE_:ZumoComs:429445:

public void startBtn_click1(GButton source, GEvent event) { //_CODE_:startBtn:689117:
  println("button1 - GButton >> GEvent." + event + " @ " + millis());
  myPort.write("e");
} //_CODE_:startBtn:689117:

public void stopBtn_click1(GButton source, GEvent event) { //_CODE_:stopBtn:349873:
  println("button2 - GButton >> GEvent." + event + " @ " + millis());
  if(event == GEvent.CLICKED)
   {
     mode1 = false;
     mode2 = false;
     mode3 = false;
     startMenu = true;
   }
   //myPort.write("e");
} //_CODE_:stopBtn:349873:

public void rControl_click1(GButton source, GEvent event) { //_CODE_:rControl:291927:
  println("rControl - GButton >> GEvent." + event + " @ " + millis());
   if(event == GEvent.CLICKED)
   {
     mode1 = true;
     mode2 = false;
     mode3 = false;
   }
   myPort.write("mode1");
} //_CODE_:rControl:291927:

public void sAuto_click1(GButton source, GEvent event) { //_CODE_:sAuto:566286:
  println("sAuto - GButton >> GEvent." + event + " @ " + millis());
  if(event == GEvent.CLICKED)
   {
     mode1 = false;
     mode2 = true;
     mode3 = false;
   }
   myPort.write("mode2");
} //_CODE_:sAuto:566286:

public void auto_click1(GButton source, GEvent event) { //_CODE_:auto:880418:
  println("auto - GButton >> GEvent." + event + " @ " + millis());
  if(event == GEvent.CLICKED)
   {
     mode1 = false;
     mode2 = false;
     mode3 = true;
   }
   myPort.write("mode3");
} //_CODE_:auto:880418:

public void foundList_change1(GTextArea source, GEvent event) { //_CODE_:foundList:679223:
  println("textarea1 - GTextArea >> GEvent." + event + " @ " + millis());
} //_CODE_:foundList:679223:

public void scanBtn_click1(GButton source, GEvent event) { //_CODE_:scanBtn:270315:
  println("scanBtn - GButton >> GEvent." + event + " @ " + millis());
  myPort.write('v');
} //_CODE_:scanBtn:270315:



// Create all the GUI controls. 
// autogenerated do not edit
public void createGUI(){
  G4P.messagesEnabled(false);
  G4P.setGlobalColorScheme(GCScheme.BLUE_SCHEME);
  G4P.setMouseOverEnabled(false);
  G4P.setDisplayFont("Arial", G4P.PLAIN, 20);
  G4P.setInputFont("Arial", G4P.PLAIN, 15);
  surface.setTitle("Sketch Window");
  backButton = new GButton(this, 181, 249, 96, 90);
  backButton.setText("reverse (S)");
  backButton.addEventHandler(this, "backButton_click1");
  leftButton = new GButton(this, 80, 198, 96, 90);
  leftButton.setText("left (A)");
  leftButton.addEventHandler(this, "leftButton_click1");
  rightButton = new GButton(this, 281, 201, 96, 90);
  rightButton.setText("right (D)");
  rightButton.addEventHandler(this, "rightButton_click1");
  fwdButton = new GButton(this, 180, 154, 96, 90);
  fwdButton.setText("forward (W)");
  fwdButton.addEventHandler(this, "fwdButton_click1");
  input = new GTextField(this, 446, 67, 275, 30, G4P.SCROLLBARS_NONE);
  input.setPromptText("(L or R) - (Y or N)");
  input.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  input.setOpaque(true);
  input.addEventHandler(this, "inputChange");
  ZumoComs = new GTextArea(this, 447, 106, 276, 122, G4P.SCROLLBARS_VERTICAL_ONLY);
  ZumoComs.setPromptText("Zumo says ...");
  ZumoComs.setLocalColorScheme(GCScheme.PURPLE_SCHEME);
  ZumoComs.setOpaque(true);
  ZumoComs.addEventHandler(this, "ZumoComs_change1");
  label1 = new GLabel(this, 440, 36, 286, 20);
  label1.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label1.setText("Enter Instructions (Press Enter)");
  label1.setOpaque(false);
  startBtn = new GButton(this, 55, 105, 90, 40);
  startBtn.setText("START");
  startBtn.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  startBtn.addEventHandler(this, "startBtn_click1");
  stopBtn = new GButton(this, 307, 105, 90, 40);
  stopBtn.setText("STOP");
  stopBtn.setLocalColorScheme(GCScheme.RED_SCHEME);
  stopBtn.addEventHandler(this, "stopBtn_click1");
  rControl = new GButton(this, 41, 30, 120, 50);
  rControl.setText("Remote Control");
  rControl.setLocalColorScheme(GCScheme.ORANGE_SCHEME);
  rControl.addEventHandler(this, "rControl_click1");
  sAuto = new GButton(this, 166, 31, 120, 50);
  sAuto.setText("Semi Auto");
  sAuto.setLocalColorScheme(GCScheme.CYAN_SCHEME);
  sAuto.addEventHandler(this, "sAuto_click1");
  auto = new GButton(this, 291, 31, 120, 50);
  auto.setText("Auto");
  auto.setLocalColorScheme(GCScheme.GOLD_SCHEME);
  auto.addEventHandler(this, "auto_click1");
  label2 = new GLabel(this, 462, 236, 149, 22);
  label2.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  label2.setText("People Found :");
  label2.setOpaque(false);
  foundList = new GTextArea(this, 447, 268, 276, 98, G4P.SCROLLBARS_NONE);
  foundList.setPromptText("No one found");
  foundList.setOpaque(false);
  foundList.addEventHandler(this, "foundList_change1");
  foundNum = new GLabel(this, 625, 236, 80, 22);
  foundNum.setTextAlign(GAlign.CENTER, GAlign.MIDDLE);
  foundNum.setLocalColorScheme(GCScheme.GREEN_SCHEME);
  foundNum.setOpaque(false);
  scanBtn = new GButton(this, 307, 318, 90, 40);
  scanBtn.setText("SCAN");
  scanBtn.setLocalColorScheme(GCScheme.SCHEME_9);
  scanBtn.addEventHandler(this, "scanBtn_click1");
}

// Variable declarations 
// autogenerated do not edit
GButton backButton; 
GButton leftButton; 
GButton rightButton; 
GButton fwdButton; 
GTextField input; 
GTextArea ZumoComs; 
GLabel label1; 
GButton startBtn; 
GButton stopBtn; 
GButton rControl; 
GButton sAuto; 
GButton auto; 
GLabel label2; 
GTextArea foundList; 
GLabel foundNum; 
GButton scanBtn; 