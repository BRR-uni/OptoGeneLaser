# Android app and Arduino code for optogenetics power source control

## A tool to control the frequency of light blinking in optogenetics stimulation experiments

This project allows to control the frequency of an electric signal sent by an Arduino to an amplifier. The aplifier is then ment to be used as a light source for optogenetic stimulation experiments. The control of the frequency is done thorugh an Android app. The comunication between the Arduino and the app is posible through a bluetooth Arduino module. The app lets the user controll the following:
- Milliseconds the laser should be on or off
- Minutes of stimulation and rest sessions in between
- Number of cycles to repeat the stimulation and rest sessions


## How to install the OptoGeneLaser project

To get the complete tool to work it is necessary to have an Arduino board connected through the right pins to a bluetooth module and to the input or the aplifier used for the stimulation experiments. Outside of the hardware instructions, the app and the arduino code instalation should follow:
1. Download the .apk file to a smartphone
2. Open the file and follow the instructions provided by the device. It might require to allow installation of software outside of playstore.
3. Once the app is installed, it might be required as well to give bluetooth management permissions to the app.
4. For the Arduino code setup, Arduino IDE should be installed beforehandstart
5. Download the .ino file of the repository and pluging in the Arduino board to a computer.
6. In the Arduino IDE select open project and browse to the corresponding file.
7. Select the usb port through which the Arduino board is plugged to the computer.
8. Finally select complie and upload the code to the board.

## Find a bug?

If you found an issue or would like to submit an improvement to this project, please submit an issue using the issues tab above. If you would like to submit a Pull Request with a fix, reference the issue you created

## Known issues (Work in progress)
This code is not perfect nor efficient. So I aknowledge some things have improvement areas:
- First of all, the arduino code is commented in Spanish.
- I found difficult to make sure that when a variable was changed in the app it was properly recieved and also changed in the Arduino. My solution to this was to make a loop where both the Arduino and the app show the value they have stored for a given variable, and exit the loop just until they agree in the value of such variable. You don't want to run a whole experiment just to realize (or worst not realize) that it was run in different conditions than what you wanted! Thus I more than double checked it with this solution (or so I hope).
- Related to the previous, my code might not have proper exceptions handling in general.
- It might not be clear to the app user that all variables are reset when the bluetooth conection is stablished for the first time or when it is restablished after a loss of conection occured.
