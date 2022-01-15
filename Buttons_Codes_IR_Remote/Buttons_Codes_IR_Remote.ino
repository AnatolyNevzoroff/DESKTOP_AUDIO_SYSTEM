
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
//#define IR_11 0x8C837 // КНОПКА "OK"
#define IR_12 0x8708F // ГЛАВНОЕ МЕНЮ 
#define IR_13 0x8B04F // НАУШНИКИ ИЛИ УСИЛИТЕЛЬ 

#include <IRremote.h>
IRrecv irrecv(2);//УКАЗЫВАЕМ ВЫВОД "2" К КОТОРОМУ ПОДКЛЮЧЁН IR ПРИЁМНИК (INPUT INFRARED RECEIVER)
decode_results ir;//ОБЪЯВЛЯЕМ ОБЪЕКТ "ir" ДЛЯ КЛАССА БИБЛИОТЕКИ ИНФРАКРАСНОГО ПРИЁМНИКА
//unsigned long от 0 до 4294967295
uint32_t irkey;

void setup(){
Serial.begin(9600); 
irrecv.enableIRIn();//ИНИЦИАЛИЗИРУЕМ БИБЛИОТЕКУ ИНФРАКРАСНОГО ПРИЁМНИКА
}

void loop() {
//////////////////////////////// ЧТЕНИЕ ДАННЫХ С ИК ПРИЕМНИКА ///////////////////////////////
if(irrecv.decode(&ir)){
Serial.print("0x");Serial.println(ir.value,HEX); 
if(ir.value!=IR_10){irkey=ir.value;delay(150);}//ПРЕДОТВРАЩАЕМ СЛУЧАЙНЫЕ ПОВТОРЫ ОТ НАЖАТИЯ
irrecv.resume();}
}
