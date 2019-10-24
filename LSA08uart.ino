
const byte UART_serial_enable = 24;
const byte Junction_Pulse = 2;

void setup()
{
  pinMode(UART_serial_enable, OUTPUT);
  digitalWrite(UART_serial_enable, HIGH);
  pinMode(Junction_Pulse, INPUT);
  Serial3.begin(9600);                          //For LSA08 to Arduino communication (Mega only)
  Serial.begin(9600);                           //For comp to Arduino communication
} 

void loop()
{
  byte dummy = 0;                               //Dummy variable to read input from LSA08
  digitalWrite(UART_serial_enable, LOW);        //Enable pin set LOW to request data from LSA08
  delay(10);                                    
  
    while (Serial3.available() <= 0)              //Check whether data from LSA08 is available
   { 
     dummy = Serial3.read();                     //Read data from LSA08
     Serial.println(dummy);
   }
   
   if (digitalRead(Junction_Pulse) == HIGH)      //Check whether the junction pin is HIGH
   {
    Serial.println("Junction");
   }
  
  digitalWrite(UART_serial_enable, HIGH);       //Enable pin set HIGH to stop requesting data from LSA08
  delay(10);
}
