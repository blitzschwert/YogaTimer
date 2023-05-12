#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
//#include <SdFat.h>                // SD card & FAT filesystem library
//#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
//#include <Adafruit_ImageReader.h>


//// The FT6206 uses hardware I2C (SCL/SDA)
//Adafruit_FT6206 ctp = Adafruit_FT6206(18, 4);
//
// The display uses SPI through the ESP32
#define TFT_CS 2    //2 to TCS
#define TFT_DC 3    //3 to DC
#define TFT_MOSI 0  //0 to MOSI
#define TFT_MISO 1  //1 to MISO
#define TFT_SCLK 10 //10 to SCK
#define TFT_RST 9   //
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);

#define SCL 5       //5 to SCL
#define SDA 6       //6 to SDA

Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

#define LED 19
#define MOTOR 18
#define BATTERY 4

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

#define BLACK 0x0000
#define ORANGE 0xD1E1
#define WHITE 0xffff
#define BROWN 0xd405

// Defining screen states and setting intial screen
#define MAIN_MENU 0
#define VAR_MENU_1 1
#define VAR_MENU_2 2
#define REST_MENU 3
#define TIMER_MENU 4
#define SETTING_MENU 5
bool screen_change = true;
int cur_screen = MAIN_MENU;
bool time_first;
bool is_discrete = false;

// Creating timer variables
bool timer_running = false;
int timer_val_1;
int timer_val_2;
int rest_val = 0;

// Peripherals and battery variables
bool battery_low = false;
int step_count = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Connected!");

  tft.begin(80000000);
  Wire.setPins(6, 5);
  ctp.begin(40);
  
  accel.begin();
  accel.setRange(ADXL343_RANGE_2_G);

  tft.setRotation(3);
  tft.setTextColor(WHITE, ORANGE);

//  esp_light_sleep_start();

//  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  analogWrite(LED, 0);

}

//Return a number based on location of a numpad
int num_input(int x, int y) {
//789 Row
  if (y > 0 && y < 59) {
    if (x > 155 && x < 209)
      return 7;
    else if (x > 210 && x < 264)
      return 8;
    else if (x > 265 && x < 319)
      return 9;
  }
//456 Row
  else if (y > 60 && y < 119) {
    if (x > 155 && x < 209)
      return 4;
    else if (x > 210 && x < 264)
      return 5;
    else if (x > 265 && x < 319)
      return 6;
  }
//123 Row
  else if (y > 120 && y < 179) {
    if (x > 155 && x < 209)
      return 1;
    else if (x > 210 && x < 264)
      return 2;
    else if (x > 265 && x < 319)
      return 3;
  }
//0 Confirm Deny Row
  else if (y > 180 && y < 239) {
    if (x > 155 && x < 209)
      return -2;
    else if (x > 210 && x < 264)
      return 0;
    else if (x > 265 && x < 319)
      return -1;
  }

  return -3;
}

void check_step(){
//  if(accel step)
//    step_count++;
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("Step Count: ");
  tft.print(step_count);
  tft.print("  ");
}

// This checks where the user touched the screen and uses the cur_screen
// variable to see what should be placed in the input string
void get_input()  {
  Serial.println("I am getting input");
  int input = 0;
  int x = 0;
  int y = 0;
  String result = "";
  TS_Point p;

//For each case
//  Take x and y
//  Find which option was picked
//  Store data into input string
  switch (cur_screen) {
    case MAIN_MENU:
      while(!ctp.touched()) {
//        check_step();
        ;
      }
      p = ctp.getPoint();
      x = p.y;
      y = 239 - p.x;
//    This is for the top 2 buttons
      if (y > 15 && y < 105) {
        if (x > 20 && x < 140) {
          screen_change = true;
          cur_screen = VAR_MENU_1;
          time_first = false;
        }
        else if (x > 180 && x < 300) {
          screen_change = true;
          cur_screen = VAR_MENU_1;
          time_first = true;
        }
        delay(1);
      }
//    This is for the bottom button
      else if (y > 135 && y < 225) {
        if (x > 100 && x < 220) {
          screen_change = true;
          cur_screen = REST_MENU;
          return;
        }
        else if (x > 260 && x < 320) {
          screen_change = true;
          cur_screen = SETTING_MENU;
        }
      }
      break;
      
//  Next 3 sections will be very similar
    case VAR_MENU_1:
      tft.setCursor(20, 70);
      tft.setTextSize(2);
      if (!time_first) {
        tft.print("# of Poses");
      }
      else {
        tft.print("Total Time");
        tft.setCursor(20, 85);
        tft.print("Minutes");
      }
      
      while (input != -1) {
        while (!ctp.touched()){;}
        p = ctp.getPoint();
        x = p.y;
        y = 239 - p.x;
        input = num_input(x, y);
        switch (input) {
          case -1:
            screen_change = true;
            cur_screen = VAR_MENU_2;
            break;
          case -2:
            if (result.length() != 0) {
              result.remove(result.length() - 1);
            }
            break;
          case -3:
            break;
          default:
            if ((result.length() == 0 && input == 0) || (result.length() == 4)) {
              break;
            }
            result += String(input);
            break;
        }
        for (int i = 0; i < 4; i++) {
          tft.setCursor(25 + i * 30, 150);
          if (result.length() <= i)
            tft.print(" ");
          else
            tft.print(result[i]);
        }
        delay(250);
      }
      timer_val_1 = result.toInt();
      break;
      
    case VAR_MENU_2:
      tft.setCursor(20, 70);
      tft.setTextSize(2);
      if (time_first) {
        tft.print("# of Poses");
      }
      else {
        tft.print("Pose Time");
        tft.setCursor(20, 85);
        tft.print("Seconds");
      }
      
      while (input != -1) {
        while (!ctp.touched()){;}
        p = ctp.getPoint();
        x = p.y;
        y = 239 - p.x;
        input = num_input(x, y);
        switch (input) {
          case -1:
            screen_change = true;
            cur_screen = TIMER_MENU;
            break;
          case -2:
            if (result.length() != 0) {
              result.remove(result.length() - 1);
            }
            break;
          case -3:
            break;
          default:
            if ((result.length() == 0 && input == 0) || (result.length() == 4)) {
              break;
            }
            result += String(input);
            break;
        }
        for (int i = 0; i < 4; i++) {
          tft.setCursor(25 + i * 30, 150);
          if (result.length() <= i)
            tft.print(" ");
          else
            tft.print(result[i]);
        }
        delay(250);
      }
      timer_val_2 = result.toInt();
      break;
      
    case REST_MENU:
      tft.setCursor(20, 70);
      tft.setTextSize(2);
      tft.print("Rest Time");
      tft.setCursor(20, 85);
      tft.print("Seconds");
      
      while (input != -1) {
        while (!ctp.touched()){;}
        p = ctp.getPoint();
        x = p.y;
        y = 239 - p.x;
        input = num_input(x, y);
        
        switch (input) {
          case -1:
            screen_change = true;
            cur_screen = MAIN_MENU;
            break;
          case -2:
            if (result.length() != 0) {
              result.remove(result.length() - 1);
            }
            break;
          case -3:
            break;
          default:
            if ((result.length() == 0 && input == 0) || (result.length() == 4)) {
              break;
            }
            result += String(input);
            break;
        }
        for (int i = 0; i < 4; i++) {
          tft.setCursor(25 + i * 30, 150);
          if (result.length() <= i)
            tft.print(" ");
          else
            tft.print(result[i]);
        }
        delay(250);
      }
      rest_val = result.toInt();
      break;
      
    case TIMER_MENU:
//    if touch x, cancel timer
      timer_running = true;
      break;

    case SETTING_MENU:
      while(!ctp.touched()) {;}
      p = ctp.getPoint();
      x = p.y;
      y = 239 - p.x;

      if(y > 15 && y < 105){
        if(x > 18 && x < 132){
          is_discrete = true;
          
        }
        else if(x > 178 && x < 292){
          is_discrete = false;
        }
      }
      else if(y > 179 && y < 239 && x > 259 && x < 319){
        screen_change = true;
        cur_screen = MAIN_MENU;
      }
      tft.setCursor(40, 120);
      tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK);
      if(is_discrete)
        tft.print("Discrete ON  ");
      else if(!is_discrete)
        tft.print("Indiscrete ON");
      tft.setTextColor(WHITE, ORANGE);
  }
  delay(250);
}

void change_screen()  {
  if (!screen_change) {
    return;
  }
  screen_change = false;

  switch (cur_screen) {
    case MAIN_MENU:
//    change background to main menu
//      reader.drawBMP("/m_menu.bmp", tft, 0, 0);
      change_mm();
      break;

    case VAR_MENU_1:
//      reader.drawBMP("/v_menu.bmp", tft, 0, 0);
      change_vm();
      break;
//    change background to var_menu
      
    case VAR_MENU_2:
//      reader.drawBMP("/v_menu.bmp", tft, 0, 0);
      change_vm();
//    change background to var_menu
      break;
      
    case REST_MENU:
//      reader.drawBMP("/v_menu.bmp", tft, 0, 0);
      change_vm();
//    change background to rest_menu
      break;
      
    case TIMER_MENU:
//      reader.drawBMP("/t_menu.bmp", tft, 0, 0);
      change_tm();
//    change background to timer_menu
      break;
      
    case SETTING_MENU:
      change_sm();
      break;
  }
}

void alert(){
  if (is_discrete){
////    flash light but no motor
//    digitalWrite(LED, HIGH);
//    delay(250);
//    digitalWrite(LED, LOW);
    analogWrite(LED, 15);
    delay(250);
    analogWrite(LED, 0);
  }
  else if (!is_discrete){
//    flash light and run motor
//    digitalWrite(LED, HIGH);
//    digitalWrite(MOTOR, HIGH);
//    delay(250);
//    digitalWrite(LED, LOW);
//    digitalWrite(MOTOR, LOW);
    analogWrite(LED, 255);
    delay(250);
    analogWrite(LED, 0);
  }
}

// Need to check if timer is still running
void check_timer()  {
  if (!timer_running) {
    return;
  }
  tft.setTextSize(2);

  int total_timer, set_timer, sets, timer;
  if (time_first){
    sets = timer_val_2;
    total_timer = timer_val_1 * 60;
    set_timer = total_timer / sets;
  }
  else {
    sets = timer_val_1;
    set_timer = timer_val_2;
    total_timer = sets * set_timer;
  }
  for (int i = 0; i < sets; i++){
    tft.setCursor(50, 194);
    tft.print(sets - i);
    tft.print("/");
    tft.print(sets);
    tft.print("  ");
    tft.setCursor(180, 194);
    tft.print(total_timer + rest_val * (sets - 1 - i) - (i + 1) * set_timer);
    tft.print(" ");
    timer = set_timer - 1;
    while (timer >= 0){
      tft.setCursor(50, 80);
      for (int mili = 900; mili >= 0; mili -= 100){
        tft.print(timer);
        tft.print(".");
        tft.print(mili);
        delay(31);
        tft.print(" ");
        tft.setCursor(50, 80);
      }
    timer--;
    }
    alert();
    if (i == sets - 1)
      break;
    timer = rest_val - 1;
    tft.setCursor(50, 100);
    tft.print("Rest");
    while (timer >= 0){
      tft.setCursor(50, 80);
      for (int mili = 900; mili >= 0; mili -= 100){
        tft.print(timer);
        tft.print(".");
        tft.print(mili);
        delay(31);
        tft.print(" ");
        tft.setCursor(50, 80);
      }
    timer--;
    }
    alert();
    tft.setCursor(50, 100);
    tft.print("    ");
  }

  cur_screen = MAIN_MENU;
  screen_change = true;
  timer_running = false;
//  delay(2000);
  change_screen();
}

void check_power(){
  int val = analogRead(BATTERY);

  tft.setCursor(0, 120);
  tft.setTextColor(WHITE);

  if (val <= 950)
    tft.print("LOW");
  else if (val > 950)
    tft.print("   ");
}

void loop() {
  change_screen();
  check_timer();

//  tft.displayOff();
//  delay(2000);
//  tft.displayOn();
  get_input();
}


//TODO:
//  Add peripherals functionality
//    Accelerometer
//  Pedometer
