
//Progam sources data from a sensor attached to an Ardunio with an Ethernet Shield, posts the results to a webserver that stores the results in a database.

#include <SPI.h>
#include <Ethernet.h>

//Sensor Setup
#include <DHT.h>
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
const int chipSelect = 4;

//Ethernet Setup - Using DHCP, server address is resovled via DNS
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
char server[] = "arduinodata.scienceontheweb.net";    //mysql + webserver
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
  for(;;)
      ;
  }
  // print your local IP address:
    Serial.print("IP Address: ");
    Serial.println(Ethernet.localIP());
   
  // give the Ethernet shield a second to initialize:
  delay(5000);
}

void loop()
{
    //Populating variables   
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    //Post results to Serial
    if (client.connect(server, 80)) {
    Serial.print("Connected: Temperature: ");
    Serial.print(t);
    Serial.print("c | Moisture=: ");
    Serial.print(h);
    Serial.println("%");
    
    
    // Make a HTTP request:
    client.print("GET /add.php?Temp1=");
    client.print(t);
    client.print("&Moi1=");
    client.print(h);
    client.println (" HTTP/1.1");
    client.println("Host: arduinodata.scienceontheweb.net");
    client.println("Connection: close");
    client.println();
    client.stop();

    //Clear Memory Buffers & delay 5mins (1000Ms x 60secs x 5mins = 300000)
    Serial.flush();
    client.flush();
    delay(300000);
  } 
  else {
    Serial.println("connection failed");
    delay(300000);
  }

}
