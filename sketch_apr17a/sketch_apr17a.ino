// Copyright (c) 2025 github.com/7-5
// Licensed under the MIT License
#include <SPI.h> // enables spi communication
#include <Wire.h> // enables i2c communication 
#include <Adafruit_GFX.h> // adafruit library for drawing text, shapes etc.
#include <Adafruit_SSD1306.h> // screen driver. i dont know if what i have is an ssd1306, my code has some weirdnesses, the tutorial i followed on getting the hardware to run had different labeled pins.
#include <DHT11.h> //
#define OLED_RESET -1 /// Reset pin # (or -1 if sharing Arduino reset pin). 
// comment is copied from the example of adafruit, this might be related to breaking that bridge thing from your arduino, unsure if it was about reset or oscillator, dont do that if you dont know what you are doing.
// the tutorial i followed had shown this also at the value 4. my memory may not be that good, i dont remember it affecting it.
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// i dont know if its 128x64 or 32, from the aspect ratio it looks like 64, but the option for 64 breaks the screen. my screen or hardware may be defective. i dont know this screen hardware.
// the important thing is it works, folks! no burnt electronics smell.
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET); // creates an object of display class, which we use to do things with the screen.
DHT11 dht11g(2); // the temperature and humidity sensor instance
extern volatile unsigned long timer0_millis; // this is where arduino stores the millis counter. we maybe werent supposed to see this lol. we need to do some changes on top of this to avoid some issues we will see later on.
// this is also what the clocks time value is, the clock display reads from here. the core, the counter itself. it counts in miliseconds, so 1000 of this is a second.

unsigned long alarmtotime(unsigned long x,int day){ //alarms[7] are integers stored in THHMM as digits, Toggle, Hour, Minute.
  return day*86400000+ (x%10000)/100*3600000 + x%100*60000;
}

void maindisplay(int temperature,int humidity,int format,int editmode,int editsection,int alarms[]){ // the function which shows us the displays of the screen and where we work the most.
// format is the [C/F][12/24] thing, you can make sense of it on the format% sections or by the switch case of it.
//  say if you wanna add kelvin or whatever R was for temperature, currently, you will need to edit that.
// editmode is whether its edit mode or not. edit section is the location of blinking cursor at edit mode.
// alarms[7] formatting given above on function named alarmtotimeç
// this display on the set text scale is 21 characters wide and 8 rows tall. writing more characters than wideness will go to the next row
// iirc we will use both write more characters than wideness, and also use \n's 
    display.clearDisplay(); // clears previous frame
  display.setCursor(0, 0); // this is the draw cursor, not the edit menu cursor.
  unsigned long t=timer0_millis; // operating on timer
  //row 1:
if (editmode) display.print("----# EDIT MENU #----"); 
else display.print("--# CLOCK DISPLAY #--");
// fit to 21 chars
  //row 2:
display.print("\n\n"); // uuh. i really need to sanitize it sometime. we are ig still at row 1, we do one space to jump to row 2, then goto row 3 with the second \n. 
  //row 3:
  display.print("Time: "); // independent of edit mode.

//for here, we have a section where we write hr min and sec with some rules.
  //hr 
 if (editsection==0 and editmode and timer0_millis%1000<500) display.print("--"); // if the blinking edit cursor is at the desired place, and its edit mode and condition to make it blink half a second 
else {
   if (t%(86400000/(1+format/2))/3600000<10) {display.print("0");display.print(t%(86400000/(1+format/2))/3600000);} // text gets written as 0:0:12 instead of 00:00:12 if we dont define these
   else display.print(t%(86400000/(1+format/2))/3600000);} //same thing, but not adding a 0, because its not smaller than 10. so, double digits is satisfied, keeping the ":"'s at place for our satisfaction.
    display.print(":"); //divider between the units
  //min
 if (editsection==1 and editmode and timer0_millis%1000<500) display.print("--");
else {
   if (t%3600000/60000<10) {display.print("0");display.print(t%3600000/60000);}
   else display.print(t%3600000/60000);}
    display.print(":");
  //sec
 if (editsection==2 and editmode and timer0_millis%1000<500) display.print("--");
else {
   if (t%60000/1000<10) {display.print("0");display.print(t%60000/1000);}
   else display.print(t%60000/1000);}
   display.print(" ");

   if (format/2) { //if format variable is larger than 1, we dont use military time
    if (t%86400000/3600000>11) display.print("PM ");
    else display.print("AM ");
   }else display.print("   ");

  if (editsection==3 and editmode and timer0_millis%1000<500) display.print("--\n"); // edit blinking cursor thing again
  else {switch(t%604800000/86400000) { // time, get modulo by week, divide by / day. get 0123456 from time for day of the week display. notice we are counting from 0 to 6 and not 1 to 7
  case 0:
  display.print("Mo\n"); // shortened Monday
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
  display.print("\n"); // if it breaks it will just not print a weekday. theres like no way it does but meh.
}}

// row 4 and 5:
  display.print("Temp: "); // temp display
  if (format%2) temperature=temperature*18/10+32;     // Fahrenheit calculator
  if (1000>temperature and temperature>99) {display.print(temperature);} // for unmoving read   temp: 101F  (note that F/C isnt printed here, just examples)
  if (100>temperature and temperature>9) {display.print("+");display.print(temperature);}     //temp: +21C
  if (10>temperature and temperature>-1) {display.print(" +");display.print(temperature);}    //temp:  +9C
  if (0>temperature and temperature>-10) {display.print(" -");display.print(temperature);}    //temp:  -2C  (idk whats the true range of the sensor...)
  if (-9>temperature and temperature>-100) {display.print("-");display.print(temperature);}   //temp: -10C
  if (temperature>999 or temperature<-99) {display.print("???");}                             //temp: ???F  (run away)
  
  display.print((format%2)?"F Hum: ":"C Hum: "); // the prophecy fulfilled
  if (humidity==100) display.print("100");               //hum: 100%       (father i cannot click the book. oh son it seems the book has shortcircuited)
  if (100!=humidity and humidity>9) display.print(" ");  //hum:  56%
  if (10>humidity) display.print("  ");                  //hum:   9%
  display.print(humidity);display.print("%\n\n");        //                (note that the number is printed here and the above were just examples of spacing)
  
//  row 6:
  if(!editmode){display.print("Alarms:\n");}
  if(editmode and (editsection>10 and editsection<25)){display.print("Alarm: "); //editsection values are defined in an odd way, this alarm section is made functional the last.
  //reminder that alarms dont have seconds support.
  //hr 
 if (editsection>10 and editsection<18 and timer0_millis%1000<500) display.print("--"); // is it alarm hour editing of any weekday? also is it blink?
else { 
   if (alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%(86400000/(1+format/2))/3600000<10) {display.print("0");display.print(alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%(86400000/(1+format/2))/3600000);}
// alarmtotime turns to time and is used on alarms[] input as modulo by date. this one here is foris if there is a 0 that needs adding. selects hour by modulo. 

   else display.print(alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%(86400000/(1+format/2))/3600000);} // when no need to add a 0
    display.print(":"); //seperator
  //min
 if (editsection>17 and editsection<25 and timer0_millis%1000<500) display.print("--");
else {
   if (alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%3600000/60000<10) {display.print("0");display.print(alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%3600000/60000);}
   else display.print(alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%3600000/60000);}
   display.print(" ");
   if (format/2) {  // alarms cant hide from the AM/PM display
    if (alarmtotime(alarms[(editsection+3)%7],(editsection+3)%7)%86400000/3600000>11) display.print("PM\n");
    else display.print("AM\n");
   }
   
   else display.print("  \n");


  }else if(editmode) display.print("Alarm editing:\n"); // i dont wanna show random time there. idk what to display


// row 7, 8:
  if (editsection==4 and editmode and timer0_millis%1000<500) display.print("-----# -"); //monday blinking
  else {if (alarms[0]/10000) display.print("-----# M"); //monday if toggled
        else display.print("-----# m");} //monday if not toggled
  if (editsection==5 and editmode and timer0_millis%1000<500) display.print("-");
  else {if (alarms[1]/10000) display.print("T"); //tuesday
        else display.print("t");}
  if (editsection==6 and editmode and timer0_millis%1000<500) display.print("-");
  else {if (alarms[2]/10000) display.print("W");
        else display.print("w");}
  if (editsection==7 and editmode and timer0_millis%1000<500) display.print("-");
  else {if (alarms[3]/10000) display.print("T");
        else display.print("t");}
  if (editsection==8 and editmode and timer0_millis%1000<500) display.print("-");
  else {if (alarms[4]/10000) display.print("F");
        else display.print("f");}
  if (editsection==9 and editmode and timer0_millis%1000<500) display.print("-");
  else {if (alarms[5]/10000) display.print("S");
        else display.print("s");}
  if (editsection==10 and editmode and timer0_millis%1000<500) display.print("- #-----\n");
  else {if (alarms[6]/10000) display.print("S #-----\n");
        else display.print("s #-----\n");} // (sunday :D)

    if (editmode) {if (editsection==25) display.print(">ok.\n"); //amazing change
   else display.print(" ok?\n");}
  display.display();
  }

  int format=0; // 0=24C 1=24F 2=12C 3=12F
  int editsection=0; //beautiful blinking cursor
  int temperature=0; //will read
  int humidity=0; //will read
  int editmode=0; //1 for editmode
  int butonl=0; // value, a value change is checked for an input
  int butonr=0; // you love interacting with your clock, dont you?
  int alarms[]={00000,00000,00000,00000,00000,00000,00000}; // OHHMM ON/OFF, HOUR, MINUTE

void setup() {
  dht11g.setDelay(200); // a story: i asked gpt for optimizations because my display was delayed.
  // i thought of calculations but it was the sensor reading, in its own library theres this function to change that.
  // gpt couldnt figure out either, it thought the display was slow, despite the adafruit example being lightning fast 
  // this is why you cant trust ai for anything. and also i was wrong to not trust calculating speed.
  // anyways. the default value is 500 miliseconds, that messes with second accuracy. this value doesnt, values lower than this do it too because sensor apparently needs some time.
  pinMode(11,OUTPUT); //buzzer pin
  pinMode(10,INPUT); //left button pin
  pinMode(9,INPUT); //right button pin
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // beautiful displaydisplay
  display.clearDisplay();
  display.setTextSize(1); //dont change if you dont know what you are doing
  display.setTextColor(WHITE); // no other color is defined... for my case ig
  display.setCursor(0, 0);
}
// heres some numbers
// week 604800000
// days 86400000
// 12hr 43200000
// hour 3600000
// minu 60000
// seco 1000

// internal design comments
// when edit mode 0
// lbutton makes editmode=1 and editsection=0. rbutton changes format.
// while edit mode 1, edited sect needs to blink
// editsection current 0 hour | 1 minute | 2 second | 3 day | [4,10] alarm day select on off | [11,17] alarm hour | [18,24] alarm minute | 25 ok confirmation |
// editsect | highlight |lbutt| rbutt
// 0        | timehour  |goto1| addmodulo to hour by timer0_millis
// 1        | timemin   |goto2| addmodulo to minu by timer0_millis
// 2        | timesec   |3    | addmodulo to seco by timer0_millis
// 3        | timeday   |4    | addmodulo to wday by timer0_millis
// 4,9      | alarmdays |+1   | if untoggled, toggle and editsect +7, if on, turn off.
// 10       | alarmdays |25   | if untoggled, toggle and editsect +7, if on, turn off.
// 11,17    | alarmhr   |+7   | addto corresponding alarm hour
// 18,24    | alarmin   |-14  | addto corresponding alarm minute
// 25       | ok?       |em=0 | editmode=0

void loop() {
  if (butonl-digitalRead(10)==1) { // checks for button left pressed and released implements the comment table above for left button
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
  if (butonr-digitalRead(9)==1) { //button right pressed and released. implements the comment table above for right button
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

  }  else format=(format+1)%4; // if not edit mode, right button will cycle format options.
  }
  butonl=digitalRead(10);butonr=digitalRead(9);
  maindisplay(temperature,humidity,format,editmode,editsection,alarms); //this is where the screen runs
  if (timer0_millis%3000<400 and timer0_millis%3000>290 ) dht11g.readTemperatureHumidity(temperature, humidity); // this function is sensor read. it will update the said values its done every 3 second. dont mess with the 290.
  if (timer0_millis>1209600000UL) timer0_millis = timer0_millis - 1209600000UL; 
  // that is double of a week. millis is a value set in limited amount of digits. if we allowed it to overflow, we would have to reset the clock every 49 days or something, i forgot.
  // this is literally the only reason i used timer0_millis and not the millis(). this is why you want to modify it, this device should not require manual rearming every day.

  int cnt=0; // this is the loop section for alarm implementation
  unsigned long v,j;
  for (unsigned long i=0; i<7;i++){
    if (alarms[i]/10000){ // if toggled alarm
  // next part defines a absolute vale function and checks if any need to run 
	if (alarmtotime(alarms[i],i)%604800000>timer0_millis%604800000) v=alarmtotime(alarms[i],i)%604800000- timer0_millis%604800000;
	else v=timer0_millis%604800000-alarmtotime(alarms[i],i)%604800000;
	if (v>302400000) j=v-302400000;
	else j=302400000-v;
	if (j>302400000-30000) cnt=1;}}           //the time duration of the alarm is that 30000. that is 30 seconds. if alarm is set at 6:00, it will play between 5:59:30 - 6:00:30
  if (cnt) digitalWrite(11,1);              //even if it is 30 seconds, because its absolute value, it will play for one minute. the buzzer sound is brutal on the ears btw.
  else digitalWrite(11,0);                  //i had another non arduino board and setting the tone for buzzers for some reason made the millis stop. its better to handle it with pins now.
}