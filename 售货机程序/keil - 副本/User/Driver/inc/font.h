#ifndef __FONT_H
#define __FONT_H


struct typFNT_GB162
{
	unsigned char Index[2];	
	char Msk[32];
};
struct typFNT_GB242
{
	unsigned char Index[2];	
	char Msk[72];
};
struct typFNT_GB241
{
	unsigned char Index[1];	
	char Msk[48];
};
struct typFNT_GB482
{
	unsigned char Index[2];	
	char Msk[288];
};
struct typFNT_GB481
{
	unsigned char Index[1];	
	char Msk[144];
};

#define hz24_num   158
#define num24_num  16
#define hz16_num   235
#define hz48_num   46
#define en48_num   36


extern const unsigned char asc16[];
extern const struct typFNT_GB162 hz16[] ;
extern const struct typFNT_GB242 hz24[] ;
extern const struct typFNT_GB241 num24[];
#endif


