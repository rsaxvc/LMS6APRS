/*
This file from https://github.com/codebrainz/morse under the following license:

The MIT License

Copyright (c) 2012 Matthew Brush <mbrush@codebrainz.ca>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "morse.h"

#define MORSE_CHAR_BASE 33

const char * const MORSE_CODES[] = {
  "-.-.--", /* 33: EXCLAMATION_MARK */
  ".-..-.", /* 34: DOUBLE_QUOTE */
  "",       /* 35: NOT USED */
  "...-..-",/* 36: DOLLAR_SIGN */
  "",       /* 37: NOT USED */
  ".-...",  /* 38: AMPERSAND */
  ".----.", /* 39: APOSTROPHE */
  "-.--.-", /* 40: OPEN_PAREN */
  "-.--.-", /* 41: CLOSE_PAREN */
  "",       /* 42: NOT USED */
  ".-.-.",  /* 43: PLUS */
  "--..--", /* 44: COMMA */
  "-....-", /* 45: MINUS */
  ".-.-.-", /* 46: PERIOD */
  "-..-.",  /* 47: SLASH */
  "-----",  /* 48: 0 */
  ".----",  /* 49: 1 */
  "..---",  /* 50: 2 */
  "...--",  /* 51: 3 */
  "....-",  /* 52: 4 */
  ".....",  /* 53: 5 */
  "-....",  /* 54: 6 */
  "--...",  /* 55: 7 */
  "---..",  /* 56: 8 */
  "----.",  /* 57: 9 */
  "---...", /* 58: COLON */
  "-.-.-.", /* 59: SEMI_COLON */
  "",       /* 60: NOT USED */
  "-...-",  /* 61: EQUALS */
  "",       /* 62: NOT USED */
  "..--..", /* 63: QUESTION_MARK */
  ".--.-.", /* 64: AT_SIGN */
  ".-",     /* 65: A */
  "-...",   /* 66: B */
  "-.-.",   /* 67: C */
  "-..",    /* 68: D */
  ".",      /* 69: E */
  "..-.",   /* 70: F */
  "--.",    /* 71: G */
  "....",   /* 72: H */
  "..",     /* 73: I */
  ".---",   /* 74: J */
  "-.-",    /* 75: K */
  ".-..",   /* 76: L */
  "--",     /* 77: M */
  "-.",     /* 78: N */
  "---",    /* 79: O */
  ".--.",   /* 80: P */
  "--.-",   /* 81: Q */
  ".-.",    /* 82: R */
  "...",    /* 83: S */
  "-",      /* 84: T */
  "..-",    /* 85: U */
  "...-",   /* 86: V */
  ".--",    /* 87: W */
  "-..-",   /* 88: X */
  "-.--",   /* 89: Y */
  "--..",   /* 90: Z */
  "",       /* 91: NOT USED */
  "",       /* 92: NOT USED */
  "",       /* 93: NOT USED */
  "",       /* 94: NOT USED */
  "..--.-", /* 95: UNDERSCORE */
  "",       /* 96: NOT USED */
  ".-",     /* 97: A */
  "-...",   /* 98: B */
  "-.-.",   /* 99: C */
  "-..",    /* 100: D */
  ".",      /* 101: E */
  "..-.",   /* 102: F */
  "--.",    /* 103: G */
  "....",   /* 104: H */
  "..",     /* 105: I */
  ".---",   /* 106: J */
  "-.-",    /* 107: K */
  ".-..",   /* 108: L */
  "--",     /* 109: M */
  "-.",     /* 110: N */
  "---",    /* 111: O */
  ".--.",   /* 112: P */
  "--.-",   /* 113: Q */
  ".-.",    /* 114: R */
  "...",    /* 115: S */
  "-",      /* 116: T */
  "..-",    /* 117: U */
  "...-",   /* 118: V */
  ".--",    /* 119: W */
  "-..-",   /* 120: X */
  "-.--",   /* 121: Y */
  "--..",   /* 122: Z */
};

static char morse_code_is_char(char asc_char)
{
	unsigned char index = (unsigned char)asc_char;
	if( index < MORSE_CHAR_BASE )
		{
		return 0;
		}
	index -= MORSE_CHAR_BASE;
	if( index > sizeof(MORSE_CODES) / sizeof(MORSE_CODES[0]) )
		{
		return 0;
		}
	return (char)(MORSE_CODES[index][0] != '\0');
}

const char *morse_code_from_asc(char asc_char)
{
  if (!morse_code_is_char(asc_char))
    return NULL;
  else
    return MORSE_CODES[(unsigned int) asc_char];
}
