/*
    etterlog -- log analyzer for ettercap log file 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    $Header: /home/drizzt/dev/sources/ettercap.cvs/ettercap_ng/utils/etterlog/el_main.c,v 1.3 2003/03/26 22:17:41 alor Exp $
*/

#include <el.h>
#include <el_functions.h>
#include <ec_version.h>

#include <fcntl.h>

/* global options */
struct globals gbls;

/* protos */

void open_log(char *file);
void progress(int value, int max);


/*******************************************/

int main(int argc, char *argv[])
{
   int ret;

   /* etterlog copyright */
   fprintf(stdout, "\n\033[01m\033[1m%s %s\033[0m copyright %s %s\n\n",
                      GBL_PROGRAM, EC_VERSION, EC_COPYRIGHT, EC_AUTHORS);
   
   /* getopt related parsing...  */
   parse_options(argc, argv);

   /* get the global header */
   ret = get_header(&GBL.hdr);
   ON_ERROR(ret, -EINVALID, "Invalid log file");
   
   printf("Log file version    : %s\n", GBL.hdr.version);
   printf("Timestamp           : %s", ctime((time_t *)&GBL.hdr.tv.tv_sec));
   printf("Type                : %s\n\n", (GBL.hdr.type == LOG_PACKET) ? "LOG_PACKET" : "LOG_INFO" );
   
   /* analyze the logfile */
   if (GBL.analyze) {
      analyze();
      return 0;
   }
  

   /* create the connection table */
   // create_conn_table();
   
   /* display the content of the logfile */
   // display();
   
   NOT_IMPLEMENTED();
   
   return 0;
}

/* 
 * open the logfile, then drop the privs
 */

void open_log(char *file)
{
   int zerr;
   
   GBL_LOGFILE = strdup(file);

   GBL_LOG_FD = gzopen(file, "rb");
   ON_ERROR(GBL_LOG_FD, NULL, "%s", gzerror(GBL_LOG_FD, &zerr));
 
   /* if we are root, drop privs... */
   
   if ( getuid() == 0 && setuid(65535) < 0)
      ERROR_MSG("Cannot drop priviledges...");

}

/* 
 * a nice progress bar
 */

void progress(int value, int max)
{
   float percent;
   int i;
  
   /* calculate the percent */
   percent = (float)(value)*100/(max);
            
   /* 
    * we use stderr to avoid scrambling of 
    * logfile generated by: ./ettercap -C > logfile 
    */
         
   switch(value % 4) {
      case 0:
         fprintf(stderr, "\r| |");
      break;
      case 1:
         fprintf(stderr, "\r/ |");
      break;
      case 2:
         fprintf(stderr, "\r- |");
      break;
      case 3:
         fprintf(stderr, "\r\\ |");
      break;
   }

   /* fill the bar */
   for (i=0; i < percent/2; i++)
      fprintf(stderr, "=");

   fprintf(stderr, ">");

   /* fill the empty part of the bar */
   for(; i < 50; i++)
      fprintf(stderr, " ");
                              
   fprintf(stderr, "| %6.2f %%", percent );

   fflush(stderr);

   if (value == max) 
      fprintf(stderr, "\r* |==================================================>| 100.00 %%\n\n");

}                    



/* EOF */


// vim:ts=3:expandtab

