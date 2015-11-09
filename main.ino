#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 10;  
int ledPin = 11;

int todo; 
int opened; 
int steps = 1;
int direct;
int place;
int mode;
int screen;

long previousMillis;
long interval = 100;

IRrecv irrecv(RECV_PIN);
decode_results results;

String digit, doneit;

String channels[] = {
  "zero",
  "http://www.youtube.com/watch?v=G17x99Sh424",
  "http://www.youtube.com/watch?v=EZbrSm5psT4",
  "http://www.youtube.com/watch?v=jUkoL9RE72o",
  "http://www.youtube.com/watch?v=7q2bNqe0Xyk",
  "http://www.youtube.com/watch?v=pMPrxg1bSPE",
  "http://www.youtube.com/watch?v=J4Mb5Yt_ylk",
  "http://www.youtube.com/watch?v=Zcps2fJKuAI",
  "http://www.youtube.com/watch?v=-SIgu2j4Nzs",
  "http://www.youtube.com/watch?v=mdJDPepGOAM",
  "http://www.shoutcast.com/shoutcast_popup_player?station_id=1254140&play_status=1&stn=Chillout%20Dreams%20-%20DIGITALLY%20IMPORTED%20-%20relax...", 
  "http://www.youtube.com/watch?v=bX5uSguogQM"
};

void setup()
{ 
  irrecv.enableIRIn(); 
  pinMode(ledPin, OUTPUT);  
  digitalWrite(ledPin, HIGH);
  
  digit = String("");
  doneit = String("");
}

void loop() {
  if (irrecv.decode(&results)) {
    if(results.value == 0x210700FF) {
      digit += 0;
    } else if (results.value == 0x2107807F) {
      digit += 1;
    } else if (results.value == 0x210740BF) {
      digit += 2;
    } else if (results.value == 0x2107C03F) {
      digit += 3;
    } else if (results.value == 0x210720DF) {
      digit += 4;
    } else if (results.value == 0x2107A05F) {
      digit += 5;
    } else if (results.value == 0x2107609F) {
      digit += 6;
    } else if (results.value == 0x2107E01F) {
      digit += 7;
    } else if (results.value == 0x210710EF) {
      digit += 8;
    } else if (results.value == 0x2107906F) {
      digit += 9;
    } else if (results.value == 0x2107EA15) {
      todo = digit.toInt();
      if (todo > 0 && todo < 12) {
        place = todo;
        doneit = channels[todo];
        doit(); 
        digit = "";
        todo = 0;
      } else {
        error();
      }
    } else if (results.value == 0x210706F9) {
      volup();
      direct = 5;
    } else if (results.value == 0x21078679) {
      voldown();
      direct = 6;
    } else if (results.value == 0x210726D9) {
      mute();
      direct = 0;
    } else if (results.value == 0x21071EE1) {
      refresh();
      direct = 0;
    } else if (results.value == 0x2107D926) {
      closewin();
      opened = 0;
      direct = 0;
    } else if (results.value == 0x210709F6) {
      if(screen == 0) {
        screen = 1;
      }
      screenswitch();
      direct = 0;
    } else if (results.value == 0x2107CE31) {
      steps = 1;
      up();
      direct = 1;
    } else if (results.value == 0x21076E91) {
      if(screen == 1) {
        Keyboard.set_key1(KEY_LEFT);
        Keyboard.send_now();
        Keyboard.set_key1(0);
        Keyboard.send_now();
      } else {
        steps = 1;
        left();
        direct = 2;
      }
    } else if (results.value == 0x21072ED1) {
      steps = 1;
      down();
      direct = 3;
    } else if (results.value == 0x2107AE51) {
      if(screen == 1) {
        Keyboard.set_key1(KEY_RIGHT);
        Keyboard.send_now();
        Keyboard.set_key1(0);
        Keyboard.send_now();
      } else {
        steps = 1;
        right();
        direct = 4;
      }
    } else if (results.value == 0x2107EE11) {
      if(screen == 1) {
        screen = 0;
        screenswitch();
      } else {
        direct = 0;
        leftmouse();
      }
    } else if (results.value == 0x21070EF1) {
      direct = 0;
      rightmouse();
    } else if (results.value == 0x21070AF5) {
      place++;
      if (place > 11) {
        place = 11;
        error();
      } else {
        doneit = channels[place];
        doit(); 
      }
    } else if (results.value == 0x21078A75) {
      place = place - 1;
      if (place < 0) {
        place = 0;
        error();
      } else if (place > 0) {
      doneit = channels[place];
      doit(); 
      } else {
        if (opened == 1) {
          closewin();
        }
      }
    }
    
    if(direct > 0 && results.value == 0xFFFFFFFF) {
      if(direct == 1) {
        up();
      } else if (direct == 2) {
        left();
      } else if (direct == 3) {
        down();
      } else if (direct == 4) {
        right();
      } else if (direct == 5) {
        volup();
      } else if (direct == 6) {
        voldown();
      } 
     
      
      unsigned long currentMillis = millis();

      if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;   

        steps++;
        
        if (steps > 19) {
          steps = 20;
        }
      }
    }    
    irrecv.resume();
  }
}

void doit()  {
  if (opened == 0) {
    opened = 1;
  } else {
    closewin();
  }
  
  Keyboard.set_modifier(MODIFIERKEY_GUI);
  Keyboard.set_key1(KEY_R);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  delay(100);
  delay(100);
  Keyboard.print(doneit); 
  delay(100);
  Keyboard.set_key1(KEY_ENTER);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
  
  doneit = "";
}

void error()  {
  digitalWrite(ledPin, LOW);
  delay(400);
  digitalWrite(ledPin, HIGH);
  delay(400);
  digitalWrite(ledPin, LOW);
  delay(400);
  digitalWrite(ledPin, HIGH);
  delay(400);
  digitalWrite(ledPin, LOW);
  delay(400);
  digitalWrite(ledPin, HIGH);
  delay(400);
  digitalWrite(ledPin, LOW);
  delay(400);
  digitalWrite(ledPin, HIGH);
}

void refresh()  {
  Keyboard.set_key1(KEY_F5);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void closewin()  {
  Keyboard.set_modifier(MODIFIERKEY_ALT);
  Keyboard.set_key1(KEY_F4);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void volup()  {
  Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
  Keyboard.set_key1(KEY_UP);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void voldown()  {
  Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
  Keyboard.set_key1(KEY_DOWN);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void mute()  {
  Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
  Keyboard.set_key1(KEY_PERIOD);
  Keyboard.send_now();
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void up()  {
  Mouse.move(0, -steps);
}

void left()  {
  Mouse.move(-steps, 0);
}

void down()  {
  Mouse.move(0, steps);
}

void right()  {
  Mouse.move(steps, 0);
}

void leftmouse()  {
  Mouse.click();
}

void rightmouse()  {
  Mouse.set_buttons(0, 0, 1);
  Mouse.set_buttons(0, 0, 0);
}

void screenswitch()  {
  if (screen == 1) {
    Keyboard.set_modifier(MODIFIERKEY_ALT);
    Keyboard.set_key1(KEY_TAB);
    Keyboard.send_now();
    Keyboard.set_key1(0);
    Keyboard.send_now();
  } else {
    Keyboard.set_modifier(0);
    Keyboard.send_now();
  }
}
