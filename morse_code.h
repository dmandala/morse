/*
PARIS is 50 dot units long. 5 * PARIS is 5 WPM

Two types of timing: Standard where all timing is derived from the speed 
of a '.' and Farnsworth code where the letters are sent at 18 WPM but the
space between letters and words are stretched to the lower speed, above 
18 WPM Farnsworth and stadard are the same, no extra shifting.

This stretched code is called Farnsworth code.

1) The length of a dot is 1 time unit.
2) A dash is 3 time units.
3) The space between symbols (dots and dashes) of the same letter is 1 time unit.
4) The space between letters is 3 time units.
5) The space between words is 7 time units.
6) Words end with a space or /n

Unless using Farnsworth timing, where 
*/

/* For fast lookups, have made an array of strings that represent all the
 * morse char's by using the ascii letter to be sent as the array offset.
 * Then walk the string pointed to until hit the null byte.  That will send 
 * a single char in morse.  Put the same morse code in the upper and lower 
 * case of each letter since there is no case in morse code.
 *
 * A space or \n will trigger a word wait.
 *
 * Between each *- of a letter of morse code, send a single time unit of 
 * silence/no light.
 *
 * At the end of each letter send 3 time units of silence/no light to
 * signify end of char.
 * 
 * When the string is a space send 7 time units of silence/no light to show
 * end of word.
 *  
 */

/* WARNING WARNING This array of strings is position sensitive WARNING WARNING */
/* Do NOT make changes unless you fully understand what this table does.       */
char *morse_code[] = {
	"", "", "", "", "", "", "", "", "", "",
	" ",		/* use space instead of \n Use this for timing of words, this is not in Morse code */
	"", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "",
	" ",	 	/* space Use this for timing of words, this is not in Morse code */	
	"-.-.--",	/* !	Not in ITU-R recommendation */
	".-..-.",	/* " */	
	"",		/* #	Char not in Morse Code */
	"",		/* $	Char not in Morse Code */
	"",		/* %	Char not in Morse Code */
	".-...",	/* & */	
	".----.",	/* ' */	
	"-.--.",	/* ( */	
	"-.--.-",	/* ) */	
	"",		/* *	Char not in Morse Code */
	".-.-.",	/* + */	
	"--..--",	/* , */	
	"-....-",	/* - */	
	".-.-.-",	/* . */	
	"-..-.",	/* / */	
	"-----",	/* 0 */	
	".----",	/* 1 */	
	"..---",	/* 2 */	
	"...--",	/* 3 */	
	"....-",	/* 4 */	
	".....",	/* 5 */	
	"-....",	/* 6 */		
	"--...",	/* 7 */		
	"---..",	/* 8 */		
	"----.",	/* 9 */		
	"---...",	/* : */		
	"",		/* ;	Char not in Morse Code */
	"",		/* <	Char not in Morse Code */
	"-...-",	/* = */	
	"",		/* >	Char not in Morse Code */
	"..--..",	/* ? */	
	".--.-.",	/* @ */	
	".-",		/* A */	
	"-...",		/* B */	
	"-.-.",		/* C */	
	"-..",		/* D */	
	".",		/* E */	
	"..-.",		/* F */	
	"--.",		/* G */	
	"....",		/* H */	
	"..",		/* I */	
	".---",		/* J */	
	"-.-",		/* K */	
	".-..",		/* L */	
	"--",		/* M */	
	"-.",		/* N */	
	"---",		/* O */	
	".--.",		/* P */	
	"--.-",		/* Q */	
	".-.",		/* R */	
	"...",		/* S */	
	"-",		/* T */	
	"..-",		/* U */	
	"...-",		/* V */	
	".--",		/* W */	
	"-..-",		/* X */	
	"-.--",		/* Y */	
	"--..",		/* Z */	
	"",		/* [	Char not in Morse Code */
	"",		/* \	Char not in Morse Code */
	"",		/* ]	Char not in Morse Code */
	"",		/* ^	Char not in Morse Code */
	"",		/* _	Char not in Morse Code */
	"",		/* `	Char not in Morse Code */
	".-",		/* a */	
	"-...",		/* b */	
	"-.-.",		/* c */	
	"-..",		/* d */	
	".",		/* e */	
	"..-.",		/* f */	
	"--.",		/* g */	
	"....",		/* h */	
	"..",		/* i */	
	".---",		/* j */	
	"-.-",		/* k */	
	".-..",		/* l */	
	"--",		/* m */	
	"-.",		/* n */	
	"---",		/* o */	
	".--.",		/* p */	
	"--.-",		/* q */	
	".-.",		/* r */	
	"...",		/* s */	
	"-",		/* t */	
	"..-",		/* u */	
	"...-",		/* v */	
	".--",		/* w */	
	"-..-",		/* x */	
	"-.--",		/* y */	
	"--.."		/* z */	
	};// Nothing below in the ASCII table in Morse Code
	
