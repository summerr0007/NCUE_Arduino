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
#define MotorR_I1     A1  //定義 I1 接腳
#define MotorR_I2     A0 //定義 I2 接腳
#define MotorL_I3    A2  //定義 I3 接腳
#define MotorL_I4    A3  //定義 I4 接腳
#define MotorR_ENA    5  //定義 ENA (PWM調速) 接腳
#define MotorL_ENB    6  //定義 ENB (PWM調速) 接腳

int SPEED_R  =     64; //定義右輪速度
int SPEED_L  =     64; //定義左輪速度

#define data 6
#define clk 7
#define stb 8

int up[8]={0,0,0,-1,-1,0,0,0};
byte a[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void setup()              //設定腳位
{
  pinMode(MotorR_I1,OUTPUT);
  pinMode(MotorR_I2,OUTPUT);
  pinMode(MotorL_I3,OUTPUT);
  pinMode(MotorL_I4,OUTPUT);
  pinMode(MotorR_ENA,OUTPUT);
  pinMode(MotorL_ENB,OUTPUT);
  pinMode(data,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(stb,OUTPUT);
  digitalWrite(stb,HIGH);
  digitalWrite(clk,HIGH);
  sendCommand(0x40);
  sendCommand(0x88);
}

void loop()
{
SPEED_R  =     70; //定義右輪速度
SPEED_L  =     66;
    up[0] = (SPEED_L/100);
    up[1] = (SPEED_L/10)%10;
    up[2] = (SPEED_L%10);
    up[5] = (SPEED_R/100);
    up[6] = (SPEED_R/10)%10;
    up[7] = (SPEED_R%10);
    setSegNumadd(up);
    //直走
//    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
//    analogWrite(MotorL_ENB,SPEED_L);
//    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
//    digitalWrite(MotorR_I2,LOW);
//    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
//    digitalWrite(MotorL_I4,LOW);
//    delay(2000);

    //後退
    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L);
    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(2700);
    /*SPEED_L=SPEED_L+22;
    up[0] = (SPEED_L/100);
    up[1] = (SPEED_L/10)%10;
    up[2] = (SPEED_L%10);
    up[5] = (SPEED_R/100);
    up[6] = (SPEED_R/10)%10;
    up[7] = (SPEED_R%10);
    setSegNumadd(up);*/
    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L);
    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH );    //馬達（左）順時針轉動
    digitalWrite(MotorL_I4,LOW);
    delay(45);

    analogWrite(MotorR_ENA,SPEED_R);    //設定馬達轉速
    analogWrite(MotorL_ENB,SPEED_L);
    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(1000);

//    //左轉
//    analogWrite(MotorR_ENA,SPEED_R/2);     //設定馬達轉速
//    analogWrite(MotorL_ENB,SPEED_L/2);
//    digitalWrite(MotorR_I1,HIGH);   //馬達（右）順時針轉動
//    digitalWrite(MotorR_I2,LOW);
//    digitalWrite(MotorL_I3,LOW);    //馬達（左）順時針轉動
//    digitalWrite(MotorL_I4,HIGH);
//    delay(2000);
//
//    //右轉
//    analogWrite(MotorR_ENA,SPEED_R/2);     //設定馬達轉速
//    analogWrite(MotorL_ENB,SPEED_L/2);
//    digitalWrite(MotorR_I1,LOW);    //馬達（右）逆時針轉動
//    digitalWrite(MotorR_I2,HIGH);
//    digitalWrite(MotorL_I3,HIGH);   //馬達（左）逆時針轉動
//    digitalWrite(MotorL_I4,LOW);
//    delay(2000);
//
//    //停止
    digitalWrite(MotorR_I1,HIGH);   //馬達（右）停止轉動
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   //馬達（左）停止轉動
    digitalWrite(MotorL_I4,HIGH);
    delay(2000);
 }


 byte get_byte()
{
  byte dat=0x00;
  pinMode(data,INPUT);
  for(int i=0;i<8;i++)
  {
    digitalWrite(clk,LOW);
    if(digitalRead(data))
    {
      dat|=(0x01<<i);
    }
    digitalWrite(clk,HIGH);
  }
  pinMode(data,OUTPUT);
  return dat;
}
byte get_Button() //讀取按鍵回傳的函數
{
  byte keys = 0x00; //初設keys為0x00(表示沒有顯示資料)
  digitalWrite(stb,LOW);
  send(0x42); //開始執行從鍵盤讀入內容
  for(int i = 0 ; i < 4 ; i++) //開始分別讀取4個byte (byte0, byte1, byte2, byte3)
  {
    byte temp = get_byte(); 
    keys |= (temp << i); //將temp內部獲得的資料左移 i 格 然後合併到keys內部
  }
  digitalWrite(stb,HIGH);
  return keys; 
}
void send(byte data1) 
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clk, LOW);
    digitalWrite(data, (data1 & 1 ? HIGH : LOW));
    data1 >>= 1;
    digitalWrite(clk, HIGH);
  }
}
void sendCommand(byte cmd)
{
  digitalWrite(stb, LOW); 
  send(cmd);
  digitalWrite(stb, HIGH);
}
void setSegNum(byte seg_address, byte pattern)
{
  digitalWrite(stb, LOW);//開始進行輸入
  send(seg_address);//指定寫入的七段顯示器位置
  send(pattern);//寫入圖案
  digitalWrite(stb, HIGH);//結束輸入
}

void setSegNumadd(int pattern[])
{
  digitalWrite(stb, LOW);
  send(0xc0);
  for(int i=0;i<8;i++){
    if(pattern[i] == -1){
      send(0x00);
    }else{
      send(a[pattern[i]]);
    }  
    send(0x00);
  }  
  digitalWrite(stb, HIGH);
}
