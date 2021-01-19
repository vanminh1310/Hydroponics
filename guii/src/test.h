 lv_obj_t * scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);

    bg_top = lv_obj_create(scr, NULL);
    lv_obj_clean_style_list(bg_top, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_YELLOW);
    lv_obj_set_size(bg_top, LV_HOR_RES, 30);

    label_status = lv_label_create(bg_top, NULL);
    lv_label_set_long_mode(label_status, LV_LABEL_LONG_SROLL_CIRC);
    lv_obj_set_width(label_status, LV_HOR_RES - 20);
    lv_label_set_text(label_status, "TOP STATUS");
    lv_obj_align(label_status, NULL, LV_ALIGN_CENTER, 0, 0);
    
    bg_middle = lv_obj_create(scr, NULL);
    lv_obj_clean_style_list(bg_middle, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_obj_set_pos(bg_middle, 0, 30);
    lv_obj_set_size(bg_middle, LV_HOR_RES, 400);

    bg_bottom = lv_obj_create(scr, NULL);
    lv_obj_clean_style_list(bg_bottom, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_ORANGE);
    lv_obj_set_pos(bg_bottom, 0, 430);
    lv_obj_set_size(bg_bottom, LV_HOR_RES, 50);

    fake_ta = lv_textarea_create(bg_bottom, NULL);
    lv_textarea_set_text(fake_ta, "Enter a message");
    lv_obj_set_pos(fake_ta, 0, 0);
    lv_obj_set_size(fake_ta, LV_HOR_RES, 50);
    lv_obj_set_event_cb(fake_ta, fake_ta_event_handler);

    bg_keyboard = lv_obj_create(scr, NULL);
    lv_obj_clean_style_list(bg_bottom, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(bg_keyboard, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(bg_keyboard, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_obj_set_pos(bg_keyboard, 0, LV_VER_RES - (LV_VER_RES / 2  + 40) );
    lv_obj_set_size(bg_keyboard, LV_HOR_RES, LV_VER_RES / 2  + 40);
    
    lv_page();
    makeKeyboard();
}

static void lv_page(void)
{
    page = lv_page_create(bg_middle, NULL);
    lv_obj_clean_style_list(page, LV_OBJ_PART_MAIN);
    
    lv_obj_set_size(page, LV_HOR_RES, 400);
    lv_obj_set_pos(page, 0, 0);
    //lv_page_set_edge_flash(page, true);
    
    itemIdx = 0;
    
    table = lv_table_create(page, NULL);
    lv_obj_clean_style_list(table, LV_OBJ_PART_MAIN);
    lv_table_set_col_cnt(table, 2);
    lv_obj_set_pos(table, 0, 0);
    lv_obj_set_click(table, false);
}

static void fake_ta_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
       lv_obj_move_foreground(bg_keyboard);
       lv_obj_set_height(page, 180);
       scrollToEndOfTable();
    }
}

static void scrollToEndOfTable(){
   float tableHeight = lv_obj_get_height(table);
   printf("tableHeight: %f\n", tableHeight);
   lv_page_scroll_ver(page, -tableHeight);
}

static void updateBottomStatus(lv_color_t color, String text){
  lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,color);
  lv_label_set_text(label_status, text.c_str());
}

static void makeKeyboard(){
    kb_ta = lv_textarea_create(bg_keyboard, NULL);
    lv_obj_set_size(kb_ta, LV_HOR_RES, 40);
    lv_obj_set_pos(kb_ta, 0, 0);
    lv_textarea_set_text(kb_ta, "");

    kb = lv_keyboard_create(bg_keyboard, NULL);
    lv_obj_set_size(kb,  LV_HOR_RES, LV_VER_RES / 2);
    lv_obj_set_pos(kb, 0, 40);
    lv_keyboard_set_cursor_manage(kb, true);
  
    lv_keyboard_set_textarea(kb, kb_ta);
    lv_obj_set_event_cb(kb, keyboard_event_cb);
    lv_obj_move_background(bg_keyboard);
}

static void keyboard_event_cb(lv_obj_t * kb, lv_event_t event){
  lv_keyboard_def_event_cb(kb, event);

  if(event == LV_EVENT_APPLY){
    lv_obj_move_background(bg_keyboard);
    lv_obj_set_height(page, 400);

    updateTable(true, lv_textarea_get_text(kb_ta));
    
    scrollToEndOfTable();
    lv_textarea_set_text(kb_ta, "");
    
  }else if(event == LV_EVENT_CANCEL){
    lv_obj_move_background(bg_keyboard);
    lv_obj_set_height(page, 400);
  }
}

static void updateTable(bool isMine, String msg){
  msg.trim();
  if(msg.length() > 0){
    lv_table_set_row_cnt(table, (itemIdx+1));
    lv_table_set_col_width(table, itemIdx, LV_HOR_RES/2);
    lv_table_set_cell_align(table, itemIdx, isMine? 1 : 0, isMine? LV_LABEL_ALIGN_RIGHT : LV_LABEL_ALIGN_LEFT); 
    lv_table_set_cell_value(table, itemIdx, isMine? 1 : 0, msg.c_str()); 
    itemIdx++;
    scrollToEndOfTable();
  }
}