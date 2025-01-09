#include <Serv tualWire.h>
const int receive_pin = 2;

// forward pin
const int flm = 3;
const int blm = 4;
const int frm = 33;
const int brm = 6;

// water motor
const int wmp = 8;

// seeding
ServoTimer2 seeder;
const int seeder_pin = 9;
const int standby_ang = 1100;
const int work_ang = 1000;
bool sstate = false;

// servo plough
ServoTimer2 plough;
ServoTimer2 plough2;
const int plough_pin = 22;
const int plough_pin2 = 23;
const int sp1 = 0;
const int neu = 1500;
const int sp2 = 5000;
bool pstate = false;

char buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;

void setup() {

  // motors
  pinMode(flm, OUTPUT);
  pinMode(blm, OUTPUT);
  pinMode(frm, OUTPUT);
  pinMode(brm, OUTPUT);

  pinMode(wmp, OUTPUT);
  digitalWrite(wmp, HIGH); // motor off

  // Servo plough
  plough.attach(plough_pin);
  plough2.attach(plough_pin2);
  seeder.attach(seeder_pin);

  Serial.begin(9600);
  Serial.println("serial open");

  seeder.write(standby_ang);
  
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_rx_start();                                                                                                                                                                                             lm, LOW);
}

void loop() {

  if (sstate) {
    seeder.write(work_ang);
    delay(200);
    seeder.write(standby_ang);
    delay(100);
  }
  if (vw_get_message(buf, &buflen)) {
    Serial.print("Got: ");
    Serial.println(buf);

    if (String(buf) == "f") {  // move forward ( set all frm and flm to high)
      digitalWrite(blm, LOW);
      digitalWrite(brm, LOW);
      digitalWrite(flm, HIGH);
      digitalWrite(frm, HIGH);
      Serial.println("forward");

    } else if (String(buf) == "b") {
      Serial.println("ackkkkkk");  // move backward (set all brm and blm to high)

      digitalWrite(flm, LOW);
      digitalWrite(frm, LOW);
      digitalWrite(blm, HIGH);
      digitalWrite(brm, HIGH);

    } else if (String(buf) == "r") {  // move right (set flm and brm high)
      digitalWrite(brm, LOW);
      digitalWrite(blm, LOW);
      digitalWrite(frm, LOW);
      digitalWrite(flm, LOW);
      digitalWrite(flm, HIGH);
      digitalWrite(brm, HIGH);

    } else if (String(buf) == "l") {  // move left (set blm and frm high)
      digitalWrite(brm, LOW);
      digitalWrite(blm, LOW);
      digitalWrite(frm, LOW);
      digitalWrite(flm, LOW);
      digitalWrite(blm, HIGH);
      digitalWrite(frm, HIGH);

    } else if (String(buf) == "p") {
      if (!pstate) {
        pstate = true;
        plough.write(sp1);
        plough2.write(sp2);
        delay(500); 
        plough.write(neu);
        plough2.write(neu);
      }

    } else if (String(buf) == "k") {
      digitalWrite(wmp, LOW);

    } else if (String(buf) == "s") {
      Serial.println("sstate true");
      sstate = true;

    } else if (String(buf) == "n") {
      digitalWrite(brm, LOW);
      digitalWrite(blm, LOW);
      digitalWrite(frm, LOW);
      digitalWrite(flm, LOW);
      if (pstate) {
        pstate = false;
        plough.write(sp2);
        plough2.write(sp1);
        delay(500);
        plough.write(neu);
        plough2.write(neu);
      }
      digitalWrite(wmp, HIGH);
      sstate = false;
      seeder.write(standby_ang);
    }
  }
}