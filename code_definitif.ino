#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <Wire.h>
//#include<ESP32Servo.h>


const char *ssid = "Vodafone CMR-D20B";
const char *password = "30786932";
const int pompe = 4;
const int servo = 18;
const int venti = 15;
const int humisol = 25;//analogique
const int pinlum = 36;//analogique
const int dhtpin = 26;
const int led = 2;
int pourcentage = 0;
//Servo myServo;
//etat led
bool etatLed = 0;
char texteEtatLed[2][10]={"éteinte","allumée"};

//etat serre
bool etatSerre = 0;
char texteEtatserre[2][20]={"automatique","connecte"};

//etat pompe      
bool etatpompe = 0;
char texteEtatpompe[2][10]={"en repos","en action"};

//etat venti
bool etatventi = 0;
char texteEtatventi[2][10]={"en repos","en action"};

//etat porte
bool etatporte = 0;
char texteEtatporte[2][10]={"ouverte","fermee"};

WebServer server(80);
DHT dht(dhtpin, DHT11);

//interface
//
//
//
//


     
void handleRoot() {
String page = "<!DOCTYPE html>";
page += "<html lang='en'>";
page += "<head>";
page += "    <meta http-equiv='refresh' content='1' charset='UTF-8'>";
page += " <meta http-equiv='refresh' content='IE-edge'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
page += "    <title>Document</title>";
page += "    <style type='text/css'> .vue{position: absolute; left: 250px; top: 130px; width: 350px; height: 400px; border: 3px black solid; border-radius: 20px; padding: 15px;}.control{position: absolute; left: 650px; top: 70px; width: 350px; height: 290px; border: 3px black solid; border-radius: 20px; padding: 15px;}.probleme{ position: absolute; left: 650px; top: 400px; width: 350px;height: 200px; border: 3px black solid; border-radius: 20px; padding: 15px;}.titre{ text-decoration: underline;}.boutton{text-decoration: none;background: white;border-radius: 5px; border: 2px black solid;color: black;}.boutton:hover{ color: cadetblue;}body{ background : linear-gradient(120deg, #01ff01, #ffffff);background-size: cover;}       .conteneur{ position: absolute; left: 230px;top: 50px;border: 3px black solid; border-radius: 9px;width:820px; height: 600px; backdrop-filter: blur(6px);}.titret{ text-decoration: blink;}</style>";
page += "</head>";
page += "<body>";
page += "    <div class='conteneur'><font size='55' color='black' id='titret'>......serre connecte......</font></div>";
page += "        <div class='vue'>";
page += "            <font size='45' color='green' class='titre'>mode serre:";page += texteEtatserre[etatSerre]; page=+"</font>";
page += "            <h1>humidite air :"; page += readDHTHumidity() ; page+="&percnt;</h1><h1 class='humair'></h1><h1>humidite sol :"; page += readhumiditesol(); page+="&percnt;</h1><h1 class='humsol'></h1><br>";
page += "            <h1>temperature :"; page += readDHTTemperature()*10; page+="<sup class='units'>&deg;C</sup></h1><h1 class='temperature'></h1><br>";
page += "            <h1>lumiere :"; page += lumiere(); page+="</h1>&percnt;<h1 class='lumiere'></h1>";
page += "        </div>";
page += "        <div class='control'> ";
page += "            <font size='45' color='green' class='titre'>control serre</font><br>";
page += "            <h2> eclairage :<a href='/on' class='boutton'>on</a>.....<a href='/off' class='boutton'>off</a>"; page += texteEtatLed[etatLed] ;page +="</h2>";
page += "            <h2>arrosage :<a href='/onpompe' class='boutton'>arroser</a><h2><a href='/offpompe' class='boutton'>arreter</a>"; page += texteEtatpompe[etatpompe];page +="</h2>";
page += "            <h2>aeration :<a href='/onventi' class='boutton'>on</a>.....<a href='/offventi' class='boutton'>off</a>"; page += texteEtatventi[etatventi];page +="</h2>";
//page += "            <h2>porte :<a href='#' class='boutton'>ouvrir</a>......<a href='#' class='boutton'>fermer</a>"; page += ;page +="</h2>";
page += "        </div>";
page += "        <div class='probleme'>";
page += "            <font size='45' color='green' class='titre'>etat systeme</font>";
page += "            <h2>niveau eau : petit/moyen/fort</h2>";
page += "            <h2>probleme systeme : oui/non</h2> ";
page += "        </div>";
page += "</body>";
page += "</html>";
    server.setContentLength(page.length());
    //
    server.send(200 ,"text/html", page);
}



//
//
//
//

//declaration des fonction control
//
//
//lumiere
void handleOn(){
etatLed=1;
etatSerre = 1;
digitalWrite(led,HIGH);
server.sendHeader("Location","/");
server.send(303);
}
void handleOff(){
etatSerre = 0;
etatLed=0;
digitalWrite(led,LOW);
server.sendHeader("Location","/");
server.send(303);
}
//pompe
void handleOnpompe(){
etatpompe=1;
digitalWrite(pompe,LOW);
server.sendHeader("Location","/");
server.send(303);
delay(5000);
digitalWrite(pompe,HIGH);
etatpompe=0;
} 
void handleOffpompe(){
etatpompe=0;
digitalWrite(pompe,HIGH);
server.sendHeader("Location","/");
server.send(303);
}
//venti
void handleOnventi(){
etatventi=1;
digitalWrite(venti,LOW);
server.sendHeader("Location","/");
server.send(303);
}
void handleOffventi(){
etatventi=0;
digitalWrite(venti,HIGH);
server.sendHeader("Location","/");
server.send(303);
}
//porte
//void handleOnporte(){
//etatporte=1;
//myServo.write(90);
//server.sendHeader("Location","/");
//server.send(303);
//}
//void handleOffporte(){
//etatporte=0;
//myServo.write(0);
//server.sendHeader("Location","/");
//server.send(303);
//}
void handleNotFound(){
  server.send(404,"text/plain","404 Not found!");
}

//
//
//
void setup(void) {

  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  pinMode(led,OUTPUT);
  pinMode(pompe,OUTPUT);
  pinMode(venti,OUTPUT);
  pinMode(humisol, INPUT);
  pinMode(pinlum, INPUT);
//  myServo.attach(servo);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
   server.on("/", handleRoot);
   server.on("/on", handleOn);
   server.on("/off", handleOff); 
   
   server.on("/onpompe", handleOnpompe);
   server.on("/offpompe", handleOffpompe);
   
   server.on("/onventi", handleOnventi);
   server.on("/offventi", handleOffventi);
//
//     server.on("/onporte", handleOnporte);
//   server.on("/offporte", handleOffporte);
//   
   server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
  
  handleOff( );
  if(lumiere()<1000 && etatSerre==1){
    handleOn();
  }
//  handleOffventi();
//  while(readDHTHumidity() < ){
//    digitalWrite(pompe,HIGH);
//    delay(8000);
//    digitalWrite(pompe,LOW);
//  }
//  while(readhumiditesol){
//     digitalWrite(pompe,HIGH);
//    delay(20000);
//    digitalWrite(pompe,LOW);
//  }

}


//fonction de la lecture
//
//
float lumiere(){
    float lum = analogRead(pinlum);
    if(isnan(lum)){
      Serial.println("failed to read la lumiere");
      return -1;
    }else{
      Serial.println(lum);
      return lum;
    }
    
}

float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}
float readhumiditesol(){
  float s = analogRead(humisol);
  float f = map(s,1023, 0, 0,1000);
  if (isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(f);
    return f;
  }
  
}
//
//
//fin fonction lecture
