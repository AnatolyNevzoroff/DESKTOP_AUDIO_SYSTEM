/////////////////////// НАСТОЛЬНАЯ АУДИОСИСТЕМА / DESKTOP AUDIO SYSTEM //////////////////////////
////////////////////////// ВЕРСИЯ 2.8 ATMEGA328 / VERSION 2.8 ATMEGA328 /////////////////////////
////////////////////// СКЕТЧ АНАТОЛИЯ НЕВЗОРОВА / CODE BY ANATOLY NEVZOROFF /////////////////////


//КОДЫ ПУЛЬТА МОЖНО ПОЛУЧИТЬ ЗАГРУЗИВ СКЕТЧ Buttons_Codes_IR_Remote
#define IR_1 0x8F807  // ГРОМКОСТЬ "+"  
#define IR_2 0x802FD  // ГРОМКОСТЬ "-"  
#define IR_3 0x8F00F  // ВВЕРХ ПО МЕНЮ  
#define IR_4 0x808F7  // ВНИЗ ПО МЕНЮ   
#define IR_5 0x8827D  // ПРЕДЫДУЩЕЕ МЕНЮ
#define IR_6 0x8E817  // СЛЕДУЮЩЕЕ МЕНЮ
#define IR_7 0x800FF  // POWER / STANDBY
#define IR_8 0x858A7  // MUTE ON / OFF 
#define IR_9 0x8D827  // ПОДМЕНЮ (СОВМЕСТНО С ДЛИТЕЛЬНЫМ НАЖАТИЕМ КНОПКИ ЭНКОДЕРА)
#define IR_10 0xFFFFFFFF // КОМАНДА ПОВТОРА ДЛЯ ИДЕНТИФИКАЦИИ ПОВТОРОВ И ИХ БЛОКИРОВКИ 
#define IR_11 0x87887 //  КНОПКА "FM RADIO" 
#define IR_12 0x8B847 //  НАУШНИКИ ИЛИ УСИЛИТЕЛЬ 


#define SUBMENU_key 5      //D5 КНОПКА ПОДМЕНЮ (КНОПКА ЭНКОДЕРА SW)
#define POWER_key 6        //D6 КНОПКА ВКЛЮЧЕНИЯ 
#define MENU_key 7         //D7 КНОПКА МЕНЮ 
#define MUTE_key 8         //D8 КНОПКА ТИШИНА MUTE LED ON/OFF
#define RADIO_key 9        //D9 КНОПКА ВКЛЮЧЕНИЯ ВСТРОЕННОГО FM РАДИОПРИЁМНИКА
#define MUTE_LED_out 10    //D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
#define BT_PWR_out 11      //D11 ПИТАНИЕ И ИНДИКАТОР BLUETOOTH МОДУЛЯ ON/OFF & LED (Синий)
#define POWER_LED_out 12   //D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
#define POWER_RELAY_out 13 //D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
#define PHONE_LED_out 14   //D14 А0 ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ HEADPHONES ON/OFF & LED (Зелёный)
#define AMP_ST_out 15      //D15 А1 ВКЛЮЧЕНИЕ УСИЛИТЕЛЕЙ С ЗАДЕРЖКОЙ 0,7SEC ON/OFF (Оранжевый)
//#define DS18B20_input 16 //D16 А2 ВХОД ТЕРМОДАТЧИКА DS18B20 (ВХОД УКАЗАН ДЛЯ БИБЛИОТЕКИ)
#define DAC_PWR_out 17     //D17 А3 ПИТАНИЕ И ИНДИКАТОР ВСТРОЕННОГО ЦАП
#define IR_LED_out 20      //D20 А6 ПОДСВЕТКА КНОПКИ FM (ИНДИКАТОР ДЛЯ СИГНАЛОВ С ПДУ)

#define p10000 10000L //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА ТИП ДАННЫХ "L" (long)
#define p2000 2000L   //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define p1000 1000L   //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА 
#define p700 700L     //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ УСИЛИТЕЛЯ
#define p500 500L     //ВРЕМЯ ОБНОВЛЕНИЯ ЭКРАНА 0,5 СЕК.
#define p100 100L     //СЛУЖЕБНАЯ ЗАДЕРЖКА ДЛЯ ТАЙМЕРА
#define ledOFF lcd.noBacklight() //ОТКЛЮЧИТЬ ПОДСВЕТКУ
#define ledON lcd.backlight()    //ВКЛЮЧИТЬ ПОДСВЕТКУ
#define minFM 8700 //МИНИМАЛЬНАЯ ЧАСТОТА
#define maxFM 10800 //МАКСИМАЛЬНАЯ ЧАСТОТА

////////////////////////////////// ПОДКЛЮЧАЕМЫЕ БИБЛИОТЕКИ //////////////////////////////////////
//#include <Wire.h>//ОБЕСПЕЧЕНИЕ ПЕРЕДАЧИ ДАННЫХ ПО ШИНЕ I2C (IIC) (АП, ЭКРАН И ЧАСЫ)
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
RDA5807 rx; //ОБЪЯВЛЯЕМ ОБЪЕКТ "rx" ДЛЯ КЛАССА БИБЛИОТЕКИ FM ТЮНЕРА RDA5807M
BD37534FV bd;//ОБЪЯВЛЯЕМ ОБЪЕКТ "bd" ДЛЯ БИБЛИОТЕКИ АУДИОПРОЦЕССОРА BD37534FV




//unsigned long от 0 до 4294967295
uint32_t irkey;//КОДЫ КНОПОК ПДУ
//ТАЙМЕРЫ ДЛЯ ФУНКЦИИ millis()
uint32_t mill,timer_autoret,timer_BTN,timer_MENU,timer_AMP,timer_RDS,timer_IR;
//int от -32768 до 32767  
int16_t channel[21],trig,freqFM;//МАССИВ И ПЕРЕМЕННЫЕ ДЛЯ РАДИО НЕ ПОМЕЩАЮЩИЕСЯ В ТИП byte

//byte от 0 до 255
//МАССИВЫ ДЛЯ ПОЗИЦИОНИРОВАНИЯ КУРСОРА В ТЕХНИЧЕСКОМ МЕНЮ 
uint8_t  poz[14]={0,3,6,9,12,17,  4,17,  2,8,2,8,  18,18};//ПО НОМЕРУ СИМВОЛА В СТРОКЕ
uint8_t line[14]={0,0,0,0, 0, 0,  1, 1,  2,2,3,3,   2, 3};//ПО НОМЕРУ СТРОКИ
uint8_t Connect[6],gain,input;//МАССИВ И ПЕРЕМЕННЫЕ ДЛЯ ЧУСТВИТЕЛЬНОСТИ ВХОДОВ
uint8_t setime,sethour,setmin,setsec,setdate,setmon,setyear,setdow;
uint8_t led,wait,power,ar,flag,num,volFM,BasFM,sm10,menu_save;
uint8_t btn,POWER_btn,MUTE_btn,MENU_btn,RADIO_btn,SUBMENU_btn;

//char от -128 до 127    
int8_t vol,fad,volfad,treb,midd,bass,graf,mute,rf,lf,rt,lt;
int8_t menu,var0,var1,var2,var3,var4,sm0,sm1,sm2,sm4,phone,oldP,newP,temp;
int8_t lon,lon_f,treb_c,mid_c,bas_c,sub_f,treb_q,mid_q,bas_q,vol_d,graf_d;

//ФЛАГИ
byte SF0,SF1,SF2,SF3,SF4,SF5,SF6,SF7,SF8,btn_flag;




//////////////////////////////////////////// SETUP ////////////////////////////////////////////////
void setup(){
//ЧИТАЕМ ПЕРЕМЕННЫЕ ИЗ EEPROM 
vol=EEPROM.read(0)-79;treb=EEPROM.read(1)-20;midd=EEPROM.read(2)-20;bass=EEPROM.read(3)-20;
input=EEPROM.read(4);num=EEPROM.read(5);volFM=EEPROM.read(6);fad=EEPROM.read(7);
phone=EEPROM.read(8);
lon=EEPROM.read(9);lon_f=EEPROM.read(10);
rf=EEPROM.read(11)-79;lf=EEPROM.read(12)-79;rt=EEPROM.read(13)-79;lt=EEPROM.read(14)-79;
treb_c=EEPROM.read(16);mid_c=EEPROM.read(17);bas_c=EEPROM.read(18);
treb_q=EEPROM.read(19);mid_q=EEPROM.read(20);bas_q=EEPROM.read(21);
led=EEPROM.read(22);wait=EEPROM.read(23);
for(byte d=0;d<6;d++){Connect[d]=EEPROM.read(40+d);}
for(byte b=0;b<21;b++){channel[b]=word(EEPROM.read(50+b),EEPROM.read(70+b));}

//Wire.begin();//ЗАПУСКАЕМ ШИНУ I2C (IIC) (ЗАПУСКАЕТСЯ ИЗ ДРУГИХ БИБЛИОТЕК)
irrecv.enableIRIn();//ИНИЦИАЛИЗИРУЕМ БИБЛИОТЕКУ ИНФРАКРАСНОГО ПРИЁМНИКА
rtc.begin();//ЗАПУСКАЕМ МОДУЛЬ ЧАСОВ
lcd.init();//ЗАПУСКАЕМ ЖИДКОКРИСТАЛИЧЕСКИЙ ЭКРАН


pinMode(SUBMENU_key,INPUT);     //D5 КНОПКА ПОДМЕНЮ (Кнопка SW энкодера)
pinMode(POWER_key,INPUT_PULLUP);//D6 КНОПКА ВКЛЮЧЕНИЯ 
pinMode(MENU_key,INPUT_PULLUP); //D7 КНОПКА МЕНЮ 
pinMode(MUTE_key,INPUT_PULLUP); //D8 КНОПКА ТИШИНА 
pinMode(RADIO_key,INPUT_PULLUP);//D9 КНОПКА ВКЛЮЧЕНИЯ ВСТРОЕННОГО FM РАДИОПРИЁМНИКА
pinMode(MUTE_LED_out,OUTPUT);   //D10 ИНДИКАТОР ОТКЛЮЧЕНИЯ ЗВУКА MUTE LED ON/OFF (Жёлтый)
pinMode(BT_PWR_out,OUTPUT);     //D11 ПИТАНИЕ И ИНДИКАТОР BLUETOOTH МОДУЛЯ ON/OFF & LED (Синий)
pinMode(POWER_LED_out,OUTPUT);  //D12 ИНДИКАТОР ВКЛЮЧЕНИЯ POWER LED ON/OFF (Красный)
pinMode(POWER_RELAY_out,OUTPUT);//D13 ПИТАНИЕ УСИЛИТЕЛЯ POWER RELAY ON/OFF (Белый)
pinMode(PHONE_LED_out,OUTPUT);  //D14 А0 ПИТАНИЕ И ИНДИКАТОР НАУШНИКОВ ON/OFF & LED (Зелёный)
pinMode(AMP_ST_out,OUTPUT);     //D15 А1 ВКЛЮЧЕНИЕ УСИЛИТЕЛЕЙ С ЗАДЕРЖКОЙ 0,7SEC (Оранжевый)
//pinMode(16,INPUT);   //D16 А2  ВХОД ТЕРМОДАТЧИКА DS18B20 (ВХОД УКАЗАН В КЛАССЕ БИБЛИОТЕКИ)
pinMode(IR_LED_out,OUTPUT);     //D17 А3 ПОДСВЕТКА КНОПКИ FM (ИНДИКАТОР ДЛЯ СИГНАЛОВ С ПДУ)




///////////////////////////////////// В Н И М А Н И Е !!! ////////////////////////////////////////
//ПЕРЕД ЗАГРУЗКОЙ СКЕТЧА В МК РЕКОМЕНДУЮ УСТАНОВИТЬ НАЧАЛЬНЫЕ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ, \
НУЛЕВЫЕ ИЛИ ОТРИЦАТЕЛЬНЫЕ ЗНАЧЕНИЯ ОСТАВШИЕСЯ В ПАМЯТИ МК МОГУТ ВЫЗВАТЬ "СТРАННОЕ" \
ПОВЕДЕНИЕДЛЯ МК, ДЛЯ ЭТОГО: \
1. РАСКОММЕНТИРУЙТЕ ФУНКЦИЮ First_start(); \
2. ЗАГРУЗИТЕ СКЕТЧ В МК \
3. ДВАЖДЫ НАЖМИТЕ КНОПКУ "POWER" (ЗАПИСАВ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ В EEPROM МК) \
4. ЗАКОМЕНТИРУЙТЕ ИЛИ УДАЛИТЕ СТРОКУ С ФУНКЦИЕЙ First_start(); \
5. ПОВТОРНО ЗАГРУЗИТЕ СКЕТЧ В МК \
6. ПОЛЬЗУЙТЕСЬ!

//First_start();


SF2=true;//ПОДНИМАЕМ ФЛАГ ДЛЯ НАЧАЛЬНОЙ ЗАГРУЗКИ ПАРАМЕТРОВ В РЕЖИМЕ "POWER ON"
}//END SETUP




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

newP=myEnc.read()/4;//ЧТЕНИЕ ПОЛОЖЕНИЯ ЭНКОДЕРА 

mill=millis();//ПЕРЕЗАПИСЬ ПЕРЕМЕННОЙ mill



//////////////////////////////// ЧТЕНИЕ ДАННЫХ С ИК ПРИЕМНИКА ///////////////////////////////
if(irrecv.decode(&ir)){//ЗАПРАШИВАЕМ ПОЛУЧЕННЫЙ ИК ПРИЁМНИКОМ КОД ПДУ
  
//ЗАЖИГАЕМ ИНДИКАТОР НА КНОПКЕ ПОЛУЧИВ СИГНАЛ С ПДУ
digitalWrite(IR_LED_out,HIGH);flag=true;

//ПРЕДОТВРАЩАЕМ СЛУЧАЙНЫЕ ПОВТОРЫ ОТ НАЖАТИЯ НА ЛЮБУЮ КНОПКУ ПДУ, СОЗДАЁМ ЗАДЕЖКУ ДЛЯ
//ДЛЯ МК ДЛИТЕЛЬНОСТЬЮ 150 мс, ДАЛЕЕ ВЫПОЛНЯЕМ КОДЫ ПОВТОРА С ЧАСТОТОЙ ЗАДАННОЙ ПДУ
if(ir.value!=IR_10){irkey=ir.value;delay(150);}

//СБРАСЫВАЕМ ТЕКУЩЕЕ ЗНАЧЕНИЕ ПОЛУЧЕННОГО КОДА И ПЕРЕКЛЮЧАЕМ ИК ПРИЁМНИК НА ПРИЁМ СЛЕДУЮЩЕГО
irrecv.resume();}

//ГАСИМ ИНДИКАТОР НА КНОПКЕ ПОСЛЕ ЗАДЕРЖКИ ОДНОКРАТНО МГНОВЕННО, ВСЕ ПОСЛЕДУЮЩИЕ СПУСТЯ 150 мс
if(flag==true&&mill-timer_IR>150){digitalWrite(IR_LED_out,LOW);timer_IR=mill;flag=false;}




//////////////////////////////////////// POWER ON/OFF //////////////////////////////////////////
/////////////////////////// МЕХАНИЧЕСКАЯ КНОПКА + КНОПКА НА ПДУ ////////////////////////////////
//ЛОГИЧЕСКИЙ "0" - НАЖАТА (ПИН ЗАКОРОЧЕН НА ЗЕМЛЮ), ЕСЛИ "1" - НЕ НАЖАТА, (ПИН В ВОЗДУХЕ) \
ПРОВЕРЕННЫЙ АЛГОРИТМ ДЛЯ "ПЛОХИХ" (НЕ ПРЕДНАЗНАЧЕННЫХ ДЛЯ СЛАБОТОЧНОЙ КОММУТАЦИИ) КНОПОК
btn=digitalRead(POWER_key);
if(btn!=POWER_btn||ir.value==IR_7){delay(40);btn=digitalRead(POWER_key);
if(!btn&&POWER_btn||ir.value==IR_7){power=!power;SF2=true;cl_ir();}
POWER_btn=btn;}




//////////////////////////////////////////// POWER ON ////////////////////////////////////////////
////////////////////// ЗАПУСКАЕМ ОСНОВНОЙ РАБОЧИЙ РЕЖИМ ДЛЯ АУДИОСИСТЕМЫ /////////////////////////
if(power==true){
//ЗАДАЁМ НАЧАЛЬНЫЕ ПАРАМЕТРЫ ДЛЯ РЕЖИМА "POWER ON"
if(SF2==true){
mute=0;menu=100;SF0=false;SF4=true;SF5=false;gain=Connect[input];
bigdigres();//ЗАГРУЖАЕМ СИМВОЛЫ ПСЕВДОГРАФИКИ ДЛЯ БОЛЬШИХ ЦИФР ГРОМКОСТИ
lcd.noCursor();lcd.noBlink();//СБРАСЫВАЕМ РЕЖИМЫ ПОДЧЁРКИВАНИЯ И МИГАНИЯ
ledON;//БЕЗОГОВОРОЧНО ВКЛЮЧАЕМ ПОДСВЕТКУ ЭКРАНА
digitalWrite(POWER_LED_out,HIGH);//ВКЛЮЧАЕМ ИНДИКАТОР "POWER ON"
headphone();//УПРАВЛЯЕМ ПИТАНИЕМ, УСИЛИТЕЛЕЙ И ИНДИКАТОРОМ НАУШНИКОВ 
Start_RDA5807M();//ЗАПУСКАЕМ РАДИОМОДУЛЬ
audio();//ПЕРЕДАЁМ ТЕКУЩИЕ НАСТРОЙКИ В АУДИОПРОЦЕССОР
SF2=false;}



//ОРГАНИЗУЕМ ЗАДЕРЖКУ ВЫХОДА ИЗ РЕЖИМА "STANDBY" НА 0,7 СЕК. ДЛЯ TDA7293 ПОСЛЕ ВКЛЮЧЕНИЯ 
//СИЛОВОГО БЛОКА ПИТАНИЯ ПРЕДОТВРАЩАЯ ЩЕЛЧКИ ДИНАМИКОВ ПРИ ВКЛЮЧЕНИИ УСИЛИТЕЛЯ МОЩНОСТИ
//И ПЕРЕВОДИМ TDA7293 В РЕЖИМ "STANDBY" ОДНОВРЕМЕННО С ОТКЛЮЧЕНИЕМ БЛОКА ПИТАНИЯ 
if(SF7==true&&mill>timer_AMP+p700){
lcd.clear();menu=0;sm0=0;SF4=true;if(!phone){digitalWrite(AMP_ST_out,HIGH);}
SF7=false;}



/////////////////////////////////// КНОПКА "MUTE" / MUTE KEY ///////////////////////////////////
//В ЭТОМ РЕЖИМЕ ОПРАШИВАЮТСЯ ТОЛЬКО КНОПКИ "POWER" И "MUTE" 
btn=digitalRead(MUTE_key);
if(btn!=MUTE_btn||ir.value==IR_8){delay(30);btn=digitalRead(MUTE_key);
if(!btn&&MUTE_btn||ir.value==IR_8){mute=!mute;audio();SF4=true;lcd.clear();
switch(mute){
case 0: menu=menu_save;digitalWrite(MUTE_LED_out,LOW);break;//MUTE OFF 
case 1: menu_save=menu;menu=20;SF0=false;digitalWrite(MUTE_LED_out,HIGH);break;}}//MUTE ON  
MUTE_btn=btn;cl_ir();}


//////////////////////////////////// ЕСЛИ "MUTE" НЕАКТИВНО /////////////////////////////////////
if(mute==false){

/////////////////////////////////// ПЕРЕКЛЮЧЕНИЕ МЕЖДУ МЕНЮ ////////////////////////////////////

//КНОПКА + ПДУ "СЛЕДУЩЕЕ МЕНЮ"
btn=digitalRead(MENU_key);
if(btn!=MENU_btn||ir.value==IR_5){delay(40);btn=digitalRead(MENU_key);
if(!btn&&MENU_btn||ir.value==IR_5){menu++;if(menu>2){menu=0;}timer_BTN=mill;
SF4=true;SF6=true;lcd.clear();cl_ir();}
MENU_btn=btn;}
//ПДУ "ПРЕДЫДУЩЕЕ МЕНЮ" 
if(ir.value==IR_6){
menu--;if(menu<0){menu=2;}if(menu>2){menu=0;}SF4=true;lcd.clear();
cl_ir();}



//////// ПЕРЕКЛЮЧЕНИЕ АУДИОСИГНАЛА МЕЖДУ УСИЛИТЕЛЕМ ДЛЯ НАУШНИКОВ И УСИЛИТЕЛЕМ МОЩНОСТИ ////////
//УДЕРЖИВАЯ КНОПКУ "MENU" БОЛЕ 0,5 СЕК. ИЛИ КНОПКОЙ НА ПДУ (HEADPHONES ON/OFF)
if(SF6==true&&MENU_btn==LOW&&mill-timer_BTN>p500||ir.value==IR_12){
phone=!phone;menu=100;headphone();ir.value=0;
SF6=false;}



/////////////////////////////// ВЫЗОВ МЕНЮ УПРАВЛЕНИЯ РАДИО ///////////////////////////////////
btn=digitalRead(RADIO_key);
if(btn!=RADIO_btn||ir.value==IR_11){delay(40);btn=digitalRead(RADIO_key);
if(btn==LOW&&RADIO_btn==HIGH||ir.value==IR_11){
if(menu!=10){menu=10;}else{menu=0;}SF4=true;lcd.clear();}
RADIO_btn=btn;ir.value=0;}



}//END GROUP BUTTONS IN "MUTE OFF" MODE



//В ЗАВИСИМОСТИ ОТ ВЫБРАННОГО ВХОДА ЗАПИТЫВАЕМ ИЛИ ОТКЛЮЧАЕМ ПИТАНИЕ ДЛЯ ИСТОЧНИКА СИГНАЛА \
FM ТЮНЕР RDA5807М ПОТРЕБЛЯЕТ КРАЙНЕ МАЛО, ЕГО НЕ ОБЕСТОЧИВАЕМ, А ВЫКЛЮЧАЕМ ПРОГРАММНО
switch(input){
  case 1:digitalWrite(BT_PWR_out,HIGH);digitalWrite(DAC_PWR_out,LOW);break;//ВТ
  case 2:digitalWrite(DAC_PWR_out,HIGH);digitalWrite(BT_PWR_out,LOW);break;//ЦАП
  case 3:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;//FM 
  case 4:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;//3,5 mm 
  case 5:digitalWrite(BT_PWR_out,LOW);digitalWrite(DAC_PWR_out,LOW);break;}//RCA 




///////////////////////////////////////// ГЛАВНОЕ МЕНЮ /////////////////////////////////////////
//////////////////////////////////// VOLUME -79 ... +15 dB /////////////////////////////////////
///////////////////////////////////// BASS -20 ... +20 dB //////////////////////////////////////
//////////////////////////////////// MIDDLE -20 ... +20 dB /////////////////////////////////////
//////////////////////////////////// TREBLE -20 ... +20 dB /////////////////////////////////////
/////////////////////////////////////// INPUT 1 ... 5 //////////////////////////////////////////
if(menu==0){
if(SF4==true){ar=uint8_t(8);cl_tmr();SF1=true;cl_enc();sm0=0;
SF4=false;}

//КНОПКОЙ ЭНКОДЕРА ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ НАСТРОЙКАМИ ЭКВАЛАЙЗЕРА
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn){delay(10);btn=digitalRead(SUBMENU_key);
if(!btn&&SUBMENU_btn){
sm0++;if(sm0>3){sm0=0;}timer_BTN=mill;SF1=true;SF6=true;cl_tmr();}
SUBMENU_btn=btn;}

//УДЕРЖИВАЯ КНОПКУ ЭНКОДЕРА БОЛЕЕ 0,5 СЕК. ИЛИ КНОПКОЙ ПДУ ВХОДИМ В РЕЖИМ ПЕРЕКЛЮЧЕНИЯ ВХОДОВ
if(SF6==true&&SUBMENU_btn==LOW&&mill-timer_BTN>p500||ir.value==IR_9){
if(sm0==4){sm0=0;}else{sm0=4;}SF6=false;cl_ir();cl_tmr();}

//КНОПКАМИ ПДУ ПЕРЕКЛЮЧАЕМСЯ МЕЖДУ НАСТРОЙКАМИ ЭКВАЛАЙЗЕРА
if(ir.value==IR_3){sm0++;if(sm0>3){sm0=0;}cl_ir();cl_tmr();}
if(ir.value==IR_4){sm0--;if(sm0<0){sm0=3;}cl_ir();cl_tmr();}

//ПРИСВАИВАЕМ ТЕКУЩЕЕ ЗНАЧЕНИЕ ПАРАМЕТРА ВРЕМЕННОЙ ПЕРЕМЕННОЙ
switch(sm0){
  case 0:volfad=vol+fad;var0=volfad;break;
  case 1:var0=treb;break;
  case 2:var0=midd;break;
  case 3:var0=bass;break;
  case 4:var0=input;break;}

//МЕНЯЕМ ВРЕМЕННУЮ ПЕРЕМЕННУЮ ЭНКОДЕРОМ ИЛИ КНОПКАМИ ПДУ
if(newP!=oldP){var0=var0+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){var0++;cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){var0--;cl_ir();cl_tmr();}


if(SF1==true){
MAIN_MENU();//ВЫВОДИМ ПАРАМЕТРЫ НА ЭКРАН
audio();//ОТПРАВЛЯЕМ ПАРАМЕТРЫ В BD37534FV
SF1=false;}
//ЗАПРАШИВАЕМ И ПЕЧАТАЕМ ТЕКУЩУЮ ТЕМПЕРАТУРУ 
Temperature();lcd.setCursor(8,3);lcd.print(temp);lcd.write(B11011111);
}//END MAIN MENU 0




/////////////////////////////////// МЕНЮ НАСТРОЙКИ ЭКВАЛАЙЗЕРА /////////////////////////////////
///////////////////////////// BASS FREQUENCY + BASS QUALITY FACTOR /////////////////////////////
/////////////////////////// MIDDLE FREQUENCY + MIDDLE QUALITY FACTOR ///////////////////////////
/////////////////////////// TREBLE FREQUENCY + TREBLE QUALITY FACTOR ///////////////////////////
/////////////////////////////// LOUDNESS CENTER + LOUDNESS GAIN ////////////////////////////////
if(menu==1){
if(SF4==true){ar=uint8_t(7);cl_tmr();SF1=true;SF4=false;SF5=true;}
ENCODER_KEY();
if(SF5==true&&newP!=oldP){
sm1=sm1+newP;if(sm1>7){sm1=0;}if(sm1<0){sm1=7;}ar=uint8_t(7);cl_enc();cl_tmr();}
if(ir.value==IR_3){sm1++;if(sm1>7){sm1=0;}ar=uint8_t(7);cl_ir();cl_tmr();}
if(ir.value==IR_4){sm1--;if(sm1<0){sm1=7;}ar=uint8_t(7);cl_ir();cl_tmr();}
  switch(sm1){
    case 0:var1=treb_c;break;
    case 1:var1=treb_q;break;
    case 2:var1=mid_c;break;
    case 3:var1=mid_q;break;
    case 4:var1=bas_c;break;
    case 5:var1=bas_q;break;
    case 6:var1=lon_f;break;
    case 7:var1=lon;break;}
if(SF5==false&&newP!=oldP){var1=var1+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){var1++;ar=uint8_t(8);cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){var1--;ar=uint8_t(8);cl_ir();cl_tmr();}
if(SF1==true){MENU_equalizer();audio();
SF1=false;}
}//END MENU 1




//////////////////////////// МЕНЮ ВЫБОРА ВХОДОВ И ИХ ЧУСТВИТЕЛЬНОСТИ ////////////////////////////
//////////////////////////////////////// INPUT SELECTION ////////////////////////////////////////
/////////////////////////////////////// GAIN 0 ... +20 dB ///////////////////////////////////////
if(menu==2){
if(SF4==true){sm2=input;ar=uint8_t(7);cl_tmr();SF5=true;SF8=true;MENU_input_gain();
SF4=false;}
ENCODER_KEY();
if(SF5==true&&newP!=oldP){
sm2=sm2+newP;if(sm2>5){sm2=1;}if(sm2<1){sm2=5;}ar=uint8_t(7);cl_enc();cl_tmr();SF8=true;}
if(ir.value==IR_3){sm2++;if(sm2>5){sm2=1;}ar=uint8_t(7);SF5=true;cl_ir();cl_tmr();SF8=true;}
if(ir.value==IR_4){sm2--;if(sm2<1){sm2=5;}ar=uint8_t(7);SF5=true;cl_ir();cl_tmr();SF8=true;}
if(SF8==true){var2=Connect[sm2];SF8=false;}
if(SF5==false&&newP!=oldP){var2=var2+newP;cl_enc();cl_tmr();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){var2++;ar=uint8_t(8);SF5=false;cl_ir();cl_tmr();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){var2--;ar=uint8_t(8);SF5=false;cl_ir();cl_tmr();}
if(SF1==true){
if(SF5==false){
input=sm2;var2=constrain(var2,0,20);
Connect[input]=var2;//ПРИСВАИВАЕМ ТЕКУЩИЙ УРОВЕНЬ ВЫБРАННОМУ ВХОДУ BD37534FV
gain=Connect[input];//УРОВЕНЬ НА ТЕКУЩЕМ ВХОДЕ ДЛЯ ПЕРЕДАЧИ В BD37534FV
audio();}
MENU_input_gain();
SF1=false;}
}//END MENU 2




////////////////////////// МЕНЮ УПРАВЛЕНИЯ РАДИОМОДУЛЕМ НА RDA5807M /////////////////////////////
/////////////////////////////////////// MENU FM RDA5807M ////////////////////////////////////////
if(menu==10){
//ЕСЛИ ОТКРЫТО ЭТО МЕНЮ ПЕРЕКЛЮЧАЕМ ВХОД BD37534FV К КОТОРОМУ ПОДКЛЮЧЕНА RDA5807M \
В НАШЕМ СЛУЧАЕ ВХОД № 3 И УСТАНАВЛИВАЕМ СООТВЕТСТВУЮЩИЙ ЕМУ УРОВЕНЬ УСИЛЕНИЯ
if(SF4==true){ar=uint8_t(8);SF0=false;SF4=false;input=3;gain=Connect[3];audio();}


//КНОПКОЙ ЭНКОДЕРА ИЛИ КНОПКАМИ ПДУ ВЫБИРАЕМ, ЧТО МЕНЯТЬ:
//1. КАНАЛ С ЗАПИСАННОЙ В НЕГО ЧАСТОТОЙ (1 - 20 КАНАЛОВ)
//2. ВЫХОДНОЙ УРОВЕНЬ АУДИОСИГНАЛА С RDA5807M (1 - 15 ПОПУГАЕВ)
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn||ir.value==IR_3||ir.value==IR_4){delay(10);btn=digitalRead(SUBMENU_key);
if(btn==LOW&&SUBMENU_btn==HIGH||ir.value==IR_3||ir.value==IR_4){timer_BTN=mill;timer_MENU=mill;
if(sm10==2){btn_flag=1;}else{btn_flag=0;}sm10++;if(sm10>1){sm10=0;}SF3=true;}
SUBMENU_btn=btn;ir.value=0;}


//УДЕРЖИВАЯ КНОПКУ ЭНКОДЕРА БОЛЕЕ 0,5 СЕК. ИЛИ КНОПКОЙ (СУБМЕНЮ) НА ПДУ ПРИВЯЗЫВАЕМ ЧАСТОТУ \
К НОМЕРУ КАНАЛА (ЗНАЧЕНИЕ В МАССИВЕ)
//ФАКТИЧЕСКИ НАДО ЗАРАННЕЕ ВЫБРАТЬ НОМЕР КАНАЛА ОТ 1 ДО 20, ПЕРЕКЛЮЧИТЬСЯ НА \
ВЫБОР ЧАСТОТЫ, ВЫБРАТЬ ЧАСТОТУ И ПОСЛЕ ЗАПИСАТЬ ЕЁ В МАССИВ ПОД НОМЕРОМ ВЫБРАННОГО КАНАЛА.
//ЧАСТОТУ МЕНЯЕМ С ШАГОМ В 100 кГц (БИБЛИОТЕКА ПОЗВОЛЯЕТ МЕНЯТЬ С ШАГОМ В 10 кГц)
//ЧАСТОТА 103,3 МГЦ ЗАПИСЫВАЕТСЯ КАК 10330
if(SF3==true&&SUBMENU_btn==LOW&&mill-timer_BTN>p500||ir.value==IR_9){SF3=false;
switch(btn_flag){
case 0: sm10=2;timer_MENU=mill;break;
case 1: channel[num]=freqFM;
timer_MENU=mill+p1000;//ЗАДЕРЖКА ПЕЧАТИ МЕНЮ ДЛЯ ОТОБРАЖЕНИЯ "SAVED!" В ТЕЧЕНИЕ 1 СЕК.
lcd.setCursor(11,1);lcd.print(F(" SAVED! "));sm10=0;break;}
btn_flag=!btn_flag;
ir.value=0;}


switch(sm10){
case 0:trig=num;break;
case 1:trig=volFM;break;
case 2:trig=(freqFM/10);break;}


//МЕНЯЕМ НОМЕР КАНАЛА ИЛИ УРОВЕНЬ АУДИОСИГНАЛА ИЛИ ЧАСТОТУ
if(newP!=oldP){trig=trig+newP;cl_enc();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){trig++;cl_ir();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){trig--;cl_ir();}


//ЕСЛИ ИЗМЕНЁН КАНАЛ, УРОВЕНЬ АУДИОСИГНАЛА ИЛИ ЧАСТОТА ОТПРАВЛЯЕМ В RDA5807M 
if(SF1==true){
switch(sm10){
case 0:num=trig;if(num<1){num=20;}if(num>20){num=1;}freqFM=channel[num];
rx.setFrequency(freqFM);break;
case 1:volFM=trig;volFM=constrain(volFM,1,15);rx.setVolume(volFM);break;
case 2:freqFM=(trig*10);if(freqFM<minFM){freqFM=maxFM;}if(freqFM>maxFM){freqFM=minFM;};
rx.setFrequency(freqFM);break;}
timer_MENU=mill;SF1=false;}


//ВЫВОДИМ ДАННЫЕ НА ЭКРАН
if(mill>timer_MENU){menuFM();time_func();Temperature();timer_MENU=mill+p1000;}


//ОПРАШИВАЕМ RDA5807M НА НАЛИЧИЕ RDS 
if(mill-timer_RDS>p100){rx.getRdsText0A();timer_RDS=mill;}


//СБРОС ОЧЕРЕДИ ПАКЕТОВ RDS
if(mill-timer_autoret>p10000){rx.clearRdsFifo();timer_autoret=mill;}

}//END MENU FM RDA5807M




//////////////////////////////////////////// MUTE MENU //////////////////////////////////////////
////////////////////////// В РЕЖИМЕ "ТИШИНА" ПЕЧАТАЕМ ОТДЕЛЬНОЕ МЕНЮ ////////////////////////////
if(menu==20){
if(mill-timer_MENU>p1000){timer_MENU=mill;var4=!var4;
lcd.setCursor(8,0); 
switch(var4){
case 0:lcd.print(F("    "));break;
case 1:lcd.print(F("MUTE"));break;}
time_func();Temperature();
lcd.setCursor(17,3);lcd.print(temp);lcd.write(B11011111);}}




//////////////////////////////////// RETURN TO THE MAIN MENU ///////////////////////////////////
//АВТОМАТИЧЕСКИЙ ВОЗВРАТ В РЕЖИМ РЕГУЛИРОВКИ ГРОМКОСТИ В ГЛАВНОЕ МЕНЮ ЧЕРЕЗ ЗАДАННЫЙ ИНТЕРВАЛ \
ИЗ МЕНЮ НАСТРОЙКИ ПАРАМЕТРОВ И ВЫБОРА ВХОДОВ 
if(SF0==true&&mill-timer_autoret>wait*p1000){if(menu!=0){lcd.clear();}menu=0;SF4=true;
SF0=false;}



}//////////////////////////////////////// END POWER ON /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// POWER OFF (STANDBY) //////////////////////////////////////
if(power==false){
//ЗАДАЁМ НАЧАЛЬНЫЕ ЗНАЧЕНИЯ ДЛЯ РЕЖИМА "STANDBY"
if(SF2==true){
mute=1;audio();menu=false;SF5=false;lcd.clear();
digitalWrite(AMP_ST_out,LOW);
digitalWrite(MUTE_LED_out,LOW);
digitalWrite(BT_PWR_out,LOW);
digitalWrite(POWER_LED_out,LOW);
digitalWrite(PHONE_LED_out,LOW);
digitalWrite(POWER_RELAY_out,LOW);
digitalWrite(DAC_PWR_out,LOW);
rx.powerDown();//ВЫКЛЮЧАЕМ RDA5807M
led?ledON:ledOFF;//УСТАНАВЛИВАЕМ ПОДСВЕТКУ ЭКРАНА В РЕЖИМЕ STANDBY ON/OFF
big_timeres();//ЗАГРУЖАЕМ СИМВОЛЫ ПСЕВДОГРАФИКИ ДЛЯ БОЛЬШИХ ЧАСОВ И УКАЗАТЕЛЕЙ
EEPROM_UPDATE();//ПЕРЕЗАПИСЬ ПАРАМЕТРОВ В ЭНЕРГОНЕЗАВИСИМУЮ ПАМЯТЬ
SF2=false;}




//КНОПКАМИ MENU НА ПАНЕЛЕ ИЛИ ПДУ ПЕРЕХОДИМ ИЗ ОСНОВНОГО В ТЕХНИЧЕСКОЕ МЕНЮ И ОБРАТНО 
btn=digitalRead(MENU_key);
if(btn!=MENU_btn||ir.value==IR_5||ir.value==IR_6){delay(30);btn=digitalRead(MENU_key);
if(btn==LOW&&MENU_btn==HIGH||ir.value==IR_5||ir.value==IR_6){menu=!menu;SF4=true;
cl_ir();lcd.clear();}
MENU_btn=btn;}




//////////// УПРАВЛЕНИЕ ПОДСВЕТКОЙ ЭКРАНА ON/OFF В РЕЖИМЕ "STANDBY" (КНОПКА "MUTE") ////////////
btn=digitalRead(MUTE_key);
if(btn!=MUTE_btn||ir.value==IR_8){delay(30);btn=digitalRead(MUTE_key);
if(!btn&&MUTE_btn||ir.value==IR_8){led=!led;led?ledON:ledOFF;cl_ir();}
MUTE_btn=btn;}




////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// ОСНОВНОЕ МЕНЮ ЧАСЫ ЗАСТАВКА (BIG WATCH) В РЕЖИМЕ "STANDBY" ////////////////////
if(menu==false){
if(SF4==true){SF4=false;lcd.noCursor();lcd.noBlink();}
if(mill-timer_MENU>p1000){timer_MENU=mill;
big_time();Temperature();
lcd.setCursor(17,3);lcd.print(temp);lcd.write(B11011111);}}




////////////////////////////////////////////////////////////////////////////////////////////////
/////// ТЕХНИЧЕСКОЕ МЕНЮ УСТАНОВКИ ВРЕМЕНИ, ДАТЫ, УРОВНЕЙ ВЫХОДНЫХ УСИЛИТЕЛЕЙ BD37534FV ////////
///// ВЫХОДА С RDA5807M И ВРЕМЕНИ АВТОМАТИЧЕСКОГО ВОЗВРАТА В ГЛАВНОЕ МЕНЮ ИЗ ДРУГИХ МЕНЮ ///////
if(menu==true){
//СЧИТЫВАЕМ И ПЕЧАТАЕМ ТЕКУЩИЕ ЗНАЧЕНИЯ ВРЕМЕНИ И ДАТЫ
if(SF4==true){SF5=true;setime=0;sm4=0;get_time();
//ПЕЧАТАЕМ ТЕКУЩИЕ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ
lcd.setCursor(0,0);lcd.print(rtc.getTimeStr());
lcd.setCursor(9,0);lcd.print(rtc.getDateStr());
lcd.setCursor(0,1);lcd.print(F("DAY "));lcd.print(wt.dow);
lcd.setCursor(6,1);lcd.print(F("SAVE TIME -Y+"));
lcd.setCursor(0,2);lcd.print(F("LS"));lcd.print(lf);
lcd.setCursor(6,2);lcd.print(F("RS"));lcd.print(rf);
lcd.setCursor(12,2);lcd.print(F("FM Bas "));lcd.print(BasFM);
lcd.setCursor(0,3);lcd.print(F("LP"));lcd.print(lt);
lcd.setCursor(6,3);lcd.print(F("RP"));lcd.print(rt);
lcd.setCursor(12,3);lcd.print(F("Return"));lcd.print(wait);
lcd.setCursor(poz[sm4],line[sm4]);lcd.blink();
SF4=false;}


//КНОПКОЙ ЭНКОДЕРА МЕНЯЕМ МИГАНИЕ НА ПОДЧЁРКИВАНИЕ КУРСОРА 
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn){delay(10);btn=digitalRead(SUBMENU_key);
if(btn==LOW&&SUBMENU_btn==HIGH){SF5=!SF5;
if(SF5==true){lcd.noCursor();lcd.blink();}else{lcd.noBlink();lcd.cursor();}}
SUBMENU_btn=btn;}


//ПОВОРОТОМ ЭНКОДЕРА МЕНЯЕМ ПОЗИЦИЮ МИГАЮЩЕГО КУРСОРА
if(SF5==true&&newP!=oldP){sm4=sm4+newP;if(sm4<0){sm4=13;}if(sm4>13){sm4=0;}
oldP=0;newP=0;myEnc.write(0);}
if(ir.value==IR_3){sm4++;if(sm4>13){sm4=0;}SF5=true;ir.value=0;lcd.noCursor();lcd.blink();}
if(ir.value==IR_4){sm4--;if(sm4<0){sm4=13;}SF5=true;ir.value=0;lcd.noCursor();lcd.blink();}
switch(sm4){
    case 0:var4=sethour;break;
    case 1:var4=setmin;break;
    case 2:var4=setsec;break;
    case 3:var4=setdate;break;
    case 4:var4=setmon;break;
    case 5:var4=setyear;break;
    case 6:var4=setdow;break;
    case 7:var4=setime;break;
    case 8:var4=lf;break;
    case 9:var4=rf;break;
   case 10:var4=lt;break;
   case 11:var4=rt;break;
   case 12:var4=BasFM;break;
   case 13:var4=wait;break;}
lcd.setCursor(poz[sm4],line[sm4]);


//МЕНЯЕМ ЗНАЧЕНИЕ ПЕРЕМЕННОЙ В ПОЗИЦИИ ПОДЧЁРКИВАНИЯ КУРСОРА 
if(SF5==false&&newP!=oldP){var4+=newP;cl_enc();}
if(ir.value==IR_1||ir.value==IR_10&&irkey==IR_1){var4++;SF5=false;cl_ir();
lcd.noBlink();lcd.cursor();}
if(ir.value==IR_2||ir.value==IR_10&&irkey==IR_2){var4--;SF5=false;cl_ir();
lcd.noBlink();lcd.cursor();}
if(SF1==true){
switch(sm4){
    case 0:sethour=var4=constrain(var4,0,23);break;
    case 1:setmin=var4=constrain(var4,0,59);break;
    case 2:setsec=var4=constrain(var4,0,59);break;
    case 3:setdate=var4=constrain(var4,1,31);break;
    case 4:setmon=var4=constrain(var4,1,12);break;
    case 5:setyear=var4=constrain(var4,20,99);break;
    case 6:setdow=var4=constrain(var4,1,7);break;  
    case 7:setime=var4=constrain(var4,-1,1);break;
    case 8:lf=var4=constrain(var4,-79,0);break;    
    case 9:rf=var4=constrain(var4,-79,0);break;    
   case 10:lt=var4=constrain(var4,-79,0);break;    
   case 11:rt=var4=constrain(var4,-79,0);break;
   case 12:BasFM=var4=constrain(var4,0,1);break;
   case 13:wait=var4=constrain(var4,3,32);break;}


//ПЕЧАТАЕМ ИЗМЕНИВШЕЕСЯ ЗНАЧЕНИЕ ПЕРЕМЕННОЙ В ПОЗИЦИИ КУРСОРА
lcd.print(var4);if(abs(var4)<10){lcd.print(' ');}//lcd.setCursor(poz[sm4],line[sm4]);


//ДЛЯ ЗАПИСИ ТЕКУЩИХ ЗНАЧЕНИЙ ВРЕМЕНИ И ДАТЫ УСТАНАВЛИВАЕМ КУРСОР В ПОЛЕ "SAVE TIME -Y+" \
В МОМЕНТ СОВПАДЕНИЯ УСТАНОВЛЕННОГО ЗНАЧЕНИЯ СЕКУНД С ЭТАЛОННЫМ ЗНАЧЕНИЕМ, ПОВОРАЧИВАЕМ \
ЭНКОДЕР В ЛЮБУЮ СТОРОНУ ИЛИ НАЖИМАЕМ +/- НА ПДУ И ВЫВОДИМ СООБЩЕНИЕ О ЗАПИСИ ДАННЫХ В DS3231
if(setime!=0){lcd.noCursor();
set_time();lcd.clear();lcd.setCursor(1,1);lcd.print(F("TIME & DATE SAVE!"));}timer_MENU=mill;
SF1=false;}


//ВЫВОДИМ НА ЭКРАН "TIME & DATE SAVE!" В ТЕЧЕНИЕ 2 СЕКУНД, И ВОЗВРАЩАЕМСЯ В ГЛАВНОЕ МЕНЮ 
if(setime!=0&&mill-timer_MENU>p2000){SF4=true;menu=0;lcd.clear();}
}//END TECHNICAL MENU


}//END POWER OFF 


}//END LOOP 
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
