
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
int pos = 0;

void setup()
{
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
}

int i = 0;

void loop()
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

      if (j == 5) {
        i = 0;
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