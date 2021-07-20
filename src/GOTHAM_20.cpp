
/**
 * @file src/GOTHAM_20.cpp
 * 
 * Contains Gotham font used on V5 display 
 */

#include "main.h"

#if USE_GOTHAM_20 != 0	/*Can be enabled in lv_conf.h*/

/***********************************************************************************
 * GothamLight.ttf 4 px Font in U+003c (<) .. U+007a (z)  range with all bpp
 * Sparse font with only these characters: <>abcdefghijklmnopqrstuvwxyz
***********************************************************************************/

/*Store the image of the letters (glyph)*/
static const uint8_t GOTHAM_20_glyph_bitmap[] = 
{
#if USE_GOTHAM_20 == 1
  /*Unicode: U+003c (<) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+003e (>) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0061 (a) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0062 (b) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0063 (c) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0064 (d) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0065 (e) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0066 (f) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0067 (g) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0068 (h) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0069 (i) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006a (j) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006b (k) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006c (l) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006d (m) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006e (n) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006f (o) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0070 (p) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0071 (q) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0072 (r) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0073 (s) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0074 (t) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0075 (u) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0076 (v) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0077 (w) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0078 (x) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0079 (y) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+007a (z) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 



#elif USE_GOTHAM_20 == 2
  /*Unicode: U+003c (<) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+003e (>) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0061 (a) , Width: 1 */
  0x00,  //. 
  0x40,  //+ 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0062 (b) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0063 (c) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0064 (d) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0065 (e) , Width: 2 */
  0x00,  //.. 
  0x40,  //+. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0066 (f) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0067 (g) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0068 (h) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0069 (i) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006a (j) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006b (k) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006c (l) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006d (m) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006e (n) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006f (o) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0070 (p) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0071 (q) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0072 (r) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0073 (s) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0074 (t) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0075 (u) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0076 (v) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0077 (w) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0078 (x) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0079 (y) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+007a (z) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 



#elif USE_GOTHAM_20 == 4
  /*Unicode: U+003c (<) , Width: 2 */
  0x00,  //.. 
  0x20,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+003e (>) , Width: 2 */
  0x00,  //.. 
  0x20,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0061 (a) , Width: 1 */
  0x00,  //. 
  0x40,  //+ 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0062 (b) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0063 (c) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0064 (d) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0065 (e) , Width: 2 */
  0x00,  //.. 
  0x40,  //+. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0066 (f) , Width: 1 */
  0x10,  //. 
  0x20,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0067 (g) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x10,  //.. 
  0x00,  //.. 


  /*Unicode: U+0068 (h) , Width: 2 */
  0x00,  //.. 
  0x30,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0069 (i) , Width: 1 */
  0x00,  //. 
  0x10,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006a (j) , Width: 1 */
  0x00,  //. 
  0x10,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006b (k) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006c (l) , Width: 1 */
  0x00,  //. 
  0x10,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006d (m) , Width: 2 */
  0x00,  //.. 
  0x32,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006e (n) , Width: 2 */
  0x00,  //.. 
  0x30,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+006f (o) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0070 (p) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0071 (q) , Width: 2 */
  0x00,  //.. 
  0x31,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0072 (r) , Width: 1 */
  0x00,  //. 
  0x20,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0073 (s) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0074 (t) , Width: 1 */
  0x00,  //. 
  0x20,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0075 (u) , Width: 2 */
  0x00,  //.. 
  0x30,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0076 (v) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0077 (w) , Width: 2 */
  0x00,  //.. 
  0x32,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0078 (x) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0079 (y) , Width: 2 */
  0x00,  //.. 
  0x30,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+007a (z) , Width: 1 */
  0x00,  //. 
  0x30,  //. 
  0x00,  //. 
  0x00,  //. 



#elif USE_GOTHAM_20 == 8
  /*Unicode: U+003c (<) , Width: 2 */
  0x06, 0x00,  //.. 
  0x26, 0x02,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+003e (>) , Width: 2 */
  0x08, 0x00,  //.. 
  0x22, 0x04,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0061 (a) , Width: 1 */
  0x00,  //. 
  0x4c,  //+ 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0062 (b) , Width: 2 */
  0x0c, 0x00,  //.. 
  0x3c, 0x14,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0063 (c) , Width: 1 */
  0x02,  //. 
  0x32,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0064 (d) , Width: 2 */
  0x02, 0x0c,  //.. 
  0x34, 0x1e,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0065 (e) , Width: 2 */
  0x02, 0x00,  //.. 
  0x4a, 0x02,  //+. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0066 (f) , Width: 1 */
  0x12,  //. 
  0x20,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0067 (g) , Width: 2 */
  0x02, 0x00,  //.. 
  0x30, 0x1e,  //.. 
  0x14, 0x02,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0068 (h) , Width: 2 */
  0x0c, 0x00,  //.. 
  0x32, 0x0c,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0069 (i) , Width: 1 */
  0x04,  //. 
  0x16,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006a (j) , Width: 1 */
  0x04,  //. 
  0x16,  //. 
  0x0a,  //. 
  0x00,  //. 


  /*Unicode: U+006b (k) , Width: 1 */
  0x0a,  //. 
  0x3c,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006c (l) , Width: 1 */
  0x0a,  //. 
  0x16,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+006d (m) , Width: 2 */
  0x02, 0x02,  //.. 
  0x3e, 0x28,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+006e (n) , Width: 2 */
  0x02, 0x00,  //.. 
  0x32, 0x0c,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+006f (o) , Width: 2 */
  0x02, 0x00,  //.. 
  0x36, 0x10,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0070 (p) , Width: 2 */
  0x02, 0x00,  //.. 
  0x3c, 0x14,  //.. 
  0x08, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0071 (q) , Width: 2 */
  0x02, 0x00,  //.. 
  0x34, 0x1e,  //.. 
  0x00, 0x08,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0072 (r) , Width: 1 */
  0x00,  //. 
  0x26,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0073 (s) , Width: 1 */
  0x02,  //. 
  0x3c,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0074 (t) , Width: 1 */
  0x0a,  //. 
  0x2a,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0075 (u) , Width: 2 */
  0x00, 0x00,  //.. 
  0x36, 0x0a,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0076 (v) , Width: 1 */
  0x00,  //. 
  0x32,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0077 (w) , Width: 2 */
  0x00, 0x00,  //.. 
  0x38, 0x26,  //.. 
  0x00, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+0078 (x) , Width: 1 */
  0x00,  //. 
  0x36,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0079 (y) , Width: 2 */
  0x00, 0x00,  //.. 
  0x32, 0x02,  //.. 
  0x0c, 0x00,  //.. 
  0x00, 0x00,  //.. 


  /*Unicode: U+007a (z) , Width: 1 */
  0x02,  //. 
  0x3c,  //. 
  0x00,  //. 
  0x00,  //. 



#endif
};


/*Store the glyph descriptions*/
static const lv_font_glyph_dsc_t GOTHAM_20_glyph_dsc[] = 
{
#if USE_GOTHAM_20 == 1
  {.w_px = 2,	.glyph_index = 0},	/*Unicode: U+003c (<)*/
  {.w_px = 2,	.glyph_index = 4},	/*Unicode: U+003e (>)*/
  {.w_px = 1,	.glyph_index = 8},	/*Unicode: U+0061 (a)*/
  {.w_px = 2,	.glyph_index = 12},	/*Unicode: U+0062 (b)*/
  {.w_px = 1,	.glyph_index = 16},	/*Unicode: U+0063 (c)*/
  {.w_px = 2,	.glyph_index = 20},	/*Unicode: U+0064 (d)*/
  {.w_px = 2,	.glyph_index = 24},	/*Unicode: U+0065 (e)*/
  {.w_px = 1,	.glyph_index = 28},	/*Unicode: U+0066 (f)*/
  {.w_px = 2,	.glyph_index = 32},	/*Unicode: U+0067 (g)*/
  {.w_px = 2,	.glyph_index = 36},	/*Unicode: U+0068 (h)*/
  {.w_px = 1,	.glyph_index = 40},	/*Unicode: U+0069 (i)*/
  {.w_px = 1,	.glyph_index = 44},	/*Unicode: U+006a (j)*/
  {.w_px = 1,	.glyph_index = 48},	/*Unicode: U+006b (k)*/
  {.w_px = 1,	.glyph_index = 52},	/*Unicode: U+006c (l)*/
  {.w_px = 2,	.glyph_index = 56},	/*Unicode: U+006d (m)*/
  {.w_px = 2,	.glyph_index = 60},	/*Unicode: U+006e (n)*/
  {.w_px = 2,	.glyph_index = 64},	/*Unicode: U+006f (o)*/
  {.w_px = 2,	.glyph_index = 68},	/*Unicode: U+0070 (p)*/
  {.w_px = 2,	.glyph_index = 72},	/*Unicode: U+0071 (q)*/
  {.w_px = 1,	.glyph_index = 76},	/*Unicode: U+0072 (r)*/
  {.w_px = 1,	.glyph_index = 80},	/*Unicode: U+0073 (s)*/
  {.w_px = 1,	.glyph_index = 84},	/*Unicode: U+0074 (t)*/
  {.w_px = 2,	.glyph_index = 88},	/*Unicode: U+0075 (u)*/
  {.w_px = 1,	.glyph_index = 92},	/*Unicode: U+0076 (v)*/
  {.w_px = 2,	.glyph_index = 96},	/*Unicode: U+0077 (w)*/
  {.w_px = 1,	.glyph_index = 100},	/*Unicode: U+0078 (x)*/
  {.w_px = 2,	.glyph_index = 104},	/*Unicode: U+0079 (y)*/
  {.w_px = 1,	.glyph_index = 108},	/*Unicode: U+007a (z)*/

#elif USE_GOTHAM_20 == 2
  {.w_px = 2,	.glyph_index = 0},	/*Unicode: U+003c (<)*/
  {.w_px = 2,	.glyph_index = 4},	/*Unicode: U+003e (>)*/
  {.w_px = 1,	.glyph_index = 8},	/*Unicode: U+0061 (a)*/
  {.w_px = 2,	.glyph_index = 12},	/*Unicode: U+0062 (b)*/
  {.w_px = 1,	.glyph_index = 16},	/*Unicode: U+0063 (c)*/
  {.w_px = 2,	.glyph_index = 20},	/*Unicode: U+0064 (d)*/
  {.w_px = 2,	.glyph_index = 24},	/*Unicode: U+0065 (e)*/
  {.w_px = 1,	.glyph_index = 28},	/*Unicode: U+0066 (f)*/
  {.w_px = 2,	.glyph_index = 32},	/*Unicode: U+0067 (g)*/
  {.w_px = 2,	.glyph_index = 36},	/*Unicode: U+0068 (h)*/
  {.w_px = 1,	.glyph_index = 40},	/*Unicode: U+0069 (i)*/
  {.w_px = 1,	.glyph_index = 44},	/*Unicode: U+006a (j)*/
  {.w_px = 1,	.glyph_index = 48},	/*Unicode: U+006b (k)*/
  {.w_px = 1,	.glyph_index = 52},	/*Unicode: U+006c (l)*/
  {.w_px = 2,	.glyph_index = 56},	/*Unicode: U+006d (m)*/
  {.w_px = 2,	.glyph_index = 60},	/*Unicode: U+006e (n)*/
  {.w_px = 2,	.glyph_index = 64},	/*Unicode: U+006f (o)*/
  {.w_px = 2,	.glyph_index = 68},	/*Unicode: U+0070 (p)*/
  {.w_px = 2,	.glyph_index = 72},	/*Unicode: U+0071 (q)*/
  {.w_px = 1,	.glyph_index = 76},	/*Unicode: U+0072 (r)*/
  {.w_px = 1,	.glyph_index = 80},	/*Unicode: U+0073 (s)*/
  {.w_px = 1,	.glyph_index = 84},	/*Unicode: U+0074 (t)*/
  {.w_px = 2,	.glyph_index = 88},	/*Unicode: U+0075 (u)*/
  {.w_px = 1,	.glyph_index = 92},	/*Unicode: U+0076 (v)*/
  {.w_px = 2,	.glyph_index = 96},	/*Unicode: U+0077 (w)*/
  {.w_px = 1,	.glyph_index = 100},	/*Unicode: U+0078 (x)*/
  {.w_px = 2,	.glyph_index = 104},	/*Unicode: U+0079 (y)*/
  {.w_px = 1,	.glyph_index = 108},	/*Unicode: U+007a (z)*/

#elif USE_GOTHAM_20 == 4
  {.w_px = 2,	.glyph_index = 0},	/*Unicode: U+003c (<)*/
  {.w_px = 2,	.glyph_index = 4},	/*Unicode: U+003e (>)*/
  {.w_px = 1,	.glyph_index = 8},	/*Unicode: U+0061 (a)*/
  {.w_px = 2,	.glyph_index = 12},	/*Unicode: U+0062 (b)*/
  {.w_px = 1,	.glyph_index = 16},	/*Unicode: U+0063 (c)*/
  {.w_px = 2,	.glyph_index = 20},	/*Unicode: U+0064 (d)*/
  {.w_px = 2,	.glyph_index = 24},	/*Unicode: U+0065 (e)*/
  {.w_px = 1,	.glyph_index = 28},	/*Unicode: U+0066 (f)*/
  {.w_px = 2,	.glyph_index = 32},	/*Unicode: U+0067 (g)*/
  {.w_px = 2,	.glyph_index = 36},	/*Unicode: U+0068 (h)*/
  {.w_px = 1,	.glyph_index = 40},	/*Unicode: U+0069 (i)*/
  {.w_px = 1,	.glyph_index = 44},	/*Unicode: U+006a (j)*/
  {.w_px = 1,	.glyph_index = 48},	/*Unicode: U+006b (k)*/
  {.w_px = 1,	.glyph_index = 52},	/*Unicode: U+006c (l)*/
  {.w_px = 2,	.glyph_index = 56},	/*Unicode: U+006d (m)*/
  {.w_px = 2,	.glyph_index = 60},	/*Unicode: U+006e (n)*/
  {.w_px = 2,	.glyph_index = 64},	/*Unicode: U+006f (o)*/
  {.w_px = 2,	.glyph_index = 68},	/*Unicode: U+0070 (p)*/
  {.w_px = 2,	.glyph_index = 72},	/*Unicode: U+0071 (q)*/
  {.w_px = 1,	.glyph_index = 76},	/*Unicode: U+0072 (r)*/
  {.w_px = 1,	.glyph_index = 80},	/*Unicode: U+0073 (s)*/
  {.w_px = 1,	.glyph_index = 84},	/*Unicode: U+0074 (t)*/
  {.w_px = 2,	.glyph_index = 88},	/*Unicode: U+0075 (u)*/
  {.w_px = 1,	.glyph_index = 92},	/*Unicode: U+0076 (v)*/
  {.w_px = 2,	.glyph_index = 96},	/*Unicode: U+0077 (w)*/
  {.w_px = 1,	.glyph_index = 100},	/*Unicode: U+0078 (x)*/
  {.w_px = 2,	.glyph_index = 104},	/*Unicode: U+0079 (y)*/
  {.w_px = 1,	.glyph_index = 108},	/*Unicode: U+007a (z)*/

#elif USE_GOTHAM_20 == 8
  {.w_px = 2,	.glyph_index = 0},	/*Unicode: U+003c (<)*/
  {.w_px = 2,	.glyph_index = 8},	/*Unicode: U+003e (>)*/
  {.w_px = 1,	.glyph_index = 16},	/*Unicode: U+0061 (a)*/
  {.w_px = 2,	.glyph_index = 20},	/*Unicode: U+0062 (b)*/
  {.w_px = 1,	.glyph_index = 28},	/*Unicode: U+0063 (c)*/
  {.w_px = 2,	.glyph_index = 32},	/*Unicode: U+0064 (d)*/
  {.w_px = 2,	.glyph_index = 40},	/*Unicode: U+0065 (e)*/
  {.w_px = 1,	.glyph_index = 48},	/*Unicode: U+0066 (f)*/
  {.w_px = 2,	.glyph_index = 52},	/*Unicode: U+0067 (g)*/
  {.w_px = 2,	.glyph_index = 60},	/*Unicode: U+0068 (h)*/
  {.w_px = 1,	.glyph_index = 68},	/*Unicode: U+0069 (i)*/
  {.w_px = 1,	.glyph_index = 72},	/*Unicode: U+006a (j)*/
  {.w_px = 1,	.glyph_index = 76},	/*Unicode: U+006b (k)*/
  {.w_px = 1,	.glyph_index = 80},	/*Unicode: U+006c (l)*/
  {.w_px = 2,	.glyph_index = 84},	/*Unicode: U+006d (m)*/
  {.w_px = 2,	.glyph_index = 92},	/*Unicode: U+006e (n)*/
  {.w_px = 2,	.glyph_index = 100},	/*Unicode: U+006f (o)*/
  {.w_px = 2,	.glyph_index = 108},	/*Unicode: U+0070 (p)*/
  {.w_px = 2,	.glyph_index = 116},	/*Unicode: U+0071 (q)*/
  {.w_px = 1,	.glyph_index = 124},	/*Unicode: U+0072 (r)*/
  {.w_px = 1,	.glyph_index = 128},	/*Unicode: U+0073 (s)*/
  {.w_px = 1,	.glyph_index = 132},	/*Unicode: U+0074 (t)*/
  {.w_px = 2,	.glyph_index = 136},	/*Unicode: U+0075 (u)*/
  {.w_px = 1,	.glyph_index = 144},	/*Unicode: U+0076 (v)*/
  {.w_px = 2,	.glyph_index = 148},	/*Unicode: U+0077 (w)*/
  {.w_px = 1,	.glyph_index = 156},	/*Unicode: U+0078 (x)*/
  {.w_px = 2,	.glyph_index = 160},	/*Unicode: U+0079 (y)*/
  {.w_px = 1,	.glyph_index = 168},	/*Unicode: U+007a (z)*/

#endif
};

/*List of unicode characters*/
static const uint32_t GOTHAM_20_unicode_list[] = {
  60,	/*Unicode: U+003c (<)*/
  62,	/*Unicode: U+003e (>)*/
  97,	/*Unicode: U+0061 (a)*/
  98,	/*Unicode: U+0062 (b)*/
  99,	/*Unicode: U+0063 (c)*/
  100,	/*Unicode: U+0064 (d)*/
  101,	/*Unicode: U+0065 (e)*/
  102,	/*Unicode: U+0066 (f)*/
  103,	/*Unicode: U+0067 (g)*/
  104,	/*Unicode: U+0068 (h)*/
  105,	/*Unicode: U+0069 (i)*/
  106,	/*Unicode: U+006a (j)*/
  107,	/*Unicode: U+006b (k)*/
  108,	/*Unicode: U+006c (l)*/
  109,	/*Unicode: U+006d (m)*/
  110,	/*Unicode: U+006e (n)*/
  111,	/*Unicode: U+006f (o)*/
  112,	/*Unicode: U+0070 (p)*/
  113,	/*Unicode: U+0071 (q)*/
  114,	/*Unicode: U+0072 (r)*/
  115,	/*Unicode: U+0073 (s)*/
  116,	/*Unicode: U+0074 (t)*/
  117,	/*Unicode: U+0075 (u)*/
  118,	/*Unicode: U+0076 (v)*/
  119,	/*Unicode: U+0077 (w)*/
  120,	/*Unicode: U+0078 (x)*/
  121,	/*Unicode: U+0079 (y)*/
  122,	/*Unicode: U+007a (z)*/
  0,    /*End indicator*/
};

lv_font_t GOTHAM_20 = 
{
    .unicode_first = 60,	/*First Unicode letter in this font*/
    .unicode_last = 122,	/*Last Unicode letter in this font*/
    .h_px = 4,				/*Font height in pixels*/
    .glyph_bitmap = GOTHAM_20_glyph_bitmap,	/*Bitmap of glyphs*/
    .glyph_dsc = GOTHAM_20_glyph_dsc,		/*Description of glyphs*/
    .glyph_cnt = 28,			/*Number of glyphs in the font*/
    .unicode_list = GOTHAM_20_unicode_list,	/*List of unicode characters*/
    .get_bitmap = lv_font_get_bitmap_sparse,	/*Function pointer to get glyph's bitmap*/
    .get_width = lv_font_get_width_sparse,	/*Function pointer to get glyph's width*/
#if USE_GOTHAM_20 == 1
    .bpp = 1,				/*Bit per pixel*/
 #elif USE_GOTHAM_20 == 2
    .bpp = 2,				/*Bit per pixel*/
 #elif USE_GOTHAM_20 == 4
    .bpp = 4,				/*Bit per pixel*/
 #elif USE_GOTHAM_20 == 8
    .bpp = 8,				/*Bit per pixel*/
#endif
    .monospace = 0,				/*Fix width (0: if not used)*/
    .next_page = NULL,		/*Pointer to a font extension*/
};

#endif /*USE_GOTHAM_20*/
