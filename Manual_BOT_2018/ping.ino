//#include <NewPing.h>
// 
//#define SONAR_NUM     2 // Number or sensors.
//#define MAX_DISTANCE 200 // Max distance in cm.
//#define PING_INTERVAL 33 // Milliseconds between pings.
// 
//unsigned long pingTimer[SONAR_NUM]; // When each pings.
//unsigned int cm[SONAR_NUM]; // Store ping distances.
//uint8_t currentSensor = 0; // Which sensor is active.
//int flag=1;
//
//
//NewPing sonar[SONAR_NUM] = { // Sensor object array.
//  NewPing(40, 38, MAX_DISTANCE),NewPing(48, 46, MAX_DISTANCE)//(triger,echo,distance);
//};
// 
//void usetup() {
//  Serial.begin(9600);
//  pingTimer[0] = millis() + 75; // First ping start in ms.
//  for (uint8_t i = 1; i < SONAR_NUM; i++)
//    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
//}
// 
//void uping() {
//  flag=1;
//
//  while(flag!=0)
//  {
//    for (uint8_t i = 0; i < SONAR_NUM; i++) {
//      if (millis() >= pingTimer[i]) {
//      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
//      if (i == 0 && currentSensor == SONAR_NUM - 1)
//        oneSensorCycle(); // Do something with results.
//      sonar[currentSensor].timer_stop();
//      currentSensor = i;
//      cm[currentSensor] = 0;
//      sonar[currentSensor].ping_timer(echoCheck);
//    }
//  }
//  // The rest of your code would go here.
//    paning();
//  }
//}
// 
//void echoCheck() { // If ping echo, set distance to array.
//  if (sonar[currentSensor].check_timer())
//    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
//}
// 
//void oneSensorCycle() { // Do something with the results.
//  for (uint8_t i = 0; i < SONAR_NUM; i++) {
//    Serial.print(i);
//    Serial.print("=");
//    Serial.print(cm[i]);
//    Serial.print("cm ");
//  }
//  Serial.println();
//}
//void paning()
//{
//  int x=cm[0]-cm[1];
//  if((x<-5))
//  {
//    clockwise(speedrun);
//    Serial.println("Clockwise");  
//  }
//    else if(x>5)
//    {
//      counterclockwise(speedrun);
//      Serial.println("CounterClockwise");
//    }
//    else
//    {
//      flag=0;
//      Serial.println("Stop");
//      stop_moving();
//    }
//    
//}
