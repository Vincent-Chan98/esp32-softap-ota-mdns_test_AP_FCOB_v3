#include "all_variables.h"

#define LEDTYPE 0x02            // led type

char strftime_buf0[64]; // char strftime_buf1[64];
int s_retry_num;
const int retry_count = 10;
//                    Cmd header          GroupAddr  DeviceAddr   Port    f.code  ledtype  reserved   ledlength   repeat

uint8_t ac[978] = {0xDD,0x55,0xEE,    0x00,0x00,0x00,0x01,    0x00,   0x99,   LEDTYPE,   0x00,0x00,  0x03,0xC0,  0x00,0x01};
int length1 = sizeof(ac)/sizeof(ac[0]);
uint8_t bc[978] = {0xDD,0x55,0xEE,    0x00,0x00,0x00,0x01,    0x00,   0x99,   LEDTYPE,   0x00,0x00,  0x03,0xC0,  0x00,0x01};
int length2 = sizeof(bc)/sizeof(bc[0]);
uint8_t cc[978] = {0xDD,0x55,0xEE,    0x00,0x00,0x00,0x01,    0x00,   0x99,   LEDTYPE,   0x00,0x00,  0x03,0xC0,  0x00,0x01};
int length3 = sizeof(cc)/sizeof(cc[0]);

unsigned char data7; 
char line[64]; 
int32_t value[6];
int32_t value1[6];
char pass[64];
double diff;
int fl = 0;
int time1[2];
char pswd[10];
bool can_flag = false;
bool wf_flag = false;
