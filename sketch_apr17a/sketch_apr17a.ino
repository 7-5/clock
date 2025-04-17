#include <SPI.h>
#include <Wire.h>
#include <DHT11.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
#define LOGO_HEIGHT 16 // Removed semicolon
#define LOGO_WIDTH 16 // Removed semicolon
DHT11 dht11g(2); 
extern volatile unsigned long timer0_millis;


void maindisplay(int temperature,int humidity,int format,int editmode,int editsection,int alarms[]){
    display.clearDisplay();
  display.setCursor(0, 0);
  unsigned long t=timer0_millis;
  //satır 1:
if (editmode) display.print("----# EDIT MENU #----");
else display.print("--# CLOCK DISPLAY #--");
  //satır 2:
display.print("\n\n");

  //satır 3:
  display.print("Time: ");
  //hr 
 if (editsection==0 and editmode and timer0_millis%2000>1000) display.print("--");
else {
   if (t%(86400000/(1+format/2))/3600000<10) {display.print("0");display.print(t%(86400000/(1+format/2))/3600000);}
   else display.print(t%(86400000/(1+format/2))/3600000);}
    display.print(":");
  //min
 if (editsection==1 and editmode and timer0_millis%2000>1000) display.print("--");
else {
   if (t%3600000/60000<10) {display.print("0");display.print(t%3600000/60000);}
   else display.print(t%3600000/60000);}
    display.print(":");
  //sec
 if (editsection==2 and editmode and timer0_millis%2000>1000) display.print("--");
else {
   if (t%60000/1000<10) {display.print("0");display.print(t%60000/1000);}
   else display.print(t%60000/1000);}
   display.print(" ");
   if (format/2) { 
    if (t%86400000/3600000>11) display.print("PM ");
    else display.print("AM ");
   }else display.print("   ");
  if (editsection==3 and editmode and timer0_millis%2000>1000) display.print("--\n");
  else {switch(t%604800000/86400000) {
  case 0:
  display.print("Mo\n");
    break;
  case 1:
  display.print("Tu\n");
    break;
  case 2:
  display.print("We\n");
    break;
  case 3:
  display.print("Th\n");
    break;
  case 4:
  display.print("Fr\n");
    break;
  case 5:
  display.print("Sa\n");
    break;
  case 6:
  display.print("Su\n");
    break;
  default:
  display.print("\n");
}}

// satır 4 ve 5:
  display.print("Temp: ");
  if (format%2) temperature=temperature*18/10+32;
  if (1000>temperature and temperature>99) {display.print(temperature);}
  if (100>temperature and temperature>9) {display.print("+");display.print(temperature);}
  if (10>temperature and temperature>-1) {display.print(" +");display.print(temperature);}
  if (0>temperature and temperature>-10) {display.print(" -");display.print(temperature);}
  if (-9>temperature and temperature>-100) {display.print("-");display.print(temperature);}
  if (temperature>999 or temperature<-99) {display.print("???");}
  
  display.print((format%2)?"F Hum: ":"C Hum: ");
  if (humidity==100) display.print("100");
  if (100!=humidity and humidity>9) display.print(" ");
  if (10>humidity) display.print("  ");
  display.print(humidity);display.print("%\n\n");
  
//  satır 6:
//Alarm: 10:51 AM Mo   #
  if(!editmode){display.print("Alarms:\n");}
  else{display.print("Alarm: ");
  //hr 
 if (editsection>10 and editsection<18 and editmode and timer0_millis%2000>1000) display.print("--");
else {
   if (t%(86400000/(1+format/2))/3600000<10) {display.print("0");display.print(t%(86400000/(1+format/2))/3600000);}
   else display.print(t%(86400000/(1+format/2))/3600000);}
    display.print(":");
  //min
 if (editsection>17 and editsection<25 and editmode and timer0_millis%2000>1000) display.print("--");
else {
   if (t%3600000/60000<10) {display.print("0");display.print(t%3600000/60000);}
   else display.print(t%3600000/60000);}
   display.print(" ");
   if (format/2) { 
    if (t%86400000/3600000>11) display.print("PM\n");
    else display.print("AM\n");
   }else display.print("  \n");


  }


// satır 7, 8:
//(3<editsection and editsection<11 and editmode and timer0_millis%2000>1000) display.print("--");
  if (editsection==4 and editmode and timer0_millis%2000>1000) display.print("-----# -");
  else {if (alarms[0]/10000) display.print("-----# M");
        else display.print("-----# m");}
  if (editsection==5 and editmode and timer0_millis%2000>1000) display.print("-");
  else {if (alarms[1]/10000) display.print("T");
        else display.print("t");}
  if (editsection==6 and editmode and timer0_millis%2000>1000) display.print("-");
  else {if (alarms[2]/10000) display.print("W");
        else display.print("w");}
  if (editsection==7 and editmode and timer0_millis%2000>1000) display.print("-");
  else {if (alarms[3]/10000) display.print("T");
        else display.print("t");}
  if (editsection==8 and editmode and timer0_millis%2000>1000) display.print("-");
  else {if (alarms[4]/10000) display.print("F");
        else display.print("f");}
  if (editsection==9 and editmode and timer0_millis%2000>1000) display.print("-");
  else {if (alarms[5]/10000) display.print("S");
        else display.print("s");}
  if (editsection==10 and editmode and timer0_millis%2000>1000) display.print("- #-----\n");
  else {if (alarms[6]/10000) display.print("S #-----\n");
        else display.print("s #-----\n");}

    if (editmode) {if (editsection==25) display.print(">ok.\n");
   else display.print(" ok?\n");}
  display.display();
  }

  int format=0; // 0=24C 1=24F 2=12C 3=12F
  int editsection=0;
  int temperature=0;
  int humidity=0;
  int editmode=0;
  int butonl=0;
  int butonr=0;
  int alarms[]={00000,00000,00000,00000,00000,00000,00000}; // OHHMM ON/OFF, HOUR, MINUTE

void setup() {
  dht11g.setDelay(200);
  pinMode(11,OUTPUT); //buzzer pin
  pinMode(10,INPUT); //sol butonpin
  pinMode(9,INPUT); //sağ buton
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Removed extra closing parenthesis
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

}
void loop() {
  if (butonl-digitalRead(10)==1) { //button left pressed
  if (!editmode) {editmode=1; editsection=0;}
  else{
  switch (editsection) {
  case 0 ... 9:editsection++;
    break;
  case 10:editsection=25;
    break;
  case 11 ...17:editsection+=7;
    break;
  case 18 ... 24:editsection-=14;
    break;
  case 25:editmode=0;
    break;
  default:
    editmode=0;
  }}}
  if (butonr-digitalRead(9)==1) { //button right pressed
  if (editmode) {
  switch (editsection) {
  case 0: timer0_millis=timer0_millis - timer0_millis%86400000  +(timer0_millis%86400000/3600000+1)%24*3600000 + timer0_millis%3600000;
    break;
  case 1: timer0_millis=timer0_millis - timer0_millis%3600000  +(timer0_millis%3600000/60000+1)%60 *60000+timer0_millis%60000;
    break;
  case 2: timer0_millis=timer0_millis - timer0_millis%60000  +(timer0_millis%60000/1000+1)%60 *1000+timer0_millis%1000;
    break;
  case 3: timer0_millis=timer0_millis - timer0_millis%604800000  +(timer0_millis%604800000/86400000+1)%7*86400000+ timer0_millis%86400000;
    break;
  case 4: 
    if (1-alarms[0]/10000) {alarms[0]+=10000; editsection+=7;}
    else alarms[0]-=10000;
    break;
  case 5:
    if (1-alarms[1]/10000) {alarms[1]+=10000; editsection+=7;}
    else alarms[1]-=10000;
    break;
  case 6:
    if (1-alarms[2]/10000) {alarms[2]+=10000; editsection+=7;}
    else alarms[2]-=10000;
    break;
  case 7:
    if (1-alarms[3]/10000) {alarms[3]+=10000; editsection+=7;}
    else alarms[3]-=10000;
    break;
  case 8:
    if (1-alarms[4]/10000) {alarms[4]+=10000; editsection+=7;}
    else alarms[4]-=10000;
    break;
  case 9:
    if (1-alarms[5]/10000) {alarms[5]+=10000; editsection+=7;}
    else alarms[5]-=10000;
    break;
  case 10:
    if (1-alarms[6]/10000) {alarms[6]+=10000; editsection+=7;}
    else alarms[6]-=10000;
    break;
  case 11:alarms[0]=alarms[0]/10000*10000 +(alarms[0]%10000/100+1)%24*100 + alarms[0]%100;
    break;
  case 12:alarms[1]=alarms[1]/10000*10000 +(alarms[1]%10000/100+1)%24*100 + alarms[1]%100;
    break;
  case 13:alarms[2]=alarms[2]/10000*10000 +(alarms[2]%10000/100+1)%24*100 + alarms[2]%100;
    break;
  case 14:alarms[3]=alarms[3]/10000*10000 +(alarms[3]%10000/100+1)%24*100 + alarms[3]%100;
    break;
  case 15:alarms[4]=alarms[4]/10000*10000 +(alarms[4]%10000/100+1)%24*100 + alarms[4]%100;
    break;
  case 16:alarms[5]=alarms[5]/10000*10000 +(alarms[5]%10000/100+1)%24*100 + alarms[5]%100;
    break;
  case 17:alarms[6]=alarms[6]/10000*10000 +(alarms[6]%10000/100+1)%24*100 + alarms[6]%100;
    break;
  case 18:alarms[0]=alarms[0]/100*100 +(alarms[0]%100+1)%60;
    break;
  case 19:alarms[1]=alarms[1]/100*100 +(alarms[1]%100+1)%60;
    break;
  case 20:alarms[2]=alarms[2]/100*100 +(alarms[2]%100+1)%60;
    break;
  case 21:alarms[3]=alarms[3]/100*100 +(alarms[3]%100+1)%60;
    break;
  case 22:alarms[4]=alarms[4]/100*100 +(alarms[4]%100+1)%60;
    break;
  case 23:alarms[5]=alarms[5]/100*100 +(alarms[5]%100+1)%60;
    break;
  case 24:alarms[6]=alarms[6]/100*100 +(alarms[6]%100+1)%60;
    break;
  case 25: editmode=0;
    break;
  default:
    editmode=0;
  }

  }  else format=(format+1)%4;
  }
  butonl=digitalRead(10);butonr=digitalRead(9);
  maindisplay(temperature,humidity,format,editmode,editsection,alarms);
  if (timer0_millis%3000<400 and timer0_millis%3000>290 ) dht11g.readTemperatureHumidity(temperature, humidity);

  if (timer0_millis>1209600000UL) timer0_millis = timer0_millis - 1209600000UL;
}

// week 604800000
// days 86400000
// 12hr 43200000
// hour 3600000
// minu 60000
// seco 1000

// edit mode 0
// lbutton editmode=1 yapar,editsection 0 yapar. rbutton format değiştirir.
// edit mode 1 iken, editlenen kısım yanıp söner
// editsection current 0 hour | 1 minute | 2 second | 3 day | [4,10] alarm day select on off | [11,17] alarm hour | [18,24] alarm minute | 25 ok confirmation |
// editsect | highlight |lbut | rbut
// 0        | timehour  |1egeç| timer0_millisdan saate ekle modülo
// 1        | timemin   |2yegç| timer0_millisdan dkkaya ekle modülo
// 2        | timesec   |3    | timer0_millisdan seconda ekle modülo
// 3        | timeday   |4    | timer0_millisdan haftaya ekle modülo
// 4,9      | alarmdays |+1   | off ise on yap, on ise editsect +7
// 10       | alarmdays |25   | off ise on yap, on ise editsect +7
// 11,17    | alarmhr   |+7   | modülo saat ekle
// 18,24    | alarmin   |-14  | modülo dakka ekle
// 25       | ok?       |em=0 | 

// display.print("ataturk"); //21x8

//--# CLOCK DISPLAY #--#
//                     #
//Time: 11:53:21 AM Mo #
//Temp: +93C Hum: 092% #
//                     #
//Alarms:              #
//-----# MTWTFss #-----#
//                     #
//######################

//----# EDIT MENU #----#
//                  AO # 
//Time: AA:BB:CC FF DD #
//Temp: FFFF Hum: 092% #
//                     #
//Alarm: 10:51 AM Mo   #
//-----# MTWTFss #-----#
//>ok?                 #
//######################

