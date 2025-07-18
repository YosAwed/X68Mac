/* 
 * Copyright (c) 2003,2008 NONAKA Kimihiro
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "common.h"
#include "joystick.h"
#include "prop.h"
#include "keyboard.h"
#include "mfp.h"
#include "windraw.h"

BYTE	KeyBufWP;
BYTE	KeyBufRP;
BYTE	KeyBuf[KeyBufSize];
BYTE	KeyEnable = 1;
BYTE	KeyIntFlag = 0;

struct keyboard_key kbd_key[] = {
#include "keytbl.inc"
};

extern BYTE traceflag;


void
Keyboard_Init(void)
{

	KeyBufWP = 0;
	KeyBufRP = 0;
	ZeroMemory(KeyBuf, KeyBufSize);
	KeyEnable = 1;
	KeyIntFlag = 0;
}

// ----------------------------------
//	てーぶる類
// ----------------------------------

#define	NC	0
#define KEYTABLE_MAX 512

//https://gamesx.com/wiki/doku.php?id=x68000:keycodes

BYTE KeyTable[KEYTABLE_MAX] = {
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x00
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	  BS, TAB,    ,    ,    , RET,    ,    		; 0x08
		0x0f,0x10,  NC,  NC,  NC,0x1d,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x10
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    , ESC,    ,    ,    ,    		; 0x18
		  NC,  NC,  NC,0x01,  NC,  NC,  NC,  NC,
	//	 SPC,  ! ,  " ,  # ,  $ ,  % ,  & ,  '		; 0x20
		0x35,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	//	  ( ,  ) ,  * ,  + ,  , ,  - ,  . ,  /		; 0x28
		0x09,0x0a,0x28,0x27,0x31,0x0c,0x32,0x33,
	//	  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7		; 0x30
		0x0b,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	//	  8 ,  9 ,  ; ,  : ,  < ,  = ,  > ,  ? 		; 0x38
		0x09,0x0a,0x28,0x27,0x31,0x0c,0x32,0x33,
	//	  @ ,  A ,  B ,  C ,  D ,  E ,  F ,  G		; 0x40
		0x1b,0x1e,0x2e,0x2c,0x20,0x13,0x21,0x22,
	//	  H ,  I ,  J ,  K ,  L ,  M ,  N ,  O		; 0x48
		0x23,0x18,0x24,0x25,0x26,0x30,0x2f,0x19,
	//	  P ,  Q ,  R ,  S ,  T ,  U ,  V ,  W		; 0x50
		0x1a,0x11,0x14,0x1f,0x15,0x17,0x2d,0x12,
	//	  X ,  Y ,  Z ,  [ ,  \ ,  ] ,  ^ ,  _		; 0x58
		0x2b,0x16,0x2a,0x1c,0x0e,0x29,0x0d,0x34,
	//	  ` ,  a ,  b ,  c ,  d ,  e ,  f ,  g		; 0x60
		0x1b,0x1e,0x2e,0x2c,0x20,0x13,0x21,0x22,
	//	  h ,  i ,  j ,  k ,  l ,  m ,  n ,  o		; 0x68
		0x23,0x18,0x24,0x25,0x26,0x30,0x2f,0x19,
	//	  p ,  q ,  r ,  s ,  t ,  u ,  v ,  w		; 0x70
		0x1a,0x11,0x14,0x1f,0x15,0x17,0x2d,0x12,
	//	  x ,  y ,  z ,  { ,  | ,  } ,  ~ ,   		; 0x78
		0x2b,0x16,0x2a,0x1c,0x0e,0x29,0x0d,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x80
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x88
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x90
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x98
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xa8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xb0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xb8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xc0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xc8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xd0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xd8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xe0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xe8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,tork, HELP	; 0xf8
		  NC,  NC,  NC,  NC,  NC,  NC,0x53, 0x54,

	//							;0x100

	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x00
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	  BS, TAB,  LF, CLR,    , RET,    ,   		; 0x08
		0x0f,0x10,0x1d,  NC,  NC,0x1d,  NC,  NC,
	//	    ,  ↑,  ↓,  →,  ←,SYSQ,    ,  		; 0x10
		  NC,0x3c,0x3e,0x3d,0x3b,  NC,  NC,  NC,
	//	    ,    ,    , ESC,    ,    ,    ,   		; 0x18
		  NC,  NC,0x63,0x01,  NC,  NC,  NC,  NC,
	//	    ,KANJ,MUHE,HENM,HENK,RONM,HIRA,KATA		; 0x20
		  NC,  NC,0x56,  NC,  NC,  NC,  NC,  NC,
	//	HIKA,ZENK,HANK,ZNHN,    ,KANA,    ,   		; 0x28
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	SFTL,    ,    ,    ,    ,    ,    ,    		; 0x30
		0x70,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,ZKOU,MKOU,   		; 0x38
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x40
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x48
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	HOME,  ←,  ↑,  →,  ↓,RLDN,RLUP, END		; 0x50
		0x36,0x3b,0x3c,0x3d,0x3e,0x39,0x38,0x3a,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x58
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,PRNT, INS,    ,    ,    ,    ,    		; 0x60
		  NC,  NC,0x5e,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,BREA,    ,    ,    ,   		; 0x68
		  NC,  NC,  NC,0x61,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x70
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x78
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	<SPC,    ,    ,    ,    ,    ,    ,    		; 0x80
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,<TAB,    ,    ,    ,<ENT,    ,  		; 0x88
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,<HOM,<←>,<↑>		; 0x90
		  NC,  NC,  NC,  NC,  NC,0x36,0x3b,0x3c,
	//	<→>,<↓>,<RDN,<RUP,<END,    ,<INS,<DEL		; 0x98
		0x3d,0x3e,0x39,0x38,0x3a,  NC,0x5e,0x37,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    , <*>, <+>, <,>, <->, <.>, </>		; 0xa8
		  NC,  NC,0x41,0x46,  NC,0x42,0x51,0x40,
	//	 <0>, <1>, <2>, <3>, <4>, <5>, <6>, <7>		; 0xb0
		0x4f,0x4b,0x4c,0x4d,0x47,0x48,0x49,0x43,
	//	 <8>, <9>,    ,    ,    ,    , f.1, f.2		; 0xb8
		0x44,0x45,  NC,  NC,  NC,  NC,0x63,0x64,
	//	 f.3, f.4, f.5, f.6, f.7, f.8, f.9,f.10		; 0xc0
		0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,
	//	f.11,f.12,f.13,f.14,f.15,    ,    ,   		; 0xc8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xd0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xd8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,SFTL,SFTR,CTLL,CTLR,CAPS,    ,METL		; 0xe0
		  NC,0x70,0x70,0x71,0x71,0x5d,  NC,0x55,
	//	METR,ALTL,ALTR,    ,    ,    ,    ,    		; 0xe8
		0x55,0x55,0x55,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    , DEL		; 0xf8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,0x37
};


BYTE KeyTableMaster[KEYTABLE_MAX] = {
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x00
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x08
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x10
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x18
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	 SPC,  ! ,  " ,  # ,  $ ,  % ,  & ,  '		; 0x20
		0x35,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	//	  ( ,  ) ,  * ,  + ,  , ,  - ,  . ,  /		; 0x28
		0x09,0x0a,0x28,0x27,0x31,0x0c,0x32,0x33,
	//	  0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7		; 0x30
		0x0b,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	//	  8 ,  9 ,  ; ,  : ,  < ,  = ,  > ,  ? 		; 0x38
		0x09,0x0a,0x28,0x27,0x31,0x0c,0x32,0x33,
	//	  @ ,  A ,  B ,  C ,  D ,  E ,  F ,  G		; 0x40
		0x1b,0x1e,0x2e,0x2c,0x20,0x13,0x21,0x22,
	//	  H ,  I ,  J ,  K ,  L ,  M ,  N ,  O		; 0x48
		0x23,0x18,0x24,0x25,0x26,0x30,0x2f,0x10,
	//	  P ,  Q ,  R ,  S ,  T ,  U ,  V ,  W		; 0x50
		0x1a,0x11,0x14,0x1f,0x15,0x17,0x2d,0x12,
	//	  X ,  Y ,  Z ,  [ ,  \ ,  ] ,  ^ ,  _		; 0x58
		0x2b,0x16,0x2a,0x1c,0x0e,0x29,0x0d,0x34,
	//	  ` ,  a ,  b ,  c ,  d ,  e ,  f ,  g		; 0x60
		0x1b,0x1e,0x2e,0x2c,0x20,0x13,0x21,0x22,
	//	  h ,  i ,  j ,  k ,  l ,  m ,  n ,  o		; 0x68
		0x23,0x18,0x24,0x25,0x26,0x30,0x2f,0x19,
	//	  p ,  q ,  r ,  s ,  t ,  u ,  v ,  w		; 0x70
		0x1a,0x11,0x14,0x1f,0x15,0x17,0x2d,0x12,
	//	  x ,  y ,  z ,  { ,  | ,  } ,  ~ ,   		; 0x78
		0x2b,0x16,0x2a,0x1c,0x0e,0x29,0x0d,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x80
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x88
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x90
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x98
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xa8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xb0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xb8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xc0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xc8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xd0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xd8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xe0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xe8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,

	//							;0x100

	//	    ,    ,    ,    ,    ,    ,    ,  		; 0x00
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	  BS, TAB,  LF, CLR,    , RET,    ,   		; 0x08
		0x0f,0x10,0x1d,  NC,  NC,0x1d,  NC,  NC,
	//	    ,    ,    ,PAUS,SCRL,SYSQ,    ,  		; 0x10
		  NC,  NC,  NC,0x61,  NC,  NC,  NC,  NC,
	//	    ,    ,    , ESC,    ,    ,    ,   		; 0x18
		  NC,  NC,0x63,0x01,  NC,  NC,  NC,  NC,
	//	    ,KANJ,MUHE,HENM,HENK,RONM,HIRA,KATA		; 0x20
		  NC,  NC,0x56,  NC,  NC,  NC,  NC,  NC,
	//	HIKA,ZENK,HANK,ZNHN,    ,KANA,    ,   		; 0x28
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	ALNU,    ,    ,    ,    ,    ,    ,    		; 0x30
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,ZKOU,MKOU,   		; 0x38
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x40
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x48
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	HOME,  ←,  ↑,  →,  ↓,RLDN,RLUP, END		; 0x50
		0x36,  NC,  NC,  NC,  NC,0x39,0x38,0x3a,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0x58
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,PRNT, INS,    ,    ,    ,    ,    		; 0x60
		  NC,  NC,0x5e,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,BREA,    ,    ,    ,   		; 0x68
		  NC,  NC,  NC,0x61,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x70
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0x78
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	<SPC,    ,    ,    ,    ,    ,    ,    		; 0x80
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,<TAB,    ,    ,    ,<ENT,    ,  		; 0x88
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,<HOM,<←>,<↑>		; 0x90
		  NC,  NC,  NC,  NC,  NC,0x36,  NC,  NC,
	//	<→>,<↓>,<RDN,<RUP,<END,    ,<INS,<DEL		; 0x98
		  NC,  NC,0x39,0x38,0x3a,  NC,0x5e,0x37,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xa0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    , <*>, <+>, <,>, <->, <.>, </>		; 0xa8
		  NC,  NC,0x41,0x46,  NC,0x42,  NC,0x40,
	//	 <0>, <1>, <2>, <3>, <4>, <5>, <6>, <7>		; 0xb0
		0x4f,0x4b,0x4c,0x4d,0x47,0x48,0x49,0x43,
	//	 <8>, <9>,    ,    ,    ,    , f.1, f.2		; 0xb8
		0x44,0x45,  NC,  NC,  NC,  NC,0x63,0x64,
	//	 f.3, f.4, f.5, f.6, f.7, f.8, f.9,f.10		; 0xc0
		0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,
	//	f.11,f.12,f.13,f.14,f.15,    ,    ,   		; 0xc8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xd0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,   		; 0xd8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,SFTL,SFTR,CTLL,CTLR,CAPS,    ,METL		; 0xe0
		  NC,0x70,0x70,0x71,0x71,0x5d,  NC,0x55,
	//	METR,ALTL,ALTR,    ,    ,    ,    ,    		; 0xe8
		0x55,0x55,0x55,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    ,    		; 0xf0
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,  NC,
	//	    ,    ,    ,    ,    ,    ,    , DEL		; 0xf8
		  NC,  NC,  NC,  NC,  NC,  NC,  NC,0x37
};

// P6K: PX68K_KEYBOARD
//      ~ ~   ~
#define P6K_UP 1
#define P6K_DOWN 2

void send_keycode(BYTE code, int flag)
{
	BYTE newwp;

	if (code != NC) {
		newwp = ((KeyBufWP + 1) & (KeyBufSize - 1));
		if (newwp != KeyBufRP) {
			KeyBuf[KeyBufWP] = code | ((flag == P6K_UP)? 0x80 : 0);
//			p6logd("KeyBuf: %x\n", KeyBuf[KeyBufWP]);
			KeyBufWP = newwp;
//			p6logd("KeyBufWP: %d\n", KeyBufWP);
		}
	}
}

static BYTE get_x68k_keycode(DWORD wp)
{
	if (wp < KEYTABLE_MAX) {
		return KeyTable[wp];
	}

    return 0xff;
}

// ----------------------------------
//	WM_KEYDOWN〜
// ----------------------------------
void
Keyboard_KeyDown(DWORD wp)
{

	BYTE code;

    code = get_x68k_keycode(wp);
	if (code < 0) {
		return;
	}

//	printf("Keyboard_KeyDown: ");
//	printf("wp=0x%x, code=0x%x\n", wp, code);

	send_keycode(code, P6K_DOWN);

}

// ----------------------------------
//	WM_KEYUP
// ----------------------------------
void
Keyboard_KeyUp(DWORD wp)
{
	BYTE code;
	code = get_x68k_keycode(wp);
	if (code < 0) {
		return;
	}
	send_keycode(code, P6K_UP);
}

// ----------------------------------
//	Key Check
//	1フレーム中に4回（2400bps/10bit/60fpsとすれば、だが）呼ばれて、MFPにデータを送る
// ----------------------------------

void
Keyboard_Int(void)
{
	if (KeyBufRP != KeyBufWP) {
//		printf("KeyBufRP:%d, KeyBufWP:%d\n", KeyBufRP, KeyBufWP);
		if (!KeyIntFlag) {
			LastKey = KeyBuf[KeyBufRP];
			KeyBufRP = ((KeyBufRP+1)&(KeyBufSize-1));
			KeyIntFlag = 1;
			MFP_Int(3);
		}
	} else if (!KeyIntFlag) {
		LastKey = 0;
	}
}


int Keyboard_IsSwKeyboard(void)
{
	return FALSE;
}
