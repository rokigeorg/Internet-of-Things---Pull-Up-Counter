int RFIDResetPin = 0;
bool validRFID = false;
bool timerFlag = false;
int redLed = 5;
int greenLed = 6;

void setup(){
    Serial.begin(9600); // the USB Serail communication between the PC and Photon 
    Serial1.begin(9600);// the Serail communication between the RFID IC and Photon 

    pinMode(RFIDResetPin, OUTPUT);
    digitalWrite(RFIDResetPin, HIGH);

    // set up status Leds if the IoT Project is counting the pull ups
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
}
 
void loop(){

  char tagString[13];
  
  getRFIDTagName(tagString);
  
  validRFID = checkTag(tagString); //Check if it is a match

  // turn on green LED if valid RFID
  setTrainLed(validRFID);

  clearTag(tagString); //Clear the char of all value
  resetReader(); //eset the RFID reader
}


void getRFIDTagName(char* tagStr){
    while(Serial1.available() > 0){
        int index = 0;
        boolean reading = false;
        //char tagStr[13];
        
        int readByte = Serial1.read(); //read next available byte
    
        if(readByte == 2) reading = true; //begining of tag
        if(readByte == 3) reading = false; //end of tag
    
        if(reading && readByte != 2 && readByte != 10 && readByte != 13){
          //store the tag
          tagStr[index] = readByte;
          index ++;
        }
        
        if(reading == false)    // when it is done reading the serail stream
        {
            Serial.println("********************");
            Serial.println('RFID Tag has been read. Tag responded with the following TagId:');
            Serial.println(tagStr);
            Serial.println("********************");
            
            //return the RFID tag value
            return;
        }
    }
}

void setTrainLed(bool trainValid) {
    
        Serial.print('timerFlag');
        Serial.println(timerFlag);


      if ( trainValid == false && timerFlag == false){
        
            digitalWrite(redLed, HIGH);
            digitalWrite(greenLed, HIGH); // needs to be Low
      
      }else{
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, HIGH);
            timerFlag = true;
      }
      return;
}

bool checkTag(char tag[]){
    ///////////////////////////////////
    //Check the read tag against known tags
    /////////////////////////////////////
    
        // Tags in the database
        char bigWhiteTag[13] = "23006E7FC7F5";
        //Serial.println("******* Check Tag ***********");
        //Serial.println(tag); //read out any unknown tag
    
        if(strlen(tag) == 0) return false; //empty, no need to contunue
      
      // check which tag it is
        if(tag == bigWhiteTag){
          return true;
        }else{
          Serial.println(tag); //read out any unknown tag
        }
        return false;
}

void resetReader(){
    ///////////////////////////////////
    //Reset the RFID reader to read again.
    ///////////////////////////////////
    
      digitalWrite(RFIDResetPin, LOW);
      digitalWrite(RFIDResetPin, HIGH);
      delay(250);
}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}
