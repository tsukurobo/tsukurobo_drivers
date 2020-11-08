# tsukurobo_drivers
つくろぼで使う基板の基本的なプログラム．  

## solonoid_board
ソレノイド基板を動かすためのプログラム．  
ピン番号を合わせれば動く．  
1秒おきにON/OFFを繰り返す．  

## ise_motor_driver
伊勢モータードライバー（伊勢MD）を動かすためのプログラム．  
 - master：伊勢MDのマスター（司令を送るarduino）側のプログラム  
 - slave：伊勢MDのスレーブ（基板上のAVR）側のプログラム  
 - arduino_writeing_machine：arduinoをAVR書き込み装置とするために必要なファイル  

### master
特筆事項無し．

### slave
- i2c_slave：司令をそのまま出力するプログラム  
- pid_i2c_sleva：角度をPID制御するプログラム  
- pid_v_i2c_slave：角速度をPID制御するプログラム  

※SM(SMB)方式とLAP方式を変更できるが今(2020/11/8)は保留．誰か書いて  
- SM(SMB)方式：回転方向と01のデューティ比の2つの司令を送る
- LAP方式：01と0-1のデューティー比を送る


### arduino_writing_machine
#### arduino unoをAVR書き込み装置にする方法．
1. 準備
    1. 書き込み用のarduinoシールドをarduinoに付ける．
    1. Arduino IDEが保存されたディレクトリ内の/hardwareに，tsukurobo_drivers/ise_motor_driver/arduino_writing_machine/hardware内のディレクトリたちをコピー．（置き換えないように注意）  
    （linux版の場合「arduino-1.8.12/hardware」のような名前）
1. arduinoをAVR書き込み装置にする
    1. シールドを付けたarduinoをPCにつなぐ
    1. Arduino IDEを開き「ファイル/スケッチの例/Arduino ISP」を開く．
    1. シールドを付けたarduinoに開いたプログラムを書き込む．
1. AVR書き込み装置を用いてAVRにプログラムを書き込む
    1. AVR書き込み装置にAVRをセットする．
    1. AVRに書き込みたいプログラムをArduino IDEで開く．
    1. 「ツール/ボード/ATmega328P」を選択
    1. 「ツール/Clock/20MHz/X'tal」を選択
    1. 「ツール/書き込み装置/Arduino as ISP」を選択
    1. 「スケッチ/書き込み装置を使って書き込み」を選択し書き込む

#### 注意
ubuntuの場合arduino 1.8.5まで，windows10の場合arduino 1.8.9まで書き込みは成功は確認されている．  
なぜかubuntuではarduino 1.8.9で書き込みできない．  

#### 参考
https://ht-deko.com/arduino/atmega328p.html
