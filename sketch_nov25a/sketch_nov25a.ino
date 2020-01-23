#include "ledTable.h"
#include "images.h"
#include "yves_image.h"
#include "tetris.h" 
unsigned long timeNow = 0;
unsigned long timeLast = 0;

  int l=0;
int YO=0;
int Y0=-60;
int X0=-30;
int refresh=100;
String function="";
int lens=0;
String mess;  
int k2=0;
int k=0;
void setup() {
system_update_cpu_freq(SYS_CPU_160MHZ);
  Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);

  uint32_t realSize = ESP.getFlashChipRealSize();
uint32_t ideSize = ESP.getFlashChipSize();
FlashMode_t ideMode = ESP.getFlashChipMode();
Serial.printf("Flash real id: %08X\n", ESP.getFlashChipId());
Serial.printf("Flash real size: %u\n\n", realSize);
Serial.printf("Flash ide size: %u\n", ideSize);
Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
Serial.printf("Flash ide mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
if(ideSize != realSize) {
Serial.println("Flash Chip configuration wrong!\n");
} else {
Serial.println("Flash Chip configuration ok.\n");
}

  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
  //if (String(WiFi.SSID()) != String(ssid)) {
  Serial.printf("Connecting ");
  WiFi.begin("WiFi-2.4-E19C", "yvesyves");
  //}

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");
  // put your setup code here, to run once:
 /* SPIFFS.begin();

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
  }
  Serial.printf("\n");*/



  WiFi.mode(WIFI_STA);
  //Serial.printf("Connecting to %s\n", ssid);
  //if (String(WiFi.SSID()) != String(ssid)) {
  //Serial.printf("Connecting ");
  //WiFi.begin(ssid, password);
  //}

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");
 //if(mdns.begin("ledtable",WiFi.localIP()))
  //Serial.println("MDNS has started");


  server.on("/s", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("s"  );
   // char* r2=server.arg("s");
    refresh = r.toInt();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/meteo", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    if (function="tetris")
         timer0_detachInterrupt();
    function = "meteo";
    fill_solid(leds, NUM_LEDS, bgColor);
      // getmeteo();
            FastLED.show();
      FastLED.delay((int)(1000 / refresh) + 1);
       server.send(200, "text/html", "done");
  });


  server.on("/c", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    Color = CRGB(r.toInt(), g.toInt(), b.toInt());
    server.send(200, "text/html", "done");
    // i=0;
  });


  server.on("/c2", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    Color2 = CRGB(r.toInt(), g.toInt(), b.toInt());
    server.send(200, "text/html", "done");
    // i=0;
  });




  server.on("/changetext", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();

    String g = server.arg("v");
    Serial.println(g);
    //strcopy(g);
    mess = g;
    k2 = 0;
    Serial.println(mess);
    lens = mess.length();
    Serial.println(lens);
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/change", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String g = server.arg("v");
     Serial.println(g);
    if (g=="pacman")
    {
      if (function="tetris")
         timer0_detachInterrupt();
       YO=0;
       calculghosts(bgColor) ;
    calculfraise(bgColor);
  
     calculpic(cerise, palec, bgColor, 14, 14,cerisecalc);
   calculpic(mario, palm, bgColor, 14, 17,mariocalc);
   function = g;
    }
    if(g=="scroll")
    {
       if (function="tetris")
         timer0_detachInterrupt();
      YO=5;
      function = g;
    }
    if(g=="tetris")
    {
      function = g;
      Serial.println(function);
          fill_solid(leds, NUM_LEDS, bgColor);
         FastLED.show();
          
           interrupts();
          newGame();
           server.send(200, "text/html", "done");
          return;
    }
      
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/up", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    YO = (YO + 1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
    server.send(200, "text/html", "done");
    // i=0;
  });

  server.on("/down", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    YO = (YO - 1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
    server.send(200, "text/html", "done");
    // i=0;
  });


  server.on("/bg", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    bgColor = CRGB(r.toInt(), g.toInt(), b.toInt());
    calculghosts(bgColor) ;
    calculfraise(bgColor);
 calculpic(cerise, palec, bgColor, 14, 14,cerisecalc);
   calculpic(mario, palm, bgColor, 14, 17,mariocalc);
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });

  server.on("/n", HTTP_GET, []() {
    fill_solid(leds, NUM_LEDS, solidColor);
    FastLED.show();
    String r = server.arg("led");
    //NUM_LEDS_ECL = r.toInt();
    //i = 0;
  });

  /*server.on("/l", HTTP_GET, []() {
    fill_solid(leds, NUM_LEDS, solidColor);
    FastLED.show();
    String r = server.arg("l");
    sperpentLength = r.toInt();
    i = 0;
  });*/


  server.on("/b", HTTP_GET, []() {
    String b = server.arg("r");

    FastLED.setBrightness(b.toInt());

    Serial.println(" in your bright");
    FastLED.show();

    server.send(200, "text/html", "done");
  });


  /*server.on("/test", HTTP_GET, []() {
    String r = server.arg("r");
    Serial.println("r=" + r);
    String g = server.arg("g");
    String b = server.arg("b");
    solidColor = CRGB(r.toInt(), g.toInt(), b.toInt());


    Serial.println(" in your ");
    fill_solid(leds, sperpentLength, solidColor);

    FastLED.show();

    server.send(200, "text/html", "done");
  });*/
/*  server.serveStatic("/", SPIFFS, "/index.html");

  server.serveStatic("/index.html", SPIFFS, "/index.html");
  
  server.serveStatic("/jquery.js", SPIFFS, "/jquery.js");
  
  server.serveStatic("/farbtastic.js", SPIFFS, "/farbtastic.js");
    
  server.serveStatic("/farbtastic.css", SPIFFS, "/farbtastic.css");
    
  server.serveStatic("/marker.png", SPIFFS, "/marker.png");
  
  server.serveStatic("/mask.png", SPIFFS, "/mask.png");
  
  server.serveStatic("/wheel.png", SPIFFS, "/wheel.png");
    server.serveStatic("/test.html", SPIFFS, "/test.html");

 server.serveStatic("/left.jpg", SPIFFS, "/left.jpg");
 server.serveStatic("/right.jpg", SPIFFS, "/right.jpg");
 server.serveStatic("/turn_left.png", SPIFFS, "/turn_left.png");
 server.serveStatic("/turn_right.png", SPIFFS, "/turn_right.png");
  //server.serveStatic("/turn_right.png", SPIFFS, "/turn_right.png");
 server.serveStatic("/images.css", SPIFFS, "/images.css");*/
    
//MDNS.addService("http","tcp",80);
//MDNS.addService("ws", "tcp", 81);
  server.begin();
  server.handleClient();
          calculghosts(solidColor);
          calculfraise(solidColor);
        
        
          
        
          calculpic(cerise, palec, solidColor, 14, 14,cerisecalc);
           
      
fill(solidColor);
initTetrisScoketControl();
calculpic(cerise, palec, solidColor, 14, 14,cerisecalc);
 FastLED.show();
 delay(2000);
 fill(solidColor);
 FastLED.show();
if(initTable(30,20))
      {
            FastLED.setBrightness(32);
            fill(bgColor);
    
        randomSeed(analogRead(0));
            // displayBitmap(picyves2,X0,Y0,103,104);
          FastLED.show();
          //delay(5000);
          Serial.println("in her");
          //calculpic(mario, palm, solidColor, 14, 17,mariocalc);
       //   calculpic(mario, pall, solidColor, 14, 17,luigicalc);
         tableOrientation=DOWN_RIGHT_INV;
         displayNextPiecePos=DISP_UP;
         isDisplayScore=true;
         isDisplayNextPiece=true;
         TETRIS_HEIGHT=25;
         TETRIS_WIDTH=12;
         inGame=false;
        // initTetris(TETRIS_WIDTH,TETRIS_HEIGHT);
           
      }
}

void loop() {
server.handleClient();
int YO=0;
int Y1=15;
int offset = -15;
int offset2 = -27;
if(isTable)
      {
    if(!inGame)
    {
     
      for(k=-16;k<0;k++)
      {
        for(k2=0;k2<16;k2++)
        {
          fill( bgColor);
          Serial.println("we are are");
           Serial.println(k);
            Serial.println(k2);
         displayBitmap(mickey, k2,k,30,32);
                FastLED.show();
          FastLED.delay((int)(1000 / 1000) + 1);
        }
      }
      //fillScreenT();
      /*
          k = (k+1) % 300;
          
          timeNow=millis();
         fill(bgColor);
          
          displaypic(cerisecalc,k +offset, YO, 14, 14);
           //FastLED.show();
          displaypic(fraiseCalc, k + offset - 14 , YO + 2, 11, 10);
         displaypic(ghostred, k + offset - 30, YO, 14, 14);
          displaypic(ghostyellow, k + offset - 46, YO, 14, 14);
          displaypic(ghostgreen, k + offset - 62, YO, 14, 14);
          displaypic(ghostPurple, k + offset - 78, YO, 14, 14);
          displaypic(ghostCyan, k + offset - 94, YO, 14, 14);
          displaypic(mariocalc, k + offset-110 , YO, 14, 17);
              displaypic(luigicalc, k + offset-127 , YO, 14, 17);
              displayBitmap(picyves2, k + offset-127-15,YO,15,14);
               displayBitmap(lapin, k + offset-127-15-50,YO,30,23);
               displayBitmap(mickey, k + offset-127-15-90,YO,30,32);
                         displayBitmap(champi, k + offset-127-90-40,YO+3,16,16);

                     displaypic(cerisecalc,k +offset2, Y1, 14, 14);
           //FastLED.show();
          displaypic(fraiseCalc, k + offset2 - 14 , Y1 + 2, 11, 10);
         displaypic(ghostred, k + offset2 - 30, Y1, 14, 14);
          displaypic(ghostyellow, k + offset2 - 46, Y1, 14, 14);
          displaypic(ghostgreen, k + offset2 - 62, Y1, 14, 14);
          displaypic(ghostPurple, k + offset2 - 78, Y1, 14, 14);
          displaypic(ghostCyan, k + offset2 - 94, Y1, 14, 14);
          displaypic(mariocalc, k + offset2-110 , Y1, 14, 17);
              displaypic(luigicalc, k + offset2-127 , Y1, 14, 17);
              displayBitmap(picyves2, k + offset2-127-15,Y1,15,14);
  
          
     //             timeNow = millis()-timeNow;
      //  Serial.print("FPS:");
       // Serial.println((long)1000/timeNow);*/
 
        
      //delay(2000);
       //  fill(solidColor);
      //FastLED.show();
   
      
      //newGame();
      // executeTetrisSocketControl();
    }
    //else
      executeTetrisSocketControl();
     
  /*        timeNow = millis();
       for(int k=0;k<180;k++)
        {
          //k = k % 170;
          
          
         fill(solidColor);
          
          displaypic(cerisecalc,k +offset, YO, 14, 14);
           //FastLED.show();
          displaypic(fraiseCalc, k + offset - 14 , YO + 2, 11, 10);
         displaypic(ghostred, k + offset - 30, YO, 14, 14);
          displaypic(ghostyellow, k + offset - 46, YO, 14, 14);
          displaypic(ghostgreen, k + offset - 62, YO, 14, 14);
          displaypic(ghostPurple, k + offset - 78, YO, 14, 14);
          displaypic(ghostCyan, k + offset - 94, YO, 14, 14);
          displaypic(mariocalc, k + offset-110 , YO, 14, 17);
              displaypic(luigicalc, k + offset-127 , YO, 14, 17);
              displayBitmap(picyves2, k + offset-127-15,YO,15,14);
         // Serial.println("on rafraichit");
          FastLED.show();
          FastLED.delay((int)(1000 / 1000) + 1);
        }
        timeNow = millis()-timeNow;
        Serial.print("FPS:");
        Serial.println((long)180*1000/timeNow);
        
      tableOrientation++;
      tableOrientation=tableOrientation % 8;*/
    /*  displayBitmap(pixel_data,0,0,32,32);
      FastLED.show();
      FastLED.delay((int)(1000 / 100) + 1);
      delay(3000);
      displayBitmap(pixel_data,0,-16,32,32);
      FastLED.show();
      FastLED.delay((int)(1000 / 100) + 1);
      delay(3000);
       displayBitmap(pixel_data2,0,0,16,16);
       FastLED.show();
      FastLED.delay((int)(1000 / 100) + 1);
        delay(3000);*/

/*for(int i=-30;i<0;i++)
 for(int j=-84;j<0;j++)
 {
        fill(solidColor);
       if((-i)%2==0)
          Y0=-84-j;
         else
         Y0=j;
      // displayBitmap(picyves2,i,Y0,103,104);
       
       FastLED.show();
      FastLED.delay((int)(1000 / 100) + 1);
       // delay(3000);
 }*/
      
      }
}

