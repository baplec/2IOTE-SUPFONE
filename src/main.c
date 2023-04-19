#include <LiquidCrystal.h>
#include <Keypad.h>
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#include <Wire.h>

// Keypad setup
const byte ROWS = 4; // Number of rows in the keypad
const byte COLS = 4; // Number of columns in the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {8, 9, A3, A2}; // Row pins of the keypad
byte colPins[COLS] = {A1, A0, 6, 7}; // Column pins of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// contacts
String contacts[] = {"0x00", "0x01", "0x02", "0x03"};
int contactLenght = sizeof(contacts)/sizeof(contacts[0]);

// functionalities
String texts[] = {"[1]: help", "[2]: send msg", "[3]: contact", "[4]: composer"};
  
// to delete
char msgTest[] = {'1','1','0','1','1'};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);




int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
    
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
int pos = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  Wire.begin();        // initialize I2C communication
}


void loop()
{
	//char i2cMethod = choseI2cAddress(); // Get the seleted i2c address method selected
	mainMenu();
  	


      pinMode(A0, INPUT);
    pinMode(8, OUTPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  
}

int i = 0;

void Melody()
{
  
  int newMelody[5];
  
  if (digitalRead(A3) == HIGH) {
    for (int j = 0; j < 5; j++) {
      Serial.print(myMelody[j]);
      if (newMelody[j] == 1) {
        tone(8, 440, 100);
        delay(10);
      } else if (newMelody[j] == 2) {
        tone(8, 494, 100);
        delay(10);
      } else {
        tone(8, 523, 100);
        delay(10);
      }
    }
  }
  
  if (digitalRead(A0) == HIGH) {
    tone(8, 440, 100);
    newMelody[i] = 0;
  }
  if (digitalRead(A1) == HIGH) {
    tone(8, 494, 100); 
    newMelody[i] = 1;
  }
  if (digitalRead(A2) == HIGH) {
    tone(8, 523, 100);
    newMelody[i] = 2;
  }
  
  i = i + 1;
  
  delay(10); 
}



char choseI2cAddress(){
    // Permit the user to chose its way to get an address (static or auto)
  	char key = keypad.getKey();
  	if (key == NO_KEY) {
      // Input the message to be sent
      lcd.setCursor(0, 0);
      lcd.print("Select addr");
      lcd.setCursor(0, 1);
      lcd.print("type:");
      lcd.setCursor(0, 1);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[1] : Automatic");
      lcd.setCursor(0, 1);
      lcd.print("[2] : static");
      delay(1);
      while (key != '1' || '2') {
        key = keypad.getKey();
        if (key == '1'){
          lcd.clear();
          lcd.print("1 selected");
          break;
        }
        if (key ==  '2'){
          lcd.clear();
          lcd.print("2 selected");
          break;
        }
      }	
      delay(1000);
      return key;
    }
}

void mainMenu(){
  //getFunctionalities();
  while (true) {
    char key = keypad.getKey();  
    switch (key) {
      case '1':
        //show help
        lcd.clear();
      	displayInfos(texts);
        break;
      case '2':
        //show contacts
      	Serial.print("2");
     	lcd.clear();
      	displayInfos(contacts);
        break;
      case '3':
        // show menu to send message
      	Serial.print("3");
		lcd.clear();
      	sendMessage();
        break;
      case '4':
        //Composer
      	lcd.clear();
      	sendMessage();
        break;
      case '5':
        //Composer
      	parseList(msgTest,5);
      	
         break;
      default:
        // if nothing else matches, do the default
        // default is optional
        break;
    }
  } 
}

void sendMessageToSalve(String message) {
  Wire.beginTransmission(0x2A); // set recipient Arduino's address
  Wire.write(message); // send message
  Wire.endTransmission();       // end transmission
  delay(1000);                  // wait for a second
}


void sendMessage(){
  char key;
  char msg[] = {};
  int iterator = 0;
  while(true){
    char key = keypad.getKey();
    if(key == 'D'){
      Serial.print("send msg");
      break;
    }
    if (key != NO_KEY){
      msg[iterator] = key;
      iterator++;
      lcd.setCursor(0,0);
      lcd.print(iterator-1);
      lcd.setCursor(0,1);
      lcd.print(msg[iterator-1]);
      Serial.print(msg[iterator-1]);
    }
  }
  for(int x = 0; x < iterator; x++){
    lcd.print(msg[x]);
    delay(1000);
  }
  delay(5000);
}

void parseList(char msgTest[], int iterator){
  String temp = "";
  char newMsg[] = {};
  for(int x = 0; x < iterator; x++){
    //Serial.println(x);
    if(msgTest[x] == '0'){
      	temp = "";
      	Serial.println("PARSE");
      	
    } else{
    	temp += msgTest[x];
    }
  }
}


char processMsg(String letter){
  // creer une fonction pour parse les msg 
      switch (letter) {
        case '1':
         return 'a';
          break;
        case '11':
          return 'b';
          break;
        case '111':
          return 'c';
          break;
        case '2':
          return 'd';
          break;
        case '22':
         return 'e';
          break;
        case '222':
         return 'f';
          break;
        case '3':
          return 'g';
          break;
        case '33':
          return 'h';
          break;
        case '333':
          return 'i';
          break;
        case '4':
         return 'j';
          break;
        case '44':
         return 'k';
          break;
        case '444':
          return 'l';
          break;
        case '5':
          return 'm';
          break;
        case '55':
          return 'n';
          break;
        case '555':
          return 'o';
          break;
        case '6':
          return 'p';
          break;
        case '66':
          return 'q';
          break;
        case '666':
          return 'r';
          break;
        case '6666':
          return 's';
          break;
        case '7':
         return 't';
          break;
        case '77':
         return 'u';
          break;
        case '777':
          return 'v';
          break;
        case '8':
          return 'w';
          break;
        case '88':
          return 'x';
          break;
        case '888':
          return 'y';
          break;
        case '8888':
          return 'z';
          break;
        default:
          // if nothing else matches, do the default
          // default is optional
          break;
       }
}

void displayInfos(String data[]){
  lcd.clear();
  char key = keypad.getKey();
  int x = 0;
  for(int x = 0; x < sizeof(contacts)/sizeof(contacts[0]); x++){
    lcd.setCursor(0, x%2);
    lcd.print(data[x]);
    if(x%2 == 1){
      delay(1500);
      lcd.clear();
    }
  }
}