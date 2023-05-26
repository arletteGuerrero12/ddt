#include <WiFi.h>
String sensorD="HOLA MUNDO";
int ledPinY = 2;
int ledPinX = 4;
int SensorY = 12;
int SensorX = 13;
int bocina=25;
int cont=0, cont2=0;
int temX,tiempo,tiempo2,temY;
int mini=5;
int maxi=100;

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------
const char* ssid     = "Brayangf";
const char* password = "brayangf";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

String header; // Variable para guardar el HTTP request

//------------------------CODIGO HTML------------------------------
String paginaInicio = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<META HTTP-EQUIV='Refresh' CONTENT='5'>"
"<title>succionamelonline</title>"
"</head>"
"<body>"
    "<h1>Tiempo:------</h1>"
    "<h1>Conteos</h1>"
    "<h1>Minimo</h1>"
    "<h1>Maximo</h1>"
"</body>"


"<body>"
"<body style='background= color: WRITE;'>"
"<center>"
"<font color='black' size='10px'><b> DETECTOR<b></font><br/>"
"<br/>"
"<font color='red'size='10px'><i>HHH!<i></font><br/>"
"<font color= 'BLUE' size='30px'><i>";

String paginaFin = "<i></font><br/>"
"</center>"
"</body>"
"</html>";


//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(ledPinX, OUTPUT);
     pinMode(ledPinY, OUTPUT);
    pinMode(SensorX, INPUT); 
    pinMode(SensorY, INPUT);
    
  
  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop(){
  sensor();
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {    
    sensor();// Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                     
            // Muestra la página web
            client.println(paginaInicio +int (cont)+ paginaFin);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
 
}

void sensor(){
  temX=digitalRead(SensorX);
  temY=digitalRead(SensorY);
  if (temX==1 ||temY==1) {
    if(temX==1){
    digitalWrite(ledPinX, HIGH);
   
 cont++;
    delay(100);


    if(temX==0){
       digitalWrite(ledPinX, LOW);
 imprimir();
      }
  }
  else
  {
 digitalWrite(ledPinY, HIGH);
   
 cont2++;
    delay(100);
    if(temY==0){
       digitalWrite(ledPinY, LOW);
 imprimir();
      }
 

  }
  }
  else{
    if (temX==1 && temY==1) {
    digitalWrite(ledPinY, HIGH);
   digitalWrite(ledPinX, HIGH);
 cont2++;
 cont++;
    delay(100);
  }
 if (temX==0 && temY==0) {
    digitalWrite(ledPinY, LOW);
   digitalWrite(ledPinX, LOW);
 cont2=0;
 cont=0;
    delay(100);
  }
  
    }
  
}
    
  
  void imprimir(){
  if(cont>5 && cont<maxi){
    Serial.println(cont);
    cont=0;    
   
    tiempo2=tiempo2*100;
    Serial.println("El tiempo en X es: "+tiempo2);
    }

    if(cont2>mini && cont2<maxi){
    Serial.println(cont2);
    cont2=0;
    tiempo=cont2*100;
    Serial.println("El tiempo en Y es: "+tiempo);
    
    }
    
  }
