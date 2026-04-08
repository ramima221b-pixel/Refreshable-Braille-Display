/* 
  Refreshable Braille Display - Manual Button Version
  Project: EEE 318 - Control System I Laboratory
  Features: Supports English & Bengali, Manual Refresh Button on Pin 8.
*/

// English Braille Patterns (a-z)
int eng_braille[26][6] = {
  {1,0,0,0,0,0}, {1,1,0,0,0,0}, {1,0,0,1,0,0}, {1,0,0,1,1,0}, {1,0,0,0,1,0}, 
  {1,1,0,1,0,0}, {1,1,0,1,1,0}, {1,1,0,0,1,0}, {0,1,0,1,0,0}, {0,1,0,1,1,0}, 
  {1,0,1,0,0,0}, {1,1,1,0,0,0}, {1,0,1,1,0,0}, {1,0,1,1,1,0}, {1,0,1,0,1,0}, 
  {1,1,1,1,0,0}, {1,1,1,1,1,0}, {1,1,1,0,1,0}, {0,1,1,1,0,0}, {0,1,1,1,1,0}, 
  {1,0,1,0,0,1}, {1,1,1,0,0,1}, {0,1,0,1,1,1}, {1,0,1,1,0,1}, {1,0,1,1,1,1}, {1,0,1,0,1,1}
};

// Bengali Braille Patterns (Mapped from IDs 101 to 161)
int bn_braille[][6] = {
  {1,0,0,0,0,0}, {0,0,1,1,1,0}, {0,1,0,1,0,0}, {0,0,1,0,1,0}, {1,0,1,0,0,1}, 
  {1,1,0,0,1,1}, {1,0,1,0,1,1}, {1,0,0,0,1,0}, {0,0,1,1,0,1}, {1,0,1,0,1,0}, 
  {0,1,0,1,1,1}, {1,0,1,0,0,0}, {1,0,0,0,0,1}, {1,1,0,1,1,0}, {1,1,0,0,0,1}, 
  {0,0,1,1,0,0}, {1,0,0,1,0,0}, {1,0,0,0,1,1}, {0,1,0,1,1,0}, {0,1,0,0,0,1}, 
  {1,1,0,0,1,0}, {0,1,1,1,0,1}, {0,1,1,0,1,1}, {0,1,1,1,1,1}, {0,1,1,0,0,1}, 
  {0,0,1,1,1,1}, {0,1,1,1,1,0}, {1,0,1,1,0,1}, {0,1,1,0,1,0}, {0,1,1,0,0,0}, 
  {1,0,1,1,1,0}, {1,1,1,1,0,0}, {1,1,0,1,0,0}, {1,1,0,0,0,0}, {1,0,0,0,1,1}, 
  {1,0,1,1,0,0}, {1,0,1,1,1,1}, {1,1,1,0,1,0}, {1,1,1,0,0,0}, {1,0,1,0,0,1}, 
  {1,1,1,1,0,1}, {0,1,1,1,0,0}, {1,1,0,0,1,0}, {1,1,1,0,1,1}, {1,1,1,1,1,1}, {0,1,0,1,1,1}, 
  {0,0,1,1,1,0}, {0,1,0,1,0,0}, {0,0,1,0,1,0}, {1,0,1,0,0,1}, {1,1,0,0,1,1}, 
  {1,0,1,0,1,1}, {1,0,0,0,1,0}, {0,0,1,1,0,1}, {1,0,1,0,1,0}, {0,1,0,1,1,1}, 
  {0,0,0,0,1,0}, {0,0,0,1,0,1}, {0,0,0,0,1,1}, {0,0,0,0,0,1}, {0,0,0,0,0,0}  
};

int controlPins[6] = {2, 3, 4, 5, 6, 7};
const int refreshButton = 8; // Refresh Button Pin

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], OUTPUT);
  }
  pinMode(refreshButton, INPUT_PULLUP); 
}

void loop() {
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();

    // English characters
    if (incomingByte >= 97 && incomingByte <= 122) {
      showPattern(eng_braille[incomingByte - 97]);
    }
    // Bengali characters
    else if (incomingByte >= 101 && incomingByte <= 161) {
      showPattern(bn_braille[incomingByte - 101]);
    }

    // --- MANUAL REFRESH BUTTON LOGIC ---
    while (digitalRead(refreshButton) == HIGH) {
      delay(10); // পিন চেক করার মাঝে সামান্য বিরতি
    }

    // বাটন ছাড়া পর্যন্ত অপেক্ষা (Debouncing)
    while (digitalRead(refreshButton) == LOW) {
      delay(10);
    }

    // Python কে সিগন্যাল পাঠানো যে পরের ক্যারেক্টার পাঠানো যাবে
    Serial.write('N'); 

    // সোলেনয়েড রিসেট করা
    for (int k = 0; k < 6; k++) {
      digitalWrite(controlPins[k], LOW);
    }
    delay(200); 
  }
}

void showPattern(int p[6]) {
  for (int k = 0; k < 6; k++) {
    digitalWrite(controlPins[k], p[k]);
  }
}