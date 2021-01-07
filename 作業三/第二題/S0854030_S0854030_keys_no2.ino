#define dataPin 6
#define clockPin 7
#define strobePin 8
byte seg_loc[8] = {0xc0, 0xc2, 0xc4,0xc6, 0xc8, 0xca, 0xcc, 0xce};
byte pattern[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07, 0x7f,0x67,0x3f};  //0,... 8,9,0(為了不使用%10，這樣就不會overflow)
short seg_now[8];               //儲存七段當前的顯示pattern index
byte pre_keys = 0x00;
void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);
  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);
  sendCommand(0x40);
  sendCommand(0x88);

  for(int i = 0; i < 8; i++){       //清空七段
    setSegNum(seg_loc[i], 0x00); 
    seg_now[i] = 0;
  }
}

void loop() {
  byte keys = getButton();
  int add=0;
  if(keys != pre_keys){       //按鍵內容有變化
    pre_keys = keys;
     if(keys&(0x80))
    {
     add=5;
    }
    for(int i = 0; i < 5; i++){          /*只掃描到5，因為只能按到S5*/
      if(keys & 0x01)                    /*處理移位*/
      {
        for(int k=7;k>=1;k--)       
          seg_now[k]=seg_now[k-1];
        seg_now[0]=(i+1+add);
        
        for(int j=0;j<8;j++)         /*真正顯示*/
            {
              if(seg_now[j]==0)
                setSegNum(seg_loc[j], 0x00);
            
                else 
                  setSegNum(seg_loc[j], pattern[seg_now[j]]);
                                       
            }
    }  
     keys >>= 1;
    }
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


byte getButton(){
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
