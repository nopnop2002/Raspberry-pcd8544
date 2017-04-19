#include <stdint.h>

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

void LCDspiwrite(uint8_t c);
void LCDcommand(uint8_t c);
void LCDdata(uint8_t c);
void LCDInit(uint8_t SCLK, uint8_t DIN, uint8_t DC, uint8_t CS, uint8_t RST, uint8_t contrast);
void LCDsetContrast(uint8_t val);
void LCDdisplay();
void LCDclear();
int LCDdrawString(uint8_t x, uint8_t page, char *str, uint8_t reverse, uint8_t enhance);
int LCDdrawChar(uint8_t x, uint8_t page, char c, uint8_t reverse, uint8_t enhance);
int LCDdrawSJISChar(FontxFile *fx, uint8_t x, uint8_t page, uint16_t sjis, uint8_t reverse, uint8_t enhance);
int LCDdrawUTF8Char(FontxFile *fx, int x,int y,uint8_t *utf8,uint8_t reverse,
                 uint8_t enhance);
int LCDdrawUTF8String(FontxFile *fx,int x,int y,unsigned char *utfs,
                   uint8_t reverse,uint8_t enhance);
