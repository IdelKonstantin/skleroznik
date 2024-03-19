#include "FS.h"
#include "LittleFS.h"

#define SPIFFS LittleFS

#define FILE_COUNT 10
#define FILENAME_PREFIX "/"
#define FILENAME_SUFFIX ".log"

int MINS_ = 43;
int FILES_ = 1;

String generateNewFilename() {
    
    return String(FILENAME_PREFIX) + String(micros()) + String(FILENAME_SUFFIX);
}

void createNewFile() {
    
    String newFilename = generateNewFilename();
    File file = SPIFFS.open(newFilename, FILE_WRITE);

    if(!file) {
      Serial.print("Cant open file: ");
      Serial.println(newFilename);
    }
    else {
      Serial.print("File created: ");
      Serial.println(newFilename);
    }

    if(file) {

        int sec = MINS_*60;
        int steps = sec*5;

        Serial.print("Rec.minutes: ");
        Serial.println(MINS_);

        Serial.print("Rec.seconds: ");
        Serial.println(sec);

        for(int i = 1; i <= steps; i++) {

          file.println("55.222222,38.666666,123,34.6,12,45,33,6677,0.12,4.56,9.78,0.03,0.06,0.98,3.6,---,242,---,24,9600;");
        }

        file.close();
    }
}



void listFiles() {

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while(file){

      Serial.println(file.name());
      
      while(file.available()){
        Serial.write(file.read());
      }
      
      file.close();
      file = root.openNextFile();
    }
}

void setup() {
  
    Serial.begin(115200);

    if(!SPIFFS.begin(true)){
       Serial.println("An Error has occurred while mounting SPIFFS");
       while(1);
    }

    SPIFFS.format();

    for(int i = 1; i <= FILES_; i++) {
      createNewFile();
    }
     
    //listFiles();

    Serial.print("Used bytes: ");
    Serial.println(SPIFFS.usedBytes());

    Serial.print("Free bytes: ");
    Serial.println(SPIFFS.totalBytes() - SPIFFS.usedBytes());
}

void loop() {
  // Your code here
}
