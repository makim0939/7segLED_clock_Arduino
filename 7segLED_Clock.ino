//RTCモジュール: DS1307
//7セグメントLED: 5641AS

//I2C通信
#include <Wire.h>
//RTCモジュール用
#include <DS3231.h>
DS3231 clock;
RTCDateTime dt;

// loop内で使う変数をグローバルに宣言しちゃう
int prev_minute = 100;
int h10 = 0;
int h1 = 0;
int m10 = 0;
int m1 = 0;

//7セグLEDの表示//array[] = {size, val1, val2, ...}
int zero[] = {7, 2, 3, 4, 5, 6, 7};
int one[] = {3, 3, 4};
int two[] = {6, 2, 3, 5, 6, 8};
int three[] = {6, 2, 3, 4, 5, 8};
int four[] = {5, 3, 4, 7, 8};
int five[] = {6, 2, 4, 5, 7, 8};
int six[] = {7, 2, 4, 5, 6, 7, 8};
int seven[] = {4, 2, 3, 4};
int eight[] = {8, 2, 3, 4, 5, 6, 7, 8};
int nine[] = {7, 2, 3, 4, 5, 7, 8};

int array[] = {zero, one, two, three, four, five, six, seven, eight, nine};

//7セグメントLEDの1桁に数字を表示する。
void show(int *num) {
  //一度全ての出力をLOWに
  for (int i=2; i<10; i++) {
     digitalWrite(i, LOW);
  }
  //数字に対応するところのみHIGH
  for(int i=1; i<num[0]; i++) {
    digitalWrite(num[i], HIGH);
  }
}

//ダイナミック点灯で表示する桁を変える。
void switch_low_pin(int pin_num){
  for(int i = 10; i < 14; i++) {
    if(i == pin_num) {
      digitalWrite(i, LOW);
    }else {
      digitalWrite(i, HIGH);
    }
    
  }
}


void setup() {
  // Initialize DS3231
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);  

  //2~9ピンが各セグメント、10~13ピンがどの桁を点灯させるか。めんどいからマクロは使わない
  //pinModeを全てアウトプットにする
  for(int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
  }
  //カソード側のピンの初期化
  for(int i = 10; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  //実時間モジュールから時間を取得
   dt = clock.getDateTime();

  //分が変わっていた場合のみ表示を変える処理を行う
  if(prev_minute != dt.minute) {
     //時間・分を桁ごとにばらす。
    int tmp = 0;
    for (int i=0; dt.hour>=tmp; i++) {
        tmp+=10;
    }
    tmp -= 10;
    h10 = tmp/10;
    h1 = dt.hour - tmp;

    tmp = 0;
    for (int i=0; dt.minute>=tmp; i++) {
        tmp+=10;
    }
    tmp -= 10;
    m10 = tmp/10;
    m1 = dt.minute - tmp;
  }

  //それぞれの桁に表示
  switch_low_pin(10);
  show(array[h10]);
  delay(1);

  switch_low_pin(11);
  show(array[h1]);
  delay(1);

  switch_low_pin(12);
  show(array[m10]);
  delay(1);

  switch_low_pin(13);
  show(array[m1]);
  delay(1);

  //最後に点灯したところだけ次の処理によって点灯時間が長くなり、明くみえてしまう対策
  switch_low_pin(0);

}
