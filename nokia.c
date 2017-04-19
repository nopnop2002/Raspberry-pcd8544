/*
 PCD8544 example

 software spi interface
 cc -o nokia nokia.c fontx.c -lwiringPi
 PCD8544    RPI
 ---------------
 1 RST  --- GPIO23   (Pin#16)
 2 CE   --- SPI CE0  (Pin#24)
 3 DC   --- GPIO24   (PIn#18)
 4 DIN  --- SPI MOSI (Pin#19)
 5 CLK  --- SPI SCLK (Pin#23)
 6 VCC  --- 3.3V
 7 BL   --- 3.3V/GND
 8 GND  --- GND
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "fontx.h"
#include "pcd8544.h"

// pin setup
#define _din  12
#define _sclk 14
#define _dc    5
#define _rst   4
#define _cs   10

#define _DEBUG_ 0

typedef struct {
  uint8_t ank;
  uint8_t utf;
  uint8_t colum;
  uint8_t reverse;
  uint8_t enhance;
  uint8_t size;
  uint8_t ascii[26];
  uint16_t sjis[26];
} SaveData;

typedef struct {
  uint8_t contrast;
  SaveData save[6];
} SaveFrame;

  
void usage(char *prog){
  printf("Usage : %s option\n",prog);
  printf("Options:\n");
  printf("  s             show display\n");
  printf("  r             remove all string\n");
  printf("  c num         set contrast to num\n");
  printf("  +1 string     add string to line#1(Ext. Font)\n");
  printf("  +2 string     add string to line#2(Ext. Font)\n");
  printf("  +3 string     add string to line#3(Ext. Font)\n");
  printf("  +4 string     add string to line#4(Ext. Font)\n");
  printf("  +5 string     add string to line#5(Ext. Font)\n");
  printf("  +6 string     add string to line#6(Ext. Font)\n");
  printf("  +a string     add string to line#1(Int. Font)\n");
  printf("  +b string     add string to line#2(Int. Font)\n");
  printf("  +c string     add string to line#3(Int. Font)\n");
  printf("  +d string     add string to line#4(Int. Font)\n");
  printf("  +e string     add string to line#5(Int. Font)\n");
  printf("  +f string     add string to line#6(Int. Font)\n");
  printf("  -1            delete string from line#1\n");
  printf("  -2            delete string from line#2\n");
  printf("  -3            delete string from line#3\n");
  printf("  -4            delete string from line#4\n");
  printf("  -5            delete string from line#5\n");
  printf("  -6            delete string from line#6\n");
//  printf("  +R n          set reverse to line#n\n");
//  printf("  -R n          unset reverse to line#n\n");
//  printf("  +U n          set underline to line#n\n");
//  printf("  -U n          unset underline to line#n\n");
  printf("  +L            line scroll up\n");
  printf("  -L            line scroll down\n");
  printf("  P1 n          set start colum n to line#1\n");
  printf("  P2 n          set start colum n to line#2\n");
  printf("  P3 n          set start colum n to line#3\n");
  printf("  P4 n          set start colum n to line#4\n");
  printf("  P5 n          set start colum n to line#5\n");
  printf("  P5 n          set start colum n to line#6\n");
}

void DumpSaveFrame(SaveFrame hoge) {
  int i,j;
  
  for(i=0;i<6;i++) {
    printf("[%d].ank=%d ",i,hoge.save[i].ank);
    printf("[%d].utf=%d ",i,hoge.save[i].utf);
    printf("[%d].colum=%d ",i,hoge.save[i].colum);
    printf("[%d].reverse=%d ",i,hoge.save[i].reverse);
    printf("[%d].enhance=%d ",i,hoge.save[i].enhance);
    printf("[%d].size=%d\n",i,hoge.save[i].size);
    for(j=0;j<hoge.save[i].size;j++) {
      if (i,hoge.save[i].ank == 1) {
        printf("[%d].ascii[%d]=%x\n",i,j,hoge.save[i].ascii[j]);
      }
      if (i,hoge.save[i].utf == 1) {
        printf("[%d].ascii[%d]=%x\n",i,j,hoge.save[i].sjis[j]);
      }
    }
  }
}

int main (int argc,char **argv)
{
  int i,j,k;
  char cpath[128];
  FILE *fp;
  SaveFrame sv;
  FontxFile fx[2];

if(_DEBUG_)  printf("argv[0]=%s\n",argv[0]);
  strcpy(cpath, argv[0]);
  for(i=strlen(cpath);i>0;i--) {
    if (cpath[i-1] == '/') {
      cpath[i] = 0;
      break;
    }
  }
  strcat(cpath,"nokia.conf");
if(_DEBUG_)  printf("cpath=%s\n",cpath);

  struct stat stat_buf;
  if (stat(cpath,&stat_buf) == 0) {
if(_DEBUG_)    printf("file found\n");
    fp = fopen(cpath,"rb");
    fread(&sv,sizeof(sv),1,fp);
    fclose(fp);
  } else {
if(_DEBUG_)    printf("file not found\n");
    memset(&sv,0,sizeof(sv));
  }
if(_DEBUG_)DumpSaveFrame(sv);

  if (argc == 1) {
    usage(basename(argv[0]));
    return 0;
  }

  // You can change font file
  Fontx_init(fx,"./fontx/4X8.FNT","./fontx/MISAKI.FNT"); // 8Dot Gothic
//  Fontx_init(fx,"./fontx/4X8.FNT","./fontx/MISAKINM.FNT"); // 8Dot Mincho
  
  int spos;
  char numc[5];
  int num;

  if ( (strcmp(argv[1],"+1") == 0) ||
       (strcmp(argv[1],"+2") == 0) ||
       (strcmp(argv[1],"+3") == 0) ||
       (strcmp(argv[1],"+4") == 0) ||
       (strcmp(argv[1],"+5") == 0) ||
       (strcmp(argv[1],"+6") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
      
if(_DEBUG_)printf("add string to line [%d]\n",num);
    sv.save[num].size = String2SJIS(argv[2], strlen(argv[2]), sv.save[num].sjis, 16);
    sv.save[num].ank = 0;
    sv.save[num].utf = 1;
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"+a") == 0) ||
       (strcmp(argv[1],"+b") == 0) ||
       (strcmp(argv[1],"+c") == 0) ||
       (strcmp(argv[1],"+d") == 0) ||
       (strcmp(argv[1],"+e") == 0) ||
       (strcmp(argv[1],"+f") == 0) ) {
    strcpy(numc,argv[1]);
    num = numc[1] - 'a';
if(_DEBUG_)printf("add ASCII string to line [%d]\n",num);

    spos = 0;
    char *ptr;
    ptr = argv[2];
    for (i=0;i<strlen(argv[2]);i++) {
if(_DEBUG_)printf("argv[2]=%c\n",*ptr);
      if (*ptr < 0x7f && spos < 16) {
        sv.save[num].ascii[spos++] = *ptr;
      }
      ptr++;
    }
    sv.save[num].ank = 1;
    sv.save[num].utf = 0;
    sv.save[num].size = spos;
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"-1") == 0) ||
       (strcmp(argv[1],"-2") == 0) ||
       (strcmp(argv[1],"-3") == 0) ||
       (strcmp(argv[1],"-4") == 0) ||
       (strcmp(argv[1],"-5") == 0) ||
       (strcmp(argv[1],"-6") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
if(_DEBUG_)printf("delete string to line [%d]\n",num);
    sv.save[num].size = 0;
    sv.save[num].reverse = 0;
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

#if 0
  if ( (strcmp(argv[1],"+R") == 0) ||
       (strcmp(argv[1],"-R") == 0) ) {
    strcpy(numc,argv[2]);
    num = (numc[0] - '0') - 1;
if(_DEBUG_)printf("set/unset reverse to line [%d]\n",num);
    if (num < 0 || num > 3) return 0;
    if (strcmp(argv[1],"+R") == 0) sv.save[num].reverse = 1;
    if (strcmp(argv[1],"-R") == 0) sv.save[num].reverse = 0;
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"+U") == 0) ||
       (strcmp(argv[1],"-U") == 0) ) {
    strcpy(numc,argv[2]);
    num = (numc[0] - '0') - 1;
if(_DEBUG_)printf("set/unset reverse to line [%d]\n",num);
    if (num < 0 || num > 3) return 0;
    if (strcmp(argv[1],"+U") == 0) sv.save[num].enhance = 1;
    if (strcmp(argv[1],"-U") == 0) sv.save[num].enhance = 0;
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }
#endif

  if ( strcmp(argv[1],"+L") == 0) {
    memcpy(&sv.save[0],&sv.save[1],sizeof(SaveData));
    memcpy(&sv.save[1],&sv.save[2],sizeof(SaveData));
    memcpy(&sv.save[2],&sv.save[3],sizeof(SaveData));
    memcpy(&sv.save[3],&sv.save[4],sizeof(SaveData));
    memcpy(&sv.save[4],&sv.save[5],sizeof(SaveData));
    memset(&sv.save[5],0,sizeof(SaveData));
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( strcmp(argv[1],"-L") == 0) {
    memcpy(&sv.save[5],&sv.save[4],sizeof(SaveData));
    memcpy(&sv.save[4],&sv.save[3],sizeof(SaveData));
    memcpy(&sv.save[3],&sv.save[2],sizeof(SaveData));
    memcpy(&sv.save[2],&sv.save[1],sizeof(SaveData));
    memcpy(&sv.save[1],&sv.save[0],sizeof(SaveData));
    memset(&sv.save[0],0,sizeof(SaveData));
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"P1") == 0) ||
       (strcmp(argv[1],"P2") == 0) ||
       (strcmp(argv[1],"P3") == 0) ||
       (strcmp(argv[1],"P4") == 0) ||
       (strcmp(argv[1],"P5") == 0) ||
       (strcmp(argv[1],"P6") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
    strcpy(numc,argv[2]);
    int col;
//    col = (numc[0] - '0') - 1;
    col = atoi(numc) - 1;
    if (col < 0 || col > 84) return 0;
if(_DEBUG_)printf("set start colum to line [%d] = %d\n",num,col);
    sv.save[num].colum = col;
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if (strcmp(argv[1],"r") == 0) {
    memset(&sv,0,sizeof(sv));
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }
  
  if (strcmp(argv[1],"c") == 0) {
if(_DEBUG_) printf("argc=%d argv[2]=%s\n",argc,argv[2]);
    if (argc != 3) return 1;
    sv.contrast = atoi(argv[2]);
if(_DEBUG_) printf("sv.contrast=%d\n",sv.contrast);
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }
  
  if (strcmp(argv[1],"s") == 0) {
if(_DEBUG_)printf("show dislay\n");
  // check wiringPi setup
    if (wiringPiSetup() == -1) {
      printf("wiringPi-Error\n");
      return 1;
    }

    // lcd contrast 
    int contrast = 50;
    if (sv.contrast != 0) contrast = sv.contrast;
if(_DEBUG_)printf("contrast=%d\n",contrast);

    // init and clear lcd
    LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
    LCDclear();

    int x;
    for (num=0;num<6;num++) {
      if (sv.save[num].size == 0) continue;
      x = sv.save[num].colum+1;
      for (i=0;i<sv.save[num].size;i++) {
         
//if(_DEBUG_)printf("x=%d,num=%d\n",x,num);
        if (sv.save[num].ank)
          x = LCDdrawChar(x,num,sv.save[num].ascii[i],
                       sv.save[num].reverse,
                       sv.save[num].enhance);
        if (sv.save[num].utf)
          x = LCDdrawSJISChar(fx,x,num,sv.save[num].sjis[i],
                       sv.save[num].reverse,
                       sv.save[num].enhance);
      }
    }
    LCDdisplay();
  }

  if (strcmp(argv[1],"D") == 0) {
    DumpSaveFrame(sv);
  }
  
  return 0;
}
