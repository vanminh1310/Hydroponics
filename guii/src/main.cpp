#include <EEPROM.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "v.c" //icon wifi
#include "dash.c"
#include "chart.c"
#include "host.c"
#include "about.c"
#include "st.c"
#include "hd.c"
#include "back50.c"
#include "right50.c"
#include <NTPClient.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>
#include <corona.c>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
// thu vien firebase
#define FIREBASE_HOST "iot59tdh-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ngRiubKaRp2TUnoEz2LOMO2nqWa9RQC24UiWSwLF"



//Define FirebaseESP32 data object
FirebaseData firebaseData;
// 
// blynk
#include <BlynkSimpleEsp32.h>
char auth[] = "bfZd5TubQBz_veStIa1u8Uc07f5h7yA1";
// 
#include <SoftwareSerial.h>
#define RX2 16
#define TX2 17
String ssidName, password;
const char *nd123;
const char *ssid;
const char *password1;

char ssida[] = "TATUYEN"; // tên wifi cần truy cập
char pass[] = "1234567890";

unsigned long timeout = 10000; // 10sec
long last = 0;
long last1 = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // dich mu gio sang mui gio Viet Nam

#define LVGL_TICK_PERIOD 60

LV_IMG_DECLARE(v);

//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_obj_t *slider_label;
int screenWidth = 320;
int screenHeight = 240;

#if USE_LV_LOG != 0
/* Serial debugging */

void my_print(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{

  Serial.printf("%s@%d->%s\r\n", file, line, dsc);
  delay(100);
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tft.startWrite();                                                                            /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++)
  {
    for (int x = area->x1; x <= area->x2; x++)
    {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite();            /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    return false;
  }

  if (touchX > screenWidth || touchY > screenHeight)
  {
    Serial.println("Y or y outside of expected parameters..");
    Serial.print("y:");
    Serial.print(touchX);
    Serial.print(" x:");
    Serial.print(touchY);
  }
  else
  {

    data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    data->point.x = touchX;
    data->point.y = touchY;
  }

  return false; /*Return `false` because we are not buffering and no more data to read*/
}

//  thiet ke lai tu dau
// EEPROM
boolean readeeprom();
static void writeeeprom();
static void iconespnow();
lv_obj_t *espnow;
//
static void icondash();
void scanwifi();
static void btnscan();
static lv_obj_t *bg_bottom;
static lv_obj_t *bg_bottom2;
lv_obj_t *ddlist;
lv_obj_t *namewifi;
lv_obj_t *src2;
lv_obj_t *src3;
lv_obj_t *btscan;
lv_obj_t *dataesp;
static lv_obj_t *label_time;
static lv_obj_t *label_icon_wifi;
static lv_obj_t *label_status;
static void event_espnow(lv_obj_t *obj, lv_event_t event);
void scanWIFITask(void *pvParameters);
static void event_handler_k(lv_obj_t *obj, lv_event_t event);
static void event_btnscanwifi(lv_obj_t *obj, lv_event_t event);
static void event_handler(lv_obj_t *obj, lv_event_t event);
static void event_handler1(lv_obj_t *obj, lv_event_t event);
static void event_handlerds(lv_obj_t *obj, lv_event_t event);
void lv_ex_dropdown_1(void);
static void iconwifi();
static void time12(String text);
static void lv_main();
const char *test;
static void timetest();
static void checkwifi();
void wifi();
void guiTask(void *pvParameters);
void readdata(void *pvParameters);
void ketnoi();
static void event_backall(lv_obj_t *obj, lv_event_t event);
static void covid19();
TaskHandle_t Task1;
TaskHandle_t Task2;

lv_obj_t *datnuoc2;
static lv_obj_t *lbPH;
static lv_obj_t *lbNd;
static lv_obj_t *lbDA;
static lv_obj_t *lbNdN;
static lv_obj_t *lbDN;
static lv_obj_t *lbMN;
static lv_obj_t *name;
static lv_obj_t *AS;
// covid 19
static lv_obj_t *timecv;
static lv_obj_t *datecv;
static lv_obj_t *countrycv;
static lv_obj_t *cases;
static lv_obj_t *deaths;
static lv_obj_t *recovered;

static void testrandom();
// ban phim
static lv_obj_t *kb;
static lv_obj_t *ta;

void lv_ex_keyboard_1(void);
static void kb_create(void);
//silde
static void slider_event_ph(lv_obj_t *slider, lv_event_t event);
static lv_obj_t *slider_ph;
static void slider_event_nd(lv_obj_t *slider, lv_event_t event);
static lv_obj_t *slider_nd;
static void slider_event_as(lv_obj_t *slider, lv_event_t event);
static lv_obj_t *slider_as;
lv_obj_t *slider;
lv_obj_t *slider1;
lv_obj_t *slider12;
//Blynk


static void checkblynk();
// uart
void uart();
void dieukhien();
//
  static char buf_nd[4];
  static char buf_ph[4];
  static char buf_as[4];

//get data firebase 
void getdatafirebase();

int auto_man;
int id;


void wifi()
{
  int i = 0;
  WiFi.begin(ssid, password1);

  while (WiFi.status() != WL_CONNECTED)
  {
    i++;
    //    delay(1000);
    //    Serial.println(i);
    delay(50);
    Serial.print(".");
    if (i == 50)
    {
      i = 0;

      break;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  xTaskCreate(guiTask,
              "gui",
              4096 * 2,
              NULL,
              2,
              NULL);

  vTaskDelay(1000);
  xTaskCreate(readdata,
              "espnowTask",
              4096*2,
              NULL,
              3,
              NULL);
  EEPROM.begin(512);
  readeeprom(); /* pin task to core 0 */
  last = millis();

}

void guiTask(void *pvParameters)
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  ledcSetup(10, 5000 /*freq*/, 20 /*resolution - do phan giai*/);
  ledcAttachPin(32, 10);
  analogReadResolution(10);
  ledcWrite(10, 768);
  Blynk.config(auth, "iot.htpro.vn", 8080);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Serial.begin(9600); /* prepare for possible serial debug */

  //readeeprom();

  //  for (int i = 0; i < 96; ++i)
  //   {
  //     EEPROM.write(i, 0); //xoa bo nho EEPROM
  //   }

  lv_init();

#if USE_LV_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(3);

  uint16_t calData[5] = {275, 3620, 264, 3532, 1};
  tft.setTouch(calData);

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display - khoi tao man hinh */
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver- khoi tao trinh dieu khien thiet bi dau vao */
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);          /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;   /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);      /*Finally register the driver*/

  //Set the theme..dat chu de
  lv_theme_t *th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL, LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
  lv_theme_set_act(th);

  lv_obj_t *scr = lv_cont_create(NULL, NULL);
  lv_disp_load_scr(scr);

  lv_main();

  while (1)
  {
    //
 //testrandom();
    
    checkwifi();
    lv_task_handler();
    Blynk.run();
    

  }
}

BLYNK_WRITE(V0)
{ // This function gets called each time something changes on the widget
  String value = String(param.asInt());
  Serial.println(value);
   // This gets the 'value' of the Widget as an integer
 
   
  lv_label_set_text(slider_nd, value.c_str());

  lv_slider_set_value(slider1, value.toInt(), LV_ANIM_ON);
}

BLYNK_WRITE(V1)
{ // This function gets called each time something changes on the widget
  String value1 = String(param.asInt());
  Serial.println(value1); // This gets the 'value' of the Widget as an integer
  lv_label_set_text(slider_ph, value1.c_str());

  lv_slider_set_value(slider, value1.toInt(), LV_ANIM_ON);
}
// ham toInt chuyen doi ve dang nguyen
BLYNK_WRITE(V2)
{ // This function gets called each time something changes on the widget
  String value2 = String(param.asInt());
  Serial.println(value2); // This gets the 'value' of the Widget as an integer
  lv_label_set_text(slider_as, value2.c_str());
  // int vale = param.asInt();
  lv_slider_set_value(slider12, value2.toInt(), LV_ANIM_ON);
}

void readdata(void *pvParameters)
{
  vTaskDelay(1000);
  Serial.print("Task2 running on core ");
// Blynk.begin(auth, ssida, pass,"iot.htpro.vn", 8080);
//  checkblynk();
 //  dong bo gia tri

  Serial.println(xPortGetCoreID());
  while (1)
  {
    // Serial.println("404!");
    // covid19();
    //Serial.println("");

    if (millis() - last >= 1000)
    {
      testrandom();
         uart();
        dieukhien();
        
  // Blynk.syncVirtual(V1);
  // Blynk.syncVirtual(V0);
  // Blynk.syncVirtual(V2);
      last = millis();
    }

  }
}
void loop()
{
}

static void lv_main()
{
  // time
  //imghd();// img hoa huong duong
  //

  lv_obj_t *src1 = lv_obj_create(NULL, NULL);

  lv_obj_t *tabview;
  lv_scr_load(src1);

  // tao tap view
  tabview = lv_tabview_create(src1, NULL);
  // lv_obj_t *tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
  lv_obj_t *tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);
  lv_obj_t *tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
  //dich trai dich phai
  lv_obj_set_style_local_pad_right(tabview, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2.5);
  lv_obj_set_style_local_bg_color(tabview, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

  // tets

  // tap menu
  // wifi

  lv_obj_t *imgwifi = lv_img_create(tab2, NULL);
  lv_img_set_src(imgwifi, &v);
  lv_obj_set_click(imgwifi, true);
  lv_obj_set_event_cb(imgwifi, event_handler);
  lv_obj_set_x(imgwifi, 20);
  lv_obj_set_y(imgwifi, 5);
  lv_obj_t *img2 = lv_label_create(tab2, NULL);
  lv_label_set_text(img2, "Wifi");
  lv_obj_set_x(img2, 28);
  lv_obj_set_y(img2, 55);

  // st
  lv_obj_t *imgst = lv_img_create(tab2, NULL);
  lv_img_set_src(imgst, &st);
  lv_obj_align(imgst, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
  // lv_obj_set_x(imgst, 20);
  // lv_obj_set_y(imgst, 80);
  lv_obj_t *img21 = lv_label_create(tab2, NULL);
  lv_label_set_text(img21, "Setting");
  lv_obj_align(img21, NULL, LV_ALIGN_IN_TOP_MID, 0, 55);
  // lv_obj_set_x(img21, 25);
  // lv_obj_set_y(img21, 130);

  //
  // dashboard

  lv_obj_t *imgds = lv_img_create(tab2, NULL);
  lv_img_set_src(imgds, &dash);
  lv_obj_align(imgds, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 5);
  lv_obj_set_click(imgds, true);
  lv_obj_set_event_cb(imgds, event_handlerds);

  lv_obj_t *img22 = lv_label_create(tab2, NULL);
  lv_label_set_text(img22, "Dash");
  lv_obj_align(img22, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 55);

  // chart
  lv_obj_t *imgchart = lv_img_create(tab2, NULL);
  lv_img_set_src(imgchart, &chart);
  lv_obj_align(imgchart, NULL, LV_ALIGN_IN_LEFT_MID, 20, 15);

  lv_obj_t *img23 = lv_label_create(tab2, NULL);
  lv_label_set_text(img23, "Chart");
  lv_obj_align(img23, NULL, LV_ALIGN_IN_LEFT_MID, 20, 50);
  // lv_obj_set_x(img21, 25);
  // lv_obj_set_y(img21, 130);
  // hostpost
  lv_obj_t *imgcorona = lv_img_create(tab2, NULL);
  lv_img_set_src(imgcorona, &corona);
  lv_obj_set_click(imgcorona, true);
  lv_obj_set_event_cb(imgcorona, event_espnow);
  lv_obj_align(imgcorona, NULL, LV_ALIGN_CENTER, 0, 15);

  lv_obj_t *img24 = lv_label_create(tab2, NULL);
  lv_label_set_text(img24, "Covid19");

  lv_obj_align(img24, NULL, LV_ALIGN_CENTER, 0, 50);

  //
  lv_obj_t *imgabout = lv_img_create(tab2, NULL);
  lv_img_set_src(imgabout, &about);
  lv_obj_align(imgabout, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 15);

  lv_obj_t *img25 = lv_label_create(tab2, NULL);
  lv_label_set_text(img25, "About");
  lv_obj_align(img25, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 50);

  //

  // tao ki tu wifi
  label_icon_wifi = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(label_icon_wifi, LV_SYMBOL_WIFI);
  lv_obj_set_x(label_icon_wifi, 280);
  lv_obj_set_y(label_icon_wifi, 20);
  if (WiFi.status() != WL_CONNECTED)
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED); //set mau cho chu ki tu
                                                                                                            // readeeprom();
  }
  else
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN); //set mau cho chu ki tu
  }

  // tao bg_bottomm
  bg_bottom = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_clean_style_list(bg_bottom, LV_OBJ_PART_MAIN);
  lv_obj_set_style_local_bg_opa(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_bg_color(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_pos(bg_bottom, 0, 220);
  lv_obj_set_size(bg_bottom, LV_HOR_RES, 20);

  lv_obj_t *label2 = lv_label_create(bg_bottom, NULL);
  lv_label_set_text(label2, LV_SYMBOL_GPS);
  lv_obj_align(label2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

  lv_obj_t *label4 = lv_label_create(bg_bottom, NULL);
  lv_label_set_text(label4, LV_SYMBOL_BATTERY_2);
  lv_obj_align(label4, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

  label_time = lv_label_create(bg_bottom, NULL);

  lv_obj_align(label_time, NULL, LV_ALIGN_IN_BOTTOM_MID, -10, 0);
  lv_obj_set_style_local_bg_color(label_time, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_label_set_text(label_time, "00.00.00");
  // tab1

  // static lv_obj_t* lbPH;
  // static lv_obj_t* lbNd;
  // static lv_obj_t* lbDA;
  // static lv_obj_t* lbNdN;
  // static lv_obj_t* lbDN;
  // static lv_obj_t* lbMN;
  // static lv_obj_t* name;
  // static lv_obj_t* AS;

  lv_obj_t *labelPH = lv_label_create(tab1, NULL);
  lv_label_set_text(labelPH, "PH: ");
  lv_obj_set_pos(labelPH, 10, 10);
  lbPH = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbPH, 40, 10);
  lv_obj_set_style_local_text_color(lbPH, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *labelphantram = lv_label_create(tab1, NULL);
  lv_label_set_text(labelphantram, " %");
  lv_obj_set_style_local_text_color(labelphantram, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(labelphantram, 55, 10);

  lv_obj_t *labelND = lv_label_create(tab1, NULL);
  lv_label_set_text(labelND, "Nhietdo:");
  lv_obj_set_pos(labelND, 80, 10);
  lbNd = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbNd, 155, 10);
  lv_obj_set_style_local_text_color(lbNd, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *labeldc = lv_label_create(tab1, NULL);
  lv_label_set_text(labeldc, "°C");
  lv_obj_set_style_local_text_color(labeldc, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(labeldc, 200, 10);

  lv_obj_t *labelAS = lv_label_create(tab1, NULL);
  lv_label_set_text(labelAS, "AS: ");
  lv_obj_set_pos(labelAS, 240, 10);
  AS = lv_label_create(tab1, NULL);
  lv_obj_set_pos(AS, 270, 10);
  lv_obj_set_style_local_text_color(AS, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *labellux = lv_label_create(tab1, NULL);
  lv_label_set_text(labellux, "lux");
  lv_obj_set_pos(labellux, 295, 10);
  lv_obj_set_style_local_text_color(labellux, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);

  lv_obj_t *labelNDN = lv_label_create(tab1, NULL);
  lv_label_set_text(labelNDN, "Temperature2: ");
  lv_obj_set_pos(labelNDN, 10, 50);
  lbNdN = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbNdN, 135, 50);
  lv_obj_set_style_local_text_color(lbNdN, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *lbdc = lv_label_create(tab1, NULL);
  lv_label_set_text(lbdc, "°C");
  lv_obj_set_style_local_text_color(lbdc, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(lbdc, 155, 50);

  lv_obj_t *labelDA = lv_label_create(tab1, NULL);
  lv_label_set_text(labelDA, "Humidity: ");
  lv_obj_set_pos(labelDA, 180, 50);
  lbDA = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbDA, 260, 50);
  lv_obj_set_style_local_text_color(lbDA, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *lbpt = lv_label_create(tab1, NULL);
  lv_label_set_text(lbpt, "%");
  lv_obj_set_style_local_text_color(lbpt, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(lbpt, 280, 50);

  lv_obj_t *labelDN = lv_label_create(tab1, NULL);
  lv_label_set_text(labelDN, "DN:  ");

  lv_obj_set_pos(labelDN, 10, 90);
  lbDN = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbDN, 48, 90);
  lv_obj_set_style_local_text_color(lbDN, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *lbdc123 = lv_label_create(tab1, NULL);
  lv_label_set_text(lbdc123, "°C");
  lv_obj_set_style_local_text_color(lbdc123, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(lbdc123, 80, 90);

  lv_obj_t *labelMN = lv_label_create(tab1, NULL);
  lv_label_set_text(labelMN, "MN: ");

  lv_obj_set_pos(labelMN, 120, 90);
  lbMN = lv_label_create(tab1, NULL);
  lv_obj_set_pos(lbMN, 155, 90);
  lv_obj_set_style_local_text_color(lbMN, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_t *lbdc1234 = lv_label_create(tab1, NULL);
  lv_label_set_text(lbdc1234, "%");
  lv_obj_set_style_local_text_color(lbdc1234, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_obj_set_pos(lbdc1234, 190, 90);
  //lv_obj_set_style_local_text_font(labelMN, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_THEME_DEFAULT_FONT_SMALL);

  // lv_obj_t *labelNAME = lv_label_create(tab1, NULL);
  // lv_label_set_text(labelNAME,"NAME: ");
}

static void timetest()
{

  timeClient.begin();
  timeClient.update();
  String time = timeClient.getFormattedTime();
  lv_label_set_text(label_time, time.c_str());
}
// chuyen text bằng cách khai báo biến toàn cục rồi gọi lại ra không nên tạo nhãn nhiều lần mà chỉ tạo 1 lần duy nhất k thì nó bị đè.

static void checkwifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED); //set mau cho chu ki tu
  }
  else
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN); //set mau cho chu ki tu

    timetest();
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("iconwifi\n");
    iconwifi();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}
static void iconwifi()
{
  src2 = lv_obj_create(NULL, NULL); // tao va load man hinh moi
  lv_scr_load(src2);
  lv_obj_set_style_local_bg_color(src2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  //  img back
  lv_obj_t *imgback = lv_img_create(src2, NULL);
  lv_img_set_src(imgback, &back50);
  lv_obj_set_click(imgback, true);
  lv_obj_set_event_cb(imgback, event_handler1);
  lv_obj_align(imgback, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  //  img right
  lv_obj_t *imgright = lv_img_create(src2, NULL);
  lv_img_set_src(imgright, &right50);
  lv_obj_set_click(imgright, true);
  lv_obj_set_event_cb(imgright, event_handler1); // hanh dong
  lv_obj_align(imgright, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  namewifi = lv_label_create(src2, NULL);
  lv_label_set_recolor(namewifi, true);
  lv_label_set_text(namewifi, "#0000ff Lua chon wifi #");
  lv_label_set_long_mode(namewifi, LV_LABEL_LONG_SROLL_CIRC);
  lv_obj_set_width(namewifi, 120);
  lv_obj_align(namewifi, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

  //  static lv_obj_t * bg_bottom;

  bg_bottom2 = lv_obj_create(src2, NULL);
  lv_obj_clean_style_list(bg_bottom2, LV_OBJ_PART_MAIN);
  lv_obj_set_style_local_bg_opa(bg_bottom2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_bg_color(bg_bottom2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_obj_set_pos(bg_bottom2, 0, 220);
  lv_obj_set_size(bg_bottom2, LV_HOR_RES, 30);

  label_status = lv_label_create(bg_bottom2, NULL);
  lv_label_set_long_mode(label_status, LV_LABEL_LONG_SROLL_CIRC);
  lv_obj_set_width(label_status, LV_HOR_RES - 20);
  lv_label_set_text(label_status, "");
  lv_obj_align(label_status, NULL, LV_ALIGN_CENTER, 0, 0);
  // list
  ddlist = lv_dropdown_create(src2, NULL);
  lv_obj_set_width(ddlist, 200);
  lv_dropdown_set_show_selected(ddlist, true);
  lv_dropdown_set_text(ddlist, "WIFI");
  lv_obj_align(ddlist, NULL, LV_ALIGN_IN_TOP_MID, 0, 25);
  lv_obj_set_event_cb(ddlist, event_handler_k);

  // button scan wifi

  btnscan();
  //updateBottomStatus(LV_COLOR_GREEN, "Da ket noi Wifi");

  // makeKeyboard();
  scanwifi();
  //lv_ex_dropdown_1();
}

// btn scanwifi
static void btnscan()
{
  btscan = lv_btn_create(src2, NULL);
  lv_obj_set_event_cb(btscan, event_btnscanwifi);
  lv_obj_align(btscan, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *label = lv_label_create(btscan, NULL);
  lv_label_set_text(label, "Scan Wifi");
}

static void event_btnscanwifi(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    //  readeeprom();
    scanwifi();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}

//

static void event_handler1(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("back\n");
    lv_main();
    readeeprom();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}

// dash

static void event_handlerds(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("dash\n");

    icondash();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}

static void event_backall(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("back all\n");
    lv_main();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}

//  san wifi
void scanwifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  //   Serial.println("Start scan");
  int n = WiFi.scanNetworks();

  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    lv_dropdown_clear_options(ddlist);
    for (int i = 0; i < n; ++i)
    {

      String item = WiFi.SSID(i);

      lv_dropdown_add_option(ddlist, item.c_str(), LV_DROPDOWN_POS_LAST);

      // ssidName = WiFi.SSID(i);
      // Serial.println(ssidName);
      Serial.println(item);
      delay(50);
    }
    Serial.println("Scan done");
    Serial.println("");
  }
}

//

static void event_handler_k(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    ssidName = String(buf);
    Serial.println(ssidName);
    printf("Option: %s\n", buf);
    lv_obj_set_hidden(btscan, 100);
    lv_ex_keyboard_1();
  }
}

static void kb_event_cb(lv_obj_t *keyboard, lv_event_t e)
{
  lv_keyboard_def_event_cb(kb, e);
  if (e == LV_EVENT_CANCEL)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_del(kb);
    lv_obj_del(ta);
    kb = NULL;
    ta = NULL;
    btnscan();
  }

  if (e == LV_EVENT_APPLY)
  {
    password = lv_textarea_get_text(ta);
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_del(kb);
    kb = NULL;
    Serial.println(password);
    ketnoi();
  }
}

static void kb_create(void)
{
  kb = lv_keyboard_create(src2, NULL);
  lv_keyboard_set_cursor_manage(kb, true);
  lv_obj_set_event_cb(kb, kb_event_cb);
  lv_keyboard_set_textarea(kb, ta);
}

static void ta_event_cb(lv_obj_t *ta_local, lv_event_t e)
{
  if (e == LV_EVENT_CLICKED && kb == NULL)
  {
    kb_create();
  }
}

void lv_ex_keyboard_1(void)
{

  /*Create a text area. The keyboard will write here*/
  ta = lv_textarea_create(src2, NULL);
  lv_obj_align(ta, NULL, LV_ALIGN_CENTER, 0, LV_DPI / 4);
  lv_obj_set_event_cb(ta, ta_event_cb);
  lv_textarea_set_text(ta, "");
  lv_obj_set_size(ta, 250, 30);
  kb_create();
}
void ketnoi()
{

  int i = 0;
  WiFi.begin(ssidName.c_str(), password.c_str());

  Serial.println(ssidName.c_str());
  Serial.println(password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    i++;
    delay(100);
    Serial.print(".");
    if (i == 50)
    {
      i = 0;
      Serial.println("mat khau khong dung xin vui long thu lai");

      break;
    }
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("mat khau khong dung xin vui long thu lai");
    lv_label_set_text(namewifi, "#ff0000 Mat khau sai vui long nhap lai#");
  }
  else
  {

    writeeeprom();
    lv_main();
    timetest();
  }
}
static void writeeeprom()
{

  Serial.println(ssidName.c_str());
  Serial.println(password.c_str());
  for (int i = 0; i < 96; ++i)
  {
    EEPROM.write(i, 0); //xoa bo nho EEPROM
  }
  Serial.println("Writing SSID to EEPROM...");
  for (int i = 0; i < ssidName.length(); ++i)
  {
    EEPROM.write(i, ssidName[i]);
  }
  Serial.println("Writing Password to EEPROM...");
  for (int i = 0; i < password.length(); ++i)
  {
    EEPROM.write(32 + i, password[i]);
  }
  EEPROM.commit();
  Serial.println("Write EEPROM done!");
}
boolean readeeprom()
{
  Serial.println("Reading EEPROM...");
  String ssid = "";
  String pass = "";
  int i = 0;
  if (EEPROM.read(0) != 0)
  { //neu duu lieu doc ra tu EEPROM khac 0 thi doc du lieu
    for (int i = 0; i < 32; ++i)
    { //32 o nho dau tieu la chua ten mang wifi SSID
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid.c_str());
    for (int i = 32; i < 96; ++i)
    { //o nho tu 32 den 96 la chua PASSWORD
      pass += char(EEPROM.read(i));
    }
    Serial.print("Password: ");
    Serial.println(pass.c_str());
    WiFi.begin(ssid.c_str(), pass.c_str());

    //ket noi voi mang WIFI duoc luu trong EEPROM
    while (WiFi.status() != WL_CONNECTED)
    {
      i++;
      delay(100);
      Serial.print(".");
      if (i == 50)
      {
        i = 0;
        Serial.println("mat khau khong dung xin vui long thu lai");
        break;
      }
    }
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("mat khau khong dung xin vui long thu lai");
      lv_label_set_text(namewifi, "#FF0000 No Connected #");
      //Blynk.begin(auth, ssid.c_str(), pass.c_str(),"iot.htpro.vn", 8080);
    }
    else
    {
      lv_label_set_text(namewifi, "#32CD32 Connected #");
    }
  }
}
// icon espnow
static void iconespnow()
{
  espnow = lv_obj_create(NULL, NULL); // tao va load man hinh moi
  lv_scr_load(espnow);
  lv_obj_set_style_local_bg_color(espnow, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  // icon back
  lv_obj_t *imgback111 = lv_img_create(espnow, NULL);
  lv_img_set_src(imgback111, &back50);
  lv_obj_set_click(imgback111, true);
  lv_obj_set_event_cb(imgback111, event_backall);
  lv_obj_align(imgback111, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  // covid 19
  timecv = lv_label_create(espnow, NULL);
  lv_label_set_text(timecv, "00.00.00");
  lv_obj_align(timecv, NULL, LV_ALIGN_IN_TOP_MID, 0, 50);
  lv_obj_set_style_local_text_color(timecv, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

  datecv = lv_label_create(espnow, NULL);
  lv_label_set_text(datecv, "10/02/2021");
  lv_obj_align(datecv, NULL, LV_ALIGN_IN_TOP_MID, 0, 70);
  lv_obj_set_style_local_text_color(datecv, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_obj_t *imgcv = lv_img_create(espnow, NULL);
  lv_img_set_src(imgcv, &corona);
  lv_obj_align(imgcv, NULL, LV_ALIGN_CENTER, 0, 0);
  // label
  lv_obj_t *datnuoc = lv_label_create(espnow, NULL);
  lv_obj_align(datnuoc, NULL, LV_ALIGN_IN_LEFT_MID, 10, 35);
  lv_label_set_text(datnuoc, "Country: ");
  lv_obj_set_style_local_text_color(datnuoc, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  lv_obj_t *truonghop = lv_label_create(espnow, NULL);
  lv_obj_align(truonghop, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 35);
  lv_label_set_text(truonghop, "Cases: ");
  lv_obj_set_style_local_text_color(truonghop, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  lv_obj_t *mats = lv_label_create(espnow, NULL);
  lv_obj_align(mats, NULL, LV_ALIGN_IN_LEFT_MID, 10, 70);
  lv_label_set_text(mats, "Deaths: ");
  lv_obj_set_style_local_text_color(mats, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  lv_obj_t *phuchoi = lv_label_create(espnow, NULL);
  lv_obj_align(phuchoi, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 70);
  lv_label_set_text(phuchoi, "Recovered: ");
  lv_obj_set_style_local_text_color(phuchoi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  countrycv = lv_label_create(espnow, NULL);
  lv_obj_align(countrycv, NULL, LV_ALIGN_IN_LEFT_MID, 85, 35);
  lv_label_set_text(countrycv, "Vietnam");
  lv_obj_set_style_local_text_color(countrycv, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

  deaths = lv_label_create(espnow, NULL);
  lv_obj_align(deaths, NULL, LV_ALIGN_IN_LEFT_MID, 80, 70);
  lv_label_set_text(deaths, "35");
  lv_obj_set_style_local_text_color(deaths, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

  cases = lv_label_create(espnow, NULL);
  lv_obj_align(cases, NULL, LV_ALIGN_IN_RIGHT_MID, -60, 35);
  lv_label_set_text(cases, "2069");
  lv_obj_set_style_local_text_color(cases, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

  recovered = lv_label_create(espnow, NULL);
  lv_obj_align(recovered, NULL, LV_ALIGN_IN_RIGHT_MID, -25, 70);
  lv_label_set_text(recovered, "1474 ");
  lv_obj_set_style_local_text_color(recovered, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
}

static void event_espnow(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("espnow\n");
    iconespnow();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}
static void testrandom()
{
  String aaa = String(random(1, 100));
  lv_label_set_text(lbPH, aaa.c_str());
  Firebase.setInt(firebaseData,"Sensor/PH",aaa.toInt());
  // lv_label_set_text(lbNd, aaa.c_str());
  lv_label_set_text(AS, aaa.c_str());
    Firebase.setInt(firebaseData,"Sensor/AS",aaa.toInt());
  lv_label_set_text(lbNdN, aaa.c_str());
      Firebase.setInt(firebaseData,"Sensor/NDN",aaa.toInt());
  lv_label_set_text(lbDA, aaa.c_str());
      Firebase.setInt(firebaseData,"Sensor/DA",aaa.toInt());
  lv_label_set_text(lbMN, aaa.c_str());
      Firebase.setInt(firebaseData,"Sensor/MN",aaa.toInt());
  // lv_label_set_text(lbDN, aaa.c_str());
  
  Blynk.virtualWrite(V6, aaa.c_str());
  
}

// static void covid19(){
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     HTTPClient http;
//     http.begin("http://coronavirus-19-api.herokuapp.com/countries/vietnam");
//     int httpCode = http.GET();

//     if (httpCode > 0)
//     {
//       const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
//       DynamicJsonBuffer jsonBuffer(bufferSize);
//       JsonObject& root = jsonBuffer.parseObject(http.getString());

//       const char* country = root["country"];
//       int cases = root["cases"];
//       int deaths = root["deaths"];
//       int recovered = root["recovered"];

//       Serial.print("country: ");
//       Serial.println(country);
//       Serial.print("cases: ");
//       Serial.println(cases);
//       Serial.print("deaths: ");
//       Serial.println(deaths);
//       Serial.print("recovered: ");
//       Serial.println(recovered);
//     }
//        http.end();
// }}
// dashboad

static void icondash()
{
  src3 = lv_obj_create(NULL, NULL); // tao va load man hinh moi
  lv_scr_load(src3);
  lv_obj_set_style_local_bg_color(src3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  // icon back
  lv_obj_t *imgback = lv_img_create(src3, NULL);
  lv_img_set_src(imgback, &back50);
  lv_obj_set_click(imgback, true);
  lv_obj_set_event_cb(imgback, event_backall);
  lv_obj_align(imgback, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  // ph
  lv_obj_t *phname = lv_label_create(src3, NULL);
  lv_obj_align(phname, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  lv_label_set_text(phname, "PH: ");
  lv_obj_set_style_local_text_color(phname, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  slider = lv_slider_create(src3, NULL);
  lv_obj_set_width(slider, LV_DPI * 2);
  lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);
  lv_obj_set_event_cb(slider, slider_event_ph);
  lv_slider_set_range(slider, 0, 10);

  slider_ph = lv_label_create(src3, NULL);
  lv_label_set_text(slider_ph, "0");
  lv_obj_set_auto_realign(slider_ph, true);
  lv_obj_set_adv_hittest(slider_ph, true);
  lv_obj_align(slider_ph, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  // nd
  lv_obj_t *ndname = lv_label_create(src3, NULL);
  lv_obj_align(ndname, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 100);
  lv_label_set_text(ndname, "NHIET DO: ");
  lv_obj_set_style_local_text_color(ndname, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  slider1 = lv_slider_create(src3, NULL);
  lv_obj_set_width(slider1, LV_DPI * 2);
  lv_obj_align(slider1, NULL, LV_ALIGN_IN_TOP_MID, 0, 130);
  lv_obj_set_event_cb(slider1, slider_event_nd);
  lv_slider_set_range(slider1, 0, 100);

  slider_nd = lv_label_create(src3, NULL);
  lv_label_set_text(slider_nd, "0");
  lv_obj_set_auto_realign(slider_nd, true);
  lv_obj_align(slider_nd, slider, LV_ALIGN_CENTER, 0, 70);
  // as
  lv_obj_t *asname = lv_label_create(src3, NULL);
  lv_obj_align(asname, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 160);
  lv_label_set_text(asname, "ANH SANG : ");
  lv_obj_set_style_local_text_color(asname, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

  slider12 = lv_slider_create(src3, NULL);
  lv_obj_set_width(slider12, LV_DPI * 2);
  lv_obj_align(slider12, NULL, LV_ALIGN_IN_TOP_MID, 0, 190);
  lv_obj_set_event_cb(slider12, slider_event_as);
  lv_slider_set_range(slider12, 0, 1000);

  slider_as = lv_label_create(src3, NULL);
  lv_label_set_text(slider_as, "0");
  lv_obj_set_auto_realign(slider_as, true);
  lv_obj_align(slider_as, slider, LV_ALIGN_CENTER, 0, 130);
  // lv_slider_set_type(slider12, LV_SLIDER_TYPE_RANGE);

  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V2);
}
static void slider_event_ph(lv_obj_t *slider, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    // static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
    snprintf(buf_ph, 4, "%u", lv_slider_get_value(slider));

    lv_label_set_text(slider_ph, buf_ph);
     Firebase.setInt(firebaseData, "Floors1/G",int(buf_ph) );
    Blynk.virtualWrite(V1, buf_ph);
  }
}

static void slider_event_nd(lv_obj_t *slider, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    // static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
    snprintf(buf_nd, 4, "%u", lv_slider_get_value(slider));

    lv_label_set_text(slider_nd, buf_nd);

    Blynk.virtualWrite(V0, buf_nd);
   
  }
}
static void slider_event_as(lv_obj_t *slider, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
  {
    // static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
    snprintf(buf_as, 4, "%u", lv_slider_get_value(slider));
    lv_label_set_text(slider_as, buf_as);

    Blynk.virtualWrite(V2, buf_as);
  }
}
static void checkblynk()
{
  if (!Blynk.connected())
  {
    Serial.println("Lost connection");
    if (Blynk.connect())
    {
      Serial.println("Reconnected");
      Blynk.syncAll();
    }
    else
    {
      Serial.println("Not reconnected");
    }
  }
}
void uart()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(Serial2);
  if (root == JsonObject::invalid())
    return;

  root.prettyPrintTo(Serial2);

  // Test if parsing succeeds.
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  int data333 = root["time"];
   nd123 = root["ND:"];
  Serial.print("Time: ");
  Serial.print(data333);
  Serial.println();
  Serial.print("Nhiet do: ");
   Firebase.setFloat(firebaseData, "Sensor/T1", atof(nd123));
   lv_label_set_text(lbNd,nd123);
    Blynk.virtualWrite(V5, nd123);
  Serial.print(nd123);
  Serial.println();
  String test = Serial2.readString();

}
void dieukhien(){
  Serial.print("Nhiet do kiem tra:");
  Serial.print(nd123);
  Serial.println();
  Serial.print("kiemtra bien: ");
  Serial.print(buf_nd);
  Serial.println();
// char to float sai ham atof()
  if (atof(nd123)>=atof(buf_nd))
  {
    Serial.println("demo");
    Serial2.println("s");
  }
  else{
     Serial.println("demo1");
       Serial2.println("a");
  }
  
}


// mai doi lai chan 34-35 thanh 16 17

// get data firebase 
void getdatafirebase(){
 if (Firebase.getInt(firebaseData, "auto_man")) {

    if (firebaseData.dataType() == "int") {
     auto_man = firebaseData.intData();// bien kiem tra che do la auto hay man 0 la man 1 la out
    }

  } 
   if (Firebase.getInt(firebaseData, "id")) {

    if (firebaseData.dataType() == "int") {
     id = firebaseData.intData();//kiem tra xem la loai cay nao 
    }

  } 
  else {
    Serial.println(firebaseData.errorReason());
  }
}
void manaul(){
  if(auto_man == 0 && id == 1){
    Serial.print("Dau tay");
  }
}