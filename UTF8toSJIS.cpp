/*
  UTF8toSJIS.cpp - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.0
  This is a library for converting from UTF-8 code string to Shift_JIS code string.
  In advance, you need to upload a conversion table file Utf8Sjis.tbl using SPIFFS file system ESP-WROOM-02(ESP8266) to flash.
  GitHub---> https://github.com/mgo-tec/UTF8_to_Shift_JIS
  
The MIT License (MIT)

Copyright (c) 2016 Mgo-tec
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Arduino.h"
#include "UTF8toSJIS.h"

UTF8toSJIS::UTF8toSJIS(){}

void UTF8toSJIS::UTF8_to_SJIS_str_cnv(const char* __UTF8SJIS_file, String __scl_txt, uint8_t* __sj_txt, uint16_t* __sj_length)
{
  uint16_t __sj_cnt = 0;
  uint16_t __fnt_cnt = 0;
  uint8_t __utf8_1, __utf8_2, __utf8_3;
  uint32_t __sp_addres=0x9DCC;//スペース
  uint8_t __sjis[2];
  
  uint16_t __str_length = __scl_txt.length();
  
  //Serial.printf("\r\nlength=%d",__str_length);
  
  while(__scl_txt[__fnt_cnt] != '\0'){
    if(__scl_txt[__fnt_cnt]>=0xC2 && __scl_txt[__fnt_cnt]<=0xD1){//2バイト文字
      __utf8_1 = __scl_txt[__fnt_cnt];
      __utf8_2 = __scl_txt[__fnt_cnt+1];
      __utf8_3 = 0x00;
      __fnt_cnt = __fnt_cnt + 2;
    }else if(__scl_txt[__fnt_cnt]>=0xE2 && __scl_txt[__fnt_cnt]<=0xEF){
      __utf8_1 = __scl_txt[__fnt_cnt];
      __utf8_2 = __scl_txt[__fnt_cnt+1];
      __utf8_3 = __scl_txt[__fnt_cnt+2];

      __fnt_cnt = __fnt_cnt +3;
    }else if(__scl_txt[__fnt_cnt]>=0x20 && __scl_txt[__fnt_cnt]<=0x7E){
      __utf8_1 = __scl_txt[__fnt_cnt];
      __utf8_2 = 0x00;
      __utf8_3 = 0x00;
      __fnt_cnt++;
    }
        //ここからShift_JIS文字列を作成
    if(__utf8_1>=0x20 && __utf8_1<=0x7E){ //半角ASCII文字を判別
      __sjis[0] = __utf8_1;
      __sj_txt[__sj_cnt] = __utf8_1;
      __sj_cnt++;
    }else if(__utf8_1<0x20){ //その他は全てスペースとする。
      __sj_txt[__sj_cnt] = 0x81;
      __sj_txt[__sj_cnt+1] = 0x40;
      __sj_cnt = __sj_cnt + 2;
    }else{
      //Serial.printf("\r\n%X,%X,%X ",__utf8_1,__utf8_2,__utf8_3);
      UTF8toSJIS::UTF8_To_SJIS_code_cnv(__utf8_1,__utf8_2,__utf8_3, &__sp_addres);
      //Serial.printf("\r\naddress=%X",__sp_addres);
      UTF8toSJIS::SPIFFS_Flash_UTF8SJIS_Table_Read(__UTF8SJIS_file, __sp_addres, __sjis);
      //Serial.printf("\r\n__sjis=%X,%X",__sjis[0],__sjis[1]);
      if((__sjis[0]>=0xA1 && __sjis[0]<=0xDF)){
        __sj_txt[__sj_cnt] = __sjis[0];
        __sj_cnt++;
      }else{
        __sj_txt[__sj_cnt] = __sjis[0];
        __sj_txt[__sj_cnt+1] = __sjis[1];
        __sj_cnt = __sj_cnt + 2;
      }
    }
  }
  *__sj_length = __sj_cnt;
}

//***********UTF-8コードをSPIFFS内の変換テーブルを読み出してShift-JISコードに変換****
void UTF8toSJIS::UTF8_To_SJIS_code_cnv(uint8_t __utf8_1, uint8_t __utf8_2, uint8_t __utf8_3, uint32_t* __spiffs_addrs)
{
  uint32_t UTF8uint = __utf8_1*256*256 + __utf8_2*256 + __utf8_3;
  
  if(__utf8_1>=0xC2 && __utf8_1<=0xD1){
    //0xB0からS_JISコード実データ。0x00-0xAFまではライセンス文ヘッダ。
    *__spiffs_addrs = ((__utf8_1*256 + __utf8_2)-0xC2A2)*2 + 0xB0; //文字"¢" UTF8コード C2A2～、S_jisコード8191
  }else if(__utf8_1==0xE2 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE28090)*2 + 0x1EEC; //文字"‐" UTF8コード E28090～、S_jisコード815D
  }else if(__utf8_1==0xE3 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE38080)*2 + 0x9DCC; //スペース UTF8コード E38080～、S_jisコード8140
  }else if(__utf8_1==0xE4 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE4B880)*2 + 0x11CCC; //文字"一" UTF8コード E4B880～、S_jisコード88EA
  }else if(__utf8_1==0xE5 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE58085)*2 + 0x12BCC; //文字"倅" UTF8コード E58085～、S_jisコード98E4
  }else if(__utf8_1==0xE6 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE6808E)*2 + 0x1AAC2; //文字"怎" UTF8コード E6808E～、S_jisコード9C83
  }else if(__utf8_1==0xE7 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE78081)*2 + 0x229A6; //文字"瀁" UTF8コード E78081～、S_jisコードE066
  }else if(__utf8_1==0xE8 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE88080)*2 + 0x2A8A4; //文字"耀" UTF8コード E88080～、S_jisコード9773
  }else if(__utf8_1==0xE9 && __utf8_2>=0x80){
    *__spiffs_addrs = (UTF8uint-0xE98080)*2 + 0x327A4; //文字"退" UTF8コード E98080～、S_jisコード91DE
  }else if(__utf8_1>=0xEF && __utf8_2>=0xBC){
    *__spiffs_addrs = (UTF8uint-0xEFBC81)*2 + 0x3A6A4; //文字"！" UTF8コード EFBC81～、S_jisコード8149
    if(__utf8_1==0xEF && __utf8_2==0xBD && __utf8_3==0x9E){
      *__spiffs_addrs = 0x3A8DE; // "～" UTF8コード EFBD9E、S_jisコード8160
    }
  }
}

void UTF8toSJIS::SPIFFS_Flash_UTF8SJIS_Table_Read(const char* __UTF8SJIS_file, uint32_t __addrs, uint8_t* __buf)
{
  String file_name = __UTF8SJIS_file;
  SPIFFS.begin();
  File __f2 = SPIFFS.open(__UTF8SJIS_file, "r");
  if(__f2){
    __f2.seek(__addrs,SeekSet);
    __buf[0] = __f2.read();
    __buf[1] = __f2.read();
    __f2.close();
  }else{
    Serial.print(__UTF8SJIS_file);
    Serial.println(" file has not been uploaded to the flash in SPIFFS file system");
    delay(30000);
  }
}