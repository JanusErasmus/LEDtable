#ifndef KEY_SIM

#include <cyg/kernel/kapi.h>

#else

#include "../key-sim/sim-config.h"

#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>

union u_conv
{
   cyg_uint8 b[4];
   cyg_uint16 s[2];
   cyg_uint32 l;
} __attribute__((__packed__));

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


cyg_uint32 util_htonl(cyg_uint32 val)
{
   u_conv in;
   u_conv out;
   in.l = val;
   out.b[0] = in.b[3];
   out.b[1] = in.b[2];
   out.b[2] = in.b[1];
   out.b[3] = in.b[0];

   return out.l;
}


cyg_uint16 util_htons(cyg_uint16 val)
{
   u_conv in;
   u_conv out;

   in.s[0] = val;
   out.b[0] = in.b[1];
   out.b[1] = in.b[0];
   return out.s[0];
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

void timeToString(cyg_uint32 now, char * string)
{
	if(now < 60)
	{
		sprintf(string, "%ds", (int)now);
	}
	else
	{
		int min = now / 60;
		now %= 60;
		if(min < 60)
		{
			sprintf(string, "%dm %ds", min, (int)now);
		}
		else
		{
			int hour = min / 60;
			min %= 60;
			if(hour < 24)
			{
				sprintf(string, "%dh %dm %ds", hour, min, (int)now);
			}
			else
			{
				int day = hour / 24;
				hour %= 24;
				sprintf(string, "%dd %dh %dm %ds", day, hour, min ,(int)now);
			}
		}
	}
}
