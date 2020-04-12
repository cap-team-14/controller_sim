#include <Arduino.h>
#include <mcp_can_stm.h>


bool led_status = 0;

/* #region  CAN Bus */
#define MCP_CS PB12
#define MCP_INT PB11
// CAN bus def
MCP_CAN CAN;
unsigned long int last_can_update = 0;	// last time a CAN message was received
int can_timeout = 5000;					// time without CAN message before error occurs
uint16_t transmit_ID = 0x86;	// CAN Bus transmit ID (messages will be sent with this ID)
uint32_t recv_id = 0x80;		// CAN Bus receive ID (messages with this ID will be read)
uint8_t recv_len;				// length of received message
uint8_t recv_msg[9];			// the message (4 bytes)
uint8_t buff [8];	            // message to be sent
/* #endregion */

/* #region  Function Prototypes */
void can_struct();
void can_send();
void can_check();
void can_process();
void calibrate();

/* #endregion */

void setup() {
    pinMode(PC13, OUTPUT);
    if(!(CAN_OK == CAN.begin(CAN_1000KBPS, MCP_CS)))
	{
		// warning = 0;
	}

	CAN.init_Filt(0, 0, recv_id);
}

void loop() {
    // check for CAN Comms error and process any waiting CAN messages
    if(CAN_MSGAVAIL == CAN.checkReceive()){
        CAN.readMsgBuf(&recv_len, recv_msg);
        Serial.println(recv_msg[0]);
        // MCP2515_ISR();
        // last_can_update = c_time;
        // warning = -1;
    }
    led_status = !led_status;
    digitalWrite(PC13, led_status);
    delay(1000);

    buff[0] = 0b10000000 | 98;
    buff[1] = 0b00000000;

    CAN.sendMsgBuf(transmit_ID, 0, 2, buff);
}


// void console_input(){
//     if (Serial.available()){
//         char c = Serial.read();
//         switch(c){
//             case 'c':
//             calibrate();
//             break;

//             case 'r':
//             Serial.print("Enter desired position: ");
//             while (!Serial.available()){}
//             int temp = Serial.parseInt();
//             if(temp <= 100 && temp >= 0){
//                 pid_sp = temp;
//                 Serial.print("\nGoing to position: "); Serial.println(pid_sp);
//             } else {
//                 Serial.println("Entered value outside limits");
//             }
//             break;

//             case 'k':
//             char quit = 0;
//             while ('q' != quit){
//                 Serial.println("Current params:");
//                 Serial.print("\tKp: "); Serial.println(Kp, 6);
//                 Serial.print("\tKi: "); Serial.println(Ki, 6);
//                 Serial.print("\tKd: "); Serial.println(Kd, 6);
//                 Serial.println("Enter \"p\" to set Kp");
//                 Serial.println("Enter \"i\" to set Ki");
//                 Serial.println("Enter \"d\" to set Kd");
//                 Serial.println("Enter \"q\" to return to normal operation");
//                 while (!Serial.available()){}
//                 char c = Serial.read();
//                 if ('p' == c){
//                     Serial.println("Enter new Kp: ");
//                     while (!Serial.available()){}
//                     Kp = Serial.parseFloat();
//                 } else if('i' == c){
//                     Serial.println("Enter new Ki: ");
//                     while (!Serial.available()){}
//                     Ki = Serial.parseFloat();
//                 } else if('d' == c){
//                     Serial.println("Enter new Kd: ");
//                     while (!Serial.available()){}
//                     Kd = Serial.parseFloat();
//                 } else if('q' == c){
//                     Serial.println("Setting params to: ");
//                     Serial.print("\tKp: "); Serial.println(Kp);
//                     Serial.print("\tKi: "); Serial.println(Ki);
//                     Serial.print("\tKd: "); Serial.println(Kd);

//                     pid.SetTunings(Kp, Ki, Kd);
//                     quit = 'q';
//                 }
//             }
//             break;

//             case 's':
//             Serial.println("STOPPING MOTOR");
//             while(true){
//                 // TODO: Send motor stop command
//                 while (!Serial.available()){}
//                 char c = Serial.read();
//                 if ('s' == c){
//                     Serial.println("RESUMING MOTOR");
//                     break;
//                 }
//             }
//             break;
//         }
//     }
// }


        // } else if ('i' == c){
        //     pwmWrite(pin_esc, td(0));
        //     Serial.println("Send input.");
        //     Serial.println("Enter \"s\" for step input.");
        //     while(true){
        //         while (!Serial.available()){}
        //         char c = Serial.read();
        //         if ('q' == c){
        //             Serial.println("Returning to normal operation.");
        //             break;
        //         } else if('s' == c){
        //             Serial.println("Enter Start Value and End Value:");
        //             while (!Serial.available()){}
        //             int8_t step [2] = {0, 0};
        //             for(uint8_t i=0; i<2; i++){
        //                 int c = Serial.parseInt();
        //                 if (c < 0 || c > 100){
        //                     Serial.println("Invalid value detected, returning to normal operation.");
        //                     break;
        //                 }
        //                 step[i] = c;
        //                 while (!Serial.available()){}
        //             }
        //             Serial.print("Step input starting at "); Serial.print(step[0]) ; Serial.print(" and ending at ");
        //             Serial.println(step[1]);
        //             Serial.print("Setting motor throttle to: "); Serial.println(step[0]);
        //             pwmWrite(pin_esc, td(step[0]));
        //             //pid_sp = step[0];
        //             Operation op;
        //             op.delay = 300;
        //             op.e_time = millis();
        //             op.position = step[1];

        //             if(last_command == 9){
        //                 command_queue[0] = op;
        //             } else {
        //                 command_queue[++last_command] = op;
        //             }
        //             break;
        //         }
        //     }


void calibrate(){
    Serial.println("Calibration");
    bool quit = 0;
    while (!quit){
        Serial.println("Enter \"0\" to set zero point or \"1\" to set fully open point");
        Serial.println("Enter \"q\" to return to normal operation");
        while (!Serial.available()){}
        char c = Serial.read();
        switch(c){
            case '0':
            // send 0 zero case
            break;

            case '1':
            // send 100% case
            break;

            case 'q':
            quit = 1;
            Serial.println("Exiting Calibration Mode");
            break;
        }
    }
}