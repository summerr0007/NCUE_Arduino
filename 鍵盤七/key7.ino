#define strobepin 8
#define clockpin 7
#define datapin 6
byte locat[8] = {0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce};
byte num[11] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67, 0x00};
int seg_now[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
byte pre_keys = 0x00;
byte output[8];
int mouse[3] = {0, 0, 1};
byte which[3][3];
void setup() {
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(strobepin, OUTPUT);
  digitalWrite(strobepin, HIGH);
  digitalWrite(clockpin, HIGH);
  sendCommand(0x40); //寫數據到暫存器
  sendCommand(0x88); //打開板子上顯示元件
  for (int i = 0; i < 8; i++)      //清空七段
    setSegNum(locat[i], 0x00);
}
int choose = 0; bool exiT = true; int count = 0;
int turn[20];
void loop() {
  byte keys = getbutton();
  if (keys != pre_keys)
  {
    pre_keys = keys;
    if (exiT == true)
    {
      for (int i = 0; i < 8; i++)
      {
        if (keys & 0x01)
        {
          for (int flow = 7; flow > 0; flow--)
          {
            seg_now[flow] = seg_now[flow - 1];
          }
          seg_now[0] = i + 1;
          address_setSegNum(seg_now);
        }
        keys >>= 1;
      }
      if (seg_now[7] != -1)
      {
        for (int i = 0; i < 8; i++)
          output[i] = num[seg_now[i]];
        for (int i = 0; i < 20; i++)
        {
          if (i <= 7)
            turn[i] = output[i] & 0x01;
          else if (i == 8)
            turn[i] = output[7]>>1 & 0x01;
          else if (i == 9)
            turn[i] = output[7]>>2 & 0x01;
          else if (i == 10)
            turn[i] = output[7]>>3 & 0x01;
          else if (i == 11)
            turn[i] = output[6]>>3 & 0x01;
          else if (i == 12)
            turn[i] = output[5]>>3 & 0x01;
          else if (i == 13)
            turn[i] = output[4]>>3 & 0x01;
          else if (i == 14)
            turn[i] = output[3]>>3 & 0x01;
          else if (i == 15)
            turn[i] = output[2]>>3 & 0x01;
          else if (i == 16)
            turn[i] = output[1]>>3 & 0x01;
          else if (i == 17)
            turn[i] = output[0]>>3 & 0x01;
          else if (i == 18)
            turn[i] = output[0]>>4 & 0x01;
          else if (i == 19)
            turn[i] = output[0]>>5 & 0x01;
        }
        exiT = false;
      }
    }
    else
    {
      if (keys & 0x01) //S1
        choose = 1;
      else if (keys & 0x02) //S2
        choose = 2;
      else
        choose = 0;

      if (choose == 1)
      {
        int temp = turn[0];
        for (int i = 0; i < 19; i++)
          turn[i] = turn[i + 1];
        turn[19] = temp;

      }
      else if (choose == 2)
      {
        int temp = turn[19];
        for (int i = 19; i > 0; i--)
          turn[i] = turn[i - 1];
        turn[0] = temp;
      }
      
      for (int i = 0; i < 20; i++)
      {
        if (i == 0)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 1)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 2)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 3)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 4)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 5)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 6)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 7)
        {
          if (turn[i] == 0)
            output[i] = output[i] & ~(0x01);
          else if (turn[i] == 1)
            output[i] = output[i] | 0x01;
        }
        else if (i == 8)
        {
          if (turn[i] == 0)
            output[7] = output[7] & ~(0x02);
          else if (turn[i] == 1)
            output[7] = output[7] | 0x02;
        }
        else if (i == 9)
        {
          if (turn[i] == 0)
            output[7] = output[7] & ~(0x04);
          else if (turn[i] == 1)
            output[7] = output[7] | 0x04;
        }
        else if (i == 10)
        {
          if (turn[i] == 0)
            output[7] = output[7] & ~(0x08);
          else if (turn[i] == 1)
            output[7] = output[7] | 0x08;
        }
        else if (i == 11)
        {
          if (turn[i] == 0)
            output[6] = output[6] & ~(0x08);
          else if (turn[i] == 1)
            output[6] = output[6] | 0x08;
        }
        else if (i == 12)
        {
          if (turn[i] == 0)
            output[5] = output[5] & ~(0x08);
          else if (turn[i] == 1)
            output[5] = output[5] | 0x08;
        }
        else if (i == 13)
        {
          if (turn[i] == 0)
            output[4] = output[4] & ~(0x08);
          else if (turn[i] == 1)
            output[4] = output[4] | 0x08;
        }
        else if (i == 14)
        {
          if (turn[i] == 0)
            output[3] = output[3] & ~(0x08);
          else if (turn[i] == 1)
            output[3] = output[3] | 0x08;
        }
        else if (i == 15)
        {
          if (turn[i] == 0)
            output[2] = output[2] & ~(0x08);
          else if (turn[i] == 1)
            output[2] = output[2] | 0x08;
        }
        else if (i == 16)
        {
          if (turn[i] == 0)
            output[1] = output[1] & ~(0x08);
          else if (turn[i] == 1)
            output[1] = output[1] | 0x08;
        }
        else if (i == 17)
        {
          if (turn[i] == 0)
            output[0] = output[0] & ~(0x08);
          else if (turn[i] == 1)
            output[0] = output[0] | 0x08;
        }
        else if (i == 18)
        {
          if (turn[i] == 0)
            output[0] = output[0] & ~(0x10);
          else if (turn[i] == 1)
            output[0] = output[0] | 0x10;
        }
        else if (i == 19)
        {
          if (turn[i] == 0)
            output[0] = output[0] & ~(0x20);
          else if (turn[i] == 1)
            output[0] = output[0] | 0x20;
        }
      }
      
      for (int i = 0; i < 8; i++)
        setSegNum(locat[i], output[i]);
    }
  }

}

byte get_byte() {
  byte data = 0x00;
  pinMode(datapin, INPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockpin, LOW);
    if (digitalRead(datapin)) {
      data |= (0x01 << i);
    }
    digitalWrite(clockpin, HIGH);
  }

  pinMode(datapin, OUTPUT);
  return data;
}

byte getbutton() {
  byte keys = 0x00;
  digitalWrite(strobepin, LOW);
  send(0x42);  //讀鍵盤的指令
  for (int i = 0; i < 4; i++) {
    byte temp = get_byte();
    keys |= (temp << i);
  }
  digitalWrite(strobepin, HIGH);//復位
  return keys;
}

void setSegNum(byte address, byte pattern)
{
  digitalWrite(strobepin, LOW);//開始進行輸入
  send(address);//指定寫入的七段顯示器位置
  send(pattern);//寫入圖案
  digitalWrite(strobepin, HIGH);//結束輸入
}

void address_setSegNum(int input[])
{
  digitalWrite(strobepin, LOW);//開始進行輸入
  send(0xc0);//指定寫入的七段顯示器位置
  for (int i = 0; i < 8; i++)
  {
    if (input[i] == -1)
    {
      send(0x00);
    }
    else
    {
      send(num[input[i]]);
    }
    send(0x00);
  }
  digitalWrite(strobepin, HIGH);//結束輸入
}

void send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockpin, LOW);
    digitalWrite(datapin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockpin, HIGH);
  }
}

void sendCommand(byte cmd)
{
  digitalWrite(strobepin, LOW);
  send(cmd);
  digitalWrite(strobepin, HIGH);
}