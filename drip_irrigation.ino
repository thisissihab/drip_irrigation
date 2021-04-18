#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "DeepWeb"
#define STAPSK  "77fai44sal22"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String str1 = "1011", str2 = "1015" , str3 = "1009", str4 = "1020";
String s1 = "1011", s2 = "980", s3 = "878", s4 = "550", s5 = "448", s6 = "610";
String report1 ="Condition: Well", report4 = "Condition: Water Needed Urgently", report5 = "Condition: Water Needed Urgently", report6 = "Condition: Water Needed but Not Urgent";
String report2 = report1;
String report3 = report2;
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /l1/on") >= 0) {
              Serial.println("Pump Connection 1 is On"); //
            } else if (header.indexOf("GET /l1/off") >= 0) {
              Serial.println("Pump Connection 1 is Off");
            }
            else if (header.indexOf("GET /l2/on") >= 0) {
              Serial.println("Pump Connection 2 is On");
            } 
            else if (header.indexOf("GET /l2/off") >= 0) {
              Serial.println("Pump Connection 2 is Off");
            
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body style=\"font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif\">");

            //image
            client.println("<div class=\"container\"><h1 style=\"background: #0a043c; color: #ffe3de; text-align: center; padding:10px\">Condition Monitoring system</h1></div><div class=\"container\"><div><img src=\"https://cdn.dnaindia.com/sites/default/files/styles/full/public/2018/10/23/746858-irrigation-drip-thinkstock-102418.jpg\" style=\"display: block; margin-left: auto; margin-right: auto; width: 100%; height: 50%; margin-bottom: 15px;\"></div>");
            // border 1
            client.println("<div style=\"border-radius: 5px;border: 1px solid #232f34; display: flex; margin-bottom: 10px;\"><h3 style=\"width:80%; color:#232f34; padding: 5px 0px 0px 10px;\">Land 1 Sensor Data</h1><a href=\"/l1/on\" class=\"btn btn-outline-success \" style=\"float: right; margin: 4px;\">Pump ON</a><a href=\"/l1/off\" class=\"btn btn-outline-danger \" style=\"float: right; margin: 4px;\">Pump OFF</a></div>");
            //land 1 sensor data
            client.println("<div class=\"block1\"><div class=\"row\">");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 1</h5><h1 style=\"text-align: center; padding: 10px;\">"+s1+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report1+"</h5></div></div></div>");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 2</h5><h1 style=\"text-align: center; padding: 10px;\">"+s2+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report2+"</h5></div></div></div>");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 3</h5><h1 style=\"text-align: center; padding: 10px;\">"+s3+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report3+"</h5></div></div></div>");
            client.println("</div></div>");
            
            client.println("<div class=\"block2\"><div class=\"row\" style=\"padding: 5px;\"></div></div>");
            
            client.println("<div style=\"border-radius: 5px;border: 1px solid #232f34; display: flex; margin-top: 25px; margin-bottom: 10px;\"><h3 style=\"width:80%; color:#232f34; padding: 5px 0px 0px 10px;\">Land 2 Sensor Data</h1><a href=\"/l2/on\" class=\"btn btn-outline-success \" style=\"float: right; margin: 4px;\">Pump ON</a><a href=\"/l2/off\" class=\"btn btn-outline-danger \" style=\"float: right; margin: 4px;\">Pump OFF</a></div>");
            
            client.println("<div class=\"block3\" style=\"margin-bottom: 20px\"><div class=\"row\">");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 1</h5><h1 style=\"text-align: center; padding: 10px;\">"+s4+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report4+"</h5></div></div></div>");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 2</h5><h1 style=\"text-align: center; padding: 10px;\">"+s5+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report5+"</h5></div></div></div>");
            client.println("<div class=\"col-sm-4\"><div class=\"card border-dark\"><div class=\"card-body\"><h5 class=\"card-title\" style=\"text-align:center; letter-spacing: 8px; font-size: 26px; background: #232f34; color: #ffe3de; padding: 2px;\">Sensor 3</h5><h1 style=\"text-align: center; padding: 10px;\">"+s6+"</h1><h5 style=\"text-align: center; padding-top:10px;\">"+report6+"</h5></div></div></div>");
            client.println("</div></div></div>");

            
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
