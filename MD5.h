/************************************************************
MD5校验和计算小程序（C）
************************************************************/
#include "stdafx.h"
#ifndef __MD5_INCLUDED__
#define __MD5_INCLUDED__
#endif

//DEFINES for MD5
#define UINT4 unsigned int


/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits. */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation. */
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
	    }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
	    }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
	    }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
	    }

// Constants for MD5 Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

//===============

//MD5摘要值结构体
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} MD5VAL;
class CMD5
{
public:
	CMD5(){};
	~CMD5(){};


/*
	//计算字符串的MD5值(若不指定长度则由函数计算)
	MD5VAL md5(char * str, unsigned int size = 0);

	//MD5文件摘要
	MD5VAL md5File(FILE * fpin);

	//直接计算文件MD5
	char * md5FileValue(char * fname);

	//字符串md5加密
	void md5Str(char * input, char * output);*/
	unsigned int CMD5::conv(unsigned int a);
	MD5VAL md5(char * str, unsigned int size);
	MD5VAL md5File(FILE * fpin);
	CString CMD5::md5Str(char * input);
	CString CMD5::md5FileValue(char * fname);
};