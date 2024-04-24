#include "all_variables.h"
#include "rs485_custom.h"
#include "custom_mbcrc.h"
#include "csv_sd.h"
#include <math.h>

uint8_t rgbs[6];
uint8_t temp[6]; 

const uart_config_t uart_config = {
    .baud_rate = 115200,                      // correct
    .data_bits = UART_DATA_8_BITS,          // correct
    .parity = UART_PARITY_DISABLE,          // correct
    .stop_bits = UART_STOP_BITS_1,          // correct
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    // .rx_flow_ctrl_thresh = 122,
    .source_clk = UART_SCLK_APB,
};
#define EN1                         GPIO_NUM_6          // transmit
#define TXD1_PIN                    GPIO_NUM_4    // TX
#define RXD1_PIN                    GPIO_NUM_7    // RX
#define EN2                         GPIO_NUM_37         // receive
#define TXD2_PIN                    GPIO_NUM_35   // TX
#define RXD2_PIN                    GPIO_NUM_38   // RX
static const int                    RX_BUF_SIZE = 1024;

int sendHex(const char* logName, const uint8_t* data, int len)
{       
    const int txBytes = uart_write_bytes(UART_NUM_1,data,len);
    // ESP_LOGI(logName,"Wrote %d bytes", txBytes);
    // ESP_LOGI(logName,"Data %02X ", );
    // ESP_LOG_BUFFER_HEXDUMP(logName, data, len, ESP_LOG_INFO);
    return txBytes;
}

void hsvrgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    float c = v*s;
    float x = c*(1-fabsf(fmodf(h/60.0,2)-1));
    float m = v-c;
    float rs,gs,bs;

    if (h>=0 && h<60)               {rs=c;   gs=x;   bs=0;}
    else if (h >= 60 && h < 120)    {rs = x;        gs = c;        bs = 0;}
    else if (h >= 120 && h < 180)    {rs = 0;        gs = c;        bs = x;}
    else if (h >= 180 && h < 240)    {rs = 0;        gs = x;        bs = c;}
    else if (h >= 240 && h < 300)    {rs = x;        gs = 0;        bs = c;}
    else                            {rs=c; gs=0; bs=x;}

    *r = (uint8_t)((rs + m) * 255);
    *g = (uint8_t)((gs + m) * 255);
    *b = (uint8_t)((bs + m) * 255);
}

void h2str(unsigned int hexValue, char *output){sprintf(output,"%c",hexValue);}

void init485()
{
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE *3, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);    
    uart_set_pin(UART_NUM_1,TXD2_PIN,RXD2_PIN,EN2,EN2);
    uart_set_mode(UART_NUM_1, UART_MODE_RS485_HALF_DUPLEX);
}

void color_setup(){
    // ------------------------ WAVE -------------------------
    float Ho = value[0]/100.0; float Ho1 = value1[0]/100.0;
    float So = value[1]/100.0; float So1 = value1[1]/100.0;
    float Vo = value[2]/100.0; float Vo1 = value1[2]/100.0;
    float Hc = value[3]/100.0; float Hc1 = value1[3]/100.0;
    float Sc = value[4]/100.0; float Sc1 = value1[4]/100.0;
    float Vc = value[5]/100.0; float Vc1 = value1[5]/100.0;

    uint16_t k=160/2;
    uint16_t j=k-1; 
    for (uint16_t m =0; m<(80/k); m++){
        for (uint16_t i = 0; i< k ; i++){

            hsvrgb((Hc - (((Hc-Hc1)/k)*i)), (Sc-(((Sc-Sc1)/((float)k))*i)), (Vc-(((Vc-Vc1)/((float)k))*i)), &bc[16+(k*m*3)+(i*3)+0], &bc[16+(k*m*3)+(i*3)+2], &bc[16+(k*m*3)+(i*3)+1]);
            hsvrgb((Hc - (((Hc-Hc1)/k)*j)), (Sc-(((Sc-Sc1)/((float)k))*j)), (Vc-(((Vc-Vc1)/((float)k))*j)), &bc[256+(k*(0+m)*3)+(i*3)+0], &bc[256+(k*(0+m)*3)+(i*3)+2], &bc[256+(k*(0+m)*3)+(i*3)+1]);
            hsvrgb((Hc - (((Hc-Hc1)/k)*j)), (Sc-(((Sc-Sc1)/((float)k))*j)), (Vc-(((Vc-Vc1)/((float)k))*j)), &bc[496+(k*m*3)+(i*3)+0], &bc[496+(k*m*3)+(i*3)+2], &bc[496+(k*m*3)+(i*3)+1]);
            hsvrgb((Hc - (((Hc-Hc1)/k)*i)), (Sc-(((Sc-Sc1)/((float)k))*i)), (Vc-(((Vc-Vc1)/((float)k))*i)), &bc[736+(k*(0+m)*3)+(i*3)+0], &bc[736+(k*(0+m)*3)+(i*3)+2], &bc[736+(k*(0+m)*3)+(i*3)+1]);

            hsvrgb((Ho - (((Ho-Ho1)/k)*j)), (So-(((So-So1)/((float)k))*j)), (Vo-(((Vo-Vo1)/((float)k))*j)), &ac[16+(k*m*3)+(i*3)+0], &ac[16+(k*m*3)+(i*3)+2], &ac[16+(k*m*3)+(i*3)+1]);
            hsvrgb((Ho - (((Ho-Ho1)/k)*i)), (So-(((So-So1)/((float)k))*i)), (Vo-(((Vo-Vo1)/((float)k))*i)), &ac[256+(k*(0+m)*3)+(i*3)+0], &ac[256+(k*(0+m)*3)+(i*3)+2], &ac[256+(k*(0+m)*3)+(i*3)+1]);
            hsvrgb((Ho - (((Ho-Ho1)/k)*i)), (So-(((So-So1)/((float)k))*i)), (Vo-(((Vo-Vo1)/((float)k))*i)), &ac[496+(k*m*3)+(i*3)+0], &ac[496+(k*m*3)+(i*3)+2], &ac[496+(k*m*3)+(i*3)+1]);
            hsvrgb((Ho - (((Ho-Ho1)/k)*j)), (So-(((So-So1)/((float)k))*j)), (Vo-(((Vo-Vo1)/((float)k))*j)), &ac[736+(k*(0+m)*3)+(i*3)+0], &ac[736+(k*(0+m)*3)+(i*3)+2], &ac[736+(k*(0+m)*3)+(i*3)+1]);
            j--;
      	}
        j=k-1;
    }
}

void midopn()
{
    ac[976] = 0xAA;
    ac[977] = 0xBB;
               
    for (uint16_t i = 0; i < 3; i++)
    {                    
        for (uint16_t i = 0; i < 3; i++)            {temp[i] = ac[16+i];}
        uint16_t j = 158;
        for (uint16_t m = 0; m < 159; m++)
        {            
            for (uint16_t i = 0; i < 3; i++)            {ac[16+((m*1)*3)+i] = ac[19+((m*1)*3)+i];}
            for (uint16_t i = 0; i < 3; i++)            {ac[499+((j*1)*3)+i] = ac[496+((j*1)*3)+i];}
            j--;
        }              
        for (uint16_t i = 0; i < 3; i++)            {ac[493+i] = temp[i];ac[496+i] = temp[i];} 
    }    
    sendHex("TX_TASK", ac, length1);
    vTaskDelay(time1[0] / portTICK_PERIOD_MS);
}

void clsmid()
{
    bc[976] = 0xAA;
    bc[977] = 0xBB;
   
    for (uint16_t i = 0; i < 3; i++)
    {                    
        for (uint16_t i = 0; i < 3; i++)            {temp[i+3] = bc[493+i];}
        uint16_t j = 158;
        for (uint16_t m = 0; m < 159; m++)
        {            
            for (uint16_t i = 0; i < 3; i++)            {bc[19+((j*1)*3)+i] = bc[16+((j*1)*3)+i];}
            for (uint16_t i = 0; i < 3; i++)            {bc[496+((m*1)*3)+i] = bc[499+((m*1)*3)+i];}
            j--;
        }                     
        for (uint16_t i = 0; i < 3; i++)            {bc[16+i] = temp[i+3];bc[973+i] = temp[i+3];} 
    }    
    sendHex("TX_TASK", bc, length2);
    vTaskDelay(time1[1] / portTICK_PERIOD_MS);
}

void clear_all_fcob()
{
    cc[976] = 0xAA;
    cc[977] = 0xBB;
    for (uint16_t k = 0; k < 960; k++){cc[16+k] = 0x00;}
    sendHex("TX_TASK", cc, length3);
    vTaskDelay(30/portTICK_PERIOD_MS);  
}


void predawn()
{
    switch (data7)
    {
    case 0x0:            clear_all_fcob();break;                                 // no opn and cls signal
    case 0x1:            while(data7!=0x2) {midopn();}break; // opn signal
    case 0x2:            clsmid();break;                             // cls signal
    default:break;
    } 
}

void morning()
{
    switch (data7)
    {
    case 0x0:            clear_all_fcob();break;                                 // no opn and cls signal
    case 0x1:            while(data7!=0x2) {midopn();}break; // opn signal
    case 0x2:            clsmid();break;                             // cls signal
    default:break;
    } 
}

void afternoon()
{
    switch (data7)
    {
    case 0x0:            clear_all_fcob();break;                                 // no opn and cls signal
    case 0x1:            while(data7!=0x2) {midopn();}break; // opn signal
    case 0x2:            clsmid();break;                             // cls signal
    default:break;
    }     
}

