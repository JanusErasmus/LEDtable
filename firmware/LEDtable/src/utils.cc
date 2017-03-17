#include <cyg/kernel/kapi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

#define htons(x) PLATFORM_HTONS(x)
#define ntohs(x) PLATFORM_HTONS(x)
#define htonl(x) PLATFORM_HTONL(x)
#define ntohl(x) PLATFORM_HTONL(x)

float nan = 0;

void initNaN()
{
    if(nan == 0)
    {
        unsigned long buff[1]={0x7fffffff};
        memcpy(&nan, buff,4);
    }
}

bool isNaN(float num)
{
    unsigned char* p = (unsigned char*)&num;

    if(p[3] == 0x7F && p[2] == 0xFF && p[1] == 0xFF && p[0] == 0xFF)
        return true;

    if(p[3] == 0xFF && p[2] == 0xFF && p[1] == 0xFF && p[0] == 0xFF)
            return true;

    return false;
}

void util_htonl(cyg_uint8 * bytes, cyg_uint32 len)
{
    cyg_uint32 temp = 0;
    for(cyg_uint32 k = 0; k < len; k+=4)
    {
        memcpy(&temp, &bytes[k], 4);
        temp = htonl(temp);
        memcpy(&bytes[k], &temp, 4);
    }
}

void util_parse_params(char *str,char *argv[],int &argc,char delim1,char delim2)
{

   int max_args = argc;
   char * cmdl = str;
   cyg_bool done = false;
   argc = 0;
   char delim = delim1;
   while ( !done )
   {
      /* Strip Leading Whitespce */
      while ( isspace(*cmdl) )
      {
         if ( *cmdl )
         {
            cmdl++;
         }
         else
         {
            done = true;
            break;
         }
      }
      /* Now we are at an arg */
      if ( !done && *cmdl )
      {
         argv[argc] = cmdl;
         argc++;
         if(argc >= max_args)
         {
            done =true;
            break;
         }
      }
      /* Go to the next delim */
      while ( delim != *cmdl )
      {
         if ( *cmdl )
         {
            cmdl++;
         }
         else
         {
            done = true;
            break;
         }
      }
      if ( *cmdl )
      {
         *cmdl = 0;
         cmdl++;
      }
      else
      {
         done = true;
      }
      if(argc)
      {
         delim = delim2;
      }
   }
}

cyg_uint64 tagToInt(cyg_uint8* tag)
{
    cyg_uint64 val = 0;

    for (int k = 4; k >= 0; --k)
    {
        val |= (cyg_uint64)tag[k] << (8*k);
    }

    return val;
}

void tagToString(char * string, cyg_uint8* tag)
{
    char txt[16];
    string[0] = 0;

    for (int k = 0; k < 5; ++k)
    {
        sprintf(txt, "%02X", tag[k]);
        strcat(string, txt);
    }
}
