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
String ssidName, password;
const char *ssid = "MF150_1B8B";
const char *password1 = "26316529";



unsigned long timeout = 10000; // 10sec

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

void scanwifi();

static lv_obj_t *bg_bottom;
static lv_obj_t *bg_bottom2;
lv_obj_t *ddlist;

lv_obj_t *src2;
static lv_obj_t *label_time;
static lv_obj_t *label_icon_wifi;
static lv_obj_t *label_status;

void scanWIFITask(void *pvParameters);
static void event_handler_k(lv_obj_t *obj, lv_event_t event);

static void event_handler(lv_obj_t *obj, lv_event_t event);
static void event_handler1(lv_obj_t *obj, lv_event_t event);
void lv_ex_dropdown_1(void);
static void iconwifi();
static void time12(String text);
static void lv_main();
const char *test;
static void timetest();
static void checkwifi();
void wifi();
void guiTask(void *pvParameters);


// ban phim 
static lv_obj_t * kb;
static lv_obj_t * ta;

void lv_ex_keyboard_1(void);
static void kb_create(void);
//

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
    timetest();
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

  lv_obj_t *namewifi = lv_label_create(src2, NULL);
  lv_label_set_recolor(namewifi, true);
  lv_label_set_text(namewifi, "#0000ff Lua chon wifi#");
  lv_label_set_long_mode(namewifi, LV_LABEL_LONG_SROLL_CIRC);
  lv_obj_set_width(namewifi, 100);
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
  lv_obj_align(ddlist, NULL, LV_ALIGN_IN_TOP_MID, 0, 25);
  lv_obj_set_event_cb(ddlist, event_handler_k);

  //updateBottomStatus(LV_COLOR_GREEN, "Da ket noi Wifi");

  // makeKeyboard();
  scanwifi();
  //lv_ex_dropdown_1();
 
     
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

//  san wifi
void scanwifi()
{
  //  WiFi.mode(WIFI_STA);
  //   WiFi.disconnect();
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

      String item = WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");

      lv_dropdown_add_option(ddlist, item.c_str(), LV_DROPDOWN_POS_LAST);

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

    printf("Option: %s\n", buf);
    lv_ex_keyboard_1();
  }
}


static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e)
{
    lv_keyboard_def_event_cb(kb, e);
    if(e == LV_EVENT_CANCEL) {
         lv_keyboard_set_textarea(kb, NULL);
        lv_obj_del(kb);
         lv_obj_del(ta);
        kb = NULL;
        ta= NULL;
    }
}

static void kb_create(void)
{
    kb = lv_keyboard_create(src2, NULL);
    lv_keyboard_set_cursor_manage(kb, true);
    lv_obj_set_event_cb(kb, kb_event_cb);
    lv_keyboard_set_textarea(kb, ta);

}

static void ta_event_cb(lv_obj_t * ta_local, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED && kb == NULL) {
        kb_create();
     
    }
}

void lv_ex_keyboard_1(void)
{

    /*Create a text area. The keyboard will write here*/
    ta  = lv_textarea_create(src2, NULL);
    lv_obj_align(ta, NULL, LV_ALIGN_CENTER, 0, LV_DPI / 4);
    lv_obj_set_event_cb(ta, ta_event_cb);
    lv_textarea_set_text(ta, "");
    lv_obj_set_size(ta,250,30);
    kb_create();

}