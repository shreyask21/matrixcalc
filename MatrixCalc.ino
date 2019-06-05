/***********************************************************************************
    Matrix Calc on Arduino with 4x4 keypad and Nokia 5110 (PCD8544) LCD.
    ©ShreyasK

    Connect the matrix keypad to Arduino Nano as follows:
    ####################################################
    D9         ->    R1 (Keypad Pin 1, from left of the keypad face)
    D8         ->    R2
    D7         ->    R3
    D6         ->    R4
    D5         ->    C1
    D4         ->    C2
    D3         ->    C3
    D2         ->    C4
    ####################################################

    Connect the Nokia LCD to Arduino Nano as follows:
    ####################################################
    A2         ->     RST
    A3         ->     CE
    A4         ->     DC
    D11        ->     DIN
    A5         ->     CLK
    VCC        ->     3.3V
    BL/Light   ->     GND or 3.3V (Optional and depends on your display, mine is wired to 3.3V.)
    GND        ->     GND
    ####################################################

    You can modify the parameters according to your board:
*/
/* Config */
/* Keypad: */
#define ColPin1 5
#define ColPin2 4
#define ColPin3 3
#define ColPin4 2
#define RowPin1 9
#define RowPin2 8
#define RowPin3 7
#define RowPin4 6
/* LCD */
#define SetLCDBias //comment that out if you have blue backlight LCD.
int RST = 16; //A2
int CE = 17; //A3
int DC = 18; //A4
int DIN = 11; //D11
int CLK = 19; //A5
/***********************************************************************************/

#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Keypad.h>

/*Keypad Layout Config*/
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
/************************/

/*Splash screen bitmap*/
const unsigned char bmp [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x00,
  0x80, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x01, 0x80, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0xf0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf1,
  0xf3, 0xe6, 0xec, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xb3, 0xfb, 0xe7, 0xec, 0xee, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x6d, 0xb3, 0x19, 0x87, 0x0c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d,
  0xb0, 0x79, 0x86, 0x0c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0xb1, 0xf9, 0x86, 0x0c, 0x38,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x33, 0x99, 0x86, 0x0c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x67, 0x33, 0x19, 0x86, 0x0c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x33, 0xf9, 0xe6, 0x0c,
  0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x31, 0xec, 0xe6, 0x0c, 0xc6, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7f, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0xc0,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x87, 0xc6, 0x1e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xe6, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x0c, 0x66, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0xe6, 0x60, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xe6, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xe0, 0x8e, 0x66, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0xcc, 0x66, 0x73, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x8f, 0xe6, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1f, 0x07, 0xb6, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfc, 0x1f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x00, 0x03, 0x02, 0x31, 0x98, 0x00, 0x00,
  0x00, 0x00, 0x06, 0x60, 0x00, 0x06, 0x01, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x06, 0xc0, 0x00,
  0x04, 0xec, 0xb8, 0x1b, 0x1e, 0x79, 0x8c, 0xf1, 0xe6, 0xc0, 0x00, 0x09, 0xfc, 0x9e, 0x1d, 0x98,
  0xcd, 0x8d, 0x1b, 0x37, 0xc0, 0x00, 0x0b, 0x9c, 0x8f, 0x19, 0x98, 0xcc, 0xd8, 0x7b, 0x87, 0x60,
  0x00, 0x0b, 0x18, 0x83, 0x99, 0x98, 0xfc, 0xd8, 0xd9, 0xe6, 0x60, 0x00, 0x0b, 0x39, 0x01, 0x99,
  0x98, 0xc0, 0xd9, 0x98, 0x76, 0x60, 0x00, 0x0b, 0xfb, 0x31, 0x99, 0x98, 0xcc, 0x71, 0x9b, 0x36,
  0x30, 0x00, 0x05, 0xdc, 0x1f, 0x19, 0x98, 0x78, 0x70, 0xf9, 0xe6, 0x30, 0x00, 0x04, 0x00, 0x80,
  0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
/************************/

/* Keypad Pins */
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {ColPin1, ColPin2, ColPin3, ColPin4};
/************************/

/*Element co-ordinates for display*/
int colpos[4] = {0, 12, 39, 65};
int rowpos[4] = {0, 5, 20, 35};
/************************/

/*Matrices as int array*/
int A[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int B[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int C[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int resultmat[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/************************/

/* Integers for storing results*/
int det = 0, i = 0;
/************************/

/* Integers for storing contrast adjustment*/
int defaultcontrast, setcontrast;
/************************/

/*Initialize and create objects*/
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //Init keypad as "keypad" object.
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST); //Init Nokia 5110 display driver.
/************************/

void setup()  {
  /*Init display with parameters and show splash screen*/
  display.begin();
  chkContrast();
  display.setContrast(EEPROM.read(256));
  display.drawBitmap(0, 0, bmp, 84, 44, 1);
  display.display();
  delay(3500);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  /************************/

  main_menu(); //Start the main menu
}

/*Main Menu*/
void main_menu() {
  display.clearDisplay();
  display.drawLine(0, 0, 83, 0, BLACK);
  display.drawLine(0, 1, 83, 1, BLACK);
  display.setCursor(0, 2);
  display.setTextColor(WHITE, BLACK);
  display.print("  MAIN  MENU  ");
  display.setTextColor(BLACK);
  display.drawLine(0, 10, 83, 10, BLACK);
  display.drawLine(0, 0, 0, 47, BLACK);
  display.drawLine(83, 0, 83, 47, BLACK);
  display.drawLine(0, 47, 83, 47, BLACK);
  display.setCursor(4, 12);
  display.print("1:MatA 2:MatB");
  display.setCursor(4, 21);
  display.print("3:Det  4:Adj");
  display.setCursor(4, 29);
  display.print("5:Inv  6:Mul");
  display.setCursor(4, 37);
  display.print("7:Add  8:Sub");
  display.display();
  char key = keypad.getKey();
  while (1)
  {
    switch (key)
    {
      case NO_KEY:
        break;
      case '1':
        getmatrixA();
        break;
      case '2':
        getmatrixB();
        break;
      case '3':
        showdet();
        break;
      case '4':
        showadj();
        break;
      case '5':
        showinv();
        break;
      case '6':
        mulmatrix();
        break;
      case '7':
        addmatrix();
        break;
      case '8':
        submatrix();
        break;
      case 'A':
        AdjustContrast();
        break;
    }
    key = keypad.getKey();
  }
}
/************************/

/*Get user input for matrix "A"*/
void getmatrixA() {
  display.clearDisplay();
  display.display();
  drawBrackets();

  A[0] = getnumber(colpos[1], rowpos[1]);
  A[1] = getnumber(colpos[2], rowpos[1]);
  A[2] = getnumber(colpos[3], rowpos[1]);
  A[3] = getnumber(colpos[1], rowpos[2]);
  A[4] = getnumber(colpos[2], rowpos[2]);
  A[5] = getnumber(colpos[3], rowpos[2]);
  A[6] = getnumber(colpos[1], rowpos[3]);
  A[7] = getnumber(colpos[2], rowpos[3]);
  A[8] = getnumber(colpos[3], rowpos[3]);

  display.display();
  delay(1000);
  splash_text("Press Any Key", 1);
  main_menu();
}
/************************/

/*Get user input for matrix "B"*/
void getmatrixB() {
  display.clearDisplay();
  display.display();
  drawBrackets();

  B[0] = getnumber(colpos[1], rowpos[1]);
  B[1] = getnumber(colpos[2], rowpos[1]);
  B[2] = getnumber(colpos[3], rowpos[1]);
  B[3] = getnumber(colpos[1], rowpos[2]);
  B[4] = getnumber(colpos[2], rowpos[2]);
  B[5] = getnumber(colpos[3], rowpos[2]);
  B[6] = getnumber(colpos[1], rowpos[3]);
  B[7] = getnumber(colpos[2], rowpos[3]);
  B[8] = getnumber(colpos[3], rowpos[3]);

  display.display();
  delay(1000);
  splash_text("Press Any Key", 1);
  main_menu();
}
/************************/

/*Calculate Adjoint*/
void showadj() {
  splash_text("  Adjoint A", 0);
  displayElements( ((A[4]*A[8]) - (A[7]*A[5])), ((A[7]*A[2]) - (A[1]*A[8])), ((A[1]*A[5]) - (A[4]*A[2])),
                   ((A[6]*A[5]) - (A[3]*A[8])), ((A[0]*A[8]) - (A[6]*A[2])), ((A[3]*A[2]) - (A[0]*A[5])),
                   ((A[3]*A[7]) - (A[6]*A[4])), ((A[6]*A[1]) - (A[0]*A[7])), ((A[0]*A[4]) - (A[3]*A[1])));

  waitforinput();

  splash_text("  Adjoint B", 0);
  displayElements( ((B[4]*B[8]) - (B[7]*B[5])), ((B[7]*B[2]) - (B[1]*B[8])), ((B[1]*B[5]) - (B[4]*B[2])),
                   ((B[6]*B[5]) - (B[3]*B[8])), ((B[0]*B[8]) - (B[6]*B[2])), ((B[3]*B[2]) - (B[0]*B[5])),
                   ((B[3]*B[7]) - (B[6]*B[4])), ((B[6]*B[1]) - (B[0]*B[7])), ((B[0]*B[4]) - (B[3]*B[1])));

  waitforinput();

  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Calculate Inverse*/
void showinv() {
  splash_text("  A Inverse", 0);
  det = getdet(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[8]);

  displayElements( ((A[4]*A[8]) - (A[7]*A[5])) / det, ((A[7]*A[2]) - (A[1]*A[8])) / det, ((A[1]*A[5]) - (A[4]*A[2])) / det,
                   ((A[6]*A[5]) - (A[3]*A[8])) / det, ((A[0]*A[8]) - (A[6]*A[2])) / det, ((A[3]*A[2]) - (A[0]*A[5])) / det,
                   ((A[3]*A[7]) - (A[6]*A[4])) / det, ((A[6]*A[1]) - (A[0]*A[7])) / det, ((A[0]*A[4]) - (A[3]*A[1])) / det);

  waitforinput();

  splash_text("  B inverse", 0);
  det = getdet(B[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7], B[8]);

  displayElements( ((B[4]*B[8]) - (B[7]*B[5])) / det, ((B[7]*B[2]) - (B[1]*B[8])) / det, ((B[1]*B[5]) - (B[4]*B[2])) / det,
                   ((B[6]*B[5]) - (B[3]*B[8])) / det, ((B[0]*B[8]) - (B[6]*B[2])) / det, ((B[3]*B[2]) - (B[0]*B[5])) / det,
                   ((B[3]*B[7]) - (B[6]*B[4])) / det, ((B[6]*B[1]) - (B[0]*B[7])) / det, ((B[0]*B[4]) - (B[3]*B[1])) / det);

  waitforinput();

  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Show Determinant of both matrices one by one*/
void showdet() {
  det = getdet(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[8]);
  displayResult("Determinant A", det);
  waitforinput();

  det = getdet(B[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7], B[8]);
  displayResult("Determinant B", det);
  waitforinput();
  main_menu();
}
/************************/

/*Calculate determinant*/
int getdet(int a, int b, int c, int d, int e, int f, int g, int h, int i) {

  int ans;
  ans = ( (a * ( (e * i) - (h * f))) - (b * ((d * i) - (g * f))) + (c * ((d * h) - (g * e))) );
  return ans;

}
/************************/

/*Multiplies matrix "A" and "B"*/
void mulmatrix() {
  splash_text("  A X B:", 0);
  displayElements(  ((A[0]*B[0]) + (A[1]*B[3]) + (A[2]*B[6])), ((A[0]*B[1]) + (A[1]*B[4]) + (A[2]*B[7])), ((A[0]*B[2]) + (A[1]*B[5]) + (A[2]*B[8])),
                    ((A[3]*B[0]) + (A[4]*B[3]) + (A[5]*B[6])), ((A[3]*B[1]) + (A[4]*B[4]) + (A[5]*B[7])), ((A[3]*B[2]) + (A[4]*B[5]) + (A[5]*B[8])),
                    ((A[6]*B[0]) + (A[7]*B[3]) + (A[8]*B[6])), ((A[6]*B[1]) + (A[7]*B[4]) + (A[8]*B[7])), ((A[6]*B[2]) + (A[7]*B[5]) + (A[8]*B[8]))
                 );
  waitforinput();

  splash_text("  B X A:", 0);
  displayElements(  ((B[0]*A[0]) + (B[1]*A[3]) + (B[2]*A[6])), ((B[0]*A[1]) + (B[1]*A[4]) + (B[2]*A[7])), ((B[0]*A[2]) + (B[1]*A[5]) + (B[2]*A[8])),
                    ((B[3]*A[0]) + (B[4]*A[3]) + (B[5]*A[6])), ((B[3]*A[1]) + (B[4]*A[4]) + (B[5]*A[7])), ((B[3]*A[2]) + (B[4]*A[5]) + (B[5]*A[8])),
                    ((B[6]*A[0]) + (B[7]*A[3]) + (B[8]*A[6])), ((B[6]*A[1]) + (B[7]*A[4]) + (B[8]*A[7])), ((B[6]*A[2]) + (B[7]*A[5]) + (B[8]*A[8]))
                 );
  waitforinput();

  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Adds matrix "A" and "B"*/
void addmatrix() {
  splash_text("  A+B:", 0);

  displayElements(  (A[0] + B[0]), (A[1] + B[1]), (A[2] + B[2]),
                    (A[3] + B[3]), (A[4] + B[4]), (A[5] + B[5]),
                    (A[6] + B[6]), (A[7] + B[7]), (A[8] + B[8]));

  waitforinput();

  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Subtracts matrix "A" and "B"*/
void submatrix() {
  splash_text("  A-B:", 0);

  displayElements(  (A[0] - B[0]), (A[1] - B[1]), (A[2] - B[2]),
                    (A[3] - B[3]), (A[4] - B[4]), (A[5] - B[5]),
                    (A[6] - B[6]), (A[7] - B[7]), (A[8] - B[8]));
  waitforinput();

  splash_text("  B-A:", 0);

  displayElements(  (B[0] - A[0]), (B[1] - A[1]), (B[2] - A[2]),
                    (B[3] - A[3]), (B[4] - A[4]), (B[5] - A[5]),
                    (B[6] - A[6]), (B[7] - A[7]), (B[8] - A[8]));

  waitforinput();

  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Waits for any key to be pressed*/
void waitforinput() {
  while (1)
  {
    char key;
    key = keypad.getKey();
    if (key != NO_KEY) {
      break;
    }
  }
}
/************************/

/*Get user input and return as a int*/
int getnumber(int col, int row) {
  delay(150); //Debounce
  int num = 0;
  char key = keypad.getKey();
  while (key != '#')
  {
    switch (key)
    {
      case NO_KEY:
        break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        delay(50);
        num = num * 10 + (key - '0');
        break;
      case '*':
        num = -(num);
        break;
      case 'C':
        num = 0;
    }
    key = keypad.getKey();
    delay(10); //Debounce
  }
  delay(150);
  display.setCursor(col, row);
  display.print(num);
  display.display();
  return num;
}
/************************/

/*Draws side brackets*/
void drawBrackets() {
  /*First Bracket*/
  //(x1, y1, x2, y2, text color)
  display.drawLine(0, 0, 0, 47, BLACK);
  display.drawLine(0, 0, 5, 0, BLACK);
  display.drawLine(0, 47, 5, 47, BLACK);
  /*Second Bracket*/
  display.drawLine(83, 0, 83, 47, BLACK);
  display.drawLine(83, 0, 78, 0, BLACK);
  display.drawLine(83, 47, 78, 47, BLACK);
  display.display();
}
/************************/

/*Shows result and title text*/
void displayResult(char text[], int result) {
  display.clearDisplay();
  display.setCursor(3, 5);
  display.drawLine(0, 0, 83, 0, BLACK);
  display.print(text);
  display.drawLine(0, 15, 83, 15, BLACK);
  display.setCursor(colpos[2], rowpos[2]);
  display.println(result);
  display.display();
}
/************************/

/*Displays result matrix*/
void displayElements(int a11, int a12, int a13, int a21, int a22, int a23, int a31, int a32, int a33) {
  display.setTextSize(1);
  drawBrackets();
  display.setCursor(colpos[1], rowpos[1]);
  display.print(a11);
  display.setCursor(colpos[2], rowpos[1]);
  display.print(a12);
  display.setCursor(colpos[3], rowpos[1]);
  display.print(a13);
  display.setCursor(colpos[1], rowpos[2]);
  display.print(a21);
  display.setCursor(colpos[2], rowpos[2]);
  display.print(a22);
  display.setCursor(colpos[3], rowpos[2]);
  display.print(a23);
  display.setCursor(colpos[1], rowpos[3]);
  display.print(a31);
  display.setCursor(colpos[2], rowpos[3]);
  display.print(a32);
  display.setCursor(colpos[3], rowpos[3]);
  display.print(a33);
  display.display();
}
/************************/

/*Shows help text*/
void splash_text(char text[], bool wait) {
  display.clearDisplay();
  display.drawLine(0, 11, 83, 11, BLACK);
  display.drawLine(0, 12, 83, 12, BLACK);
  display.drawLine(0, 37, 83, 37, BLACK);
  display.drawLine(0, 38, 83, 38, BLACK);
  display.setCursor(5, 20);
  display.print(text);
  display.display();
  if (wait = 1) {
    waitforinput();
    display.clearDisplay();
    display.display();
  }
}
/************************/

/*Sets LCD bias voltage*/
void setbias(byte cmd) {
  digitalWrite(DC, LOW);
  digitalWrite(CE, LOW);
  shiftOut(DIN, CLK, MSBFIRST, cmd);
  digitalWrite(CE, HIGH);
}
/************************/

/*Menu for adjusting contrast*/
void AdjustContrast() {
  display.clearDisplay();
  display.display();
  display.setCursor(20, 5);
  display.setTextColor(WHITE, BLACK);
  display.print("Contrast");
  display.setCursor(20, 29);
  display.setTextColor(BLACK);
  display.print("Contrast");
  display.setCursor(0, 41);
  display.setTextColor(BLACK);
  display.print("Saved @ EEPROM");
  display.display();
  char key = keypad.getKey();
  while (key != '#')
  {
    switch (key)
    {
      case NO_KEY:
        break;
      case '1':
        setcontrast = defaultcontrast - 20;
        display.setContrast(defaultcontrast - 20);
        break;
      case '2':
        setcontrast = defaultcontrast - 15;
        display.setContrast(setcontrast);
        break;
      case '3':
        setcontrast = defaultcontrast - 10;
        display.setContrast(setcontrast);
        break;
      case '4':
        setcontrast = defaultcontrast - 5;
        display.setContrast(setcontrast);
        break;
      case '5':
        setcontrast = defaultcontrast;
        display.setContrast(setcontrast);
        break;
      case '6':
        setcontrast = defaultcontrast + 5;
        display.setContrast(setcontrast);
        break;
      case '7':
        setcontrast = defaultcontrast + 10;
        display.setContrast(setcontrast);
        break;
      case '8':
        setcontrast = defaultcontrast + 15;
        display.setContrast(setcontrast);
        break;
      case '9':
        setcontrast = defaultcontrast + 20;
        display.setContrast(setcontrast);
        break;
    }
    key = keypad.getKey();
    delay(100);
  }
  EEPROM.update(64, 64);
  EEPROM.update(256, setcontrast);
  display.clearDisplay();
  display.display();
  main_menu();
}
/************************/

/*Checks if contrast value is stored in EEPROM*/
void chkContrast() {
  display.clearDisplay();
#ifdef SetLCDBias
  defaultcontrast = 36;
  setbias(0x9);
  if (EEPROM.read(64) != 64) {
    EEPROM.update(256, defaultcontrast);
  }
#endif
#ifndef SetLCDBias
  defaultcontrast = 45;
  if (EEPROM.read(64) != 64) {
    EEPROM.update(256, defaultcontrast);
  }
#endif
}
/************************/

void loop() {
  //Forever alone :(
}
