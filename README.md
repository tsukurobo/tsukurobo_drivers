# tsukurobo_drivers
つくろぼで使う基板の基本的なプログラム．  

## how_to_motor-driver.pdf
伊勢モードラと金城ステピモードラの（わかりにくい）資料

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
 - master：現行バージョンのマスタープログラム  
 - pre_master：前バージョンのマスタープログラム  

### slave
 - ise_motor_driver_ip：現行バージョンのマスタープログラム  
 - pre：前バージョンのスレーブプログラム  
 	 - i2c_slave：司令をそのまま出力するプログラム  
	 - pid_i2c_sleva：角度をPID制御するプログラム  
	 - pid_v_i2c_slave：角速度をPID制御するプログラム  

※~~SM(SMB)方式とLAP方式を変更できるが今(2020/11/8)は保留．~~(←伊勢モードラでは機能しない)誰か書いて  
- SM(SMB)方式：回転方向と01のデューティ比の2つの司令を送る．ショートブレーキがある  
- LAP方式：01と0-1のデューティー比を送る  
https://tattatatakemori.hatenablog.com/entry/2017/07/20/232827  
https://techweb.rohm.co.jp/motor/knowledge/basics/basics-03/206  

### 新バージョンでの変更点
 - encoder()関数とsetSpeed()関数の実行速度の高速化(内部実装を改変し、従来の大体1/3程度に短縮)  
 - I2C通信の通信速度を変更できるようにした  

### 2020/11/19での変更点
 - master側からPWMの周波数とA3921の動作モードを変更できるようにした  

### 新バージョンで新たに実装した関数(IseMotorDriverクラス)
 - void IseMotorDriver::begin(uint8_t mode) (静的メンバ関数)
 	- Wire.begin()の実行と通信速度の変更を行う  
	- 引数で通信速度を指定。STANDARD: 100kbps, FAST: 400kbps  
 - void IseMotorDriver::setMode(uint8_t freq, uint8_t mode) (通常のメンバ関数)  
 	- PWM周波数とA3921の動作モードの変更を行う  
	- 第一引数はPWM_1KHZ, PWM_8KHZ, PWM_64KHZのいずれか。各々PWM周波数を1khz, 8khz, 64khzにすることを表す  
	- 第二引数はSM_BRAKE_LOW_SIDE, SM_BRAKE_HIGH_SIDE, SM_COAST, LAPのいずれか。左から順にSM方式駆動(ショートブレーキあり(high side recirculation/low side recirculation)、ショートブレーキ無し)、LAP方式駆動  
	- **(注意)伊勢モードラでは配線の都合上LAPが(実質的に)機能しないので、引数にLAPを指定することは推奨されない**  

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
