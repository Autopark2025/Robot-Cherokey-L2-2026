#include <Servo.h>

// --- CONFIGURATION PINS ---
int speedPin_M1 = 5; int speedPin_M2 = 6;
int directionPin_M1 = 4; int directionPin_M2 = 7;
int URPWM = 3; int URTRIG = 10;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(speedPin_M1, OUTPUT); pinMode(speedPin_M2, OUTPUT);
  pinMode(directionPin_M1, OUTPUT); pinMode(directionPin_M2, OUTPUT);
  pinMode(URTRIG, OUTPUT); digitalWrite(URTRIG, HIGH);
  pinMode(URPWM, INPUT);
  
  myservo.attach(9);
  myservo.write(90); // Centré
  Serial.println("Robot Pret !");
  delay(1000);
}

void loop() {
  int dist = MeasureDistance();
  Serial.print("Dist: "); Serial.println(dist);

  // Seuil de détection réduit à 20cm pour éviter les détections fantômes
  if (dist > 0 && dist <= 20) {
    Serial.println("Obstacle !");
    carStop(); delay(300);
    
    // Recul de dégagement
    carBack(150, 150); delay(500); 
    carStop(); delay(300);
    
    // Analyse des côtés
    myservo.write(30); delay(600);
    int dL = MeasureDistance();
    
    myservo.write(150); delay(600);
    int dR = MeasureDistance();
    
    myservo.write(90); delay(400);

    // Virage
    if (dL > dR) {
      carTurnLeft(200, 200);
    } else {
      carTurnRight(200, 200);
    }
    delay(350); 
    carStop();
    
    // FORCER L'AVANCÉE pour sortir de la zone de détection
    carAdvance(150, 150);
    delay(600);
    
  } else {
    carAdvance(130, 130);
  }
}

// --- FONCTIONS MOTEURS ---
void carStop() { analogWrite(speedPin_M1, 0); analogWrite(speedPin_M2, 0); }
void carAdvance(int s1, int s2) {
  digitalWrite(directionPin_M1, LOW); digitalWrite(directionPin_M2, LOW);
  analogWrite(speedPin_M1, s1); analogWrite(speedPin_M2, s2);
}
void carBack(int s1, int s2) {
  digitalWrite(directionPin_M1, HIGH); digitalWrite(directionPin_M2, HIGH);
  analogWrite(speedPin_M1, s1); analogWrite(speedPin_M2, s2);
}
void carTurnLeft(int s1, int s2) {
  digitalWrite(directionPin_M1, HIGH); digitalWrite(directionPin_M2, LOW);
  analogWrite(speedPin_M1, s1); analogWrite(speedPin_M2, s2);
}
void carTurnRight(int s1, int s2) {
  digitalWrite(directionPin_M1, LOW); digitalWrite(directionPin_M2, HIGH);
  analogWrite(speedPin_M1, s1); analogWrite(speedPin_M2, s2);
}

// --- FONCTION CAPTEUR ---
int MeasureDistance() {
  digitalWrite(URTRIG, LOW); digitalWrite(URTRIG, HIGH);
  unsigned long duration = pulseIn(URPWM, LOW, 50000);
  if (duration == 0) return 999;
  return duration / 50;
