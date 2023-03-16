# Zumo Search And Rescue Assignment

## GUI
I used processing and the G4P library to create a GUI. The GUI allows for the mode to be selected, manual control of the robot, recieving messages from the zumo as well as sending instructions

## How To Build and Run
The sketch that need to be uploaded onto the Zumo Robot is in the ZumoProject folder and is called ZumoProject.ino.
First the robot needs the ZumoProject.ino sketch loaded onto it by ensuring Arduino is set to the Pololu A-Atar 32U4 board and clicking uplos with the robot plugged in via USB. Once uploaded the GUI can be launched by finding the sketch in the ZumoGUI folder and is called ZumoGUI.pde, this can be opening in processing to run.
Once the GUI has been run the Zumo robot should be turned on and will be ready to go.
## SetUp
Once its turned on the yellow light will turn on while the gyro is calibrating, Its important the robot isnt touched while this is happening.
Once this has been completed and the yellow light has turned off the A button can be pressed on the robot to start the line calibration. Once this has completed the GUI will ask for a mode to be selected.

## Mode 1
  On the GUI you can select the Remote Control Button to change to mode 1.
  Then the buttons to move the robot Forward(w), Left (a), Right (d) and stop 
  (s) will become available. The robot can be controlled by clicking these 
  buttons or by pressing the corresponding keys.
  
  To search a room the scan button can be used to activate the scanning procedure. The GUI will ask if a Left or Right room that needs scanning.

## Mode 2
  On the GUI you can select the Semi-Auto button to change to mode 2.
  The buttons to manually control the robot will be greyed out until enabled 
  at specific times when needed within mode 2.
  
  The robot will first calibrate the gyro and the yellow light will show until this has been completed. Button A will then need to be pressed to activate the line 
  calibration. The robot will go left and right running over the lines to initially calibrate the line sensors so it knows the difference between the black walls and 
  the white background.
  Once calibration is completed and the robot is ready to go the GUI will ask for a mode to be selected
  
  ### Navigating the corridors
  The Robot will search its surrounding by driving up to the walls on its left and right and front to identify its position.
  
  ### Turning Corners
  When it has identified a Left or Right Corner the Manual Control mode will be activated so the user can navigate the corner manually. Once the user has turned the 
  corner the user can activate the autonomous control again by pressing the START button. To ensure the robot is aligned with the walls correctly once the Manual 
  Control Mode has finished the robot will drive up to a wall on the left or right and reverse until both the left and right line sensors are just off the line but 
  also parallel with the line. The robot will then turn back to the direction it was going and sets to gyro angle to 0 to ensure the robot continues driving 
  relatively straight.
  
  ### Searching Rooms
  When a room is found the GUI will ask if the robot should enter the room by entering 'y' or 'n' into the instruction input box and pressing enter.
  If the 'y' command is entered the robot will proceed to enter the room and then search for a person.
  
  It does this by scanning from on side of the room to another by rotating and then moving forward until it has either found something or searched the whole room and 
  found nothing.
  
  Once the search is complete Manual Control Mode will be activated/// so the robot can be manually navigated out of the room. Once complete pressing the START 
  button will reactivate the autonomous mode.
  
  ### Navigating a Junction
  When the robot approaches and identifies a junction it will ask if the robot should turn Left or Right. Once and instruction has been recieved by entering 'l' or 
  'r' into the input box and pressing enter, the robot will make the turn and continue searching.
  
  ### End of a Corridoor
  At the end of a corridoor the GUI will ask if the robot should turn around by entering 'y' or 'n' into the input box.
  If the 'y' command is entered the robot will proceed to turn 180 degrees and continue back to the junction.
  If the 'n' command is entered the robot will proceed to ask about returing to the start.
  
  ### Returning to the Start
  If the 'y' command is entered the robot will proceed to return to the start via the shortest route autonomously.

## Mode 3
  On the GUI you can select the Auto button to change to mode 3.
  
  The robot will perform the tasks described in the Semi-autonomous mode but with out any intervention from the user.
  
  It will report back to the GUI the position it is currently in and the actions it is taking as well as if any people have been found on its route

## References
1. Andrew Brown. (n.d.). Zumo 32U4 Synchronize Motor | A. Brown Design. A. Brown Design. Retrieved March 16, 2023, from http://www.abrowndesign.com/2017/02/25/zumo-32u4-synchronize-motor/

2. Lager, P. (n.d.). Quarks Place. Www.lagers.org.uk. Retrieved March 16, 2023, from http://www.lagers.org.uk/g4p/

3. Pololu. (2023, February 7). Zumo32U4 library. GitHub. https://github.com/pololu/zumo-32u4-arduino-library/blob/master/examples/MazeSolver/TurnSensor.h

4. Andrew Brown. (n.d.-a). Zumo 32U4 – Squaring to an Edge or Line | A. Brown Design. A. Brown Design. Retrieved March 16, 2023, from http://www.abrowndesign.com/2016/10/14/zumo-32u4-squaring-to-an-edge-or-line/

