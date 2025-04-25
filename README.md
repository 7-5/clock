# Arduino Alarm Clock
 alarm clock on arduino for a school course project with temperature and humidity sensor, inspired by casio watches UI (this project fits more to a table clock). user documentation is below, i just have to flex my memory. 
 
 i built the electronics first, and then coded it from what i planned on my mind. didnt need to adjust anything in electronics in the coding stage, also didnt even pay for anything news and some are scrap parts. my friends were impressed at how i implemented it solo. i mean i dont wanna be that guy, i get that i have great memory too, but our past curriculum included good courses on c/c++, dsa and analysis, electronics, software development architecture so i think its still doable with maybe doing diagrams too? idk. 
 
![[thesketch]](readmethings/fritzingsketchedited.png)

this sketch edited to fit what i have as hardware, the dht11 sensor is actually on a small PCB, the main sensor has 4 pins, mine has 3 unlabeled ones, the rightmost one has a minus sign at the side. i also have a different looking buzzer, its like 1 centimeter radius and is small.

## user documentation
 if you replicate the entire electronics and the code and end up with the same setup, you will end up with a clock thats similar to casio watches. 
 
 it will have two mods, namely clock display and the edit mode. clock display will show time accurate to seconds, the date, temperature and humidity, also it will show if a day of the week has a set alarm. it supports the previously mentioned things, also clock localization format editing and setting alarms for each day of the week and navigation of menus via two buttons provided. 
 
 the right button on clock display mode will change the localization format, by presses (1,2,3,4) (modulo 4, 4th one will be default) corresponding to (F24,C12,F12,C24), 24 being military time (14:53), 12 being non-boomer time (2:53 PM), F being Fahrenheit and C as celsius. while in the clock display mode, pressing the left button will enter edit menu. 
 
 pressing right button in the edit menu will increment modulo the highlighted (blinking) section, and increments wont affect the flow of time, so if you are at \[00\]:06:01 and press the right button, it will be \[01\]:06:01 and not \[01\]:00:00. modulo addition wont add to the higher digit (by saying higher digit here i mean minute, hour, day), thus respecting your editing intentions and also keeping track of the flowing time in parallel without pausing. on the edit mode, using right button will get you to editing sections (starting from 0 presses) (hour,minute,second,day of the week, alarms for mon,tue,wed,thu,fri,sat,sun, confirm button.) using any of the buttons will get you out of the edit menu back into the clock display screen. on alarm editing section, lets say that the wednesday is highlighted by edit cursor. i will write actions in (press1:action;press2:action2;) format to describe what is happening (R:capitalises the W standing for Wednesday, indicating alarm on the determined time is set to play, also sending the cursor to wednesday alarms hour section;R: modulo increments hour by 1;L:shifts the cursor to Wednesdays alarm minute edit;L:shifts back to alarm day setting,R:lowercases the letter of W, unsetting the alarm;L:moves cursor to thursday's t;)

## coders documentation
 code has explanatory comments, you are free to do things for self use, dont claim credit for my work, dont ever do that or ***i will find you***.

 buttons need higher resistances, a book i use suggests 10k ohm resistances for buttons on 5V but i just didnt have sufficient, i could easily parallel wire but i dont want the resistances exposed with their long legs, i dont want to cut them because i have limited electronics, i also dont want them touching. if you know me in real life and have the means to do so, PLEASE DONATE ME ELECTRONICS I BEG YOU I WILL DO ANYTHING. (im a skeptical buyer i dont even purchase things online. (i also dont possess much money, also no cypto curengcy (typed funny to not get attention of scrapers made by evil people. (maybe github is a mistake)))) anyhow if you have them you can put 10k resistors, if not, only down side i saw from low resistance for buttons is, on another project, the buttons were more prone to noise and errors from magnetic fields, idk if that's related to it or not, probably is.
 
 be careful which dht11 library you use or you will have problems. its repo is provided by a comment next to the header for it.

 feel free to open an issue, i will read it.

## todo list
 i want to do things, if you want to do them by yourself, i wont stop you, but the things here, i want to do by myself, keeping the written-by-me aspect of code, later on, maybe. this is like a what is there to implement kind of thing. now this project by itself is complete as of school project by the way it is rn, but id love to expand the features.

 to do: add kelvin and the R temperature thing i forgot whatever that is. make a menu for the format thing inspired by the old nokia settings thing. or maybe not. make ok option into ok or cancel changes. add dates, although that would require a lot of effort to edit. also add a way to implement some sort of battery to keep it running. but with all that, i will need a way to hard reset the clock. we could even, if we are crazy enough, implement the other calendar types like a total madman. i could also get the pesky numbers and strings without the string library to work with a platform that allows implementation of arbitrary coder time measurement systems, giving the users to maybe add mars time, or user defined non 24:60:60 type of thing, which may need something like 000:00:00 kind of thing. maybe its better to handle things with creating other source files. the format divider can be subject to edit too, no need to just rely on ":". an option to shut/postpone the alarms while ringing, set the time the buzzer will ring for, set the postpone timer. display of the next alarm date, a bell like icon to tell the user an alarm exists in the next 24 hours. an option to close the next alarm without unsetting it. updating documentation and writing with better grammar check and ensuring better readability, creating a separate documentation markdown file. make alarm not run on +-time, but on the said time instead. and last as stated too.

## License
 This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
 
