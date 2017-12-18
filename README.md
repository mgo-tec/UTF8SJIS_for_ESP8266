# This is the Arduino IDE library using the UTF8_to_Shift_JIS conversion table for the ESP8266.

これはESP-WROOM-02(ESP8266)の Arduino IDE ライブラリです。  
これは、SPIFFSファイルシステム専用です。  
Beta Version 1.50  
  
UTF8 to Shift_JIS 変換テーブルファイル "Utf8Sjis.tbl" を予めSPIFFSファイルシステムを使用して、ESP8266のフラッシュへアップロードしておく必要があります。    
UTF-8コードのString文字列をShift_JIS文字列コードに変換できます。  
  
変換テーブルファイルURL--> https://github.com/mgo-tec/UTF8_to_Shift_JIS  
  
JIS第一水準、第二水準、１３区、半角カナが変換可能  
  
【更新履歴】  
(Beta ver 1.50)  
ShinonomeFontreadライブラリ beta 1.40 と組み合わせて、ファイルハンドルを共有できるように修正しました。  
  
(Beta ver 1.41)  
ライブラリ内ソースのif文の分類をswich文の分類に修正しました。  
  
(Beta ver 1.4)  
UTF-8の２バイト文字(C2A2～D191)を変換できるように修正しました。  
それと、計算を少々高速化しました。  
  	
(Beta ver 1.33)  
Whileループの中に yield(); を追加しました。  
  
参照ブログ https://www.mgo-tec.com  
  
The MIT license