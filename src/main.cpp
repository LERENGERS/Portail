//MICHEL Bastien #LERENGERS

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

extern void wifi_connect();
extern void PAGE_WEB(String servername);  

unsigned long timerDelay = 5000;

int etat=0;

Adafruit_MPU6050 mpu;

RTC_DATA_ATTR int conteur = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");

 // mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
 // Serial.print("Filter bandwidth set to: ");

  Serial.println("");
  delay(1000);

  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(false);
  mpu.setMotionInterrupt(true);
}

void loop() {
  if (conteur == 0){//debut machine a etat 
    conteur = 1;
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,HIGH);
    esp_deep_sleep_start();
  }
  else{
    if(mpu.getMotionInterruptStatus()) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      Serial.print("Acceleration X: ");
      Serial.print(a.acceleration.x);
      Serial.print(", Y: ");
      Serial.print(a.acceleration.y);

      Serial.print("Rotation X: ");
      Serial.print(g.gyro.x);
      Serial.print(", Y: ");
      Serial.print(g.gyro.y);

      Serial.print("Temperature: ");
      Serial.print(temp.temperature);
      Serial.println(" degC");

      Serial.println("");
      
      delay(1000);

      if(etat==0){
        if (a.acceleration.x<=5){

          wifi_connect();
          Serial.print("envoie");
          PAGE_WEB("https://eoqf9wfwq6wmj94.m.pipedream.net");
          etat=1;
          delay(1000);
        }
        else{
          esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,HIGH);
          esp_deep_sleep_start();
        }
      }
      if(etat==1){
        if (a.acceleration.x>=5){

          PAGE_WEB("https://eo7ahrk6u0far5o.m.pipedream.net");

          etat=0;

          esp_sleep_enable_ext0_wakeup(GPIO_NUM_27,HIGH);
          esp_deep_sleep_start();
        }
      }
    }
  }  
}