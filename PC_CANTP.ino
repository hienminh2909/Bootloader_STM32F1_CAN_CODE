#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS_PIN 10  // Chân CS cho MCP2515

MCP_CAN CAN(CAN_CS_PIN);

SoftwareSerial Serial1(3, 2); // RX=3, TX=2

uint8_t rxBuffer[9];
uint8_t byteCount = 0;
uint8_t tmp;
void setup() {
    Serial.begin(57600);    // Debug
    Serial1.begin(57600);   // Nhận dữ liệu từ PC
    Serial.println("Arduino ready to receive firmware...");
    
    while (CAN_OK != CAN.begin(CAN_STDID,CAN_500KBPS, MCP_8MHZ)) {
        Serial.println("CAN init failed, retrying...");
        delay(100);
    }
    
    CAN.setMode(MCP_NORMAL);
    
    Serial.println("CAN init OK!");
}

void loop() {
    if (Serial1.available() >0) {
        
        if(byteCount<8)
        {
        rxBuffer[byteCount] = Serial1.read();
        byteCount++;
        }

        if (byteCount >= 8) {
            while (Serial1.available()) {
               Serial1.read();
            }
            
            //* In dữ liệu dạng hex
            for (uint8_t i = 0; i < 8; i++) {
                if (rxBuffer[i] < 0x10) Serial.print("0");
                Serial.print(rxBuffer[i], HEX);
            }
            byteCount = 0;
            Serial.println();

            // Gửi ACK về PC
            //Serial1.println("OK");
            
            Serial.println("Sent CAN message: ID 0x124");
            
            CAN.sendMsgBuf(0x7E0, 0, 8, rxBuffer); // 0 = Standard ID
            //Serial.print("OK");
             // reset để nhận gói tiếp
        }
    }
    
    uint32_t rxId;
    uint8_t  len = 0;
    uint8_t rxBuf[8];
    
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        CAN.readMsgBuf(&rxId, &len, rxBuf); // Đọc ID, độ dài và dữ liệu
        Serial.print("Received CAN message: ID 0x");
        Serial.print(rxId, HEX);
        Serial.print(" Data: ");
        if(rxId = 0x7E8){
        for (int i = 0; i < len; i++) {
            Serial1.write(rxBuf[i]);
            Serial.print("0x");
            Serial.print(rxBuf[i], HEX);
            Serial.print(" ");
        }
        }
        Serial.println();
        /*
        if(rxBuf[0]==0x79)
         {
          //Serial1.println("OK");
          Serial1.write(rxBuf[0]);
          Serial.println(rxBuf[0],HEX);

        }*/ 

    }


    
}
