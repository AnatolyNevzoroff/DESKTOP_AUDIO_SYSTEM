/////////////////////////////////////// ФУНКЦИИ / FUNCTIONS ////////////////////////////////////

//ФУНКЦИЯ ОБНУЛЕНИЯ ТАЙМЕРА ДО АВТОВОЗВРАТА В ОСНОВНОЕ МЕНЮ, ПОДНИМАЕМ ФЛАГ
void cl_tmr(){timer_autoret=mill;S.F0=true;}


//ОБНУЛЯЕМ ПЕРЕМЕННУЮ С ИК ПРИЁМНИКА, ПОДНИМАЕМ ФЛАГ 
void cl_ir(){ir.value=0;S.F1=true;}


//ОБНУЛЯЕМ ПОЛОЖЕНИЕ ЭНКОЛЕРА, ПОДНИМАЕМ ФЛАГ
void cl_enc(){oldP=0;newP=0;myEnc.write(0);S.F1=true;}


//ФУНКЦИЯ ОБРАБОТКИ НАЖАТИЯ НА КНОПКУ ЭНКОДЕРА
void ENCODER_KEY(){
btn=digitalRead(SUBMENU_key);
if(btn!=SUBMENU_btn||ir.value==IR_9){delay(10);btn=digitalRead(SUBMENU_key);
if(!btn&&SUBMENU_btn||ir.value==IR_9){S.F5=!S.F5;if(S.F5==true){ar=uint8_t(7);}else{ar=uint8_t(8);}
ir.value=0;timer1=mill;S.F1=true;timer_autoret=mill;S.F0=true;}SUBMENU_btn=btn;}}



//ФУНКЦИЯ ПЕРЕКЛЮЧЕНИЯ УСИЛИТЕЛЕЙ 
void headphone(){
lcd.clear();lcd.setCursor(4,1);lcd.print(F("AMPLIFIER "));
switch(phone){
case 0: //AMPLIFIER ON \ HEADPHONES OFF
digitalWrite(POWER_RELAY_out,HIGH);//ВКЛЮЧАЕМ СИЛОВОЙ БЛОК ПИТАНИЯ
digitalWrite(PHONE_LED_out,LOW);//ОТКЛЮЧАЕМ ИНДИКАТОР НАУШНИКОВ
lcd.print(F("ON"));break;
case 1: //HEADPHONES ON \ AMPLIFIER OFF
digitalWrite(AMP_ST_out,LOW);//ПЕРЕВОДИМ УСИЛИТЕЛИ В РЕЖИМ STANDBY & MUTE ON 
digitalWrite(PHONE_LED_out,HIGH);//ВКЛЮЧАЕМ ИНДИКАТОР НАУШНИКОВ
digitalWrite(POWER_RELAY_out,LOW);//ОТКЛЮЧАЕМ СИЛОВОЙ БЛОК ПИТАНИЯ
lcd.print(F("OFF"));break;}
S.F7=true;timer3=mill;}//ПОДНИМАЕМ ФЛАГ ДЛЯ ВКЛЮЧЕНИЯ УСИЛИТЕЛЯ ЧЕРЕЗ 0,7 СЕК.



//ФУНКЦИЯ ПЕЧАТИ ГЛАВНОГО МЕНЮ 
void MAIN_MENU(){
//VOLUME+FADER
lcd.setCursor(0,0);
if(sm0==0){volfad=constrain(trig1,-79,40);lcd.write(ar);}else{lcd.print(' ');}
if(volfad>15){fad=volfad-15;lcd.print(F("FADER "));graf=fad-25;grafprint();graf=fad;bigdig();}
if(volfad<=15){vol=volfad;lcd.print(F("VOLUME"));fad=0;graf=vol;grafprint();graf=vol+84;bigdig();}
//TREBLE 
lcd.setCursor(0,1);
if(sm0==1){treb=constrain(trig1,-20,20);lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("TRE"));graf=treb;grafprint();lcd.print(' ');lcd.print(F("IN"));
//MIDDLE 
lcd.setCursor(0,2);
if(sm0==2){midd=constrain(trig1,-20,20);lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("MID"));graf=midd;grafprint();
//BASS 
lcd.setCursor(0,3);
if(sm0==3){bass=constrain(trig1,-20,20);lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("BAS"));graf=bass;grafprint();
//INPUT 
lcd.setCursor(8,2);
if(sm0==4){input=trig1;if(input>5){input=1;}if(input<1){input=5;}lcd.write(ar);gain=Connect[input];}
else{lcd.print(' ');}lcd.print(input);}



//ФУНКЦИЯ ПЕЧАТИ МЕНЮ НАСТРОЙКИ ЭКВАЛАЙЗЕРА И ТОНКОРРЕКЦИИ
void MENU_equalizer(){
//"Trebble Central Frequency"
lcd.setCursor(0,0);
if(sm1==0){treb_c=trig1;if(treb_c>3){treb_c=0;}if(treb_c<0){treb_c=3;}lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("TREB "));
  switch(treb_c){
    case 0:lcd.print(F(" 7.5"));break;
    case 1:lcd.print(F("10.0"));break;
    case 2:lcd.print(F("12.5"));break;
    case 3:lcd.print(F("15.0"));break;}lcd.print(F(" kHz"));
//"Trebble Q-Factor"
lcd.setCursor(15,0);
if(sm1==1){treb_q=trig1;if(treb_q>1){treb_q=0;}if(treb_q<0){treb_q=1;}lcd.write(ar);}
else{lcd.print(' ');}//lcd.print("Q ");
switch(treb_q){
  case 0:lcd.print(F("0.75"));break;
  case 1:lcd.print(F("1.25"));break;}
//"Middle Central Frequency"
lcd.setCursor(0,1);
if(sm1==2){mid_c=trig1;if(mid_c>3){mid_c=0;}if(mid_c<0){mid_c=3;}lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("MIDD  "));
  switch(mid_c){
    case 0:lcd.print(F("0.5"));break;
    case 1:lcd.print(F("1.0"));break;
    case 2:lcd.print(F("1.5"));break;
    case 3:lcd.print(F("2.5"));break;}lcd.print(F(" kHz"));
//"Middle Q-Factor"
lcd.setCursor(15,1);
if(sm1==3){mid_q=trig1;if(mid_q>3){mid_q=0;}if(mid_q<0){mid_q=3;}lcd.write(ar);}
else{lcd.print(' ');}//lcd.print("Q ");
  switch(mid_q){
    case 0:lcd.print(F("0.75"));break;
    case 1:lcd.print(F("1.00"));break;
    case 2:lcd.print(F("1.25"));break;
    case 3:lcd.print(F("1.50"));break;}
//"Bass Central Frequency"
lcd.setCursor(0,2);
if(sm1==4){
bas_c=trig1;if(bas_c>3){bas_c=0;}if(bas_c<0){bas_c=3;}lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("BASS  "));
  switch(bas_c){
    case 0:lcd.print(F(" 6"));break;
    case 1:lcd.print(F(" 8"));break;
    case 2:lcd.print(F("10"));break;
    case 3:lcd.print(F("12"));break;}lcd.print(F("0  Hz"));
//"Bass Q-Factor"
lcd.setCursor(15,2);
if(sm1==5){bas_q=trig1;if(bas_q>3){bas_q=0;}if(bas_q<0){bas_q=3;}lcd.write(ar);}
else{lcd.print(' ');}//lcd.print("Q ");
  switch(bas_q){
    case 0:lcd.print(F("0.5"));break;
    case 1:lcd.print(F("1.0"));break;
    case 2:lcd.print(F("1.5"));break;
    case 3:lcd.print(F("2.0"));break;}
//"Loudness Frequency"
lcd.setCursor(0,3);
if(sm1==6){lon_f=trig1;if(lon_f>3){lon_f=0;}if(lon_f<0){lon_f=3;}lcd.write(ar);}
else{lcd.print(' ');}lcd.print(F("LOUDNES "));
switch(lon_f){
  case 0:lcd.print(F("25"));break;
  case 1:lcd.print(F("40"));break;
  case 2:lcd.print(F("80"));break;
  case 3:lcd.print(F("  "));break;}lcd.print(F("0Hz"));
//"Loudness Gain"
lcd.setCursor(15,3);
if(sm1==7){lon=trig1;lon=constrain(lon,0,20);lcd.write(ar);}
else{lcd.print(' ');}graf=lon;grafprint();}



//ФУНКЦИЯ ПЕРЕКЛЮЧЕНИЯ И ОТОБРАЖЕНИЯ ЧУСТВИТЕЛЬНОСТИ ВХОДОВ 
void MENU_input_gain(){
lcd.setCursor(0,0); if(sm2==1){lcd.write(ar);}else{lcd.print(' ');}lcd.print(F("[1]"));
lcd.setCursor(1,1); graf=Connect[1];grafprint();
lcd.setCursor(4,0); if(sm2==2){lcd.write(ar);}else{lcd.print(' ');}lcd.print(F("[2]"));
lcd.setCursor(5,1); graf=Connect[2];grafprint();
lcd.setCursor(8,0); if(sm2==3){lcd.write(ar);}else{lcd.print(' ');}lcd.print(F("[3]"));
lcd.setCursor(9,1); graf=Connect[3];grafprint();
lcd.setCursor(12,0);if(sm2==4){lcd.write(ar);}else{lcd.print(' ');}lcd.print(F("[4]"));
lcd.setCursor(13,1);graf=Connect[4];grafprint();
lcd.setCursor(16,0);if(sm2==5){lcd.write(ar);}else{lcd.print(' ');}lcd.print(F("[5]"));
lcd.setCursor(17,1);graf=Connect[5];grafprint();
lcd.setCursor(0,3);lcd.write(ar);
switch(sm2){
    case 1:lcd.print(F("[1] Bluetooth IN"));break;
    case 2:lcd.print(F("[2] Built-in DAC"));break;
    case 3:lcd.print(F("[3] FM Receiver "));break;
    case 4:lcd.print(F("[4] 3,5 mm Input"));break;
    case 5:lcd.print(F("[5] RCA Input   "));break;}}



//ФУНКЦИЯ ЗАПУСКА И УСТАНОВКИ НЕКОТОРЫХ ПАРАМЕТРОВ ДЛЯ РАДИОМОДУЛЯ RDA5807M
void Start_RDA5807M(){
rx.setup();//УСТАНОВКА НАЧАЛЬНЫХ ПАРАМЕТРОВ ПРИ ВКЛЮЧЕНИИ RDA5807M
rx.setMono(false);//ОТКЛЮЧАЕМ РЕЖИМ ПРИНУДИТЕЛЬНОГО МОНО (СНИЖАЕТ ЧУСТВИТЕЛЬНОСТЬ)
rx.setBass(false);//ОТКЛЮЧАЕМ УСИЛЕНИЕ БАСОВ
rx.setNewMethod(true);//ТОЛЬКО В МОДИФИЦИРОВАННОЙ ВЕРСИИ БИБЛИОТЕКИ (ЕСТЬ В АРХИВЕ)
rx.setFmDeemphasis(1);//ЗАДЕРЖКА ПРЕДИСКАЖЕНИЙ "1" ДЛЯ ЕВРОППЫ 50МСЕК ("0" ДЛЯ АМЕРИКИ 75МСЕК)
rx.setRDS(true);//ВКЛЮЧАЕМ ПРИЁМ ДАННЫХ RDS 
//rx.setSoftmute(false);//ОТКЛЮЧАЕМ БЕСШУМНУЮ НАСТРОЙКУ
//rx.setRBDS(false);//ОТКЛЮЧАЕМ ПРИЁМ ДАННЫХ RВDS (ДЛЯ СЕВЕРОАМЕРИКАНСКИХ FM-РАДИОСТАНЦИЙ)
rx.setRdsFifo(true);//ВКЛЮЧАЕМ ОЧЕРЁДНОСТЬ ПАКЕТОВ
rx.setVolume(volFM);//УСТАНАВЛИВАЕМ СОХРАНЁННЫЙ ПАРАМЕТР ГРОМКОСТИ
//rx.getVolume();//ЧИТАЕМ ЗНАЧЕНИЕ ГРОМКОСТИ
freqFM=channel[num];//ВЫБИРАЕМ ЧАСТОТУ ЗАПИСАННУЮ ПОД ТЕКУЩИМ НОМЕРОМ КАНАЛА
rx.setFrequency(freqFM);}//ПЕРЕДАЁМ ЧАСТОТУ В RDA5807M



//МЕНЮ ПЕЧАТИ ПАРАМЕТРОВ RDA5807M НА ЭКРАНЕ
void menuFM(){
//ПЕЧАТАЕМ НОМЕР КАНАЛА В КОТОРОМ ЗАПИСАНА ЧАСТОТА СТАНЦИИ
lcd.setCursor(0,0);if(sm4==0){lcd.write(ar);}else{lcd.print(' ');}
if(num<10){lcd.print(' ');}lcd.print(num);
//ПЕЧАТАЕМ ЧАСТОТУ СТАНЦИИ
lcd.setCursor(4,0);if(sm4==1){lcd.write(ar);}else{lcd.print(' ');}
if(freqFM<p10000){lcd.print(' ');}lcd.print((float)freqFM/100,1);
lcd.setCursor(10,0);lcd.print(F("MHz"));
//ПЕЧАТАЕМ СИМВОЛ АНТЕННЫ ПРИ ЗАХВАТЕ СТАНЦИИ (ТОЛЬКО В МОДИФИЦИРОВАННОЙ ВЕРСИИ БИБЛИОТЕКИ!)
lcd.setCursor(14,0);if(rx.getFMtrue()){lcd.write(uint8_t(6));}else{lcd.print(' ');}
//ПЕЧАТАЕМ УРОВЕНЬ СИГНАЛА В "dBuV"  
lcd.print(rx.getRssi());
//ЕСЛИ СИГНАЛ СТЕРЕО ПЕЧАТАЕМ "ST" ЕСЛИ НЕТ ТО "MO"
lcd.setCursor(18,0);lcd.print(rx.isStereo()?"ST":"MO");
//ЕСЛИ СТАНЦИЯ ПЕРЕДАЁТ R.D.S. ПЕЧАТАЕМ НАЗВАНИЕ СТАНЦИИ, ЕСЛИ НЕТ ПЕЧАТАЕМ "RDS LOST"
lcd.setCursor(6,1);if(rx.hasRdsInfo()){lcd.print(rx.getRdsText0A());}
else{lcd.print(F("RDS LOST"));}
//ПЕЧАТАЕМ ТЕМПЕРАТУРУ С ДАТЧИКА 
lcd.setCursor(17,3);lcd.print(temp);lcd.write(B11011111);}



//ФУНКЦИЯ "ПРАВИЛЬНОЙ ПЕЧАТИ" ЦИФР С УЧЁТОМ ЗНАКА
void grafprint(){
if(graf>-10&&graf<10){lcd.print(' ');}if(graf>0){lcd.print('+');}if(graf==0){lcd.print(' ');}
lcd.print(graf);}



//ФОРМИРУЕМ СИМВОЛЫ ПСЕВДОГРАФИКИ ДЛЯ ОГРОМНЫХ ЦИФР ГРОМКОСТИ И ДРУГИЕ В РЕЖИМЕ "POWER ON"
void bigdigres(){
byte v1[8]={31,31,31,31,31,31,31,31};//СИМВОЛ ПОЛНОЕ ЗАПОЛНЕНИЕ 
byte v2[8]={ 0, 0, 0,31,31,31,31,31};//СИМВОЛ ЗАПОЛНЕНИЕ ПОЛОВИНКИ СНИЗУ
byte v3[8]={31,31,31,31,31, 0, 0, 0};//СИМВОЛ ЗАПОЛНЕНИЕ ПОЛОВИНКИ СВЕРХУ
byte v4[8]={ 0, 0, 0, 0, 0,31,31,31};//СИМВОЛ ЗАПОЛНЕНИЕ ТРЕТИ СНИЗУ
byte v5[8]={31,31,31, 0, 0, 0, 0, 0};//СИМВОЛ ЗАПОЛНЕНИЕ ТРЕТИ СВЕРХУ
byte v6[8]={21,21,14, 4, 4, 4, 4, 0};//СИМВОЛ АНТЕННЫ
//byte v7[8]={24,28,14, 7,14,28,24, 0};//СИМВОЛ СТРЕЛОЧКИ - УКАЗАТЕЛЯ
//byte v8[8]={24,28,30,31,30,28,24, 0};//СИМВОЛ ТРЕУГОЛЬНИКА - УКАЗАТЕЛЯ
//ЗАПИСЫВАЕМ ПЕРЕМЕННЫЕ В ПАМЯТЬ LCD
lcd.createChar(1,v1);lcd.createChar(2,v2);lcd.createChar(3,v3);lcd.createChar(4,v4);
lcd.createChar(5,v5);lcd.createChar(6,v6);}//lcd.createChar(7,v7);lcd.createChar(8,v8);



//ФУНКЦИЯ ФОРМИРОВАНИЯ ОГРОМНЫХ ЦИФР ГРОМКОСТИ ИЗ СИМВОЛОВ ПСЕВДОГРАФИКИ \
ПОДРОБНЫЕ КОМЕНТАРИИ КАК РАБОТАЕТ АНАЛОГИЧНАЯ ФУНКЦИЯ ПРИВЕДЕНЫ ДЛЯ ЧАСОВ-ЗАСТАВКИ
void bigdig(){
uint8_t a[2],i,e1,e2,e3,e4,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16;
a[0]=(graf)/10;
a[1]=(graf)%10;
for(i=0;i<2;i++){
switch(i){
case 0:e1=11,e2=12,e3=13,e4=14;break;
case 1:e1=16,e2=17,e3=18,e4=19;break;}
switch(a[i]){
case 0:d1=2,d2=3,d3=3,d4=2,   d5=1,d6=32,d7=32,d8=1,  d9=1,d10=32,d11=32,d12=1,  d13=3,d14=2,d15=2,d16=3;break;
case 1:d1=4,d2=2,d3=1,d4=32,  d5=32,d6=32,d7=1,d8=32, d9=32,d10=32,d11=1,d12=32, d13=2,d14=2,d15=1,d16=2;break;
case 2:d1=2,d2=3,d3=3,d4=2,   d5=32,d6=32,d7=4,d8=3,  d9=4,d10=3,d11=32,d12=32,  d13=1,d14=2,d15=2,d16=1;break;// 2 5
case 3:d1=2,d2=3,d3=3,d4=2,   d5=32,d6=4,d7=4,d8=3,   d9=32,d10=32,d11=5,d12=2,  d13=3,d14=2,d15=2,d16=3;break;
case 4:d1=32,d2=32,d3=2,d4=5, d5=2,d6=5,d7=4,d8=32,   d9=5,d10=5,d11=1,d12=5,    d13=32,d14=32,d15=1,d16=32;break;
case 5:d1=1,d2=3,d3=3,d4=3,   d5=1,d6=4,d7=4,d8=32,   d9=32,d10=32,d11=32,d12=1, d13=3,d14=2,d15=2,d16=3;break;
case 6:d1=2,d2=3,d3=3,d4=2,   d5=1,d6=4,d7=4,d8=32,   d9=1,d10=32,d11=32,d12=1,  d13=3,d14=2,d15=2,d16=3;break;
case 7:d1=1,d2=3,d3=3,d4=1,   d5=32,d6=32,d7=32,d8=1, d9=32,d10=32,d11=1,d12=32, d13=32,d14=1,d15=32,d16=32;break;
case 8:d1=2,d2=3,d3=3,d4=2,   d5=3,d6=4,d7=4,d8=3,    d9=1,d10=32,d11=32,d12=1,  d13=3,d14=2,d15=2,d16=3;break;
case 9:d1=2,d2=3,d3=3,d4=2,   d5=3,d6=4,d7=4,d8=1,    d9=32,d10=32,d11=32,d12=1, d13=3,d14=2,d15=2,d16=3;break;}
lcd.setCursor(e1,0);lcd.write(uint8_t(d1));lcd.setCursor(e2,0);lcd.write(uint8_t(d2));
lcd.setCursor(e3,0);lcd.write(uint8_t(d3));lcd.setCursor(e4,0);lcd.write(uint8_t(d4));
lcd.setCursor(e1,1);lcd.write(uint8_t(d5));lcd.setCursor(e2,1);lcd.write(uint8_t(d6));
lcd.setCursor(e3,1);lcd.write(uint8_t(d7));lcd.setCursor(e4,1);lcd.write(uint8_t(d8));
lcd.setCursor(e1,2);lcd.write(uint8_t(d9));lcd.setCursor(e2,2);lcd.write(uint8_t(d10));
lcd.setCursor(e3,2);lcd.write(uint8_t(d11));lcd.setCursor(e4,2);lcd.write(uint8_t(d12));
lcd.setCursor(e1,3);lcd.write(uint8_t(d13));lcd.setCursor(e2,3);lcd.write(uint8_t(d14));
lcd.setCursor(e3,3);lcd.write(uint8_t(d15));lcd.setCursor(e4,3);lcd.write(uint8_t(d16));}}



//ДЛЯ ПОКАЗА БОЛЬШИХ ЧАСОВ В РЕЖИМЕ "POWER OFF" СОЗДАЁМ В ПАМЯТИ LCD2004 ДО 8 \
(ОГРАНИЧЕННО ПАМЯТЬЮ ЭКРАНА) ПЕРЕМЕННЫХ ОБРАЗУЮЩИХ ПСЕВДОСИМВОЛЫ ДЛЯ \
МАНИПУЛЯЦИИ ВНУТРИ ФУНКЦИИ big_time() 
void big_timeres(){
byte v1[8]={ 7, 7, 7, 7, 7, 7, 7, 7};
byte v2[8]={28,28,28,28,28,28,28,28};
byte v3[8]={31,31,31, 0, 0, 0, 0, 0};
byte v4[8]={28,28,28, 0, 0, 0, 0, 0};
byte v5[8]={ 7, 7, 7, 0, 0, 0, 0, 0};
byte v6[8]={ 0, 0, 0, 0, 0,12,12, 0};//ТОЧКА ДЛЯ ДВОЕТОЧИЯ - РАЗДЕЛИТЕЛЯ
byte v7[8]={24,28,14, 7,14,28,24, 0};//СИМВОЛ СТРЕЛОЧКИ - УКАЗАТЕЛЯ
byte v8[8]={24,28,30,31,30,28,24, 0};//СИМВОЛ ТРЕУГОЛЬНИКА - УКАЗАТЕЛЯ
lcd.createChar(1,v1);lcd.createChar(2,v2);lcd.createChar(3,v3);lcd.createChar(4,v4);
lcd.createChar(5,v5);lcd.createChar(6,v6);lcd.createChar(7,v7);lcd.createChar(8,v8);}



//ФУНКЦИЯ ФОРМИРУЕТ БОЛЬШИЕ ЧАСЫ - ЗАСТАВКУ В РЕЖИМЕ "POWER OFF"
void big_time(){
uint8_t d1,d2,d3,d4,d5,d6,d7,d8,d9,i,e1,e2,e3;//ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
uint8_t a[6];//МАССИВ ИЗ 6 ПЕРЕМЕННЫХ
wt=rtc.getTime();//СЧИТЫВАЕМ ТЕКУЩЕЕ ВРЕМЯ С МОДУЛЯ RTC
//ПРИСВАИВАЕМ 6-ти ПЕРЕМЕННЫМ ИЗ МАССИВА а[6] ЗНАЧЕНИЯ КАЖДОГО РАЗРЯДА ВРЕМЕНИ 00:00:00
a[0]=wt.hour/10;//ПЕРВЫЙ РАЗРЯД ЧАСЫ
a[1]=wt.hour%10;//ВТОРОЙ РАЗРЯД ЧАСЫ
a[2]=wt.min/10;//ПЕРВЫЙ РАЗРЯД МИНУТЫ
a[3]=wt.min%10;//ВТОРОЙ РАЗРЯД МИНУТЫ
a[4]=wt.sec/10;//ПЕРВЫЙ РАЗРЯД СЕКУНДЫ
a[5]=wt.sec%10;//ВТОРОЙ РАЗРЯД СЕКУНДЫ
//ДЛЯ ФОРМИРОВАНИЯ 6-ти ЦИФР ДЛЯ ЧАСОВ ТИПА 00:00:00 ЗАПУСКАЕМ ЦИКЛ ФОРМИРОВАНИЯ КАЖДОЙ ЦИФРЫ \
ИЗ СИМВОЛОВ ПСЕВДОГРАФИКИ
//ПЕРВАЯ ЦИФРА, НАПРИМЕР 20 ЧАСОВ ИЛИ 8 ВЕЧЕРА, СОСТОИТ ИЗ ЦИФР "2" И "0" \
ЦИФРА 2 ФОРМИРУЕТСЯ ИЗ 9 СИМВОЛОВ, \
3 СИМВОЛА В СТРОКЕ 0 С АДРЕСАМИ 0,1,2 \
3 СИМВОЛА В СТРОКЕ 1 С АДРЕСАМИ 0,1,2 \
3 СИМВОЛА В СТРОКЕ 2 С АДРЕСАМИ (ПОПРОБУЙТЕ ДОГАДАТЬСЯ С КАКИМИ АДРЕСАМИ :-)) \
ТЕПЕРЬ ВНИМАТЕЛЬНО СЛЕДИМ ЗА РАБОТОЙ АЛГОРИТМА, НАПОМИНАЕТ ИГРУ В НАПЁРСТКИ, ТОЛЬКО БЕЗ ОБМАНА! \
ПЕРВЫЕ 3 АДРЕСА В ЦИКЛЕ ПРИСВАИВАЕМ 3 ПЕРЕМЕННЫМ е1,е2,е3 ЗНАКОМЫЕ АДРЕСА 0,1,2 \
ЗАБЕГАЯ ВПЕРЁД, ВТОРАЯ ЦИФРА РАСПОЛОГАЕТСЯ ПО АДРЕСАМ 3,4,5 НАДО ТОЛЬКО СТРОКИ МЕНЯТЬ
for(i=0;i<6;i++){
switch(i){
  case 0:e1=0, e2=1, e3=2;break;
  case 1:e1=3, e2=4, e3=5;break;
  case 2:e1=7, e2=8, e3=9;break;
  case 3:e1=10,e2=11,e3=12;break;
  case 4:e1=14,e2=15,e3=16;break;
  case 5:e1=17,e2=18,e3=19;break;}
//ВОЗВРАЩАЕМСЯ К МАССИВУ a[i] \
ЗАПРОСИВ У RTC МОДУЛЯ ВРЕМЯ a[0]=wt.hour/10 МЫ ЗНАЕМ, ЧТО ПЕРВАЯ ПЕРЕМЕННАЯ, ЭТО ПЕРВАЯ \
ЦИФРА ЧАСОВ, В НАШЕМ УСЛОВНОМ ПРИМЕРЕ РАВНА ЦИФРЕ "2" ПОТОМУ, ЧТО УСЛОВНО СЕЙЧАС 20 ЧАСОВ \
ЦИФРА 2 ФОРМИРУЕТСЯ ИЗ 9 СИМВОЛОВ ОБОЗНАЧЕННЫХ ПЕРЕМЕННЫМИ d1,d2,d3,  d4,d5,d6,  d7,d8,d9  \
УСЛОВНО! ПЕРЕМЕННЫЕ РАЗБИТЫ НА 3 СТОЛБЦА, ПЕРВЫЕ 3 ЗНАЧЕНИЯ ДЛЯ СТРОКИ 0, ВТОРАЯ ГРУППА ИЛИ \
ВТОРОЙ СТОЛБЕЦ, ЭТО ЗНАЧЕНИЯ ДЛЯ СТРОКИ 1, НУ А ТРЕТЬЯ ГРУППА ДОГАДАЙТЕСЬ ДЛЯ КАКОЙ СТРОКИ САМИ \
ТАКИМ ОБРАЗОМ ДЛЯ ФОРМИРОВАНИЯ ЦИФРЫ 2 ПЕРЕМЕННЫЕ ПООЧЕРЁДНО ПРИМУТ СЛЕДУЮЩИЕ ЗНАЧЕНИЯ: \
d1=5,d2=3,d3=2,   d4=1,d5=3,d6=4,    d7=5,d8=3,d9=4;
switch(a[i]){
  case 0:d1=1,d2=3,d3=2,   d4=1,d5=32,d6=2,   d7=5,d8=3,d9=4;  break; //Цифра "0" 
  case 1:d1=5,d2=2,d3=32,  d4=32,d5=2,d6=32,  d7=5,d8=3,d9=4;  break; //Цифра "1" 
  case 2:d1=5,d2=3,d3=2,   d4=1,d5=3,d6=4,    d7=5,d8=3,d9=4;  break; //Цифра "2" 
  case 3:d1=5,d2=3,d3=2,   d4=32,d5=3,d6=2,   d7=5,d8=3,d9=4;  break; //Цифра "3" 
  case 4:d1=1,d2=32,d3=2,  d4=5,d5=3,d6=2,    d7=32,d8=32,d9=4;break; //Цифра "4" 
  case 5:d1=1,d2=3,d3=4,   d4=5,d5=3,d6=2,    d7=5,d8=3,d9=4;  break; //Цифра "5" 
  case 6:d1=1,d2=3,d3=4,   d4=1,d5=3,d6=2,    d7=5,d8=3,d9=4;  break; //Цифра "6" 
  case 7:d1=5,d2=3,d3=2,   d4=32,d5=32,d6=2,  d7=32,d8=32,d9=4;break; //Цифра "7" 
  case 8:d1=1,d2=3,d3=2,   d4=1,d5=3,d6=2,    d7=5,d8=3,d9=4;  break; //Цифра "8" 
  case 9:d1=1,d2=3,d3=2,   d4=5,d5=3,d6=2,    d7=5,d8=3,d9=4;  break;}//Цифра "9" 
//НАПЕЧАТАЕМ НА ЭКРАНЕ ПЕРВУЮ ЦИФРУ (В НАШЕМ УСЛОВНОМ ПРИМЕРЕ "2") \
ОБРАТИТЕ ВНИМАНИЕ, ЧТО В САМОЙ ВЕРХНЕЙ (ПЕРВОЙ) СТРОКЕ ПОД НОМЕРОМ "0" \
ПЕЧАТАЕМ ЗНАЧЕНИЯ ПЕРЕМЕННЫХ d1,d2,d3 \
ПО УЖЕ ОПРЕДЕЛЁННЫМ ВЫШЕ АДРЕСАМ В КАЖДОЙ СТРОКЕ е1,е2,е3 ДЛЯ НАШЕГО СЛУЧАЯ ЦИФРА "2", \
САМАЯ ПЕРВАЯ ЦИФРА РАСПОЛОЖЕННАЯ ПО АДРЕСАМ 1,2,3 В КАЖДОЙ СТРОКЕ \
ДЛЯ ВТОРОЙ ЦИФРЫ ЭТО БУДУТ АДРЕСА БУДУТ 3,4,5 НО С ДРУГИМИ ЗНАЧЕНИЯМИ ПЕРЕМЕННЫХ d1,d2,d3 \
ПОТОМУ, ЧТО ЦИФРА БУДЕТ ДРУГАЯ, В НАШЕМ ПРИМЕРЕ ЭТО ЦИФРА "0" (ДЛЯ ПРИМЕРА 20 ЧАСОВ ВЕЧЕРА)  
//ЗАПОЛНЯЕМ СИМВОЛАМИ ПЕРЕМЕННЫХ d1,d2,d3 ПЕРВУЮ СТРОКУ ПОД НОМЕРОМ "0"
lcd.setCursor(e1,0);lcd.write(uint8_t(d1));
lcd.setCursor(e2,0);lcd.write(uint8_t(d2));
lcd.setCursor(e3,0);lcd.write(uint8_t(d3));
//ЗАПОЛНЯЕМ СИМВОЛАМИ ПЕРЕМЕННЫХ d4,d5,d6 ВТОРУЮ СТРОКУ ПОД НОМЕРОМ "1"
lcd.setCursor(e1,1);lcd.write(uint8_t(d4));
lcd.setCursor(e2,1);lcd.write(uint8_t(d5));
lcd.setCursor(e3,1);lcd.write(uint8_t(d6));
//ЗАПОЛНЯЕМ СИМВОЛАМИ ПЕРЕМЕННЫХ d7,d8,d9 ТРЕТЬЮ СТРОКУ ПОД НОМЕРОМ "2"
lcd.setCursor(e1,2);lcd.write(uint8_t(d7));
lcd.setCursor(e2,2);lcd.write(uint8_t(d8));
lcd.setCursor(e3,2);lcd.write(uint8_t(d9));}
//ПЕЧАТАЕМ СИМВОЛЫ ТОЧЕК МЕЖДУ ЧАСАМИ И МИНУТАМИ И МЕЖДУ МИНУТАМИ И СЕКУНДАМИ, ДЛЯ КРАСОТЫ
lcd.setCursor(6,0);lcd.write(uint8_t(6));
lcd.setCursor(6,1);lcd.write(uint8_t(6));
lcd.setCursor(13,0);lcd.write(uint8_t(6));
lcd.setCursor(13,1);lcd.write(uint8_t(6));
//В ЧЕТВЕРТОЙ СТРОКЕ ПЕЧАТАЕМ ДАТУ, ТЕМПЕРАТУРУ И ДЕНЬ НЕДЕЛИ
lcd.setCursor(0,3);lcd.print(rtc.getDateStr());
lcd.print(' ');lcd.print(wt.dow);}



//СЧИТЫВАЕМ С ЕДИНСТВЕННОГО ДАТЧИКА DS18B20
void Temperature(){ 
uint8_t data[2];  //МАССИВ byte ДЛЯ ЗНАЧЕНИЯ ТЕМПЕРАТУРЫ 
ds.reset();       //ИНИЦИАЛИЗАЦИЯ СОВМЕСТНО СО СБРОСОМ ШИНЫ ДАННЫХ
ds.write(0xCC);   //СБРОС ПОИСКА ДАТЧИКА ПО АДРЕСУ
ds.write(0x44);   //ИЗМЕРЕНИЕ ТЕМПЕРАТУРЫ С ПЕРЕНОСОМ ДАННЫХ В ПАМЯТЬ 
if(mill-timer3>p1000){timer3=mill;//МОЖНО И ЧАЩЕ, ТОЛЬКО СМЫСЛА НЕТ
ds.reset();       //ПОВТОРНАЯ ИНИЦИАЛИЗАЦИЯ СОВМЕСТНО СО СБРОСОМ ШИНЫ ДАННЫХ
ds.write(0xCC);   //ПОВТОРНЫЙ СБРОС ПОИСКА ДАТЧИКА ПО АДРЕСУ
ds.write(0xBE);   //ЧТЕНИЕ ДАННЫХ ИЗ ПАМЯТИ ДАТЧИКА
data[0]=ds.read();//ЧТЕНИЕ ПАМЯТИ byte low 
data[1]=ds.read();//ЧТЕНИЕ ПАМЯТИ byte high 
temp=((data[1]<<8)|data[0])/16;}}//СКЛАДЫВАЕМ ЗНАЧЕНИЯ МАССИВА И ПОЛУЧАЕМ ТЕМПЕРАТУРУ 



//ФУНКЦИЯ ЧТЕНИЯ ИЗ МОДУЛЯ DS3231 ВРЕМЕНИ И ДАТЫ И ПРИСВОЕНИЯ ЗНАЧЕНИЙ ПЕРЕМЕННЫМ
void get_time(){
wt=rtc.getTime();//ЗАПРАШИВАЕМ АКТУАЛЬНЫЕ ДАННЫЕ
sethour=wt.hour;//ЧАСЫ
setmin=wt.min;//МИНУТЫ
setsec=wt.sec;//СЕКУНДЫ
setdate=wt.date;//ДЕНЬ МЕСЯЦА
setmon=wt.mon;//МЕСЯЦ
setyear=(wt.year-2000);//ГОД
setdow=wt.dow;}//ДЕНЬ НЕДЕЛИ



//ФУНКЦИЯ ЗАПИСИ ЗНАЧЕНИЙ ПЕРЕМЕННЫХ ВРЕМЕНИ И ДАТЫ В МОДУЛЬ DS3231 
void set_time(){
rtc.setTime(sethour,setmin,setsec);
rtc.setDate(setdate,setmon,(setyear+2000));
rtc.setDOW(setdow);}



//ФУНКЦИЯ ВЫВОДА НА ЭКРАН ВРЕМЕНИ, ДАТЫ И ДНЯ НЕДЕЛИ
void time_func(){
lcd.setCursor(1,2);lcd.print(rtc.getTimeStr());
lcd.setCursor(11,2);lcd.print(rtc.getDOWStr());
lcd.setCursor(1,3);lcd.print(rtc.getDateStr());
//lcd.setCursor(13,3);lcd.print(rtc.getTemp(),0);lcd.write(B11011111);//МОЖНО ЗАКОМЕНТИРОВАТЬ
}

//ФУНКЦИЯ ЗАПИСИ В ПАРАМЕТРОВ В АУДИОПРОЦЕССОР BD37534FV
void audio(){
bd.setLoudness_f(lon_f); //Initial setup 3 = default in IC (0)
   //250Hz / 400Hz / 800Hz / Prohibition, (0...3)

bd.setIn(input);//Input Selector = default in IC (0) //0...6

bd.setIn_gain(gain,mute);//Input Gain, Mute ON/OFF = default in IC (0,0)
   //Input Gain = 0 dB ... 20 dB, (0...20)
   //Mute ON/OFF = OFF / ON, (0 / 1)

bd.setVol(vol);      //Volume Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setFront_1(rf+fad);//Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setFront_2(lf+fad);//Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setRear_1(rt+fad); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
bd.setRear_2(lt+fad); //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)
//bd.setSub(sub);     //Fader Gain / Attenuation -79 dB ... +15 dB, (-79...15) default in IC (-∞dB)

bd.setBass_setup(bas_q,bas_c);//Bass setup, default in IC (0,0)
   //Bass Q = 0.5 / 1.0 / 1.5 / 2.0, (0...3)
   //Bass fo = 60Hz / 80Hz / 100Hz / 120Hz, (0...3)
bd.setMiddle_setup(mid_q,mid_c);//Middle setup, default in IC (0,0)
   //Middle Q = 0.75 / 1.0 / 1.25 / 1.5, (0...3)
   //Middle fo = 500Hz / 1kHz / 1.5kHz / 2.5kHz, (0...3)
bd.setTreble_setup(treb_q,treb_c);//Treble setup, default in IC (0,0)
   //Treble Q = 0.75 / 1.25, (0 / 1)
   //Treble fo = 7.5kHz / 10kHz / 12.5kHz / 15kHz, (0...3)

bd.setBass_gain(bass);    //Bass gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setMiddle_gain(midd);  //Middle gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setTreble_gain(treb);  //Treble gain = -20 dB ... +20 dB, (-20 ... 20) default in IC (0)
bd.setLoudness_gain(lon); //Loudness Gain = 0 dB ... +20 dB, (0 ... 20) default in IC (0)

//bd.setSetup_2(sub_f,sub_out,0,faza);//Initial setup 2, default in IC (0,0,0,0)
   //Subwoofer LPF fc = OFF / 55Hz / 85Hz / 120Hz / 160Hz, (0...4)
   //Subwoofer Output Select = LPF / Front / Rear / Prohibition, (0...3)
   //Level Meter RESET = HOLD / REST, (0 / 1)
   //LPF Phase = 0 / 180, (0 / 1)

//bd.mix(); // default in IC (0)

//bd.setSetup_1(0,2,1);//Initial setup 1 = default in IC (0,2,1)
   //Advanced switch time of Mute = 0.6 msec / 1.0 msec / 1.4 msec / 3.2 msec, (0...3)
   //Advanced switch time of Input = 4.7 msec / 7.1 msec / 11.2 msec / 14.4 msec, (0...3)
   //Advanced switch ON/OFF = OFF / ON, (0 / 1)
//bd.System_Reset();
}

//ФУНКЦИЯ ПЕРЕЗАПИСИ ПЕРЕМЕННЫХ В СЛУЧАЕ ИХ ИЗМЕНЕНИЯ
void EEPROM_UPDATE(){
EEPROM.update(0,vol+79);EEPROM.update(1,treb+20);EEPROM.update(2,midd+20);
EEPROM.update(3,bass+20);EEPROM.update(4,input);
EEPROM.update(9,lon);EEPROM.update(10,lon_f);EEPROM.update(11,rf+79);
EEPROM.update(12,lf+79);EEPROM.update(13,rt+79);EEPROM.update(14,lt+79);EEPROM.update(15,fad);
EEPROM.update(16,treb_c);EEPROM.update(17,mid_c);EEPROM.update(18,bas_c);
EEPROM.update(20,treb_q);EEPROM.update(21,mid_q);EEPROM.update(22,bas_q);EEPROM.update(23,led);
EEPROM.update(25,wait);EEPROM.update(27,phone);
EEPROM.update(90,num);EEPROM.update(91,volFM);
//ЦИКЛ ПЕРЕЗАПИСИ 20 ЧАСТОТ В EEPROM, АДРЕСА ЯЧЕЕК (highByte С 50 ПО 69, lowByte С 70 ПО 89)
for(byte b=0;b<20;b++){EEPROM.update(50+b,highByte(channel[b]));
                       EEPROM.update(70+b, lowByte(channel[b]));}
//ЦИКЛ ПЕРЕЗАПИСИ УРОВНЯ ЧУСТВИТЕЛЬНОСТИ ДЛЯ КАЖДОГО ИЗ 5 ВХОДОВ
for(byte c=1;c<6;c++){EEPROM.update(29+c,Connect[c]);}}


///////////////////////////////////////// END ///////////////////////////////////////////////
// int8_t   = char          от        -128 до 127
// uint8_t  = byte          от           0 до 255
// int16_t  = int           от      -32768 до 32767
// uint16_t = word          от           0 до 65535 
// int32_t  = long          от -2147483648 до 2147483647 
// uint32_t = unsigned long от           0 до 4294967295
