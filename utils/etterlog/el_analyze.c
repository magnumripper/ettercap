/*
    etterlog -- analysis module

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

    $Header: /home/drizzt/dev/sources/ettercap.cvs/ettercap_ng/utils/etterlog/el_analyze.c,v 1.3 2003/03/26 22:17:40 alor Exp $
*/

#include <el.h>
#include <ec_log.h>
#include <el_functions.h>

#include <sys/stat.h>

void analyze(void);
void analyze_packet(void);
void analyze_info(void);

/*******************************************/

void analyze(void)
{
   switch(GBL.hdr.type) {
      case LOG_PACKET:
         analyze_packet();
         break;
      case LOG_INFO:
         analyze_info();
         break;
   }
}

/* analyze an inf log file */

void analyze_info(void)
{
   NOT_IMPLEMENTED();
}

/* analyze a packet log file */

void analyze_packet(void)
{
   struct log_header_packet pck;
   int ret, count = 0;
   int tot_size = 0, pay_size = 0;
   u_char *buf;
   struct stat st;
   
   printf("\nAnalyzing the log file (one dot for 100 packets)\n");
  
   /* read the logfile */
   LOOP {
      ret = get_packet(&pck, &buf);

      /* on error exit the loop */
      if (ret != ESUCCESS)
         break;
      
      count++;
      tot_size += sizeof(struct log_header_packet) + pck.len;
      pay_size += pck.len;
    
      if (count % 100 == 0) {
         printf(".");
         fflush(stdout);
      }
      
      SAFE_FREE(buf);
   }

   /* get the file stat */
   stat(GBL.logfile, &st);
   
   printf("\n\n");
   printf("Log file size (compressed)   : %d\n", (int)st.st_size);   
   printf("Log file size (uncompressed) : %d\n", tot_size);
   printf("Effective payload size       : %d\n", pay_size);
   printf("Wasted percentage            : %.2f %%\n\n", 100 - ((float)pay_size * 100 / (float)tot_size) );
   
   printf("Number of packets            : %d\n", count);
   printf("Average size per packet      : %d\n", pay_size / count );
   printf("\n");
   
   return;
}

/* EOF */

// vim:ts=3:expandtab

