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
//    第1節：L298N馬達驅動板的應用
//
//*******************************************************************************
#define MotorR_I1    A1  //定義 I1 接腳
#define MotorR_I2    A0  //定義 I2 接腳
#define MotorL_I3    A2  //定義 I3 接腳
#define MotorL_I4    A3  //定義 I4 接腳
#define MotorR_ENA    5  //定義 ENA (PWM調速) 接腳
#define MotorL_ENB    6  //定義 ENB (PWM調速) 接腳

#define SPEED_R       150 //定義右輪速度
#define SPEED_L       151 //定義左輪速度

#define dataPin 6
#define clockPin 7
#define strobePin 8
byte pattern[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
byte seg_loc[8] = {0xc0, 0xc2, 0xc4,0xc6, 0xc8, 0xca, 0xcc, 0xce};
void setup()              //設定腳位
{
  pinMode(MotorR_I1,OUTPUT);
  pinMode(MotorR_I2,OUTPUT);
  pinMode(MotorL_I3,OUTPUT);
  pinMode(MotorL_I4,OUTPUT);
  pinMode(MotorR_ENA,OUTPUT);
  pinMode(MotorL_ENB,OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);
  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);
  sendCommand(0x40); //寫入資料
  sendCommand(0x88); //打開七段

  for(int i=0;i<8;i++)
  {
    setSegNum(seg_loc[i],0x00);
  }
}

void loop()
{
    set7(150,151);
    //直走
    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L);
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
    digitalWrite(MotorR_I2,LOW);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorL_I4,LOW);
    delay(1000);
    

    //後退
//    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
//    analogWrite(MotorL_ENB,SPEED_L);
//    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
//    digitalWrite(MotorR_I2,HIGH);
//    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
//    digitalWrite(MotorL_I4,HIGH);
//    delay(2000);

    //左轉
//    analogWrite(MotorR_ENA,SPEED_R/2);     //設定馬達轉速
//    analogWrite(MotorL_ENB,SPEED_L/2);
//    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
//    digitalWrite(MotorR_I2,LOW);
//    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
//    digitalWrite(MotorL_I4,HIGH);
//    delay(2000);

    //右轉
    analogWrite(MotorR_ENA,SPEED_R/4);     //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L/5);
    digitalWrite(MotorR_I1,HIGH);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,LOW);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorL_I4,LOW);
    delay(500);

    //停止
//    digitalWrite(MotorR_I1,HIGH);   //馬達（右）停止轉動
//    digitalWrite(MotorR_I2,HIGH);
//    digitalWrite(MotorL_I3,HIGH);   //馬達（左）停止轉動
//    digitalWrite(MotorL_I4,HIGH);
//    delay(500);
    
    //直走
    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L);
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
    digitalWrite(MotorR_I2,LOW);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorL_I4,LOW);
    delay(500);

    //停止
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）停止轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）停止轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(3000);

}

void set7(int r,int l)
{
  for(int i=3;i>=1;i--)
  {
    setSegNum(seg_loc[i], pattern[r%10]);
    r/=10;
  }
  for(int i=7;i>=5;i--)
  {
    setSegNum(seg_loc[i], pattern[l%10]);
    l/=10;
  }
}

void sendCommand(byte cmd)
{
  digitalWrite(strobePin, LOW);
  send(cmd);
  digitalWrite(strobePin, HIGH);
}

void send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockPin, HIGH);
  }
}

void setSegNum(byte seg_address, byte pattern)
{
  digitalWrite(strobePin, LOW);//開始進行輸入
  send(seg_address);//指定寫入的七段顯示器位置
  send(pattern);//寫入圖案
  digitalWrite(strobePin, HIGH);//結束輸入
}


byte getButton(){               //掃描鍵盤
  byte keys = 0x00;
  digitalWrite(strobePin, LOW); //strobePin轉LOW開始傳輸
  send(0x42);                   //送出讀鍵盤指令
  for(int i = 0; i < 4; i++){
    byte temp = get_byte();     //取得byte i
    keys |= (temp << i);        //將取得的byte與鍵盤對應位置對齊
  }
  digitalWrite(strobePin, HIGH);//復位
  return keys;
}

byte get_byte(){
  byte data = 0x00;
  pinMode(dataPin, INPUT);        //將dataPin改為input

  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);  //將clockPin從HIGH轉LOW，將data從dataPin輸出一個bit
    if (digitalRead(dataPin)) {   //若讀取到HIGH
      data |= (0x01 << i);        //將對應bit位置轉為1
    }
    digitalWrite(clockPin, HIGH); //將clockPin復位為HIGH
  }

  pinMode(dataPin, OUTPUT);       //將dataPin復位為output
  return data;
}
