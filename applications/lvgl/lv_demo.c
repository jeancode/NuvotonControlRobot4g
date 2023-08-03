/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-6-1       Wayne         First version
 */

#include <lvgl.h>


//tcp conextion
#include <rtthread.h>
#include <sys/socket.h> /* To use BSD socket, you need to include the socket.h header file. */
#include <netdb.h>
#include <string.h>
#include <finsh.h>

#define BUFSZ   1024

lv_obj_t * dropdown;
lv_obj_t * dropdown2;

int ret;
char *recv_data;
struct hostent *host;
int sock, bytes_received;
struct sockaddr_in server_addr;
const char *url = "47.87.134.53";
int port = 30;



LV_IMG_DECLARE(A01);

LV_IMG_DECLARE(A11);

LV_IMG_DECLARE(A21);

LV_IMG_DECLARE(A31);

lv_obj_t *imgP;

bool boton0State = false;
bool boton1State = false;
bool boton2State = false;
bool boton3State = false;

char *speedA = "0";
char *timeA = "0";

lv_obj_t *label4;
lv_obj_t *label5;
lv_obj_t *div1;
lv_obj_t *ta1;

bool conectado = false;

void sendData(char *data){
    if(conectado){
        send(sock, data, strlen(data), 0);//socket, data, size, flags
    }
}


//btn0_event_cb
static void btn0_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        
        lv_img_set_src(imgP, &A01);
        lv_obj_align(imgP, LV_ALIGN_CENTER, 0, 0);

        
        boton2State =  true;
        boton3State =  true;

    }
}

static void btn1_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        
        lv_img_set_src(imgP, &A11);
        lv_obj_align(imgP, LV_ALIGN_CENTER, 0, 0);

        boton2State =  false;
        boton3State =  false;

    }
}

//btn2_event_cb
static void btn2_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {

        boton2State =  true;
        lv_img_set_src(imgP, &A21);
        lv_obj_align(imgP, LV_ALIGN_CENTER, 0, 0);

        boton1State =  false;
 

    }
}

//btn3_event_cb
static void btn3_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
  

        boton3State =  true;
        lv_img_set_src(imgP, &A31);
        lv_obj_align(imgP, LV_ALIGN_CENTER, 0, 0);

        boton2State =  false;

    }
}

//btn4_event_cb
static void btn4_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {

        // char data[100] = "Rt-Admin|m1 s-10000 t10000 m2 s-10000 t10000";
        char *data;
   

        if(boton2State == true && boton3State == true){

     
            data = "Rt-Admin|m1 s-1000 t2000 m2 s-1000 t2000";
   
   

        }else if(boton2State == true && boton3State == false){

   
           data = "Rt-Admin|m1 s-1000 t2000 m2 s0 t0";
   
   

        }else if(boton2State == false && boton3State == true){

            data = "Rt-Admin|m1 s0 t0 m2 s-1000 t2000";
   

        }else{
                
            data = "Rt-Admin|m1 s0 t0 m2 s0 t0";
        }
   

        //append data textarea
        lv_textarea_add_text(ta1, "Sending: ");
        lv_textarea_add_text(ta1, data);

        
        //data
       
        sendData(data);
        

    }
}

static void event_cb(lv_event_t * e)
{
    lv_obj_t * dropdown = lv_event_get_target(e);
    char buf[64];
    lv_dropdown_get_selected_str(dropdown, buf, sizeof(buf));
    
    char buf2[64] = "Speed:";
    //remove \n

    speedA = buf;


    strcat(buf2, buf);
    
    //label4 change text
    lv_label_set_text(label4, buf2);


    LV_LOG_USER("'%s' is selected", buf);

}

//event_cb2
static void event_cb2(lv_event_t * e)
{
    lv_obj_t * dropdown = lv_event_get_target(e);
    char buf[64];
    lv_dropdown_get_selected_str(dropdown, buf, sizeof(buf));

    //remove \n
    buf[strlen(buf)-1] = '\0';


    timeA = buf;
    
    char buf2[64] = "Time:";

    char buf3[64] = "s";

    strcat(buf2, buf);
    strcat(buf2, buf3);
    
    //label4 change text
    lv_label_set_text(label5, buf2);

}




RT_WEAK void lv_user_gui_init(void)
{



    /* display demo; you may replace with your LVGL application at here and disable related definitions. */
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, &lv_font_montserrat_12);

    lv_obj_t * scr =lv_scr_act();

    //flag scroll none
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);





    imgP = lv_img_create(lv_scr_act());
    lv_img_set_src(imgP, &A11);
    //CENTRE
    lv_obj_set_pos(imgP, 0, 0);
    lv_obj_align(imgP, LV_ALIGN_CENTER, 0, 0);



    //select all
    lv_obj_t *btn0 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn0, LV_ALIGN_CENTER, 0, -150);
    //border radios
    lv_obj_set_style_radius(btn0, LV_RADIUS_CIRCLE, 0);
    //backgroundcolor
    lv_obj_set_style_bg_color(btn0, lv_color_make(33,33,33), 0);
    //bordercolor
    lv_obj_set_style_border_color(btn0, lv_color_make(255,255,255), 0);
    //border wi
    lv_obj_set_style_border_width(btn0, 1, 0);
    //border opa
    lv_obj_set_style_border_opa(btn0, 50, 0);

    lv_obj_t *label0 = lv_label_create(btn0);
    lv_label_set_text(label0, "SELECT ALL");
    lv_obj_add_event_cb(btn0, btn0_event_cb, LV_EVENT_ALL, NULL);




    //boton1 
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -200);
    //border radios
    lv_obj_set_style_radius(btn1, LV_RADIUS_CIRCLE, 0); 
    //backgroundcolor
    lv_obj_set_style_bg_color(btn1, lv_color_make(33,33,33), 0);
    //bordercolor
    lv_obj_set_style_border_color(btn1, lv_color_make(255,255,255), 0);
    //borderwidth
    lv_obj_set_style_border_width(btn1, 1, 0);
    //border opa
    lv_obj_set_style_border_opa(btn1, 50, 0);


    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "UNSELECTED");
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_ALL, NULL);



    //boton2
    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn2, LV_ALIGN_CENTER, -150, -10);
    //size
    lv_obj_set_size(btn2, 100,100);
    //opacity
    lv_obj_set_style_bg_opa(btn2, 0, 0);


    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Left");

    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_ALL, NULL);



    //boton3
    lv_obj_t *btn3 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn3, LV_ALIGN_CENTER, 20, 20);
    //size
    lv_obj_set_size(btn3, 100,100);
    //opacity
    lv_obj_set_style_bg_opa(btn3, 0, 0);

    lv_obj_t *label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Right");

    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_ALL, NULL);



    //controls

    //label color white Speed

    label4 = lv_label_create(lv_scr_act());
    lv_label_set_text(label4, "Speed:");
    lv_obj_align(label4, LV_ALIGN_CENTER, -200, 200);
    lv_obj_set_style_text_color(label4, lv_color_make(255,255,255), 0);

    //label color white Time

    label5 = lv_label_create(lv_scr_act());
    lv_label_set_text(label5, "Time:");
    lv_obj_align(label5, LV_ALIGN_CENTER, -100, 200);
    lv_obj_set_style_text_color(label5, lv_color_make(255,255,255), 0);





   /*Create a drop down list*/
    dropdown = lv_dropdown_create(lv_scr_act());
    lv_obj_align(dropdown, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_dropdown_set_options(dropdown, "-1000\n"
                            "-800\n"
                            "-700\n"
                            "-500\n"
                            "0\n"
                            "500\n"
                            "700\n"
                            "800\n"
                            "1000\n"
                            "2000");

    /*Set a fixed text to display on the button of the drop-down list*/
    lv_dropdown_set_text(dropdown, "Speed:0");

    /*Use a custom image as down icon and flip it when the list is opene"700\n"R | LV_STATE_CHECKED);

    /*In a menu we don't need to show the last clicked item*/
    lv_dropdown_set_selected_highlight(dropdown, false);

    lv_obj_add_event_cb(dropdown, event_cb, LV_EVENT_VALUE_CHANGED, NULL);


    //create drop time
    dropdown2 = lv_dropdown_create(lv_scr_act());
    lv_obj_align(dropdown2, LV_ALIGN_TOP_LEFT, 150, 10);
    lv_dropdown_set_options(dropdown2, "1\n"
                            "2\n"
                            "3\n"
                            "4\n"
                            "5\n"
                            "6\n"
                            "7\n"
                            "8\n"
                            "9\n"
                            "10");

    /*Set a fixed text to display on the button of the drop-down list*/
    lv_dropdown_set_text(dropdown2, "Time:0");

    /*Use a custom image as down icon and flip it when the list is opene"700\n"R | LV_STATE_CHECKED);

    /*In a menu we don't need to show the last clicked item*/
    lv_dropdown_set_selected_highlight(dropdown2, false);

    lv_obj_add_event_cb(dropdown2, event_cb2, LV_EVENT_VALUE_CHANGED, NULL);


    //boton sendComand
    lv_obj_t *btn4 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn4, LV_ALIGN_CENTER, 200, 200);
    //border radios
    lv_obj_set_style_radius(btn4, LV_RADIUS_CIRCLE, 0);
    //backgroundcolor
    lv_obj_set_style_bg_color(btn4, lv_color_make(33,33,33), 0);
    //bordercolor
    lv_obj_set_style_border_color(btn4, lv_color_make(255,255,255), 0);
    //borderwidth
    lv_obj_set_style_border_width(btn4, 1, 0);
    //border opa
    lv_obj_set_style_border_opa(btn4, 50, 0);


    lv_obj_t *label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "Send Command");
    lv_obj_add_event_cb(btn4, btn4_event_cb, LV_EVENT_ALL, NULL);


    //creare div
    ta1 = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(ta1, 200, 300);
    lv_obj_align(ta1, LV_ALIGN_CENTER, 200, 0);
    lv_textarea_set_one_line(ta1, false);
    lv_obj_add_state(ta1, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

    //background opa
    lv_obj_set_style_bg_opa(ta1, 50, 0);


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        
        //append text
        lv_textarea_add_text(ta1, "socket error");
        //lv_textarea_set_cursor_hidden(ta, true);


    }else{
            
        //append text
        lv_textarea_add_text(ta1, "socket ok");
        /* Initialize the pre-connected server address */


        host = gethostbyname(url);
        recv_data = rt_malloc(BUFSZ);

        if (host == NULL)
        {
            //append text
            lv_textarea_add_text(ta1, "gethostbyname error");
            //lv_textarea_set_cursor_hidden(ta, true);
        }else{
            //append text
            lv_textarea_add_text(ta1, "gethostbyname ok");
            //lv_textarea_set_cursor_hidden(ta, true);

            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            server_addr.sin_addr = *((struct in_addr *)host->h_addr);
            memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

            //time out
            struct timeval timeout = {3, 0};
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
            {
                //append text
                lv_textarea_add_text(ta1, "connect error");
                //lv_textarea_set_cursor_hidden(ta, true);
                conectado = false;
            }else{
                //append text
                lv_textarea_add_text(ta1, "connect ok");
                //lv_textarea_set_cursor_hidden(ta, true);
                conectado = true;
            }



        }


   
        
    }
    



}
