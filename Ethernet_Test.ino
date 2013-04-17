#include <SPI.h>
#include <Ethernet.h>

/*
Arduino program to test Ethernet shield
tested using Arduino v1.0.4 IDE
Arduino UNO R3
Arduino Ethernet Shield R3

This is a combination of code examples found in the Arduino IDE and on the arduino.cc forums 
along with original coding by John Burden
*/

boolean printdebug = true;          //enable debug messages

boolean WebBackground = false;             //web page background status flag

// Ethernet Settings are here
String EtherinputString = "";

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xDF, 0x08 };          // MAC address for ethernet board
IPAddress ip(192,168,1,30);         // ethernet board
IPAddress mask(255,255,255,0);      // network mask
IPAddress gateway(192,168,1,1);     // default gateway
IPAddress dnsserver(192,168,1,1);         // dns server
int EtherPort = 80;

EthernetServer server(80);

void setup()
{
  // start the Ethernet connection and the server:
  EtherinputString.reserve(200);
  Ethernet.begin(mac, ip, dnsserver, gateway, mask);
  server.begin();
  
  Serial.begin(9600);               // initialize serial:
  Serial.println(F("setup complete"));
}

void loop() {
  Ether();                          // Process requests to Web Server
}


void Ether(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        
        if (EtherinputString.length() < 100)       //read char by char HTTP request
           {
             EtherinputString += c;                //store characters to string
           } 

        if (c == '\n')                             //if HTTP request has ended
          {      

            if (printdebug){Serial.print(EtherinputString);}      //print to serial monitor for debuging 
            

            if(EtherinputString.indexOf("togbg") >0)     //checks for toggle
              {
                if (WebBackground)
                  {
                    WebBackground = false;
                  }
                  else
                  {
                    WebBackground = true;
                  } 
              }


                  client.println(F("HTTP/1.1 200 OK")); //send new page
                  client.println(F("Content-Type: text/html"));
                  client.println();
                  client.println();
                  
                  if (WebBackground)
                    {  
                      client.print(F("<body style=background-color:WHITE>")); //set background to WHITE
                    }
                    else
                    {  
                      client.print(F("<body style=background-color:BLACK>")); //set background to BLACK
                    }
                  
//                  client.println(F("<META HTTP-EQUIV=REFRESH CONTENT=20 URL=>"));         
                  client.println(F("<HTML>"));  
                  client.print(F("<HEAD><TITLE>Control</TITLE>"));  

                  client.println(F("<center>"));
                  client.println(F("<font color=’green’><h1>Arduino Ethernet Test</font>"));//send first heading
                  client.println(F("</center>"));                  

                  client.println(F("<center>"));
                  client.println(F("<hr />"));                  
                  client.print(F("<form method=get name=Background>"));
                  client.println(F("<button name=comtogbg value=0 type=submit style=font-weight:bold;color:red;height:50px;width:200px>Background Toggle</button>"));
                  client.println(F("</center>"));  
                  
                  client.println(F("<hr />"));  // print a header on webpage ---------------------
                
                  client.println(F("</body></html>"));
             
                  //stopping client
                  client.stop();

                  //clearing string for next read
                  EtherinputString="";

                }
      } // end if client.available
      } // end while client.connected
      } // end if (client)
      } // end Ether()



