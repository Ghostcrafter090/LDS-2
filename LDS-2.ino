// Digital Pins
int Peizo = 8;
int greenLed = 3;
int yellowLed = 5;
int floodLight = 6;
int fan = 10;

// Vars
String readString;

int an = 0;
int power = 1023;
int an2 = 0;
int fn = 0;
int ft = 0;
int anL = 0;
int anH = 0;
int anLow = 0;
int anHigh = 0;
int timen = 0;
int timel = 0;
int freqn = 0;
int dangerLevel = 0;
int light = 0;
int freql = 0;
int temp = 0;
int loopTic = 0;
int simulateStrike = 0;
int fanl = 0;
int lol = 0;

unsigned long secn = 0;
unsigned long calibt = millis() + 30000;
unsigned long floodLightt = 0;
unsigned long dt = 0;
unsigned long dTic = 0;
unsigned long calTic = 0;
unsigned long fanLow = 0;
unsigned long retryFan = 0;

unsigned long tempCal[6] = {0, 0, 0, 0, 0, 0};

bool fl = 0;
bool calibrate = 0;
bool tonef;
bool powerout;
bool fanMode = 0;
bool runFan = 1;

void setup() {
  Serial.begin(2000000);
  pinMode(Peizo, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(floodLight, OUTPUT);
  pinMode(fan, OUTPUT);
  tone(Peizo, 1000, 1000);
  delay(1000);
}

void tempCalibrate() {
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(fan, LOW);
  Serial.println("");
  Serial.print("Standard Reading: ");
  tempCal[0] = 0;
  loopTic = 0;
  while (loopTic < 1000) {
    tempCal[0] = tempCal[0] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[0] = tempCal[0] / 999;
  Serial.print(tempCal[0]);
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(Peizo, HIGH);
  Serial.println("");
  Serial.print("Peizo On Reading: ");
  loopTic = 0;
  tempCal[1] = 0;
  while (loopTic < 1000) {
    tempCal[1] = tempCal[1] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[1] = tempCal[1] / 999;
  Serial.print(tempCal[1]);
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(greenLed, HIGH);
  Serial.println("");
  Serial.print("greenLed On Reading: ");
  loopTic = 0;
  tempCal[2] = 0;
  while (loopTic < 1000) {
    tempCal[2] = tempCal[2] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[2] = tempCal[2] / 999;
  Serial.print(tempCal[2]);
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(yellowLed, HIGH);
  Serial.println("");
  Serial.print("yellowLed On Reading: ");
  loopTic = 0;
  tempCal[3] = 0;
  while (loopTic < 1000) {
    tempCal[3] = tempCal[3] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[3] = tempCal[3] / 999;
  Serial.print(tempCal[3]);
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(floodLight, HIGH);
  Serial.println("");
  Serial.print("floodLight On Reading: ");
  loopTic = 0;
  tempCal[4] = 0;
  while (loopTic < 1000) {
    tempCal[4] = tempCal[4] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[4] = tempCal[4] / 999;
  Serial.print(tempCal[4]);
  digitalWrite(Peizo, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(floodLight, LOW);
  digitalWrite(fan, HIGH);
  Serial.println("");
  Serial.print("Fan On Reading: ");
  loopTic = 0;
  tempCal[5] = 0;
  while (loopTic < 1000) {
    tempCal[5] = tempCal[5] + analogRead(1);
    loopTic = loopTic + 1;
  }
  tempCal[5] = tempCal[5] / 999;
  Serial.print(tempCal[5]);
  tempCal[1] = tempCal[1] - tempCal[0];
  tempCal[2] = tempCal[2] - tempCal[0];
  tempCal[3] = tempCal[3] - tempCal[0];
  tempCal[4] = tempCal[4] - tempCal[0];
  tempCal[5] = tempCal[5] - tempCal[0];
}

void loop() {

  // Command Manager
  if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') {
      if (readString.length() > 1) {
        Serial.println("");
        Serial.print(readString); //prints string to serial port out
        if (readString == "\nhelp") {
          Serial.println("");
          Serial.println("Help Menu");
          Serial.println("---------");
          Serial.println("return {value}");
          Serial.println(" -----> - dangerLevel");
          Serial.println(" -----> - senserReading");
          Serial.println(" -----> - calibratedValues");
          Serial.println("simulateStrike");
          Serial.println("lowerDanger");
          Serial.println("");
          Serial.println("Include EOL char \",\" at end of each command");
        }
        if (readString == "\nreturn dangerLevel") {
          Serial.println("");
          Serial.print("dangerLevel value; ");
          Serial.print(dangerLevel);
          Serial.print("; dTic value; ");
          Serial.print(dTic);
          Serial.print("; at tic; ");
          Serial.print(millis());
        }
        if (readString == "\nreturn sensorReading") {
          Serial.println("");
          Serial.print("lightningSensor reading; ");
          Serial.print(an);
          Serial.print("; lightSensor reading; ");
          Serial.print(light);
          // Serial.print(";lightningSensor reading; ");
          // Serial.print(an);
          Serial.print("; tempSensor reading; ");
          Serial.print(temp);
          Serial.print("; at tic; ");
          Serial.print(millis());
        }
        if (readString == "\nreturn calibratedValues") {
          Serial.println("");
          Serial.print("calibratedValues; High, Low; ");
          Serial.print(anHigh);
          Serial.print(", ");
          Serial.print(anLow);
          Serial.print("; at tic; ");
          Serial.print(millis());
        }
        if (readString == "\nsimulateStrike") {
          simulateStrike = 1;
        }
        if (readString == "\nlowerDanger") {
          dangerLevel = dangerLevel - 1;
          if (tonef != 1) {
            digitalWrite(Peizo, LOW);
            tone(Peizo, 440, 100);
            secn = 100 + millis();
            tonef = 1;
          }
          Serial.println("");
          Serial.print("dangerLevel lowered to value; ");
          Serial.print(dangerLevel);
          Serial.print("; dTic value; ");
          Serial.print(dTic);
          Serial.print("; at tic; ");
          Serial.print(millis());
        }
        if (readString == "\ncalibrateTemp") {
          tempCalibrate();
        }
        // Serial.println(','); //prints delimiting ","
        //do stuff with the captured readString
        readString = ""; //clears variable for new input
      }
    }
    else {
      readString += c; //makes the string readString
    }
  }

  // Read dummy input and sensor input
  an = analogRead(4);
  an2 = analogRead(5);
  light = analogRead(0);
  power = analogRead(2);
  an = an - an2;
  temp = analogRead(1);

  // Temp Calibration Calibrator
  if (powerout != 1) {
    if (calTic < millis()) {
      calTic = millis() + 3600000;
      tempCalibrate();
    }
  }
  if (digitalRead(Peizo) != 0) {
    temp = (-1 * tempCal[1]) + temp;
  }
  if (digitalRead(greenLed) != 0) {
    temp = (-1 * tempCal[2]) + temp;
  }
  if (digitalRead(yellowLed) != 0) {
    temp = (-1 * tempCal[3]) + temp;
  }
  if (digitalRead(floodLight) != 0) {
    temp = (-1 * tempCal[4]) + temp;
  }
  if (digitalRead(fan) != 0) {
    temp = (-1 * tempCal[5]) + temp;
  }

  // Power Outage Handler
  if (power < 300 and powerout != 1) {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(floodLight, LOW);
    digitalWrite(fan, LOW);
    powerout = 1;
    calibrate = 0;
    calibt = millis() + 30000;
    Serial.println("");
    Serial.print("Power outage detected.; ");
    Serial.print("Entering power outage mode...");
    Serial.print("; at tic; ");
    Serial.print(millis());
    noTone(Peizo);
    tone(Peizo, 1000, 1000);
    delay(1000);
    noTone(Peizo);
    delay(1000);
    tone(Peizo, 1000, 1000);
    delay(1000);
    noTone(Peizo);
    delay(1000);
    tone(Peizo, 1000, 1000);
    delay(1000);
    noTone(Peizo);
    delay(1000);
    tone(Peizo, 1000, 1000);
    delay(1000);
    noTone(Peizo);
    delay(1000);
    tone(Peizo, 1000, 1000);
    delay(1000);
    noTone(Peizo);
    delay(1000);
    Serial.println("");
    Serial.print("Starting initial calibration...");
    Serial.print("; at tic; ");
    Serial.print(millis());
    floodLightt = millis() + 120000;
  }

  // Power Restoration Handler
  if (powerout == 1 and power > 300) {
    Serial.println("");
    Serial.print("Power restoration detected.");
    Serial.print("; at tic; ");
    Serial.print(millis());
    Serial.println("");
    Serial.print("Starting initial calibration...");
    Serial.print("; at tic; ");
    Serial.print(millis());
    powerout = 0;
    calibrate = 0;
    calibt = millis() + 30000;
    floodLightt = millis() + 120000;
  }

  // Fan Module
  runFan = 1;
  if (powerout != 1) {
    if (temp < 781) {
      if (analogRead(3) > lol) {
        lol = analogRead(3);
      }
      if (runFan == 1) {
        fanl = fanl + 1;
        if (fanl > 10000) {
          fanl = 0;
          if (lol < 810) {
            if (retryFan < millis()) {
              retryFan = millis() + 5000;
            }
          }
          lol = 0;
        }
      }
      if ((retryFan - 4500) > millis()) {
        digitalWrite(fan, LOW);
      } else {
        if (runFan == 1) {
          digitalWrite(fan, HIGH);
        } else {
          digitalWrite(fan, LOW);
        }
      }
      fanMode = 1;
    } else {
      fanMode = 0;
      if (calibrate == 0) {
        if (analogRead(3) > lol) {
          lol = analogRead(3);
        }
        if (runFan == 1) {
          fanl = fanl + 1;
          if (fanl > 10000) {
            fanl = 0;
            if (lol < 810) {
              if (retryFan < millis()) {
                retryFan = millis() + 5000;
              }
            }
            lol = 0;
          }
        }
        if ((retryFan - 4500) > millis()) {
          digitalWrite(fan, LOW);
        } else {
          if (runFan == 1) {
            digitalWrite(fan, HIGH);
          } else {
            digitalWrite(fan, LOW);
          }
        }
      } else {
        analogWrite(fan, 0);
      }
    }
  } else {
    fanMode = 0;
    digitalWrite(fan, LOW);
  }


  // Flood Light Manager
  if (floodLightt > millis()) {
    if (dangerLevel < 5) {
      if (light > 1000) {
        digitalWrite(floodLight, HIGH);
      }
    } else {
      if (dangerLevel < 5) {
        digitalWrite(floodLight, LOW);
      }
    }
  }

  // Call Audio Manager
  if (secn < millis()) {
    noTone(Peizo);
    tonef = 0;
  }

  // Calibration Code
  if (anLow > an) {
    anLow = an;
    timel = millis();
    if (tonef != 1) {
      digitalWrite(Peizo, HIGH);
    }
    if (calibrate != 1) {
      Serial.println("");
      Serial.print("Calibrated low value to; ");
      Serial.print(anLow);
      Serial.print("; at tic; ");
      Serial.print(millis());
    }
  } else if (anHigh < an) {
    anHigh = an;
    timel = millis();
    if (tonef != 1) {
      digitalWrite(Peizo, HIGH);
    }
    if (calibrate != 1) {
      Serial.println("");
      Serial.print("Calibrated high value to; ");
      Serial.print(anHigh);
      Serial.print("; at tic; ");
      Serial.print(millis());
    }
  } else {
    if (tonef != 1) {
      digitalWrite(Peizo, LOW);
    }
  }
  timen = millis();
  timen = timen - timel;
  if (timen > 10000) {
    timel = millis();
    anL = (anL + anLow) / 2;
    anH = (anH + anHigh) / 2;
    anLow = 0;
    anHigh = 0;
    if (tonef != 1) {
      digitalWrite(Peizo, LOW);
      tone(Peizo, 650, 10);
      secn = 10 + millis();
      tonef = 1;
    }
  }

  // Initial Calibration
  if (((an > (anH + (anH / 3)) or an < (anL + (anL / 3))) and calibrate != 1)) {
    calibt = millis() + 30000;
    Serial.println("");
    Serial.print("System Calibrate Triggered at value; ");
    Serial.print(an);
    Serial.print("; at tic; ");
    Serial.print(millis());
  }
  if (calibt < millis() and calibrate != 1) {
    Serial.println("");
    Serial.print("System Calibrated. Entering normal operation...");
    Serial.print("; at tic; ");
    Serial.print(millis());
    if (powerout != 1) {
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(Peizo, 500, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, HIGH);
      tone(Peizo, 675, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(Peizo, 600, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, HIGH);
      tone(Peizo, 800, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, HIGH);
      tone(Peizo, 800, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(Peizo, 600, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, HIGH);
      tone(Peizo, 675, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(Peizo, 500, 1000);
      delay(1000);
      noTone(Peizo);
      digitalWrite(yellowLed, LOW);
      digitalWrite(greenLed, LOW);
    }
    calibrate = 1;
  }

  // Strike Detection
  if (calibrate == 1) {
    if (fanMode != 1) {
      if ((((an > (anH + (anH / 3)) or an < (anL + (anL / 3)))) and (an > 0)) or (simulateStrike == 1)) {
        if (powerout == 1) {
          floodLightt = millis() + 120000;
        }
        simulateStrike = 0;
        Serial.println("");
        Serial.print("Strike Detected! reading of; ");
        Serial.print(an);
        Serial.print("; High, Low; ");
        Serial.print(anH);
        Serial.print(", ");
        Serial.print(anL);
        Serial.print("; Current Danger Level; ");
        if (tonef != 1) {
          digitalWrite(Peizo, LOW);
          tone(Peizo, 750, 1000);
          secn = 1000 + millis();
          tonef = 1;
        }
        dangerLevel = dangerLevel + 1;
        Serial.print(dangerLevel);
        Serial.print("; at tic; ");
        Serial.print(millis());
      }
    } else {
      if (((an > (anH + (anH / 2)) or an < (anL + (anL / 2)))) or simulateStrike == 1) {
        if (powerout == 1) {
          floodLightt = millis() + 120000;
        }
        simulateStrike = 0;
        Serial.println("");
        Serial.print("Strike Detected! reading of; ");
        Serial.print(an);
        Serial.print("; High, Low; ");
        Serial.print(anH);
        Serial.print(", ");
        Serial.print(anL);
        Serial.print("; Current Danger Level; ");
        if (tonef != 1) {
          digitalWrite(Peizo, LOW);
          tone(Peizo, 750, 1000);
          secn = 1000 + millis();
          tonef = 1;
        }
        dangerLevel = dangerLevel + 1;
        Serial.print(dangerLevel);
        Serial.print("; at tic; ");
        Serial.print(millis());
      }
    }
  }

  // Danger Level
  if (dTic < millis()) {
    dTic = millis() + 240000;
    dangerLevel = dangerLevel - 1;
    if (dangerLevel != -1) {
      Serial.println("");
      Serial.print("dangerLevel lowered to value; ");
      Serial.print(dangerLevel);
      Serial.print("; dTic value; ");
      Serial.print(dTic);
      Serial.print("; at tic; ");
      Serial.print(millis());
    }
    if (dangerLevel < 0) {
      if (tonef != 1) {
        digitalWrite(Peizo, LOW);
        tone(Peizo, 440, 100);
        secn = 100 + millis();
        tonef = 1;
      }
    }
  }
  if (dangerLevel < 0) {
    dangerLevel = 0;
  }
  if (dangerLevel > 5) {
    dangerLevel = 5;
  }

  // Display Code
  if (dt < (millis())) {
    dt = millis() + 1000;
  }
  if (calibrate != 0) {
    if (powerout != 1) {
      if (dangerLevel == 0) {
        if (light < 950) {
          digitalWrite(greenLed, HIGH);
          digitalWrite(yellowLed, LOW);
        } else {
          analogWrite(greenLed, 40);
          digitalWrite(yellowLed, LOW);
        }
      }
      if (dangerLevel == 1) {
        if (light < 950) {
          digitalWrite(greenLed, HIGH);
          digitalWrite(yellowLed, HIGH);
        } else {
          analogWrite(greenLed, 40);
          digitalWrite(yellowLed, 40);
        }
      }
      if (dangerLevel == 2) {
        if (light < 950) {
          if ((dt - 500) < millis()) {
            digitalWrite(greenLed, HIGH);
          } else {
            digitalWrite(greenLed, LOW);
          }
          digitalWrite(yellowLed, HIGH);
        } else {
          if ((dt - 500) < millis()) {
            analogWrite(greenLed, 40);
          } else {
            digitalWrite(greenLed, LOW);
          }
          digitalWrite(yellowLed, 40);
        }
      }
      if (dangerLevel == 3) {
        if (light < 950) {
          digitalWrite(greenLed, LOW);
          digitalWrite(yellowLed, HIGH);
        } else {
          digitalWrite(greenLed, LOW);
          analogWrite(yellowLed, 40);
        }
      }
      if (dangerLevel == 4) {
        if (light < 950) {
          digitalWrite(greenLed, LOW);
          if ((dt - 500) < millis()) {
            digitalWrite(yellowLed, HIGH);
          } else {
            digitalWrite(yellowLed, LOW);
          }
        } else {
          digitalWrite(greenLed, LOW);
          if ((dt - 500) < millis()) {
            analogWrite(yellowLed, 40);
          } else {
            digitalWrite(yellowLed, LOW);
          }
        }
      }
    }
    if (dangerLevel > 4) {
      if ((dt - 500) < millis()) {
        if (powerout != 1) {
          digitalWrite(yellowLed, HIGH);
        } else {
          digitalWrite(yellowLed, LOW);
        }
        freql = 750;
        if (powerout == 1) {
          digitalWrite(floodLight, HIGH);
        } else {
          digitalWrite(floodLight, LOW);
        }
        if (tonef != 1) {
          digitalWrite(Peizo, LOW);
          tone(Peizo, 750, 490);
          secn = 490 + millis();
          tonef = 1;
        }
      } else {
        if (powerout != 1) {
          digitalWrite(yellowLed, LOW);
          if (tonef != 1) {
            freql = freql + 100;
            digitalWrite(Peizo, LOW);
            tone(Peizo, freql, 50);
            secn = 50 + millis();
            tonef = 1;
            if (fl == 1) {
              fl = 0;
              digitalWrite(floodLight, HIGH);
            } else {
              fl = 1;
              digitalWrite(floodLight, LOW);
            }
          }

        } else {
          digitalWrite(yellowLed, LOW);
          if (tonef != 1) {
            freql = freql + 100;
            digitalWrite(Peizo, LOW);
            tone(Peizo, freql, 50);
            secn = 50 + millis();
            tonef = 1;
            if (fl == 1) {
              fl = 0;
              digitalWrite(floodLight, LOW);
            } else {
              fl = 1;
              digitalWrite(floodLight, HIGH);
            }
          }
        }
      }
    }
  } else {
    if (powerout != 1) {
      digitalWrite(greenLed, HIGH);
    }
    digitalWrite(yellowLed, LOW);
  }
}
