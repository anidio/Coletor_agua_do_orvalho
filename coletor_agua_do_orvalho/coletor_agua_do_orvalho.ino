//biblioteca do sensor de temperatura interno DS18B20
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>

//biblioteca do sensor de umidade e temperatura
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h> 

//clock de data e hora
#include "RTClib.h"
RTC_DS3231 rtc;

//declaração das váriavéis da semana em PT e EN
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"}; //Dias da semana
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

//definição dos pinos do arduino
#define DHTPIN 5 
#define ONE_WIRE_BUS 2
#define DHTTYPE DHT22 
#define PINO_RELE1 4
#define PINO_RELE2 7

// Inicializa Sensor DHT, 16mhz
DHT dht(DHTPIN, DHTTYPE); 

// Prepara uma instância oneWire para comunicar com qualquer outro dispositivo oneWire
OneWire oneWire(ONE_WIRE_BUS);

// Passa uma referência oneWire para a biblioteca DallasTemperature
DallasTemperature sensors(&oneWire);

float hum; //Armazena valores de umidade
float temp; //Armazena valores de temperatura
uint32_t timer = 0;

// constantes do relê     
const int HORA = 8;
const int HORA1 = 15;
const int HORA2 = 9;
const int HORA3 =16;

      
void setup()

{ Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.print("DS3231 não encontrado");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.print("DS3231 OK!");

    //USE AS DUAS LINHAS COMENTADAS ABAIXO QUANDO FOR NECESSÁRIO ATUALIZAR A DATA OU A HORA DO rtc
    
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2021,05,07, 15,02, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(2000);

  Serial.begin(9600);
  //Serial.println("CÁLCULO DO PONTO DE ORVALHO");
  //Serial.println("\n");
  Serial.begin(9600);
  dht.begin();

 pinMode(PINO_RELE1, OUTPUT);
 digitalWrite(PINO_RELE1, LOW);
 pinMode(PINO_RELE2, OUTPUT);
 digitalWrite(PINO_RELE2, LOW);
}

void loop() {

if (millis() - timer >= 5000) {
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
    //Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" ");

    //Serial.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    Serial.print(" "); //QUEBRA DE LINHA NA SERIAL

    // Manda comando para ler temperaturas
    sensors.requestTemperatures();


    //Serial.print("Umidade: ");
    Serial.print(hum);
    Serial.print(" ");

    //Serial.print(" %, Temperatura: ");
    Serial.print(temp);
    Serial.print(" ");


    //Serial.print("Ponto de Orvalho = ");
    Serial.print(dewPoint(temp, hum), 1);
    Serial.print(" ");

    // Escreve a temperatura em Celsius
    //Serial.print("Temperatura: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.print("\n");


    //Serial.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA

    delay(150000);





      if ((now.hour() >= HORA) && (now.hour() < HORA1)) {  //se no instante que hora atual for igual a hora da variavel
    digitalWrite(PINO_RELE2, LOW);                             //aciona rele
  } else {                                                     //senao
    digitalWrite(PINO_RELE2, HIGH);                              //desaciona o rele
  }

       if ((now.hour() >= HORA2) && (now.hour() < HORA3)) {  //se no instante que hora atual for igual a hora da variavel
    digitalWrite(PINO_RELE1,LOW);                             //aciona rele
  } else {                                                     //senao
    digitalWrite(PINO_RELE1,HIGH);                              //desaciona o rele
  }

  }


}
double dewPoint(double celsius, double humidity) {
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}
