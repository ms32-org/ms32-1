#include <VirtualWire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// lcd declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);
// transmitter pin declaration
const int transmit_pin = 11;
// switch declarations: y axis
const int right = 4;
const int left = 2;
// switch declaration: x axis
const int forward = 3;
const int back = 5;
// switch declaration: functions
const int seed = 6;
const int pest = 7;
const int plough = 8;

// switch declaration: mode
const int mode = 10;
bool last_state;
bool state;  // true =  hold , false = toggle
// GND pins 
const int modegnd = 9;
const int transgnd = 12;
// toggle mode variables storing last state
bool fwas = false;
bool bwas = false;
bool lwas = false;
bool rwas = false;
bool pwas = false;
bool kwas = false;
bool swas = false;

bool fdwas = false;
bool bdwas = false;
bool ldwas = false;
bool rdwas = false;
bool pdwas = false;
bool kdwas = false;
bool sdwas = false;
bool idwas = false;
// default msg to be sent and ignored by the main
char *msg = "n";

void setup() {
  //lcd setup
  lcd.init();
  lcd.backlight();
  // gnd pinmodes
  pinMode(transgnd, OUTPUT);digitalWrite(transgnd, LOW);
  pinMode(modegnd, OUTPUT);digitalWrite(modegnd, LOW);
  // switches
  pinMode(forward, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);

  pinMode(plough, INPUT_PULLUP);
  pinMode(pest, INPUT_PULLUP);
  pinMode(seed, INPUT_PULLUP);

  pinMode(mode, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Remote ready h sahab");
  display("Remote ready h");
  // transmitter setup
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true);
  vw_setup(2000);


  // read mode state
  if (digitalRead(mode) == LOW) {
    state = true;
    last_state = true;
    Serial.println("on hold mode");
  } else {
    state = false;
    last_state = false;
    Serial.println("on toggle mode");
  }
}


void loop() {
  if (digitalRead(mode) == LOW) {
    state = true;
    if (state != last_state) {
      last_state = state;
      display("On hold mode");
      Serial.println("On hold mode");
    }
    hold();
  } else if (digitalRead(mode) == HIGH) {
    state = false;
    if (state != last_state) {
      last_state = state;
      display("On toggle mode");
      Serial.println("On toggle mode");
    }
    toggle();
  }
}
void display(char msg[33]) {

  lcd.clear();
  lcd.print(msg);
}
void toggle() {
  if (digitalRead(forward) == LOW) {
    fwas = !fwas;
    if (fwas) {
      msg = "f";
      lcd.clear();
      Serial.println("Forward ON");
      display("Forward ON");
    } else {
      msg = "n";
      Serial.println("Forward OFF");

      lcd.clear();
      display("Forward OFF");
    }
  } else if (digitalRead(back) == LOW) {
    bwas = !bwas;
    if (bwas) {
      msg = "b";
      Serial.println("Backward ON");
      lcd.clear();
      display("Backward ON");
    } else {
      msg = "n";
      Serial.println("Backward OFF");
      lcd.clear();
      display("Backward OFF");
    }
  } else if (digitalRead(right) == LOW) {
    rwas = !rwas;
    if (rwas) {
      msg = "r";
      Serial.println("Right ON");
      lcd.clear();
      display("Right ON");

    } else {
      msg = "n";
      Serial.println("Right OFF");
      lcd.clear();
      display("Right OFF");
    }
  } else if (digitalRead(left) == LOW) {
    lwas = !lwas;
    if (lwas) {
      msg = "l";
      Serial.println("Left ON");
      lcd.clear();
      display("Left ON");
    } else {
      msg = "n";
      Serial.println("Left OFF");
      display("Left OFF");
    }
  } else if (digitalRead(plough) == LOW) {
    pwas = !pwas;
    if (pwas) {
      msg = "p";
      Serial.println("Plough ON");
      display("Plough ON");
    } else {
      msg = "n";
      Serial.println("Plough OFF");
      display("Plough OFF");
    }
  } else if (digitalRead(pest) == LOW) {
    kwas = !kwas;
    if (kwas) {
      msg = "k";
      Serial.println("Pest ON");
      display("Pest ON");
    } else {
      msg = "n";
      Serial.println("Pest OFF");
      display("Pest OFF");
    }
  } else if (digitalRead(seed) == LOW) {
    swas = !swas;
    if (swas) {
      msg = "s";
      Serial.println("Seeding ON");
      display("Seeding ON");
    } else {
      msg = "n";
      Serial.println("Seeding OFF");
      display("Seeding OFF");
    }
  }
  send(msg);
  delay(200);
}
void hold() {
  if (digitalRead(forward) == LOW) {
    send("f");
    Serial.println("Forward ON");
    idwas = false;
    if (!fdwas) {
      fdwas = true;
      display("Forward ON");
    }
  } else if (digitalRead(back) == LOW) {
    send("b");
    Serial.println("Backward ON");
    idwas = false;
    if (!bdwas) {
      bdwas = true;
      display("Backward ON");
    }
  } else if (digitalRead(right) == LOW) {
    send("r");
    Serial.println("Going Right");
    idwas = false;
    if (!rdwas) {
      rdwas = true;
      display("Going Right");
    }
  } else if (digitalRead(left) == LOW) {
    send("l");
    Serial.println("Going Left");
    idwas = false;
    if (!ldwas) {
      ldwas = true;
      display("Going Left");
    }
  } else if (digitalRead(plough) == LOW) {
    send("p");
    Serial.println("Plough ON");
    idwas = false;
    if (!pdwas) {
      pdwas = true;
      display("Plough ON");
    }
  } else if (digitalRead(pest) == LOW) {
    send("k");
    Serial.println("Pest on");
    idwas = false;
    if (!kdwas) {
      kdwas = true;
      display("Pest ON");
    }
  } else if (digitalRead(seed) == LOW) {
    send("s");
    Serial.println("Seeding on");
    idwas = false;
    if (!sdwas) {
      sdwas = true;
      display("Seeding ON");
    }
  } else {
    send("n");
    fdwas = false;
    bdwas = false;
    ldwas = false;
    rdwas = false;
    pdwas = false;
    kdwas = false;
    sdwas = false;
    if (!idwas) {
      idwas = true;
      display("IDLE");
    }
  }
}
void send(char *msg) {
  vw_send((uint8_t *)msg, 1);
  vw_wait_tx();
  delay(4);
}