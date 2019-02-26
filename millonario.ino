#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532Interface.h>
#include <PN532.h>

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);


void setup()
{    
    Serial.begin(115200);
    Serial.println("-------Peer to Peer HCE--------");
    
    nfc.begin();
    
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
    
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    //nfc.setPassiveActivationRetries(0xFF);
    
    // configure board to read RFID tags
    nfc.SAMConfig();
}

void loop()
{
  bool success;
  int m, n;
  
  Serial.println("Esperando (ISO14443A)");
  
  // set shield to inListPassiveTarget
  success = nfc.inListPassiveTarget();

  if(success) {
   
     Serial.println("Encontrada!");
     uint8_t response1[255];
     uint8_t responseLength1 = 255;
     uint8_t response2[255];
     uint8_t responseLength2 = 255;
     uint8_t response3[255];
     uint8_t responseLength3 = 255;
     uint8_t response4[255];
     uint8_t responseLength4 = 255;

///////////// LLAMADA 1                                        

    //uint8_t selectApdu1[] = {0x00, 0xa4, 0x04, 0x00, 0x0e, 0x31, 0x50, 0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
    uint8_t selectApdu1[] = { 0x00, 0xa4, 0x04, 0x00, 0x0e, 0x32, 0x50, 0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
    Serial.print("ENVÍO 1: "); nfc.PrintHexChar(selectApdu1, sizeof(selectApdu1));    
    success = nfc.inDataExchange(selectApdu1, sizeof(selectApdu1), response1, &responseLength1);
    
    if(success) {
                Serial.print("RESPUESTA 1: "); Serial.println(responseLength1);
                nfc.PrintHexChar(response1, responseLength1);
      
///////////// LLAMADA 2

               uint8_t selectApdu2[] = {0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10, 0x00};
               Serial.print("ENVÍO 2: "); nfc.PrintHexChar(selectApdu2, sizeof(selectApdu2));    
               success = nfc.inDataExchange(selectApdu2, sizeof(selectApdu2), response2, &responseLength2);
    
               if(success) {
                           Serial.print("RESPUESTA 2: "); Serial.println(responseLength2);
                           nfc.PrintHexChar(response2, responseLength2);

//////////// LLAMADA 3

                           //uint8_t selectApdu3[] = {0x80, 0xa8, 0x00, 0xB2, 0x01, 0x8C, 0x00, 0x00};
                           //uint8_t selectApdu3[] = {0x00, 0xb2, 0x01, 0x0c, 0x00}; /// PAN
                           uint8_t selectApdu3[] = {0x00, 0xb2, 0x01, 0x0c, 0x00}; 
                           /*                             
                              Serial.print("ENVÍO "); nfc.PrintHexChar(selectApdu3, sizeof(selectApdu3));    
                                         success = nfc.inDataExchange(selectApdu3, sizeof(selectApdu3), response3, &responseLength3);
    
                                         if(success) {
                                               Serial.print("RESPUESTA -->> "); Serial.println(responseLength3);
                                               nfc.PrintHexChar(response3, responseLength3);}        */                    
                                               
                                               
                            /*                   
                              uint8_t selectApdu4[] = {0x00, 0xb2, 0x04, 0x14, 0x00}; 
                                                        
                              Serial.print("ENVÍO "); nfc.PrintHexChar(selectApdu4, sizeof(selectApdu4));    
                                         success = nfc.inDataExchange(selectApdu4, sizeof(selectApdu4), response4, &responseLength4);
    
                                         if(success) {
                                               Serial.print("RESPUESTA -->> "); Serial.println(responseLength4);
                                               nfc.PrintHexChar(response4, responseLength4);}       */                    

                           for ( uint8_t m = 0x01 ; m <= 0xfe ; m++)
                           {                                    
                                    selectApdu3[2] = m;
                                    for (uint8_t n = 0x01 ; n <= 0xfe; n++)
                                    {
                                         selectApdu3[3] = n;
                                         //delay(1000);
                                         Serial.print("ENVÍO ("); Serial.print(m); Serial.print(" - "); Serial.print(n); Serial.print(") -->> "); nfc.PrintHexChar(selectApdu3, sizeof(selectApdu3));    
                                         success = nfc.inDataExchange(selectApdu3, sizeof(selectApdu3), response3, &responseLength3);
    
                                         if(success)
                                         {
                                               Serial.print("RESPUESTA -->> "); Serial.print(responseLength3); Serial.print(" bytes -->> ");
                                               if (responseLength3 > 10)
                                               {
                                                   nfc.PrintHexChar(response3, responseLength3);
                                               }
                                         }
                                         else
                                         { 
                                             Serial.println("No satisfactorio"); 
                                         }
                                         memset(&response3,255,300);
                                         responseLength3 = sizeof(response3);

                                   }
                          }
                          Serial.print("Fin del FOR");
                
/*
//////////// LLAMADA 4

                                       uint8_t selectApdu4[] = {0x40, 0x01, 0x00, 0x00, 0xB2, 0x01, 0x12, 0x00};
                                       Serial.print("ENVÍO 4: "); nfc.PrintHexChar(selectApdu4, sizeof(selectApdu4));    
                                       success = nfc.inDataExchange(selectApdu4, sizeof(selectApdu4), response4, &responseLength4);
    
                                       if(success) {
                                                   Serial.print("RESPUESTA 4: "); Serial.println(responseLength4);
                                                   nfc.PrintHexChar(response4, responseLength4);
                           }*/
                           
                           
                           }

    }
    else {
             Serial.println("Failed sending SELECT AID"); 
         }
  }
  else {
   
    Serial.println("Didn't find anything!");
  }
  Serial.println("FIN!");
  delay(10000);
}

void printResponse(uint8_t *response, uint8_t responseLength) {
  
   String respBuffer;

    for (int i = 0; i < responseLength; i++) {
      
      if (response[i] < 0x10) 
        respBuffer = respBuffer + "0"; //Adds leading zeros if hex value is smaller than 0x10
      
      respBuffer = respBuffer + String(response[i], HEX) + " ";                        
    }

    Serial.print("response: "); Serial.println(respBuffer);
}

void setupNFC() {
 
  nfc.begin();
    
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig(); 
}
