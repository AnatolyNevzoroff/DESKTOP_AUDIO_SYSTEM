/////////////////////// НАСТОЛЬНАЯ АУДИОСИСТЕМА / DESKTOP AUDIO SYSTEM //////////////////////////
//////////////////////////////////// ВЕРСИЯ 2.3 / VERSION 2.3 ///////////////////////////////////
////////////////////// СКЕТЧ АНАТОЛИЯ НЕВЗОРОВА / CODE BY ANATOLY NEVZOROFF /////////////////////

//РАСКОМЕНТИРОВАТЬ ДЛЯ ВКЛЮЧЕНИЯ СЕРИЙНОГО ПОРТА И ОТОБРАЖЕНИЯ КОДОВ КНОПОК 
//#define SERIAL_PORT_ENABLE 
//ЗАКОМЕНТИРОВАТЬ ДЛЯ ПОВЫШЕНИЯ БЫСТРОДЕЙСТВИЯ И ЭКОНОМИИ ПАМЯТИ В ОБЫЧНОМ РЕЖИМЕ РАБОТЫ
//КОДЫ ПУЛЬТА МОЖНО ПОЛУЧИТЬ В МОНИТОРЕ ПОРТА
#define IR_1 0x8F807 // ГРОМКОСТЬ "+"  
#define IR_2 0x802FD // ГРОМКОСТЬ "-"  
#define IR_3 0x8F00F // ВВЕРХ ПО МЕНЮ  
#define IR_4 0x808F7 // ВНИЗ ПО МЕНЮ   
#define IR_5 0x8827D // ПРЕДЫДУЩЕЕ МЕНЮ
#define IR_6 0x8E817 // СЛЕДУЮЩЕЕ МЕНЮ
#define IR_7 0x800FF // POWER / STANDBY
#define IR_8 0x818E7 // MUTE ON / OFF
#define IR_9 0x8D827 // ПОДМЕНЮ (СОВМЕСТНО С ДЛИТЕЛЬНЫМ НАЖАТИЕМ КНОПКИ ЭНКОДЕРА)
#define IR_10 0xFFFFFFFF // КОМАНДА ПОВТОРА ДЛЯ ИДЕНТИФИКАЦИИ ПОВТОРОВ И ИХ БЛОКИРОВКИ 
#define IR_11 0x8B04F // КНОПКА "FM RADIO"
#define IR_12 0x840BF // НАУШНИКИ ИЛИ УСИЛИТЕЛЬ  


#define SUBMENU_key 5      //D5 КНОПКА ПОДМЕНЮ (КНОПКА ЭНКОДЕРА SW)
#define POWER_key 6        //D6 КНОПКА ВКЛЮЧЕНИЯ 
#define MENU_key 7         //D7 КНОПКА МЕНЮ 
#define MUTE_key 8         //D8 КНОПКА ТИШИНА MUTE LED ON/OFF
#define PHONE_key 9        //D9 КНОПКА ВКЛЮЧЕНИЯ НАУШНИКОВ HEADPHONES ON/OFF
#define MUTE_LED_out 10    //D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
#define BT_PWR_out 11      //D11 ПИТАНИЕ И ИНДИКАТОР BLUETOOTH МОДУЛЯ ON/OFF & LED (Синий)
#define POWER_LED_out 12   //D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
#define POWER_RELAY_out 13 //D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
#define PHONE_LED_out 14   //D14 А0 ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ HEADPHONES ON/OFF & LED (Зелёный)
#define AMP_ST_out 15      //D15 А1 ВКЛЮЧЕНИЕ УСИЛИТЕЛЕЙ С ЗАДЕРЖКОЙ 0,7SEC ON/OFF (Оранжевый)
//#define DS18B20_input 16 //D16 А2 ВХОД ТЕРМОДАТЧИКА DS18B20 (ВХОД УЖЕ УКАЗАН ДЛЯ БИБЛИОТЕКИ)
#define DAC_PWR_out 17     //D17 А3 ПИТАНИЕ И ИНДИКАТОР ВСТРОЕННОГО ЦАП


#define p10000 10000L //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА ТИП ДАННЫХ "L" (long)
#define p2000 2000L   //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define p1000 1000L   //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define p700 700L     //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ УСИЛИТЕЛЯ
#define p500 500L     //ВРЕМЯ ОБНОВЛЕНИЯ ЭКРАНА 0,5 СЕК.
//#define p200 200L     //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА
#define p100 100L     //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА
//#define p50 50L       //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА
#define ledOFF lcd.noBacklight() //ОТКЛЮЧИТЬ ПОДСВЕТКУ
#define ledON lcd.backlight()    //ВКЛЮЧИТЬ ПОДСВЕТКУ
 

////////////////////////////////// ПОДКЛЮЧАЕМЫЕ БИБЛИОТЕКИ //////////////////////////////////////
//#include <Wire.h>//ОБЕСПЕЧЕНИЕ ПЕРЕДАЧИ ДАННЫХ ПО ШИНЕ I2C (IIC) (ЭКРАН И ЧАСЫ)
#include <EEPROM.h>//ЧТЕНИЕ И ЗАПИСЬ ПЕРЕМЕННЫХ В ЭНЕРГОНЕЗАВИСИМУЮ ПАМЯТЬ EEPROM
#include <LiquidCrystal_I2C.h>//ПЕЧАТЬ СИМВОЛОВ НА ЖИДКОКРИСТАЛИЧЕСКОМ ЭКРАНЕ ЧЕРЕЗ I2C
#include <Encoder.h>//ОБРАБОТКА ПОВОРОТА ЭНКОДЕРА
#include <DS3231.h>// http://www.rinkydinkelectronics.com/download.php?f=DS3231.zip (ЕСТЬ В АРХИВЕ)
#include <OneWire.h>// https://github.com/milesburton/Arduino-Temperature-Control-Library 
#include <RDA5807.h>// https://github.com/pu2clr/RDA5807 БАЗОВАЯ БИБЛИОТЕКА (МОД ЕСТЬ В АРХИВЕ)
#include <IRremote.h>// ВНИМАНИЕ!!! ИСПОЛЬЗУЕТСЯ СТАРАЯ ВЕРСИЯ БИБЛИТЕКИ 2.2.3. (ЕСТЬ В АРХИВЕ)
#include <BD37534FV.h>// https://github.com/liman324/BD37534FV/archive/master.zip

LiquidCrystal_I2C lcd(0x27,20,4);//УСТАНАВЛИВАЕМ АДРЕС, ЧИСЛО СИМВОЛОВ, СТРОК У ЖКИ ЭКРАНА
IRrecv irrecv(2);//УКАЗЫВАЕМ ВЫВОД "2" К КОТОРОМУ ПОДКЛЮЧЁН IR ПРИЁМНИК (INPUT INFRARED RECEIVER)
decode_results ir;//ОБЪЯВЛЯЕМ ОБЪЕКТ "ir" ДЛЯ КЛАССА БИБЛИОТЕКИ ИНФРАКРАСНОГО ПРИЁМНИКА
Encoder myEnc(3,4);// DT и CLK ИЛИ S1 и S2 ВЫВОДЫ ЭНКОДЕРА
DS3231 rtc(SDA,SCL);Time wt;//ПОДКЛЮЧАЕМ МОДУЛЬ ЧАСОВ; ОБЪЯВЛЯЕМ ОБЪЕКТ "wt" ДЛЯ КЛАССА ЧАСОВ
OneWire ds(16);//ОБЪЯВЛЯЕМ ОБЪЕКТ "ds", ДЛЯ ЧТЕНИЯ ТЕМПЕРАТУРЫ С ПОРТА "16" (А2) DS18B20
RDA5807 rx; //ОБЪЯВЛЯЕМ ОБЪЕКТ "rx" ДЛЯ КЛАССА БИБЛИОТЕКИ FM ТЮНЕРА RDA5807
BD37534FV bd;//ОБЪЯВЛЯЕМ ОБЪЕКТ "bd" ДЛЯ БИБЛИОТЕКИ АУДИОПРОЦЕССОРА BD37534

//unsigned long от 0 до 4294967295
uint32_t irkey;
uint32_t mill,timer_autoret,timer1,timer2,timer3,timer_rds;//ТАЙМЕРЫ ДЛЯ ФУНКЦИИ millis()

//int от -32768 до 32767  
int16_t trig,channel[20],freqFM;//

//byte от 0 до 255
//МАССИВЫ ДЛЯ ПОЗИЦИОНИРОВАНИЯ КУРСОРА В ТЕХНИЧЕСКОМ МЕНЮ 
uint8_t  poz[14]={0,3,6,9,12,17,  4,17,  2,8,2,8,  18,18};//ПО НОМЕРУ СИМВОЛА В СТРОКЕ
uint8_t line[14]={0,0,0,0, 0, 0,  1, 1,  2,2,3,3,   2, 3};//ПО НОМЕРУ СТРОКИ
uint8_t Connect[5],gain,input;
uint8_t setime,sethour,setmin,setsec,setdate,setmon,setyear,setdow;
uint8_t led,wait,power,ar,flag,num,volFM,sm4;//
uint8_t btn,POWER_btn,MUTE_btn,MENU_btn,PHONE_btn,SUBMENU_btn;

//char от -128 до 127    
int8_t vol,fad,volfad,treb,midd,bass,graf,rf,lf,rt,lt;
int8_t menu,trig1,sm0,sm1,sm2,sm5,phone,mute,oldP,newP,temp;
int8_t lon,lon_f,treb_c,mid_c,bas_c,sub_f,treb_q,mid_q,bas_q,vol_d,graf_d;
//char *stationName;trig0,trig2,
//gain1,gain2,gain3,gain4,gain5,

//СТРУКТУРА ДЛЯ ЭКОНОМИИ ПАМЯТИ НА ФЛАГАХ
struct flags{bool F0:1,F1:1,F2:1,F3:1,F4:1,F5:1,F6:1,F7:1,F8:1;};flags S;


//////////////////////////////////////////// SETUP ////////////////////////////////////////////////
void setup(){
//ЧИТАЕМ ПЕРЕМЕННЫЕ ИЗ EEPROM 
vol=EEPROM.read(0)-79;treb=EEPROM.read(1)-20;midd=EEPROM.read(2)-20;bass=EEPROM.read(3)-20;
input=EEPROM.read(4);lon=EEPROM.read(9);lon_f=EEPROM.read(10);rf=EEPROM.read(11)-79;
lf=EEPROM.read(12)-79;rt=EEPROM.read(13)-79;lt=EEPROM.read(14)-79;fad=EEPROM.read(15);
treb_c=EEPROM.read(16);mid_c=EEPROM.read(17);bas_c=EEPROM.read(18);treb_q=EEPROM.read(20);
mid_q=EEPROM.read(21);bas_q=EEPROM.read(22);led=EEPROM.read(23);
wait=EEPROM.read(25);phone=EEPROM.read(27);
num=EEPROM.read(90);volFM=EEPROM.read(91);
for(byte b=1;b<6;b++){Connect[b]=EEPROM.read(29+b);}
for(byte c=0;c<20;c++){channel[c]=word(EEPROM.read(50+c),EEPROM.read(70+c));}
//gain=EEPROM.read(28);


#ifdef SERIAL_PORT_ENABLE 
Serial.begin(9600); 
#endif

//Wire.begin();//ЗАПУСКАЕМ ШИНУ I2C (IIC)
irrecv.enableIRIn();//ИНИЦИАЛИЗИРУЕМ БИБЛИОТЕКУ ИНФРАКРАСНОГО ПРИЁМНИКА
rtc.begin();//ЗАПУСКАЕМ МОДУЛЬ ЧАСОВ
lcd.init();//ЗАПУСКАЕМ ЖИДКОКРИСТАЛИЧЕСКИЙ ЭКРАН


pinMode(SUBMENU_key,INPUT);     //D5 КНОПКА ПОДМЕНЮ (Кнопка SW энкодера)
pinMode(POWER_key,INPUT_PULLUP);//D6 КНОПКА ВКЛЮЧЕНИЯ 
pinMode(MENU_key,INPUT_PULLUP); //D7 КНОПКА МЕНЮ 
pinMode(MUTE_key,INPUT_PULLUP); //D8 КНОПКА ТИШИНА 
pinMode(PHONE_key,INPUT_PULLUP);//D9 КНОПКА ВКЛЮЧЕНИЯ НАУШНИКОВ HEADPHONES ON/OFF
pinMode(MUTE_LED_out,OUTPUT);   //D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
pinMode(BT_PWR_out,OUTPUT); //D11 ПИТАНИЕ И ИНДИКАТОР BLUETOOTH МОДУЛЯ ON/OFF & LED (Синий)
pinMode(POWER_LED_out,OUTPUT);  //D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
pinMode(POWER_RELAY_out,OUTPUT);//D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
pinMode(PHONE_LED_out,OUTPUT);  //D14 А0 ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ ON/OFF & LED (Зелёный)
pinMode(AMP_ST_out,OUTPUT);     //D15 А1 ВКЛЮЧЕНИЕ УСИЛИТЕЛЕЙ С ЗАДЕРЖКОЙ 0,7SEC (Оранжевый)
//pinMode(16,INPUT);   //D16 А2  ВХОД ТЕРМОДАТЧИКА DS18B20    DS18B20_input

//digitalWrite(POWER_RELAY_out,LOW);//ОТКЛЮЧАЕМ ПИТАНИЕ БП 
S.F2=true;
}//END SETUP




/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

newP=myEnc.read()/4;//ЧТЕНИЕ ПОЛОЖЕНИЯ ЭНКОДЕРА 

mill=millis();//ПЕРЕЗАПИСЬ ПЕРЕМЕННОЙ mill



//////////////////////////////// ЧТЕНИЕ ДАННЫХ С ИК ПРИЕМНИКА ///////////////////////////////
if(irrecv.decode(&ir)){
  
#ifdef SERIAL_PORT_ENABLE 
Serial.print("0x");Serial.println(ir.value,HEX); 
#endif
//МИГАЕМ ИНДИКАТОРОМ MUTE О ПРИЁМЕ СИГНАЛА С ПДУ, ПОСЛЕ НАСТРОЙКИ МОЖНО ЗАКОМЕНТИРОВАТЬ
flag=true;digitalWrite(MUTE_LED_out,HIGH);

if(ir.value!=IR_10){irkey=ir.value;delay(150);}//ПРЕДОТВРАЩАЕМ СЛУЧАЙНЫЕ ПОВТОРЫ ОТ НАЖАТИЯ
irrecv.resume();}

//МИГАЕМ ИНДИКАТОРОМ MUTE О ПРИЁМЕ СИГНАЛА С ПДУ, ПОСЛЕ НАСТРОЙКИ МОЖНО ЗАКОМЕНТИРОВАТЬ
if(flag==true){digitalWrite(MUTE_LED_out,LOW);flag=false;}



/////////////////////////////////// КНОПКА + ПДУ POWER ON/OFF //////////////////////////////////
//ЛОГИЧЕСКИЙ "0" - НАЖАТА (ПИН ЗАКОРОЧЕН НА ЗЕМЛЮ), ЕСЛИ "1" - НЕ НАЖАТА, (ПИН В ВОЗДУХЕ) \
ХОРОШИЙ АЛГОРИТМ ДЛЯ "ПЛОХИХ" (НЕ ПРЕДНАЗНАЧЕННЫХ ДЛЯ СЛАБОТОЧНОЙ КОММУТАЦИИ) КНОПОК
btn=digitalRead(POWER_key);
if(btn!=POWER_btn||ir.value==IR_7){delay(40);btn=digitalRead(POWER_key);
if(!btn&&POWER_btn||ir.value==IR_7){power=!power;S.F2=true;cl_ir();}
POWER_btn=btn;}



//////////////////////////////////////////// POWER ON ////////////////////////////////////////////
////////////////////// ЗАПУСКАЕМ ОСНОВНОЙ РАБОЧИЙ РЕЖИМ ДЛЯ АУДИОСИСТЕМЫ /////////////////////////
if(power==true){
//ЗАДАЁМ НАЧАЛЬНЫЕ ПАРАМЕТРЫ ДЛЯ РЕЖИМА "POWER ON"
if(S.F2==true){
mute=0;menu=100;S.F0=false;S.F4=true;S.F5=false;cl_enc();bigdigres();gain=Connect[input];
lcd.noCursor();lcd.noBlink();//СБРАСЫВАЕМ РЕЖИМЫ ПОДЧЁРКИВАНИЯ И МИГАНИЯ
headphone();//УПРАВЛЯЕМ ПИТАНИЕМ, УСИЛИТЕЛЕЙ И ИНДИКАТОРОМ НАУШНИКОВ 
ledON;//БЕЗОГОВОРОЧНО ВКЛЮЧАЕМ ПОДСВЕТКУ ЭКРАНА
Start_RDA5807M();//ЗАПУСКАЕМ РАДИОМОДУЛЬ
digitalWrite(POWER_LED_out,HIGH);//ВКЛЮЧАЕМ ИНДИКАТОР "POWER ON"
S.F2=false;}


//СПУСТЯ 0,7 СЕК. ПОСЛЕ ВКЛЮЧЕНИЯ БЛОКА ПИТАНИЯ ВЫВОДИМ УСИЛИТЕЛИ ИЗ РЕЖИМА STANDBY
if(S.F7==true&&mill>timer3+p700){
lcd.clear();menu=0;sm0=0;S.F4=true;if(!phone){digitalWrite(AMP_ST_out,HIGH);}
S.F7=false;}


/////////////////////////////////////////// MUTE KEY ///////////////////////////////////////////
////////// КНОПКА "ТИШИНА", В ЭТОМ РЕЖИМЕ РАБОТАЮТ ТОЛЬКО КНОПКИ "POWER ON" И "MUTE" ///////////
btn=digitalRead(MUTE_key);
if(btn!=MUTE_btn||ir.value==IR_8){delay(30);btn=digitalRead(MUTE_key);
if(!btn&&MUTE_btn||ir.value==IR_8){mute=!mute;audio();cl_ir();S.F4=true;lcd.clear();
switch(mute){
case 0: menu=0;cl_enc();S.F4=true;digitalWrite(MUTE_LED_out,LOW);break;//MUTE OFF 
case 1: menu=20;S.F0=false;break;}}//MUTE ON  
MUTE_btn=btn;}

//////////////////////////////////// ЕСЛИ "MUTE" НЕАКТИВНО //////////////////////////////////////
//ПЕРЕКЛЮЧЕНИЕ МЕЖДУ МЕНЮ
if(mute==false){
//КНОПКА + ПДУ "СЛЕДУЩЕЕ МЕНЮ"
btn=digitalRead(MENU_key);
if(btn!=MENU_btn||ir.value==IR_5){delay(40);btn=digitalRead(MENU_key);
if(!btn&&MENU_btn||ir.value==IR_5){menu++;if(menu>2){menu=0;}timer1=mill;
S.F4=true;S.F6=true;lcd.clear();cl_ir();cl_tmr();}
MENU_btn=btn;}
//ПДУ "ПРЕДЫДУЩЕЕ МЕНЮ" 
if(ir.value==IR_6){
menu--;if(menu<0){menu=2;}if(menu>2){menu=0;}S.F4=true;lcd.clear();cl_tmr();
cl_ir();}


//ВЫЗОВ МЕНЮ УПРАВЛЕНИЯ РАДИО УДЕРЖИВАЯ КНОПКУ "MENU" БОЛЕ 0,5 СЕК. 
if(S.F6==true&&MENU_btn==LOW&&mill-timer1>p500){menu=10;S.F4=true;lcd.clear();
S.F6=false;}
//ВЫЗОВ МЕНЮ УПРАВЛЕНИЯ РАДИО НАЖАТИЕМ КНОПКИ НА ПДУ 
if(ir.value==IR_11){if(menu!=10){menu=10;}else{menu=0;}S.F4=true;lcd.clear();
ir.value=0;}


//ПЕРЕКЛЮЧЕНИЕ АУДИОСИГНАЛА МЕЖДУ УСИЛИТЕЛЕМ ДЛЯ НАУШНИКОВ И УСИЛИТЕЛЕМ МОЩНОСТИ 
btn=digitalRead(PHONE_key);//HEADPHONES ON/OFF 
if(btn!=PHONE_btn||ir.value==IR_12){delay(40);btn=digitalRead(PHONE_key);
if(btn==LOW&&PHONE_btn==HIGH||ir.value==IR_12){phone=!phone;menu=100;
headphone();}
cl_ir();PHONE_btn=btn;}
}//END GROUP BUTTONS IN "MUTE OFF" MODE


//В ЗАВИСИМОСТИ ОТ ВЫБРАННОГО ВХОДА ЗАПИТЫВАЕМ ИЛИ ОТКЛЮЧАЕМ ПИТАНИЕ ДЛЯ ИСТОЧНИКА СИГНАЛА \
FM ТЮНЕР RDA5807 ПОТРЕБЛЯЕТ КРАЙНЕ МАЛО, ЕГО МОЖНО НЕ ОБЕСТОЧИВАТЬ, А ВЫКЛЮЧИТЬ ПРОГРАММНО
switch(input){
  case 1:digitalWrite(BT_PWR_out,HIGH);digitalWrite(DAC_PWR_out,LOW);break;//ВТ
  case 2:digitalWrite(DAC_PWR_out,HIGH);digitalWrite(BT_PWR_out,LOW);break;//ЦАП
  case 3:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;
  case 4:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;
  case 5:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;}




//////////////////////////////////// VOLUME -79 ... +15 dB //////////////////////////////////////
///////////////////////////////////// BASS -20 ... +20 dB ///////////////////////////////////////
//////////////////////////////////// midd -20 ... +20 dB //////////////////////////////////////
//////////////////////////////////// TREBLE -20 ... +20 dB //////////////////////////////////////
if(menu==0){
if(S.F4==true){ar=uint8_t(8);S.F1=true;
S.F4=false;}

//КНОПКОЙ ЭНКОДЕРА ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ НАСТРОЙКАМИ ЭКВАЛАЙЗЕРА
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn){delay(10);btn=digitalRead(SUBMENU_key);
if(!btn&&SUBMENU_btn){
sm0++;if(sm0>3){sm0=0;}cl_tmr();timer1=mill;S.F1=true;S.F6=true;}SUBMENU_btn=btn;}

//УДЕРЖИВАЯ КНОПКУ ЭНКОДЕРА БОЛЕЕ 0,5 СЕК. ИЛИ КНОПКОЙ ПДУ ВХОДИМ В РЕЖИМ ПЕРЕКЛЮЧЕНИЯ ВХОДОВ
if(S.F6==true&&SUBMENU_btn==LOW&&mill-timer1>p500||ir.value==IR_9){
if(sm0==4){sm0=0;}else{sm0=4;}S.F6=false;cl_ir();cl_tmr();}

//КНОПКАМИ ПДУ ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ НАСТРОЙКАМИ ЭКВАЛАЙЗЕРА
if(ir.value==IR_3){sm0++;if(sm0>3){sm0=0;}cl_ir();cl_tmr();}
if(ir.value==IR_4){sm0--;if(sm0<0){sm0=3;}cl_ir();cl_tmr();}

//ПРИСВАИВАЕМ ТЕКУЩЕЕ ЗНАЧЕНИЕ ПАРАМЕТРА ВРЕМЕННОЙ ПЕРЕМЕННОЙ
switch(sm0){
  case 0:volfad=vol+fad;trig1=volfad;break;
  case 1:trig1=treb;break;
  case 2:trig1=midd;break;
  case 3:trig1=bass;break;
  case 4:trig1=input;break;}

//МЕНЯЕМ ВРЕМЕННУЮ ПЕРЕМЕННУЮ ЭНКОДЕРОМ ИЛИ КНОПКАМИ ПДУ
if(newP!=oldP){trig1=trig1+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig1++;cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig1--;cl_ir();cl_tmr();}

//ВЫВОДИМ ПАРАМЕТРЫ НА ЭКРАН
if(S.F1==true){MAIN_MENU();audio();S.F1=false;}
Temperature();lcd.setCursor(8,3);lcd.print(temp);lcd.write(B11011111);
}//END MAIN MENU 0




///////////////////////////// BASS FREQUENCY + BASS QUALITY FACTOR /////////////////////////////
/////////////////////////// midd FREQUENCY + midd QUALITY FACTOR ///////////////////////////
/////////////////////////// TREBLE FREQUENCY + TREBLE QUALITY FACTOR ///////////////////////////
/////////////////////////////// LOUDNESS CENTER + LOUDNESS GAIN ////////////////////////////////
if(menu==1){
if(S.F4==true){ar=uint8_t(7);S.F1=true;S.F4=false;S.F5=true;}
ENCODER_KEY();
if(S.F5==true&&newP!=oldP){
sm1=sm1+newP;if(sm1>7){sm1=0;}if(sm1<0){sm1=7;}ar=uint8_t(7);cl_enc();cl_tmr();}
if(ir.value==IR_3){sm1++;if(sm1>7){sm1=0;}ar=uint8_t(7);cl_ir();cl_tmr();}
if(ir.value==IR_4){sm1--;if(sm1<0){sm1=7;}ar=uint8_t(7);cl_ir();cl_tmr();}
  switch(sm1){
    case 0:trig1=treb_c;break;
    case 1:trig1=treb_q;break;
    case 2:trig1=mid_c;break;
    case 3:trig1=mid_q;break;
    case 4:trig1=bas_c;break;
    case 5:trig1=bas_q;break;
    case 6:trig1=lon_f;break;
    case 7:trig1=lon;break;}
if(S.F5==false&&newP!=oldP){trig1=trig1+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig1++;ar=uint8_t(8);cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig1--;ar=uint8_t(8);cl_ir();cl_tmr();}
if(S.F1==true){MENU_equalizer();audio();
S.F1=false;}
}//END MENU 1




//////////////////////////////////////// INPUT SELECTION //////////////////////////////////////////
/////////////////////////////////////// GAIN 0 ... +20 dB /////////////////////////////////////////
if(menu==2){
if(S.F4==true){sm2=input;ar=uint8_t(7);S.F4=false;S.F5=true;MENU_input_gain();}
ENCODER_KEY();
if(S.F5==true&&newP!=oldP){
sm2=sm2+newP;if(sm2>5){sm2=1;}if(sm2<1){sm2=5;}ar=uint8_t(7);cl_enc();cl_tmr();S.F8=true;}
if(ir.value==IR_3){sm2++;if(sm2>5){sm2=1;}ar=uint8_t(7);S.F5=true;cl_ir();cl_tmr();S.F8=true;}
if(ir.value==IR_4){sm2--;if(sm2<1){sm2=5;}ar=uint8_t(7);S.F5=true;cl_ir();cl_tmr();S.F8=true;}
if(S.F8==true){trig1=Connect[sm2];S.F8=false;}
if(S.F5==false&&newP!=oldP){trig1=trig1+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig1++;ar=uint8_t(8);S.F5=false;cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig1--;ar=uint8_t(8);S.F5=false;cl_ir();cl_tmr();}
if(S.F1==true){
if(S.F5==false){
input=sm2;trig1=constrain(trig1,0,20);
Connect[input]=trig1;//ПРИСВАИВАЕМ ТЕКУЩИЙ УРОВЕНЬ ВЫБРАННОМУ ВХОДУ BD37534FV
gain=Connect[input];//УРОВЕНЬ НА ТЕКУЩЕМ ВХОДЕ ДЛЯ ПЕРЕДАЧИ В BD37534FV
audio();}
MENU_input_gain();
S.F1=false;}
}//END MENU 2



/////////////////////////////////////// MENU FM RDA5807M/////////////////////////////////////////
////////////////////////// МЕНЮ УПРАВЛЕНИЯ РАДИОМОДУЛЕМ НА RDA5807M /////////////////////////////
if(menu==10){
//ВЫБИРАЕМ ЧАСТОТУ ПОСЛЕДНЕГО НАСТРОЕННОГО КАНАЛА 
if(S.F4==true){ar=uint8_t(8);S.F0=false;S.F4=false;S.F5=true;input=3;gain=Connect[input];audio();}
//КНОПКОЙ ЭНКОДЕРА ИЛИ КНОПКАМИ ПДУ ВЫБИРАЕМ ЧТО МЕНЯТЬ КАНАЛ С ЗАПИСАННОЙ В НЕГО ЧАСТОТОЙ ИЛИ \
НЕПОСРЕДСТВЕННО ЧАСТОТУ НА ВЫБРАННОМ КАНАЛЕ (ОДИН ШАГ = 100 кГц) \
БИБЛИОТЕКА ПОЗВОЛЯЕТ МЕНЯТЬ С ШАГОМ В 10 кГц ЧАСТОТА 103,3 МГЦ ЗАПИСЫВАЕТСЯ КАК 10330
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn||ir.value==IR_3||ir.value==IR_4){delay(10);btn=digitalRead(SUBMENU_key);
if(btn==LOW&&SUBMENU_btn==HIGH||ir.value==IR_3||ir.value==IR_4){
sm4=!sm4;S.F6=true;timer1=mill;timer2=mill-p1000;ir.value=0;}
SUBMENU_btn=btn;}
switch(sm4){
case 0:trig=num;break;
case 1:trig=(freqFM/10);break;}
//ПРИВЯЗЫВАЕМ ТЕКУЩУЮ ЧАСТОТУ К НОМЕРУ ТЕКУЩЕГО КАНАЛА УДЕРЖИВАЯ КНОПКУ ЭНКОДЕРА БОЛЕЕ 0,5 СЕК. \
ИЛИ КНОПКОЙ ПДУ, ФАКТИЧЕСКИ НАДО ЗАРАННЕЕ ВЫБРАТЬ НОМЕР КАНАЛА ОТ 1 ДО 20, ПЕРЕКЛЮЧИТЬСЯ НА \
ВЫБОР ЧАСТОТЫ, ВЫБРАТЬ ЧАСТОТУ И ПОСЛЕ ЗАПИСАТЬ ЕЁ В ЯЧЕЙКУ УЖЕ ВЫБРАННОГО КАНАЛА.
if(S.F6==true&&SUBMENU_btn==LOW&&mill-timer1>p500||ir.value==IR_9){ir.value=0;
timer2=mill+p500;//ЗАДЕРЖКА ПЕЧАТИ МЕНЮ ДЛЯ ОТОБРАЖЕНИЯ "SAVED!" В ТЕЧЕНИЕ 1,5 СЕК.
channel[num]=freqFM;lcd.setCursor(6,1);lcd.print(F(" SAVED! "));
S.F6=false;}

//МЕНЯЕМ НОМЕР КАНАЛА ИЛИ ЧАСТОТУ
if(newP!=oldP){trig=trig+newP;cl_enc();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig++;cl_ir();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig--;cl_ir();}

//ЕСЛИ ИЗМЕНЁН КАНАЛ ИЛИ ЧАСТОТА ОТПРАВЛЯЕМ В RDA5807M ТЕКУЩУЮ ЧАСТОТУ
if(S.F1==true){
switch(sm4){
case 0:num=trig;if(num<1){num=20;}if(num>20){num=1;}freqFM=channel[num];break;
case 1:freqFM=(trig*10);if(freqFM<8700){freqFM=10800;}if(freqFM>10800){freqFM=8700;};break;}
rx.setFrequency(freqFM);timer2=mill-p1000;
S.F1=false;}

//ОПРАШИВАЕМ RDA5807M НА НАЛИЧИЕ R.D.S. 
if(mill-timer_rds>p100){rx.getRdsText0A();timer_rds=mill;}

//СБРОС ОЧЕРЕДИ ПАКЕТОВ R.D.S.
if(mill-timer_autoret>p10000){rx.clearRdsFifo();timer_autoret=mill;}

//ВЫВОДИМ ДАННЫЕ НА ЭКРАН
if(mill>timer2+p1000){menuFM();time_func();Temperature();timer2=mill;}
}//END MENU FM RDA5807M



//////////////////////////////////////////// MUTE MENU //////////////////////////////////////////
////////////////////////// В РЕЖИМЕ "ТИШИНА" ПЕЧАТАЕМ ОТДЕЛЬНОЕ МЕНЮ ////////////////////////////
if(menu==20){
if(mill>timer1+p1000){trig1=!trig1;
lcd.setCursor(8,0); 
switch(trig1){
case 0:lcd.print(F("    "));break;
case 1:lcd.print(F("MUTE"));break;}
time_func();
Temperature();
lcd.setCursor(17,3);lcd.print(temp);lcd.write(B11011111);timer1=mill;}
digitalWrite(MUTE_LED_out,HIGH);}



//////////////////////////////////// RETURN TO THE MAIN MENU ////////////////////////////////////
/////////////// АВТОМАТИЧЕСКИЙ ВОЗВРАТ В ГЛАВНОЕ МЕНЮ ЧЕРЕЗ ЗАДАННЫЙ ИНТЕРВАЛ ///////////////////
if(S.F0==true&&mill-timer_autoret>(wait*p1000)){if(menu!=0){lcd.clear();}
menu=0;sm0=0;S.F4=true;S.F0=false;}



}///////////////////////////////////////// END POWER ON /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// POWER OFF (STANDBY) //////////////////////////////////////
if(power==false){
if(S.F2==true){
//ЗАДАЁМ НАЧАЛЬНЫЕ ЗНАЧЕНИЯ ДЛЯ РЕЖИМА "STANDBY"
EEPROM_UPDATE();mute=1;audio();menu=false;S.F5=false;lcd.clear();
digitalWrite(AMP_ST_out,LOW);
digitalWrite(MUTE_LED_out,LOW);
digitalWrite(BT_PWR_out,LOW);
digitalWrite(POWER_LED_out,LOW);
digitalWrite(PHONE_LED_out,LOW);
digitalWrite(POWER_RELAY_out,LOW);
digitalWrite(DAC_PWR_out,LOW);
rx.powerDown();//ВЫКЛЮЧАЕМ RDA5807M
led?ledON:ledOFF;//ПОДСВЕТКА ЭКРАНА В РЕЖИМЕ STANDBY ON/OFF
big_timeres();//СИМВОЛЫ ПСЕВДОГРАФИКИ ДЛЯ БОЛЬШИХ ЧАСОВ И УКАЗАТЕЛЕЙ
S.F2=false;}

//КНОПКА MENU ДЛЯ ПЕРЕХОДА ИЗ ОСНОВНОГО В ТЕХНИЧЕСКОЕ МЕНЮ И ОБРАТНО 
btn=digitalRead(MENU_key);
if(btn!=MENU_btn||ir.value==IR_5||ir.value==IR_6){delay(30);btn=digitalRead(MENU_key);
if(btn==LOW&&MENU_btn==HIGH||ir.value==IR_5||ir.value==IR_6){menu=!menu;S.F4=true;
cl_ir();lcd.clear();}
MENU_btn=btn;}

//УПРАВЛЕНИЕ ПОДСВЕТКОЙ ЭКРАНА В РЕЖИМЕ "STANDBY" ON/OFF (КНОПКА MUTE)
btn=digitalRead(MUTE_key);
if(btn!=MUTE_btn||ir.value==IR_8){delay(30);btn=digitalRead(MUTE_key);
if(!btn&&MUTE_btn||ir.value==IR_8){led=!led;led?ledON:ledOFF;cl_ir();}
MUTE_btn=btn;}



////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// ОСНОВНОЕ МЕНЮ ЧАСЫ ЗАСТАВКА (BIG WATCH) В РЕЖИМЕ "STANDBY" ////////////////////
if(menu==false){
if(S.F4==true){S.F4=false;lcd.noCursor();lcd.noBlink();}
if(mill-timer1>p1000){timer1=mill;big_time();}
Temperature();lcd.setCursor(17,3);lcd.print(temp);lcd.write(B11011111);}



////////////////////////////////////////////////////////////////////////////////////////////////
/////// ТЕХНИЧЕСКОЕ МЕНЮ УСТАНОВКИ ВРЕМЕНИ, ДАТЫ, УРОВНЕЙ ВЫХОДНЫХ УСИЛИТЕЛЕЙ BD37534FV ////////
///// ВЫХОДА С RDA5807M И ВРЕМЕНИ АВТОМАТИЧЕСКОГО ВОЗВРАТА В ГЛАВНОЕ МЕНЮ ИЗ ДРУГИХ МЕНЮ ///////
if(menu==true){
//СЧИТЫВАЕМ И ПЕЧАТАЕМ ТЕКУЩИЕ ЗНАЧЕНИЯ ВРЕМЕНИ И ДАТЫ
if(S.F4==true){S.F5=true;setime=0;get_time();sm5=0;
//ПЕЧАТАЕМ ТЕКУЩИЕ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ
lcd.setCursor(0,0);lcd.print(rtc.getTimeStr());
lcd.setCursor(9,0);lcd.print(rtc.getDateStr());
lcd.setCursor(0,1);lcd.print(F("DAY "));lcd.print(wt.dow);
lcd.setCursor(6,1);lcd.print(F("SAVE TIME -Y+"));
lcd.setCursor(0,2);lcd.print(F("LS"));lcd.print(lf);
lcd.setCursor(6,2);lcd.print(F("RS"));lcd.print(rf);
lcd.setCursor(12,2);lcd.print(F("FM Out"));lcd.print(volFM);
lcd.setCursor(0,3);lcd.print(F("LP"));lcd.print(lt);
lcd.setCursor(6,3);lcd.print(F("RP"));lcd.print(rt);
lcd.setCursor(12,3);lcd.print(F("Return"));lcd.print(wait);
lcd.setCursor(poz[sm5],line[sm5]);lcd.blink();
S.F4=false;}

//КНОПКОЙ ЭНКОДЕРА МЕНЯЕМ МИГАНИЕ НА ПОДЧЁРКИВАНИЕ КУРСОРА 
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn){delay(10);btn=digitalRead(SUBMENU_key);
if(btn==LOW&&SUBMENU_btn==HIGH){S.F5=!S.F5;
if(S.F5==true){lcd.noCursor();lcd.blink();}else{lcd.noBlink();lcd.cursor();}}
SUBMENU_btn=btn;}

//ПОВОРОТОМ ЭНКОДЕРА МЕНЯЕМ ПОЗИЦИЮ МИГАЮЩЕГО КУРСОРА
if(S.F5==true&&newP!=oldP){sm5=sm5+newP;if(sm5<0){sm5=13;}if(sm5>13){sm5=0;}
oldP=0;newP=0;myEnc.write(0);}
if(ir.value==IR_3){sm5++;if(sm5>13){sm5=0;}S.F5=true;ir.value=0;lcd.noCursor();lcd.blink();}
if(ir.value==IR_4){sm5--;if(sm5<0){sm5=13;}S.F5=true;ir.value=0;lcd.noCursor();lcd.blink();}

switch(sm5){
    case 0:trig1=sethour;break;
    case 1:trig1=setmin;break;
    case 2:trig1=setsec;break;
    case 3:trig1=setdate;break;
    case 4:trig1=setmon;break;
    case 5:trig1=setyear;break;
    case 6:trig1=setdow;break;
    case 7:trig1=setime;break;
    case 8:trig1=lf;break;
    case 9:trig1=rf;break;
   case 10:trig1=lt;break;
   case 11:trig1=rt;break;
   case 12:trig1=volFM;break;
   case 13:trig1=wait;break;}
lcd.setCursor(poz[sm5],line[sm5]);

//МЕНЯЕМ ЗНАЧЕНИЕ ПЕРЕМЕННОЙ В ПОЗИЦИИ ПОДЧЁРКИВАНИЯ КУРСОРА 
if(S.F5==false&&newP!=oldP){trig1+=newP;cl_enc();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig1++;S.F5=false;cl_ir();
lcd.noBlink();lcd.cursor();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig1--;S.F5=false;cl_ir();
lcd.noBlink();lcd.cursor();}
if(S.F1==true){
switch(sm5){
    case 0:sethour=trig1=constrain(trig1,0,23);break;
    case 1:setmin=trig1=constrain(trig1,0,59);break;
    case 2:setsec=trig1=constrain(trig1,0,59);break;
    case 3:setdate=trig1=constrain(trig1,1,31);break;
    case 4:setmon=trig1=constrain(trig1,1,12);break;
    case 5:setyear=trig1=constrain(trig1,20,99);break;
    case 6:setdow=trig1=constrain(trig1,1,7);break;  
    case 7:setime=trig1=constrain(trig1,-1,1);break;
    case 8:lf=trig1=constrain(trig1,-79,-25);break;    
    case 9:rf=trig1=constrain(trig1,-79,-25);break;    
   case 10:lt=trig1=constrain(trig1,-79,-25);break;    
   case 11:rt=trig1=constrain(trig1,-79,-25);break;
   case 12:volFM=trig1=constrain(trig1,1,15);break;
   case 13:wait=trig1=constrain(trig1,3,32);break;}

//ПЕЧАТАЕМ ИЗМЕНИВШЕЕСЯ ЗНАЧЕНИЕ ПЕРЕМЕННОЙ В ПОЗИЦИИ КУРСОРА
lcd.print(trig1);if(abs(trig1)<10){lcd.print(' ');}//lcd.setCursor(poz[sm5],line[sm5]);

//ДЛЯ ЗАПИСИ ТЕКУЩИХ ЗНАЧЕНИЙ ВРЕМЕНИ И ДАТЫ УСТАНАВЛИВАЕМ КУРСОР В ПОЛЕ "SAVE TIME -Y+" \
В МОМЕНТ СОВПАДЕНИЯ УСТАНОВЛЕННОГО ЗНАЧЕНИЯ СЕКУНД С ЭТАЛОННЫМ ЗНАЧЕНИЕМ, ПОВОРАЧИВАЕМ \
ЭНКОДЕР В ЛЮБУЮ СТОРОНУ ИЛИ ИЛИ НАЖИМАЕМ +/- НА ПДУ И ВЫВОДИМ СООБЩЕНИЕ О ЗАПИСИ ДАННЫХ В DS3231
if(setime!=0){lcd.noCursor();
set_time();lcd.clear();lcd.setCursor(1,1);lcd.print(F("TIME & DATE SAVE!"));}timer1=mill;
S.F1=false;}

//СПУСТЯ 2 СЕКУНДЫ ОТОБРАЖЕНИЯ СООБЩЕНИЯ, ВОЗВРАЩАЕМСЯ В МЕНЮ ОТОБРАЖЕНИЯ БОЛЬШИХ ЧАСОВ
if(setime!=0&&mill-timer1>p2000){S.F4=true;menu=0;lcd.clear();}
}//END TECHNICAL MENU

}//END POWER OFF 

}//END LOOP 
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
