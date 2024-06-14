// Inclusão de bibiotecas/módulos
#include <Adafruit_LiquidCrystal.h> 
Adafruit_LiquidCrystal display_1(0);

#include <Servo.h>
Servo cortinaLux;

// Declaração de variaveis das portas

int led13 = 13;
int led12 = 12;
int led11 = 11;
int led10 = 10;
int led9 = 9;
// Leds declaraveis

byte pinoLDR = A1;
float valorLDR = 0;
float LuxMap;
// LDR's

byte sensorTemp = A0;
int valorTemp = 1;
float temperaturaC;
// Sensor Temperatura

int sensorKoppen = A2;
float KoppenRead;
float umidadeGeral;
// Sensor Umidade

float brilho;
float brilhoInversamente;
// Fading LED

int motorVent = 8;
// servo = digital 7
int lampada6 = 6;
int motorWater = 5;
// 
float tensao;
  
void setup() {

// Pins de Sensor
pinMode(sensorTemp, INPUT);
pinMode(pinoLDR, INPUT);
Serial.begin(9600);

// Pins de LED
pinMode(led13, OUTPUT);
pinMode(led12, OUTPUT);
pinMode(led11, OUTPUT);
pinMode(led10, OUTPUT);
pinMode(led9, OUTPUT);

// DISPLAY
display_1.begin(16, 2);

// SERVO
cortinaLux.attach(7);

// General PinMode Configuration
pinMode(lampada6, OUTPUT);
pinMode(A2, INPUT);
pinMode(motorVent, OUTPUT);
analogRead(A1);
pinMode(A1, INPUT);
pinMode(motorWater, OUTPUT);
}

void loop() {
// As funções de valor serial estão DESATIVADAS, recolocar caso necessário
valorTemp = analogRead(sensorTemp);
temperaturaC = valorTemp * 5.0 /1024;
temperaturaC = temperaturaC * 100 - 50;
// SerialTemp();
SerialTempLCD();
maintenanceHighTemp();
maintenanceLowTemp();
  
LuxMap = analogRead(pinoLDR);
valorLDR = map(LuxMap, 0, 1023, 0, 100);
// SerialLux();
SerialLuxLCD();
maintenanceLux();

KoppenRead = analogRead(A2);
umidadeGeral = map(KoppenRead, 0, 1023, 0, 100);
// SerialKoppen();
SerialKoppenLCD();
maintenanceKoppen();
}


void SerialLux(){
// É o programa para exibir o valor de lux por meio de comunicação
// serial, propriamente no console. 
Serial.print("Intensidade Luminosa: ");
Serial.println(valorLDR);
Serial.println(" %");
}


void SerialLuxLCD(){
// É o programa para exibir os valores de lux no display LCD 2x16 IEC
// acoplado ao projeto.
  display_1.setCursor(9,0);
  display_1.print("LuxAmb");
  delay(100); 
  display_1.setCursor(9,1);
  display_1.print(valorLDR);
  delay(1000);
  display_1.clear();
}


void SerialTemp(){
// É o programa para exibir o valor de temperatura por meio de comunicação
// serial, propriamente no console. 
Serial.print("Temperatura Atual: ");
Serial.println(temperaturaC);
Serial.println(" °C");
} 


void SerialTempLCD(){
// É o programa para exibir os valores de temperatura no display LCD 2x16 IEC
// acoplado ao projeto.
  display_1.setCursor(1,0);
  display_1.print("Temp =");
  delay(100);
  display_1.setCursor(1,1);
  display_1.print(temperaturaC);
  delay(100);
  display_1.setCursor(6,1);
  display_1.print("C");
  delay(500);
}


void SerialKoppen(){]
// É o programa para exibir o valor de umidade por meio de comunicação
// serial, propriamente no console.
Serial.print("Umidade Atual: ");
Serial.println(umidadeGeral);
Serial.println(" %");
}


void SerialKoppenLCD(){
// É o programa para exibir os valores de umidade no display LCD 2x16 IEC
// acoplado ao projeto.
  display_1.setCursor(2,0);
  display_1.print("UmidadeAtual");
  delay(100);
  display_1.setCursor(5,1);
  display_1.print(umidadeGeral);
  display_1.setCursor(10,1);
  display_1.print("%");
  delay(500);
  display_1.clear();  
}


void fault(){ //FALHA GENÉRICA
// Ativa um sinal de falha caso seja chamado, visível nos LED's enfileirados
digitalWrite(led13, HIGH), digitalWrite(led12, HIGH), digitalWrite(led11, HIGH),delay(250);
  digitalWrite(led13, LOW), digitalWrite(led12, LOW), digitalWrite(led11, LOW), delay(250);
}


void maintenanceHighTemp(){
// Essa função é responsável por ligar um ventilador (para resfriar/movimentar
// o ar presente) enquanto a temperatura estiver maior que 28° Celsius
if (temperaturaC >= 28){
  digitalWrite(motorVent, HIGH), digitalWrite(led9, HIGH);}
  else if ((temperaturaC >= 23) && (temperaturaC <= 26)) {
    digitalWrite(motorVent, LOW), digitalWrite(led9, LOW);}
    else{}
}


void maintenanceLowTemp(){
// Caso a temperatura esteja menor que 23° Celsius, é ligada uma lampada "solar"
// para aquecer o ambiente, ela desliga após atingir uma temperatura maior que 
// 24° Celsius
if (temperaturaC <= 23){
  digitalWrite(lampada6, HIGH), digitalWrite(led10, HIGH);}
  else if (temperaturaC >= 24){
    digitalWrite(lampada6, LOW), digitalWrite(led10, LOW);}
    else{}
}


void maintenanceKoppen(){
// Koppen é uma palavra utilizada para probabilidade de chuva baseada na pressão
// atmosférica. Nesse caso, representando nesse sistema, a umidade do solo. Se a
// umidade estiver menor que 65%, um irrigador será ligado brevemente.  
if (umidadeGeral <= 65){
  digitalWrite(motorWater, HIGH), fault();}
  else if (umidadeGeral >= 70){
    digitalWrite(motorWater, LOW);}
    else{}
}


void maintenanceLux(){
// Lux foi a nomenclatura definida para representar as cargas de fotons, então nesse
// caso, a iluminação geral. Caso a iluminação estivesse MUITO forte, uma cortina é
// direcionada seguindo o valor do LDR e então um servoMotor puxaria para diminuir 
// parcialmente o efeito da luz forte.
cortinaLux.write(valorLDR);
delay(200);  
}
