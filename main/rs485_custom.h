#ifndef RS485_CUSTOM_H
#define RS485_CUSTOM_H

int sendHex(const char* logName, const unsigned char* data, int len);
void hsvrgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
void h2str(unsigned int hexValue, char *output);
void init485();
void color_setup();

void midopn();
void clsmid();
void clear_all_fcob();

void predawn();
void morning();
void afternoon();

#endif