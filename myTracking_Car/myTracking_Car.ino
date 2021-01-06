//*******************************************************************************
//
//    本程式僅供學習使用，未經作者允許，不得用於其他用途。 版權所有，仿冒必究！
//    1.開發環境：Arduino IDE
//    2.測試使用開發板型號：Arduino UNO R3
//    3.網址：http://store.ruten.com.tw/yuhhann_online
//    4.e-mail：yuhhann.studio@gmail.com、yuhhann.studio@yahoo.com
//    5.FB 粉絲專頁：https://www.facebook.com/yuhhann.studio
//    6.LINE ID：yuhhann.studio
//    7.作者：鈺瀚工作室
//
//    第2節：智能車循線實驗
//
//*******************************************************************************
#include "TRSensors.h"
// L298N 馬達驅動板
#define MotorR_I1     A1  //定義 I1 接腳
#define MotorR_I2     A0 //定義 I2 接腳
#define MotorL_I3    A2  //定義 I3 接腳
#define MotorL_I4    A3  //定義 I4 接腳
#define MotorR_ENA    5  //定義 ENA (PWM調速) 接腳
#define MotorL_ENB    6  //定義 ENB (PWM調速) 接腳
#define NUM_SENSORS 5
int SPEED_R  =     64; //定義右輪速度
int SPEED_L  =     64; //定義左輪速度

// 循線模組
#define SensorLeft    A0  //定義 左感測器 輸入腳
#define SensorMiddle  A1  //定義 中感測器 輸入腳
#define SensorRight   A2  //定義 右感測器 輸入腳
int off_track = 0;        //宣告變數

TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];

void setup()              //設定腳位
{
  Serial.begin(9600);

  pinMode(MotorR_I1,  OUTPUT);
  pinMode(MotorR_I2,  OUTPUT);
  pinMode(MotorL_I3,  OUTPUT);
  pinMode(MotorL_I4,  OUTPUT);
  pinMode(MotorR_ENA, OUTPUT);
  pinMode(MotorL_ENB, OUTPUT);

  pinMode(SensorLeft,   INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight,  INPUT);
}

void advance(int a)    // 前進
{
    digitalWrite(MotorR_I1,LOW);   //馬達（右）順時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,LOW);   //馬達（左）逆時針轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(a * 100);
}

void turnR(int d)    //右轉
{
    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorL_I4,LOW);
    delay(d * 100);
}

void turnL(int e)    //左轉
{
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
    digitalWrite(MotorR_I2,LOW);
    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(e * 100);
}

void stopRL(int f)  //停止
{
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）停止轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）停止轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(f * 100);
}

void back(int g)    //後退
{
    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(g * 100);
}

void loop()
{
  // 讀取感測器狀態
  trs.AnalogRead(sensorValues);
  
  int SR = sensorValues[4];
  int SRM = sensorValues[3];
  int SM = sensorValues[2];
  int SLM = sensorValues[1];
  int SL = sensorValues[0];
  if((SM > 450) && (SL > 450) && (SR > 450)&& (SLM > 450) && (SRM > 450))  // 小車脫離線
  {
    analogWrite(MotorR_ENA, SPEED_R);    //設定馬達 (右) 轉速
    analogWrite(MotorL_ENB, SPEED_L);    //設定馬達 (左) 轉速
    // 小車有時會因為循線感測器誤判或黑線轉角太大, 認為脫離黑線了而停車
    // 加上尋回黑線機制, 避免小車誤動作
    // 您可以修改程式讓 循線/尋線 機制更完美
    if(off_track < 3)
    {
      switch(off_track)
      {
        case 0: back(1); break;
        case 1: turnR(1); break;
        case 2: turnL(2); break;
      }
      off_track++;
    }
    else
    {
      stopRL(0);
    }
  }
  else
  {
    off_track = 0;

    if(SM < 450)  //中感測器在黑色區域
    {
      if((SL > 450) && (SR < 450)&& (SLM > 450))  // 左白右黑, 車體偏右校正
      {
        analogWrite(MotorR_ENA, SPEED_R);    //設定馬達 (右) 轉速
        analogWrite(MotorL_ENB, SPEED_L/2);    //設定馬達 (左) 轉速
        advance(0);
      }
      else if((SL < 450) && (SR > 450)&& (SRM > 450))  // 左黑右白, 車體偏左校正
      {
        analogWrite(MotorR_ENA, SPEED_R/2);    //設定馬達 (右) 轉速
        analogWrite(MotorL_ENB, SPEED_L);    //設定馬達 (左) 轉速
        advance(0);
      }
      else  // 其他, 直走
      {
        analogWrite(MotorR_ENA, SPEED_R);    //設定馬達 (右) 轉速
        analogWrite(MotorL_ENB, SPEED_L);    //設定馬達 (左) 轉速
        advance(0);
      }
    }
    else // 中感測器在白色區域, 車體已大幅偏離黑線
    {
      if((SL > 450) && (SR < 450)&& (SLM > 450) ) // 左白右黑, 車體快速右轉
      {
        analogWrite(MotorR_ENA, SPEED_R);    //設定馬達 (右) 轉速
        analogWrite(MotorL_ENB, SPEED_L);    //設定馬達 (左) 轉速
        turnR(0);
      }
      else if((SL < 450) && (SR > 450)&& (SRM > 450))  // 左黑右白, 車體快速左轉
      {
        analogWrite(MotorR_ENA, SPEED_R);    //設定馬達 (右) 轉速
        analogWrite(MotorL_ENB, SPEED_L);    //設定馬達 (左) 轉速
        turnL(0);
      }
    }
  }
}
