#include <UTF8toSJIS.h>

const char* UTF8SJIS_file = "/Utf8Sjis.tbl"; //SPIFFSファイルシステムで予めこのファイルをアップロードしておくこと

UTF8toSJIS u8ts;

void setup() {
  Serial.begin(115200);
  String str = "こんにちは！！　今日は晴れ時々曇り。半角ｶﾀｶﾅも表示できます。";
  
  uint8_t sj_txt[str.length()];
  
  uint16_t sj_length;
  
  u8ts.UTF8_to_SJIS_str_cnv(UTF8SJIS_file, str, sj_txt, &sj_length);
  
  Serial.println();
  int i=0;
  for(i=0;i<sj_length;i++){ //Shift_JISコードを表示
    Serial.printf("%X,",sj_txt[i]);
  }
  Serial.println();
  for(i=0;i<sj_length;i++){ //Shift_JISコードをシリアルモニターにバイナリ出力
    Serial.write(sj_txt[i]);
  }
  Serial.println();
  
}

void loop() {
}
