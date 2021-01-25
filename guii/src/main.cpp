#include <lvgl.h>
#include <TFT_eSPI.h>
#include "v.c" //icon wifi
#include "dash.c"
#include "chart.c"
#include "host.c"
#include "about.c"
#include "st.c"
#include <NTPClient.h>
#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>

const char *ssid = "MF150_1B8B";
const char *password = "26316529";
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

    /*Save the state and save the pressed coordinate*/
    //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);

    /*Set the coordinates (if released use the last pressed coordinates)*/
    data->point.x = touchX;
    data->point.y = touchY;

    //Serial.print("Data x");
    //Serial.println(touchX);

    // Serial.print("Data y");
    // Serial.println(touchY);
  }

  return false; /*Return `false` because we are not buffering and no more data to read*/
}

//  thiet ke lai tu dau

static lv_obj_t *bg_bottom;

static lv_obj_t *label_time;
static lv_obj_t *label_icon_wifi;
static lv_obj_t *imgwifi;
static lv_obj_t *imgwifi1;
static lv_obj_t *tab1;
static lv_obj_t *tab2;
static void event_handler(lv_obj_t *obj, lv_event_t event);
static void event_handler1(lv_obj_t *obj, lv_event_t event);
static void iconwifi();

static void time12(String text);
static void lv_main();
const char *test;
static void timetest();
static void checkwifi();
void wifi();
void guiTask(void *pvParameters);

//

void wifi()
{
  int i = 0;
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    i++;
    //    delay(1000);
    //    Serial.println(i);
    delay(50);
    Serial.print(".");
    if (i == 10)
    {
      i = 0;

      break;
    }
  }
}

void setup()
{
  Serial.begin(9600);

  xTaskCreate(guiTask,
              "gui",
              4096 * 2,
              NULL,
              2,
              NULL);
}

void guiTask(void *pvParameters)
{

  ledcSetup(10, 5000 /*freq*/, 20 /*resolution - do phan giai*/);
  ledcAttachPin(32, 10);
  analogReadResolution(10);
  ledcWrite(10, 768);

  Serial.begin(9600); /* prepare for possible serial debug */

  // wifi
  wifi();
  timeClient.begin();
  //

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
    timetest();
    checkwifi();
    lv_task_handler();
  }
}

void loop()
{
}

static void lv_main()
{
  // time

  //

  lv_obj_t *src = lv_obj_create(NULL, NULL);
  lv_obj_t *tabview;
  lv_scr_load(src);

  // tao tap view
  tabview = lv_tabview_create(src, NULL);
  lv_obj_t *tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
  lv_obj_t *tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);
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
  lv_obj_t *imghost = lv_img_create(tab2, NULL);
  lv_img_set_src(imghost, &host);
  lv_obj_align(imghost, NULL, LV_ALIGN_CENTER, 0, 15);

  lv_obj_t *img24 = lv_label_create(tab2, NULL);
  lv_label_set_text(img24, "Esp Now");
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
}

static void timetest()
{
  timeClient.update();
  String time = timeClient.getFormattedTime();
  time12(time);
}
// chuyen text bằng cách khai báo biến toàn cục rồi gọi lại ra không nên tạo nhãn nhiều lần mà chỉ tạo 1 lần duy nhất k thì nó bị đè.
static void time12(String text)
{
  lv_label_set_text(label_time, text.c_str());
}

static void checkwifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED); //set mau cho chu ki tu
    wifi();
  }
  else
  {
    lv_obj_set_style_local_text_color(label_icon_wifi, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN); //set mau cho chu ki tu
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("Test\n");
    iconwifi();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}
static void iconwifi()
{
  lv_obj_t *test = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_height(test, 240);
  lv_obj_set_width(test, 320);
  lv_obj_clean_style_list(test, LV_OBJ_PART_MAIN);
  lv_obj_set_style_local_bg_opa(test, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_bg_color(test, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_t *labeltest = lv_label_create(test, NULL);
  lv_label_set_text(labeltest, "Quay lai");
  lv_obj_align(labeltest, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_event_cb(test, event_handler1);
}

static void event_handler1(lv_obj_t *obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    printf("Test\n");
    lv_main();
  }
  else if (event == LV_EVENT_VALUE_CHANGED)
  {
    printf("tests22222\n");
  }
}