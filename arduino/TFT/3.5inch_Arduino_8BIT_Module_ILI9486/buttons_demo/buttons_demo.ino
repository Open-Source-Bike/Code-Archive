/***********************************************************************************
*This program is a demo of how to use the touch function in a phone GUI
*This demo was made for LCD modules with 8bit or 16bit data port.
*This program requires the the LCDKIWI library.

* File                : light_control.ino
* Hardware Environment: Arduino UNO&Mega2560
* Build Environment   : Arduino

*Set the pins to the correct ones for your development shield or breakout board.
*This demo use the BREAKOUT BOARD only and use these 8bit data lines to the LCD,
*pin usage as follow:
*                  LCD_CS  LCD_CD  LCD_WR  LCD_RD  LCD_RST  SD_SS  SD_DI  SD_DO  SD_SCK 
*     Arduino Uno    A3      A2      A1      A0      A4      10     11     12      13                            
*Arduino Mega2560    A3      A2      A1      A0      A4      10     11     12      13                           

*                  LCD_D0  LCD_D1  LCD_D2  LCD_D3  LCD_D4  LCD_D5  LCD_D6  LCD_D7  
*     Arduino Uno    8       9       2       3       4       5       6       7
*Arduino Mega2560    8       9       2       3       4       5       6       7 

*Remember to set the pins to suit your display module!
**********************************************************************************/

#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV my_lcd(ILI9486,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

                             /*  r     g    b */
#define BLACK        0x0000  /*   0,   0,   0 */
#define BLUE         0x001F  /*   0,   0, 255 */
#define RED          0xF800  /* 255,   0,   0 */
#define GREEN        0x07E0  /*   0, 255,   0 */
#define CYAN         0x07FF  /*   0, 255, 255 */
#define MAGENTA      0xF81F  /* 255,   0, 255 */
#define YELLOW       0xFFE0  /* 255, 255,   0 */
#define WHITE        0xFFFF  /* 255, 255, 255 */
#define NAVY         0x000F  /*   0,   0, 128 */
#define DARKGREEN    0x03E0  /*   0, 128,   0 */
#define DARKCYAN     0x03EF  /*   0, 128, 128 */
#define MAROON       0x7800  /* 128,   0,   0 */
#define PURPLE       0x780F  /* 128,   0, 128 */
#define OLIVE        0x7BE0  /* 128, 128,   0 */
#define LIGHTGREY    0xC618  /* 192, 192, 192 */
#define DARKGREY     0x7BEF  /* 128, 128, 128 */
#define ORANGE       0xFD20  /* 255, 165,   0 */
#define GREENYELLOW  0xAFE5  /* 173, 255,  47 */
#define PINK         0xF81F  /* 255,   0, 255 */

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//touch sensitivity for X
#define TS_MINX 906
#define TS_MAXX 116

//touch sensitivity for Y
#define TS_MINY 92
#define TS_MAXY 952

//touch sensitivity for press
#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//Open Source Bike Constants
#define PADDING 8
#define RADIUS 10
#define BASEHEIGHT 60

//Open Source Bike Helpers
int MAXY = my_lcd.Get_Display_Width()-1;
int MAXX = my_lcd.Get_Display_Height()-1;
int FULLWIDTH =  my_lcd.Get_Display_Width()-(PADDING*2);
int HALFWIDTH =  (FULLWIDTH/2)-(PADDING/2);
int THIRDWIDTH =  (FULLWIDTH/3)-(PADDING-2);
int FOURTHWIDTH =  (FULLWIDTH/4)-(PADDING);
int FIFTHWIDTH =  (FULLWIDTH/5)-(PADDING-2);
int STARTX = 0;
int STARTY = 0;
int AGGY = 0;
int AGGX = 0;
int FOOTERHEIGHT = 30;
int FONTSIZE = 2;
int ROWHEIGHT = BASEHEIGHT;

// Display box button
void show_box_button(uint8_t *str,int16_t x,int16_t y,int16_t w,int16_t h,uint8_t csize,uint16_t fc,uint16_t bc,boolean mode,uint16_t dc,uint16_t lc, uint8_t charxpad, uint8_t charypad)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Set_Draw_color(dc);
    my_lcd.Fill_Round_Rectangle(x,y,x+w,y+h,RADIUS); 
    show_string(str,x+PADDING+charxpad,y+PADDING+charypad,csize,fc,bc,mode);
    AGGX = AGGX+w; 
}

// Display string
void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

// Set X/Y values to start new row of a given height
void new_row(uint8_t height, uint8_t add_padding)
{
  STARTX = PADDING;
  STARTY = AGGY+PADDING+add_padding;
  ROWHEIGHT = height;
  AGGY = AGGY+PADDING+height+add_padding;
  AGGX = PADDING;
}

// Add padding space to the current AGGX
void row_space(uint8_t pixels)
{
  AGGX = AGGX+pixels;
}

//display the UI
void show_screen() {
  
  // Draw the footer first so we reserve the space
  show_footer();
  
  // A half-height button to turn on headlights
  new_row(BASEHEIGHT/2,0);
  show_box_button("HEADLIGHT",STARTX,STARTY,FULLWIDTH,ROWHEIGHT,FONTSIZE,WHITE,WHITE,0,DARKGREY,LIGHTGREY,90,0);
  
  // Double size buttons for left and right turn signal
  new_row(BASEHEIGHT*2,0);
  FONTSIZE=4;
  show_box_button("LEFT",STARTX,STARTY,HALFWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,20,30);
  row_space(PADDING);
  show_box_button("RIGHT",AGGX,STARTY,HALFWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,10,30);
  
  // Half-height running light button
  new_row(BASEHEIGHT/2,0);
  FONTSIZE=2;
  show_box_button("RUNNING LIGHTS",STARTX,STARTY,FULLWIDTH,ROWHEIGHT,FONTSIZE,WHITE,WHITE,0,DARKGREY,LIGHTGREY,60,0);
  
  // Slightly oversize buttons for left and right
  new_row(BASEHEIGHT*1.5,0);
  FONTSIZE=3;
  show_box_button("LEFT",STARTX,STARTY,THIRDWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,5,25);
  row_space(PADDING);
  show_box_button("OFF",AGGX,STARTY,THIRDWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,12,25);
  row_space(PADDING);
  show_box_button("RIGHT",AGGX,STARTY,THIRDWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,0,25);

  // Normal button size for 4 button set
  new_row(BASEHEIGHT,0);
  FONTSIZE=3;
  show_box_button("1",STARTX,STARTY,FOURTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,19,13);
  row_space(PADDING+2);
  show_box_button("2",AGGX,STARTY,FOURTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,17,13);
  row_space(PADDING+2);
  show_box_button("3",AGGX,STARTY,FOURTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,17,13);
  row_space(PADDING+2);
  show_box_button("4",AGGX,STARTY,FOURTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,17,13);

  // Normal button size for 4 button set
  new_row(BASEHEIGHT*.9,0);
  FONTSIZE=2;
  show_box_button("A",STARTX,STARTY,FIFTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,13,11);
  row_space(PADDING+2);
  show_box_button("B",AGGX,STARTY,FIFTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,13,11);
  row_space(PADDING+2);
  show_box_button("C",AGGX,STARTY,THIRDWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,34 ,11);
  row_space(PADDING+2);
  show_box_button("D",AGGX,STARTY,FOURTHWIDTH,ROWHEIGHT,FONTSIZE,BLACK,BLACK,0,WHITE,LIGHTGREY,21,11);
}

void show_footer() {
  FONTSIZE = 2;
  my_lcd.Draw_Fast_HLine(0,MAXX-FOOTERHEIGHT,MAXY);
  show_string("< Open Source Bike />",CENTER,MAXX-FOOTERHEIGHT+FONTSIZE+5,FONTSIZE,WHITE,WHITE,1);
  MAXX=MAXX-FOOTERHEIGHT;
} 
                           
void setup(void) 
{
  Serial.begin(9600);
   my_lcd.Init_LCD();
   Serial.println(my_lcd.Read_ID(), HEX);
   my_lcd.Fill_Screen(BLACK); 
   show_screen();
}

void loop(void)
{
  uint16_t i;
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    //p.x = my_lcd.Get_Display_Width()-map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(), 0);
    //p.y = my_lcd.Get_Display_Height()-map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(), 0);
    p.x = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(),0);
    p.y = map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(),0);

  }
}
