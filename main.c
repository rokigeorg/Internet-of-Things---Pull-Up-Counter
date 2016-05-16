int RFIDResetPin = 0;

void setup(){
    Serial.begin(9600); // the USB Serail communication between the PC and Photon 
    Serial1.begin(9600);// the Serail communication between the RFID IC and Photon 

  pinMode(RFIDResetPin, OUTPUT);
  //digitalWrite(RFIDResetPin, HIGH);
digitalWrite(RFIDResetPin, HIGH);

}
 
void loop(){

  char tagString[13];
  int index = 0;
  boolean reading = false;

  while(Serial1.available() > 0){

    int readByte = Serial1.read(); //read next available byte

    if(readByte == 2) reading = true; //begining of tag
    if(readByte == 3) reading = false; //end of tag

    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
      //store the tag
      tagString[index] = readByte;
      index ++;
    }
    
    if(reading == false)    // when it is done reading the serail stream
    {
        Serial.println("********************");
        Serial.println('RFID Tag has been read. Tag responded with the following TagId:');
        Serial.println(tagString);
        Serial.println("********************");
    }
        
    }
   

  checkTag(tagString); //Check if it is a match
  clearTag(tagString); //Clear the char of all value
  resetReader(); //eset the RFID reader
}

void checkTag(char tag[]){
///////////////////////////////////
//Check the read tag against known tags
///////////////////////////////////

  if(strlen(tag) == 0) return; //empty, no need to contunue
  
  Serial.println(tag); //read out any unknown tag
  return;
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
