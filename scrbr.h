#define TRUE 1
#define FALSE 0

float str_to_pct(char *pct);
void itoa(int input, char *buf);
void die(char *msg);
void reverse(char *buf);
void safe_read(FILE *file, char *buf, int len);
void safe_write(FILE *file, char *buf, int len);
void set_brightness(float pct, int sign);

const char *max_br_fname = "/sys/class/backlight/gmux_backlight/max_brightness";
const char *cur_br_fname = "/sys/class/backlight/gmux_backlight/actual_brightness";
const char *set_br_fname = "/sys/class/backlight/gmux_backlight/brightness";
const int BUFSIZE = 256;
