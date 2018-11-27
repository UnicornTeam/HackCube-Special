#include "em4095.h"

void EM4095::Start() {
  for (int i = 0; i < 9; i++) {
    bit1();
  }
}
void EM4095::bit1() {
   //Serial.print("1");
  digitalWrite(MOD, LOW);
  delayMicroseconds(delay_125);
  digitalWrite(MOD, HIGH);
  delayMicroseconds(delay_125);
}
void EM4095::bit0() {
  //Serial.print("0");
  digitalWrite(MOD, HIGH);
  delayMicroseconds(delay_125);
  digitalWrite(MOD, LOW);
  delayMicroseconds(delay_125);
}

void EM4095::transmit(int  * data) {
  Start();
  for (int i = 0; i < 11; i++) {
    int flag = data[i];
    for (int a = 5; a > 0; a--) {
      //((val & 0x1) << (a - 1))
      // (x[i] & (1 << a)) >> a;
      if (flag & (1 << (a - 1))) {
        bit1();
      } else { 
        bit0();
      }
    }
  }
//  Serial.println("");
}



void EM4095::transmit(unsigned long long  data) {
  for (int i = 64; i > 0; i--) {
    if (data >> (i-1 ) & 1)
      bit1();
    else
      bit0();
  }
}


void  EM4095::simulation_bak(int vd, unsigned long id) {
  int  x[13]; 
  int l = 2;
  long f = 0xf;
  x[0] = (vd & 0xF0)>>4; x[1] = vd & 0x0F;
  
  for (int i = 7; i >= 0; i--) {
    x[l]   = (id & f << i * 4) >> i * 4;
    l++;
  }
  transmit(parity_check(x));
}



void  EM4095::simulation(int vd, unsigned long id) {
  int  x[13];
  int l = 2;
  long f = 0xf;
  x[0] = (vd & 0xF0)>>4; x[1] = vd & 0x0F;
  for (int i = 7; i >= 0; i--) {
    x[l]   = (id & f << i * 4) >> i * 4;
    l++;
  }
  int * xx=parity_check(x);
 
  unsigned long long NFC_DATA=0xFF80000000000000;
  //unsigned long NFC_DATA_MSB=0xFF800000;
  //int a=55;
  unsigned long long  temp;
  for(int i=0;i<11;i++){
    temp=xx[i];
    temp=temp<<50-i*5;//50= - (9) - (5)
    NFC_DATA+=temp;
  }
  transmit(NFC_DATA);
}

int  *  EM4095::parity_check(int  * x)
{

  for (int i = 0; i < 10; i++) {
    int val = 0;
    unsigned int flag = x[i];
    while (flag)
    {
      val ^= flag;
      flag >>= 1;
    }
    x[i] = x[i] << 1;
    x[i] = x[i] + (val & 0x1);
  }
  int yval, da , val = 0;
  for (int a = 4; a > 0; a--) {
    //val=x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]+x[9]+x[10];
    //val=(x[0]&(1<<a-1))+(x[1]&(1<<a-1))+(x[2]&(1<<a-1))+(x[3]&(1<<a-1))+(x[4]&(1<<a-1))+(x[5]&(1<<a-1))+(x[6]&(1<<a-1))+(x[7]&(1<<a-1))+(x[8]&(1<<a-1))+(x[9]&(1<<a-1));
    for (int i = 0; i < 10; i++) {
      val ^= (x[i] & (1 << a)) >> a;
      // Serial.print((x[i]&(1<<a))>>a,HEX);
      //Serial.print(val&0x01,HEX);
      //Serial.print(" ");
    }

    da = ((val & 0x1) << (a - 1));
    tempdata += da;
    //Serial.print(((val & 0x1) << (a - 1)), HEX);
    //Serial.print(da1);
    //Serial.print(" ");
    val = 0;
  } //Serial.println(" ");
  x[10] = tempdata << 1;
  tempdata = 0;
  yval = 0;

  return x;
}





bool EM4095::decodeTag(unsigned long &udata,int &ReadVd)
{
  unsigned long outtime= millis();

  unsigned char i = 0;
  unsigned short timeCount;
  unsigned char timeOutFlag = 0;
  unsigned char row, col;
  unsigned char row_parity;
  unsigned char col_parity[5];
  int dat;
  unsigned char searchCount = 0;
  unsigned char j;
  //unsigned long udata;

  ReadVd = 0;
  while (1)
  {
    if(millis() - outtime > 100)
      return false;

    timeCount = 0;
    while (0 == digitalRead(ID_OUT)) //watch for ID_OUT to go low
    {

      if (timeCount >= TIMEOUT) //if we pass TIMEOUT milliseconds, break out of the loop
      {
        break;
      }
      else
      {
        timeCount++;
      }
    }

    if (timeCount >= 1200)
    {
      return false;
    }
    timeCount = 0;

    delayMicroseconds(DELAYVAL);
    if (digitalRead(ID_OUT) == HIGH)
    {

      for (i = 0; i < 8; i++) // 9 header bits
      {
        timeCount = 0; //restart counting
        while (1 == digitalRead(ID_OUT)) //while DEMOD out is high
        {

          if (timeCount == TIMEOUT)
          {
            timeOutFlag = 1;
            break;
          }
          else
          {
            timeCount++;
          }
        }

        if (timeOutFlag)
        {
          break;
        }
        else
        {
          delayMicroseconds(DELAYVAL);
          /*              digitalWrite(PA3,HIGH);
              delayMicroseconds(20);
               digitalWrite(PA3,LOW);*/
          if ( 0 == digitalRead(ID_OUT) )
          {
            break;
          }
        }
      }
      if (timeOutFlag)
      {
        timeOutFlag = 0;
        return false;
      }
      if (i == 8) //识别到九个前导码 开始接收数据
      {

        /* digitalWrite(PA3, HIGH);
          delayMicroseconds(20);
          digitalWrite(PA3, LOW);*/


        timeOutFlag = 0;
        timeCount = 0;

        while (1 == digitalRead(ID_OUT))
        {
          if (timeCount == TIMEOUT)
          {
            timeOutFlag = 1;
            break;
          }
          else
          {
            timeCount++;
          }

          if (timeOutFlag)
          {
            timeOutFlag = 0;
            return false;
          }
        }

        col_parity[0] = col_parity[1] = col_parity[2] = col_parity[3] = col_parity[4] = 0;
        for (row = 0; row < 11; row++)
        {
          /* digitalWrite(PA3, HIGH);
            delayMicroseconds(15);
            digitalWrite(PA3, LOW);
          */
          row_parity = 0;
          j = row - 1;
          for (col = 0, row_parity = 0; col < 5; col++)
          {
            /*    digitalWrite(PA3, HIGH);
                delayMicroseconds(2);
                digitalWrite(PA3, LOW);*/
            delayMicroseconds(DELAYVAL);
            if (digitalRead(ID_OUT)) {
              dat = 1;
            } else {
              dat = 0;
            }
            if (col < 4 && row < 10 && row > 1)
            {
              udata = udata << 1;
              udata += dat;
            } else if (col < 4 && row < 2) {
              ReadVd = ReadVd << 1;
              ReadVd += dat;
            }


            row_parity += dat;
            col_parity[col] += dat;
            timeCount = 0;

            while (digitalRead(ID_OUT) == dat)
            {
              if (timeCount == TIMEOUT)
              {
                timeOutFlag = 1;
                break;
              }
              else
                timeCount++;

            }
            if (timeOutFlag)
              break;
          }

          if (row < 10)
          {
            if ((row_parity & 0x01) || timeOutFlag) //Row parity
            {
              timeOutFlag = 1;
              break;
            }
          }
        }
        if ( timeOutFlag || (col_parity[0] & 0x01) || (col_parity[1] & 0x01) || (col_parity[2] & 0x01) || (col_parity[3] & 0x01) ) //Column parity
        {
          timeOutFlag = 0;
          return false;
        }
        else
        {
          ReadVd = ReadVd & 0xff;
          //Serial.println(ReadVd);
          //return (udata);
          return true;
        }
      }//end receive data
      return false;
    }
  }
};
