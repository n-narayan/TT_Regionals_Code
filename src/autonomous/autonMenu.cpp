#include "main.h"

bool redSide;  //auton will run based upon these conditionals
bool protectedZone;

lv_obj_t * redBtn;
lv_style_t redBtnStyleREL; //released style
lv_style_t redBtnStylePR; //pressed style
lv_obj_t * redLabel;  //text for red button

lv_obj_t * blueBtn;
lv_style_t blueBtnStyleREL; //released style
lv_style_t blueBtnStylePR; //pressed style
lv_obj_t * blueLabel; //text for blue label

lv_obj_t * selectBtn;
lv_style_t selectBtnStyleREL; //released style
lv_style_t selectBtnStylePR; //pressed style
lv_obj_t * selectLabel ;  //text for unprotected

lv_obj_t * scr = lv_scr_act();

bool selected = false;  //external variables
int auton = 0;

static lv_res_t btn_action(lv_obj_t * btn){

  uint8_t id = lv_obj_get_free_num(btn);

  if (id == 0){ //red button
    redSide = true;
    auton++;

    if (auton >= 3)
      auton = 1;

    switch(auton){
      case 1:
      protectedZone = false;
      char reda[100];
      sprintf(reda, "Auton: Red Protected");
      lv_label_set_text(redLabel, reda);
      break;

      case 2:
      protectedZone = true;
      char redb[100];
      sprintf(redb, "Auton: Red Unprotected");
      lv_label_set_text(redLabel, redb);
      break;
    }
  }

  else if(id == 1){ //blue button
    redSide = false;
    auton++;

    if (auton >= 3)
      auton = 1;

    switch(auton){
      case 1:
      protectedZone = false;
      char bluea[100];
      sprintf(bluea, "Auton: Blue Protected");
      lv_label_set_text(blueLabel, bluea);
      break;

      case 2:
      protectedZone = true;
      char blueb[100];
      sprintf(blueb, "Auton: Blue Unprotected");
      lv_label_set_text(blueLabel, blueb);
      break;
    }
  }

  else{ //select button
    selected = true;
    switch(auton){
      case 1:
        if(redSide){
          char redProt[100];
          sprintf(redProt, "Selected: Red Protected");
          lv_label_set_text(selectLabel, redProt);
          auton = 1;
        }
        else{
          char blueProt[100];
          sprintf(blueProt, "Selected: Blue Protected");
          lv_label_set_text(selectLabel, blueProt);
          auton = 2;
        }
        break;
      case 2:
        if(redSide){
          char redUn[100];
          sprintf(redUn, "Selected: Red Unprotected");
          lv_label_set_text(selectLabel, redUn);
          auton = 3;
        }
        else{
          char blueUn[100];
          sprintf(blueUn, "Selected: Blue Unprotected");
          lv_label_set_text(selectLabel, blueUn);
          auton = 4;
        }
      break;
      /*case 3:
        char blueProt[100];
        sprintf(blueProt, "Selected: Blue Protected");
        lv_label_set_text(selectLabel, blueProt);
        break;
      case 4:
        char blueUn[100];
        sprintf(blueUn, "Selected: Blue Unprotected");
        lv_label_set_text(selectLabel, blueUn);
        break;*/
    }
  }

  return LV_RES_OK;
}

void selectAuton(){

  //Creates style for RED buttons (bright when unpressed, darkens once pressed)
  lv_style_copy(&redBtnStyleREL, &lv_style_plain);
    redBtnStyleREL.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    redBtnStyleREL.body.grad_color = LV_COLOR_MAKE(100, 0, 0);
    redBtnStyleREL.body.radius = 0;
    redBtnStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

  lv_style_copy(&redBtnStylePR, &lv_style_plain);
    redBtnStylePR.body.main_color = LV_COLOR_MAKE(150, 0, 0);
    redBtnStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 0);
    redBtnStylePR.body.radius = 0;
    redBtnStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);
//----------------------------------------------------------------------------
//Creates style for BLUE buttons (bright when unpressed, darkens once pressed)
  lv_style_copy(&blueBtnStyleREL, &lv_style_plain);
    blueBtnStyleREL.body.main_color = LV_COLOR_MAKE(0, 140, 255);
    blueBtnStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
    blueBtnStyleREL.body.radius = 0;
    blueBtnStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

  lv_style_copy(&redBtnStylePR, &lv_style_plain);
    blueBtnStylePR.body.main_color = LV_COLOR_MAKE(0, 83, 150);
    blueBtnStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 0);
    blueBtnStylePR.body.radius = 0;
    blueBtnStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);
//----------------------------------------------------------------------------
//Creates style for SELECT buttons (bright when unpressed, darkens once pressed)
  lv_style_copy(&selectBtnStyleREL, &lv_style_plain);
    selectBtnStyleREL.body.main_color = LV_COLOR_MAKE(245, 160, 50);
    selectBtnStyleREL.body.grad_color = LV_COLOR_MAKE(165, 100, 0);
    selectBtnStyleREL.body.radius = 100;
    selectBtnStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

  lv_style_copy(&selectBtnStylePR, &lv_style_plain);
    selectBtnStylePR.body.main_color = LV_COLOR_MAKE(255, 200, 120);
    selectBtnStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 0);
    selectBtnStylePR.body.radius = 100;
    selectBtnStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);
//----------------------------------------------------------------------------

  //CREATE RED BUTTON + LABEL
  redBtn = lv_btn_create(scr, NULL);
    lv_obj_set_free_num(redBtn, 0); //ID of first button is 0
    lv_btn_set_action(redBtn, LV_BTN_ACTION_CLICK, btn_action);
    lv_btn_set_style(redBtn, LV_BTN_STYLE_REL, &redBtnStyleREL);
    lv_btn_set_style(redBtn, LV_BTN_STYLE_PR, &redBtnStylePR);
    lv_obj_set_size(redBtn, 210, 200);
    lv_obj_align(redBtn, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

  redLabel =lv_label_create(redBtn, NULL); //create label and puts it inside of the button
    lv_label_set_text(redLabel, "Red Side"); //sets label text

  //CREATE BLUE BUTTON + LABEL
  blueBtn = lv_btn_create(scr, NULL);
    lv_obj_set_free_num(blueBtn, 1); //ID of first button is 1
    lv_btn_set_action(blueBtn, LV_BTN_ACTION_CLICK, btn_action);
    lv_btn_set_style(blueBtn, LV_BTN_STYLE_REL, &blueBtnStyleREL);
    lv_btn_set_style(blueBtn, LV_BTN_STYLE_PR, &blueBtnStylePR);
    lv_obj_set_size(blueBtn, 210, 200);
    lv_obj_align(blueBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, 10, -10);

  blueLabel =lv_label_create(blueBtn, NULL); //create label and puts it inside of the button
    lv_label_set_text(blueLabel, "Blue Side"); //sets label text

  //CREATE SELECT BUTTON + LABEL
  selectBtn = lv_btn_create(scr, NULL);
    lv_obj_set_free_num(selectBtn, 2); //ID of first button is 1
    lv_btn_set_action(selectBtn, LV_BTN_ACTION_CLICK, btn_action);
    lv_btn_set_style(selectBtn, LV_BTN_STYLE_REL, &selectBtnStyleREL);
    lv_btn_set_style(selectBtn, LV_BTN_STYLE_PR, &selectBtnStylePR);
    lv_obj_set_size(selectBtn, 300, 100);
    lv_obj_align(selectBtn, NULL, LV_ALIGN_IN_BOTTOM_MID, 10, -10);

    selectLabel =lv_label_create(selectBtn, NULL); //create label and puts it inside of the button
      lv_label_set_text(selectLabel, "SELECT:"); //sets label text
}
