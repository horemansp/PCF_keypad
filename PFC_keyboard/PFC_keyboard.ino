#include <Wire.h>

int PCADDRESS = 0x20;

char key_array[4][4] = {{'1','2','3','A'},
                  {'4','5','6','B'},
                  {'7','8','9','C'},
                  {'*','0','#','D'}};
/*
key pad
cols
4 3 2 1

1 2 3 A       r   8
4 5 6 B       o   7
7 8 9 C       w   6
* 0 # D       s   5
*/
byte row_mask=   0b00001111;
//byte rows =      0b10000000;
byte row_bytes;
byte col_bytes;
byte write_bytes;
int rows;
int cols;
bool statusResult;
byte col_bytes_write;
byte row_bytes_write;

void write_byte(int toAddress, byte byteToWrite){
      Wire.beginTransmission(PCADDRESS); 
      Wire.write(byteToWrite);   
      Wire.endTransmission();     
  }

byte read_byte(int fromAddress){
      byte returnByte;
      Wire.requestFrom(fromAddress, 1);
      while(Wire.available())    // slave may send less than requested
      {
        returnByte = Wire.read();    // receive a byte as character
    
      }
     return returnByte;
  }

void setup()
{
  Wire.begin();   
  Serial.begin(115200);  
  Wire.beginTransmission(PCADDRESS); 
  Wire.write(0b00001111);             // sends value byte
  Wire.endTransmission();     // stop transmitting
}

void loop()
{
  bool pressed = false;
  byte col_mask = 0b00001111;
  byte row_mask = 0b11110000;
     while (not pressed){
        row_bytes = 0b10000000;
        for(rows=0;rows<4;rows++){
            col_bytes = 0b11110111;
            for(cols=0;cols<4;cols++){
                col_bytes_write = col_bytes & col_mask; //11110111 & 00001111 = 00000111
                row_bytes_write = row_bytes & row_mask; //10000000 & 11110000 = 10000000
                write_bytes = row_bytes_write | col_bytes_write;//0b10000000 | 0b00000111 = 0b10000111
                write_byte(PCADDRESS,write_bytes);
                statusResult=(read_byte(PCADDRESS)  & row_bytes);
                if ((statusResult) == 0){
                    Serial.println(key_array[rows][cols]);
                    pressed  =true;
                    while (pressed){
                        statusResult = (read_byte(PCADDRESS) & row_bytes);
                        if (statusResult != 0){
                            pressed = false;
                        }
                    }
                }
                col_bytes = col_bytes >> 1;
            }
            row_bytes = row_bytes >> 1;
        }
     }
}
