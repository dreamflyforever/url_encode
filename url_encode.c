#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <uuid/uuid.h>

/* Converts a hex character to its integer value */
char from_hex(char ch)
{
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code)
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str)
{
	char *pstr = str;
	char *buf = malloc(strlen(str) * 3 + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) ||
				*pstr == '-' ||
				*pstr == '_' ||
				*pstr == '.' ||
				*pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ') 
			*pbuf++ = '+';
		else 
			*pbuf++ = '%',
				*pbuf++ = to_hex(*pstr >> 4),
				*pbuf++ = to_hex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str)
{
	char *pstr = str;
	char *buf = malloc(strlen(str) + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4
					| from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}
/**
 * Create random UUID
 *
 * @param buf - buffer to be filled with the uuid string
 */
char *random_uuid(char buf[37])
{
	const char *c = "89ab";
	char *p = buf;
	int n;

	for(n = 0; n < 16; ++n) {
		int b = rand()%255;
		switch( n )
		{
		case 6:
			sprintf(p, "4%x", b%15 );
			break;
		case 8:
			sprintf(p, "%c%x", c[rand()%strlen(c)], b%15);
			break;
		default:
			sprintf(p, "%02x", b);
			break;
		}

		p += 2;

		switch( n )
		{
		case 3:
		case 5:
		case 7:
		case 9:
			*p++ = '-';
			break;
		default:
			break;
		}
	}
	*p = 0;
	return buf;
}

int main()
{
	char *url = url_encode("世界你好！");
	printf("%s\n", url);
	free(url);
	return 0;
}
