#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define  trig  D1
#define  echo  D2
#define batasPenuh 5
#define batasKurang 15
#define kranAir D5


int data;
int count = 0;
bool airPenuh = true;
long duration, cm;
long previousMillis = 0;
long interval = 1000;
int distance;

// You should get Auth Token in the Blynk App.

char auth[] = "f9z1XR774cvuQYFlatZZ1mNMsVbVn-NA";
char ssid[] = "Red";   // your ssid 
char pass[] = "12345678"; // your pass

BlynkTimer timer;


void bacaAir(){
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  cm = (duration*0.034)/2;

  Serial.print(cm);
  Serial.print("cm");
}

void setup()
{
  // Debug console
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Inpu
  pinMode(kranAir, OUTPUT);
  digitalWrite(kranAir,LOW);
  delay(100);
  
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  bacaAir();
  if((cm <= batasPenuh)){
    if(airPenuh){
      Serial.println(" Bak Penuh");
      Blynk.notify("Bak Penuh");
      digitalWrite(kranAir, LOW);
      airPenuh = false;
    }
  }
  else{
    airPenuh = true;
  }
  if((cm >= batasKurang)){
    Serial.println(" Bak Belum Penuh");
    digitalWrite(kranAir, HIGH);
  }

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval){
    previousMillis = currentMillis;
    if(cm > batasPenuh){
      count++;
      delay(1000);
      if(count == 300){
        Serial.println("Bak Mandi Masih Belum Penuh");
        digitalWrite(kranAir, HIGH);
        count = 0;
      }
    }
  }
  Blynk.run();
  timer.run();
}
void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low

  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance

  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  
  Blynk.virtualWrite(V0, distance);
  delay(1000);                        //Pause for 3 seconds and start measuring distance again
}
