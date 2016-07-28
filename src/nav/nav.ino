#include <path.h>

Path path;
void setup() {
  Serial.begin(9600);
  Serial.println("Initialized");
}

int turn;

boolean findPath = false;
uint8_t t, n;
boolean start = false;
boolean stop = false;

void loop() {
  path.find();
  turn = path.turn();
  switch (turn) {
    case 0: Serial.print("backwards\n");
      break;
    case 1: Serial.print("left\n");
      break;
    case 2: Serial.print("straight\n");
      break;
    case 3: Serial.print("right\n");
      break;
    case 4: Serial.print("uturn\n");
  }
  path.stats();
  while ( !start && !stop ) {
    start = startbutton();
    stop = stopbutton();
  }
  if (stop) {
    Serial.print("collision!\n");
    path.avoid();
  }
  else {
    path.update();
  }
  stop = false;
  start = false;
  delay(500);
}

void looop()
{
  while (!startbutton());
  if (!findPath) {

    findPath = true;
    t = path.turn();

    switch (t) {
    case 0: Serial.print("backwards\n");
      break;
    case 1: Serial.print("left\n");
      break;
    case 2: Serial.print("straight\n");
      break;
    case 3: Serial.print("right\n");
      break;
    case 4: Serial.print("uturn\n");
  }
  
    path.stats();
    path.update();
  }
  else if (findPath) {
    n = path.find();
    findPath = false;
  }
}

