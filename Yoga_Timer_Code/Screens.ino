void change_mm(){
  tft.fillScreen(BLACK);
  tft.fillRect(19, 15, 112, 90, ORANGE);
  tft.fillRect(179, 15, 112, 90, ORANGE);
  tft.fillRect(100, 135, 112, 90, ORANGE);
  tft.fillRect(260, 135, 80, 90, BROWN);

  tft.setTextSize(3);
  tft.setCursor(30, 27);
  tft.print("Time");
  tft.setCursor(30, 50);
  tft.print("Per");
  tft.setCursor(30, 73);
  tft.print("Pose");

  tft.setCursor(190, 27);
  tft.print("Pose");
  tft.setCursor(190, 50);
  tft.print("Per");
  tft.setCursor(190, 73);
  tft.print("Time");
  
  tft.setTextSize(4);
  tft.setCursor(111, 155);
  tft.print("Rest");
  tft.setCursor(111, 185);
  tft.print("Time");
}

void change_vm(){
  tft.fillScreen(BLACK);
  tft.fillRect(5, 60, 145, 120, ORANGE);
  tft.drawRect(5, 60, 145, 120, WHITE);
  tft.fillRect(164, 0, 155, 239, ORANGE);
  tft.drawRect(164, 0, 155, 239, WHITE);

//Lines for input
  tft.drawRect(20, 169, 20, 2, WHITE);
  tft.drawRect(50, 169, 20, 2, WHITE);
  tft.drawRect(80, 169, 20, 2, WHITE);
  tft.drawRect(110, 169, 20, 2, WHITE);

//Lines for pad
  tft.drawRect(219, 0, 2, 240, WHITE);
  tft.drawRect(274, 0, 2, 240, WHITE);
  tft.drawRect(164, 59, 155, 2, WHITE);
  tft.drawRect(164, 119, 155, 2, WHITE);
  tft.drawRect(164, 179, 155, 2, WHITE);

//Numbers
  tft.setTextSize(4);
  tft.setCursor(182, 16);
  tft.print("7");
  tft.setCursor(237, 16);
  tft.print("8");
  tft.setCursor(292, 16);
  tft.print("9");
  tft.setCursor(182, 76);
  tft.print("4");
  tft.setCursor(237, 76);
  tft.print("5");
  tft.setCursor(292, 76);
  tft.print("6");
  tft.setCursor(182, 136);
  tft.print("1");
  tft.setCursor(237, 136);
  tft.print("2");
  tft.setCursor(292, 136);
  tft.print("3");
  tft.setCursor(182, 196);
  tft.print((char) 0x1B);
  tft.setCursor(237, 196);
  tft.print("0");
  tft.setCursor(292, 196);
  tft.print((char) 0xF4);
}

void change_tm(){
  tft.fillScreen(BLACK);
  tft.fillRect(40, 10, 240, 140, ORANGE);
  tft.fillRect(40, 170, 110, 50, ORANGE);
  tft.fillRect(170, 170, 110, 50, ORANGE);

  tft.setCursor(45, 15);
  tft.setTextSize(3);
  tft.print("Current Timer");
  tft.setCursor(50, 174);
  tft.setTextSize(1);
  tft.print("Poses Left");
  tft.setCursor(180, 174);
  tft.print("Time Left");
//  tft.setCursor(295, 212);
//  tft.setTextSize(4);
//  tft.setTextColor(WHITE, BROWN);
//  tft.print("X");
//  tft.setTextColor(WHITE, ORANGE);
}

void change_sm(){
  tft.fillScreen(BLACK);
  tft.fillRect(19, 15, 112, 90, ORANGE);
  tft.fillRect(179, 15, 112, 90, ORANGE);
  tft.fillRect(260, 180, 60, 60, BROWN);

  tft.setTextSize(2);
  tft.setCursor(30, 27);
  tft.print("Discrete");
  tft.setCursor(30, 50);
  tft.print("ON");

  tft.setCursor(190, 27);
  tft.print("Indiscrete");
  tft.setCursor(190, 50);
  tft.print("ON");

  tft.setCursor(40, 120);
  tft.setTextColor(WHITE, BLACK);
  if(is_discrete)
    tft.print("Discrete ON  ");
  else if(!is_discrete)
    tft.print("Indiscrete ON");
  tft.setTextColor(WHITE, ORANGE);

  tft.setCursor(295, 212);
  tft.setTextSize(4);
  tft.setTextColor(WHITE, BROWN);
  tft.print("X");
  tft.setTextColor(WHITE, ORANGE);
}
