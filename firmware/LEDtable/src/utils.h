#ifndef _UTILS_H_
#define _UTILS_H_
# include <cyg/hal/hal_endian.h>
//#define VT100_OFF

#define INFO_TRACE(_class, _string, ...)    diag_printf( "%15s : " _string, _class, ##__VA_ARGS__)

#define PRINT_REG(_base, _x)    diag_printf(#_x": 0x%08X\n", *((volatile CYG_WORD32 *)(_base + _x)));

#ifndef VT100_OFF
#define COLOR(__c,__x)    "\x1b[3" #__c "m" __x "\x1b[0m"
#define COLOR_BOLD(__c,__x)    "\x1b[3" #__c ";1m" __x "\x1b[0m"
#define UNDERLINE(__x) "\x1b[4m" __x "\x1b[0m"
#define CLEAR_SCREEN    "\x1b[2J\x1b[H"
#else
#define COLOR(__c,__x)    __x
#define COLOR_BOLD(__c,__x) __x
#define UNDERLINE(__x) __x
#define CLEAR_SCREEN
#endif
#define RED(__x)        COLOR(1, __x )
#define GREEN(__x)        COLOR(2, __x )
#define YELLOW(__x)        COLOR(3, __x )
#define BLUE(__x)        COLOR(4, __x )
#define MAGENTA(__x)    COLOR(5, __x )
#define CYAN(__x)        COLOR(6, __x )
#define RED_B(__x)        COLOR_BOLD(1, __x )
#define GREEN_B(__x)        COLOR_BOLD(2, __x )
#define YELLOW_B(__x)        COLOR_BOLD(3, __x )
#define BLUE_B(__x)        COLOR_BOLD(4, __x )
#define MAGENTA_B(__x)    COLOR_BOLD(5, __x )
#define CYAN_B(__x)        COLOR_BOLD(6, __x )

#define PLATFORM_HTONS(__val) CYG_CPU_TO_BE16(__val)
#define PLATFORM_HTONL(__val) CYG_CPU_TO_BE32(__val)

extern float nan;
void initNaN();
bool isNaN(float num);

void util_htonl(cyg_uint8 * bytes, cyg_uint32 len);

void util_parse_params(char *str,char *argv[],int &argc,char delim1,char delim2);

void tagToString(char * string, cyg_uint8* tag);
cyg_uint64 tagToInt(cyg_uint8* tag);

#endif //Include Guard
