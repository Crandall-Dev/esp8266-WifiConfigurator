/**
 * ESP8266 - Wifi Settings Configurator
 * 
 * Simple program to set the wifi networking configuration of
 * an ESP8266 over the serial console.
 * The primary use is to not enter your network ssid or password
 * into scripts that could be found later, especially in your
 * git repositories.
 * 
 * The ESP8266 has a set aside portion of the flash for wifi credentials.
 * This section is not overwritten by default for programming the board.
 * Once the wifi info is set, you don't need to have it set every time
 * you reprogram the board unless you explicitly clear it out.
 * This script lets you enter that info, check it if you like, and 
 * then go back to coding on whatever other program you're working on.
 * At that point, the given ESP8266 will connect to the wifi and no
 * security information will be stored in your scripts.
 * 
 * This only sets the ESP8266 into station mode.
 * 
 * 
 * Author: Aaron S. Crandall <crandall@gonzaga.edu>
 * Copyright: 2022
 * 
 */

// Library includes
#include <ESP8266WiFi.h>


// Global variables
char ssid[33] = "notyourssid";
char password[64] = "notyourpassword";


// ** **************************************************************************
void printNetworkMenu() {
  Serial.printf("*******************************************************\n");
  Serial.printf("**            Current Networking Options             **\n");
  Serial.printf("*******************************************************\n");
  Serial.printf("\n");
  Serial.printf(" [1] SSID:     %s\n", ssid);
  Serial.printf(" [2] Password: %s\n", password);
  Serial.printf("\n");
  Serial.printf(" [L] Load settings from flash memory\n");
  Serial.printf(" [S] Save settings to flash memory\n");
  Serial.printf(" [T] Test settings by connecting to wifi (also saves)\n");
  Serial.printf("\n");
  Serial.printf(" Select menu option: ");
}


// ** ******************************************************************
void loadNetworkSettings() {
  strcpy(ssid, WiFi.SSID().c_str());
  strcpy(password, WiFi.psk().c_str());
}


// ** ******************************************************************
void handleUserSelection(char userSelection) {
  Serial.printf("Handling user selection of: %c\n", userSelection);

  switch(userSelection) {
    case '\n':
      Serial.println("Entered endline -- do nothing");
      break;
    case '1':
      Serial.println("Set SSID");
      break;
    case '2':
      Serial.println("Set PSK");
      break;
    case 'L':
      Serial.println("Loading saved configs");
      loadNetworkSettings();
      Serial.println("WiFi configuration settings loaded.");
      break;
    case 'S':
      Serial.println("Save new configs");
      saveCurrentSettings();
      Serial.println("WiFi configuration settings saved.");
      break;
    case 'T':
      Serial.println("Test configuration by connecting to network.");
      attemptConnectionToWiFi();
      break;
    case 'G':
      Serial.println("** Go Zags! **");
      printBulldog();
      break;
    default:
      Serial.println("Invalid input, try again");
      break;
  }

  delay(1000);
}


// ** ******************************************************************
void handleUserInput() {
    String userInput = "";
    
    while(userInput == "" ) {
      userInput = Serial.readString();
    }
    Serial.printf("\n\n");
    userInput.toUpperCase();

    char userSelected = (userInput.c_str())[0];
    handleUserSelection(userSelected);
}


// ** ******************************************************************
void doUpdateConfiguration() {
  Serial.println("Running configuration update");
  while(true) {
    Serial.printf("\n\n");
    printNetworkMenu();
    handleUserInput();
  }
}


// ** ******************************************************************
// Saves configuration without attempting to connect to a network
void saveCurrentSettings() {
  int32_t channel = 0;
  const uint8_t* bssid = NULL;
  bool connect = false;
  WiFi.begin(ssid, password, channel, bssid, connect);
}


// ** ******************************************************************
void setup() {
  Serial.begin(115200);
  delay(1000);

  loadNetworkSettings();
  doUpdateConfiguration();
}


// ** ******************************************************************
void loop() {
  delay(100);
}


/*
 * Attempt to connect to WiFi
 */
void attemptConnectionToWiFi() {
  Serial.printf("\n\nAttempting WiFi Connection...");

  WiFi.begin(ssid, password);

  for(int retryCount = 0; (WiFi.status() != WL_CONNECTED) && (retryCount < 15); retryCount++) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" - ");
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("SUCCESS - WiFi connected!"));
    Serial.print(" Current ESP8266 IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    WiFi.printDiag(Serial);
  } else {
    Serial.println(F("FAILED - WiFi connection unsuccessful"));
  }
}


// ** ******************************************************************************
void printBulldog() {
Serial.println(" ,--.____                                     ____.--.   ");
Serial.println("/  .'.'\"``--...----------.___.----------...--''\"`.`.  \\  ");  
Serial.println("| .'.'         .                       .         `.`. |    ");
Serial.println("`. .'|     . ' - . _    `-----'    _ . - ' .     |`. .'    ");
Serial.println(" `.' `|   .'   _     \"-._     _.-\"     _   `.   |' `.'     ");
Serial.println("      |  |        \" -.           .- \"        |  |          ");
Serial.println("       \\|        ;;..  \"|i. .i|\"  ..;;        |/           ");
Serial.println("       `|      ,---.``.   ' '   .'',---.      |'           ");
Serial.println("        |    <'(__.'>.'---` '---`.<`.__)`>    |            ");
Serial.println("        |   `. `~  .'  ,-------.  `.  ~'.'    |            ");
Serial.println("        |  |=_\"`=.'  . `-.___.-' .  `.='\"_=|  |            ");
Serial.println("        |  |  ==/  : ` :   i   : ' :  \\==  |  |            ");
Serial.println("        |  | ==/      /\\___|___/\\      \\== |  |            ");
Serial.println("         `.| =Y      .' \"\"\"_\"\"\" `.      Y= |.'             ");
Serial.println("           L ||      ;  .==\"==.  ;      || J               ");
Serial.println("            \\ ;     .' '       ` `.     ; /                ");
Serial.println("             `.     ;             ;     .'                 ");
Serial.println("              ;    ;'\\           /`;    ;                  ");
Serial.println("              `;  .'.'/.       ,\\`.`.  ;'                  ");
Serial.println("               `-=;_-'  `-----'  `-_;=-'            -bodom-");

}
