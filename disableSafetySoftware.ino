e// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// /////     __________________________________________________________________________________________________________________
// /////                                ABS SAFETY SOFTWARE DISABLE SWITCH FOR RACING BY JAKKA351
// /////                                    
// /////     __________________________________________________________________________________________________________________
// /////      |--------------------------------------------------------------------------------------------------------------|
// /////      |          https://github.com/jakka351  | jakka351@outlook.com |      https://facebook.com/testerPresent       |
// /////      |--------------------------------------------------------------------------------------------------------------|
// /////      | Copyright (c) 2022/2023 Jack Leighton 0434 645 485                                                           |          
// /////      | All rights reserved.                                                                                         |
// /////      |--------------------------------------------------------------------------------------------------------------|
// /////        Redistribution and use in source and binary forms, with or without modification, are permitted provided that
// /////        the following conditions are met:
// /////        1.    With the express written consent of the copyright holder.
// /////        2.    Redistributions of source code must retain the above copyright notice, this
// /////              list of conditions and the following disclaimer.
// /////        3.    Redistributions in binary form must reproduce the above copyright notice, this
// /////              list of conditions and the following disclaimer in the documentation and/or other
// /////              materials provided with the distribution.
// /////        4.    Neither the name of the organization nor the names of its contributors may be used t
// /////              endorse or promote products derived from this software without specific prior written permission.
// /////      _________________________________________________________________________________________________________________
// /////      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// /////      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// /////      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// /////      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// /////      SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// /////      WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// /////      USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// /////      _________________________________________________________________________________________________________________
// /////
// /////       This software can only be distributed with my written permission. It is for my own educational purposes and  
// /////       is potentially dangerous to ECU health and safety. Gracias a Gato Blancoford desde las alturas del mar de chelle.                                                        
// /////      _________________________________________________________________________________________________________________
// /////
// /////
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <mcp_can.h>
#include <mcp_can_dfs.h>
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////                     .__      ___.   .__                 
// //// ___  _______ _______|__|____ \_ |__ |  |   ____   ______
// //// \  \/ /\__  \\_  __ \  \__  \ | __ \|  | _/ __ \ /  ___/
// ////  \   /  / __ \|  | \/  |/ __ \| \_\ \  |_\  ___/ \___ \ 
// ////   \_/  (____  /__|  |__(____  /___  /____/\___  >____  >
// ////             \/              \/    \/          \/     \/
// //// 
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const int SPI_CS_PIN = 9;               // CAN Bus Shield
// const int SPI_CS_PIN = 17;              // CANBed V1
const int SPI_CS_PIN                 = 3;                  // CANBed M0
const int SPI_MCP2515_CS_PIN         = 10;                  // CANBed M0
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
MCP_CAN CAN1(SPI_MCP2515_CS_PIN);                                    // Set CS pin
long unsigned int canId;                         //
char mes[8]                          = { 0, 0, 0, 0, 0, 0, 0, 0 };
char msgString[128];                            // Array to store serial string
unsigned char len = 0;                          //
unsigned char rxBuf[8];                         //
// Define pin numbers
const int switchPin = 2;    // Pin connected to the switch
const int ledPin = 13;      // Pin connected to the LED
// Variable to store the switch state
int switchState = 0;
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////                __                
// ////   ______ _____/  |_ __ ________  
// ////  /  ___// __ \   __\  |  \____ \ 
// ////  \___ \\  ___/|  | |  |  /  |_> >
// //// /____  >\___  >__| |____/|   __/ 
// ////      \/     \/           |__|    
// //// 
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    // Set the switch pin as input with an internal pull-up resistor
    pinMode(switchPin, INPUT_PULLUP);
    // Set the LED pin as output
    pinMode(ledPin, OUTPUT);
    // Initialize LED to be off
    digitalWrite(ledPin, LOW);
    Serial.begin(115200);
    // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
    while (CAN_OK != CAN.begin(CAN_500KBPS))  // init can bus : baudrate = 500k  8MHZ crystal
    {
        Serial.println("[ ABS Safety Software Disable //  Error Initializing MCP2515. can0 ]");
        delay(100);
    }
    Serial.println("[ ABS Safety Software Disable // MCP2515 Initialized can0 up");  
    
    // ////////////////////////////////////////////////////////////////////////////
}
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////  .__                        a
// ////  |  |   ____   ____ ______  
// ////  |  |  /  _ \ /  _ \\____ \ 
// ////  |  |_(  <_> |  <_> )  |_> >
// ////  |____/\____/ \____/|   __/ 
// ////                     |__|    
// ////
void startTesterPresentTask()
{
    // Start 1 hour tester present task
    for (int i = 0; i <= 3600; i++)
    {
        unsigned char testerPresent[8] = {0x02, 0x3E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
        CAN.sendMsgBuf(0x760, 0, 8, testerPresent); // send testerPresent
        delay(1000);
    }
}
void disableSafetySoftware()
{
    for (int i = 0; i <= 5; i++)
    {
        unsigned char disableSafetySoftware[8] = {0x05, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        CAN.sendMsgBuf(0x760, 0, 8, disableSafetySoftware); 
    }
}
void returnToStandardState()
{
    unsigned char standardSession[8] = {0x022 0x10, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x760, 0, 8, standardSession); 
}

void loop()
{
    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                             .__                                   ___.                                                                              
    // _______   ____   ____  ____ |__|__  __ ____     ____ _____    ____\_ |__  __ __  ______   _____   ____   ______ ___________     ____   ____   ______
    // \_  __ \_/ __ \_/ ___\/ __ \|  \  \/ // __ \  _/ ___\\__  \  /    \| __ \|  |  \/  ___/  /     \_/ __ \ /  ___//  ___/\__  \   / ___\_/ __ \ /  ___/
    //  |  | \/\  ___/\  \__\  ___/|  |\   /\  ___/  \  \___ / __ \|   |  \ \_\ \  |  /\___ \  |  Y Y  \  ___/ \___ \ \___ \  / __ \_/ /_/  >  ___/ \___ \ 
    //  |__|    \___  >\___  >___  >__| \_/  \___  >  \___  >____  /___|  /___  /____//____  > |__|_|  /\___  >____  >____  >(____  /\___  / \___  >____  >
    //              \/     \/    \/              \/       \/     \/     \/    \/           \/        \/     \/     \/     \/      \//_____/      \/     \/     // 
    try
    {
        unsigned char buf[8];
        unsigned char len = 0;
        switchState = digitalRead(switchPin);
        if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
        {
        // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Read the state of the switch (LOW when pressed, HIGH when not pressed)
            if (switchState == LOW) 
            {
                bool disabled = true;
                // Switch is ON (button pressed), do something
                digitalWrite(ledPin, HIGH);  // Turn on the LED
                // NOQW SEND COMMAND TO DISABLE SAFETY SOFRTWARE // OPEN DIAGNOSTIC SESSION ?? SEND COMMAND
                disableSafetySoftware();
                attemptTwo:
                    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
                    Serial.println("Reading Response from Antilock Brake Module");
                    unsigned long canId = CAN.getCanId();        
                    if (canId == 0x768) 
                    {
                        switch((int)rxBuf[1])
                        {
                            case 0x50:
                                Serial.println("Positive Response to Disable Command, ABS is disabled for one hour");
                                //red flashing light
                                startTesterPresentTask();
                                break;
                            case 0x7F:
                                Serial.println("Negative Response recieved to command - ABS not disabled.");
                                //green flashing light
                                //try again
                                disableSafetySoftware();
                                goto attemptTwo
                                break;
                            default:
                                Serial.println("No Comms with ABS Module");
                                // Flashing Orange light
                                break;
                        }
                    }                
            } 
            else 
            {
                bool disabled = false;
                // Switch is OFF (button released), do something else
                digitalWrite(ledPin, LOW);   // Turn off the LED
                // Add any other code here to execute when the switch is OFF
                //flash green light to say ABS is on and functioning
                returnToStandardState();
                return;
            } 
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
