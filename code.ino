#include <Wire.h> //To allow communication with i2c  devices
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h> //provides graphic functions  for OLED Display
#include "U8glib.h"
//#define SCREEN_WIDTH 128    // OLED display width, in pixels
//#define SCREEN_HEIGHT 64    // OLED display height, in pixels
//#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  //To initialize display
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
double temp_amb;
double temp_obj;
void draw(double obj, double amb) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  // For ambient temperature
  u8g.drawStr(15, 22, "AMB"); //will print  AMB at (15,22) position
  u8g.setPrintPos(55, 22); //Set the pointer to (55,22)
  u8g.print(amb);
  u8g.drawCircle(105, 12, 2);
  u8g.drawStr(110, 22, "C");
  // For object temprature
  u8g.drawStr(15, 52, "OBJ");
  u8g.setPrintPos(55, 52);
  u8g.print(obj);
  u8g.drawCircle(105, 42, 2);
  u8g.drawStr(110, 52, "C");
}
void initial(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(20, 22, "Thermometer");
  u8g.drawStr(15, 52, "Initializing");
}
void setup(void) {
  Serial.begin(9600); //This starts serial communication, so that the Arduino can send out commands through the USB connection.
  mlx.begin();  //Initialize MLX90614
  Serial.begin(9600);
  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);  // white
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);  // max intensity
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);  // pixel on
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  pinMode(8, OUTPUT);
  Serial.println("Temperature Sensor MLX90614");
  u8g.firstPage();
  do {
    initial();
  } while (u8g.nextPage());
  delay(5000);
}
void loop() {
  //Reading room temperature and object temp
  //for reading Fahrenheit values, use
  //mlx.readAmbientTempF() , mlx.readObjectTempF() )
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();
  //Serial Monitor
  Serial.print("Room Temp = ");
  Serial.println(temp_amb);
  Serial.print("Object temp = ");
  Serial.println(temp_obj);
  u8g.firstPage();
  do {
    draw(temp_obj, temp_amb);
  } while (u8g.nextPage());
  // rebuild the picture after some delay
  //delay(50);
  delay(1000);
}
