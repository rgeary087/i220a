#include "bcd.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
/** Returns the digit of a bcd at a specified index
 *
 *(0x12, 1) -> 1
 */
Bcd
get_bcd_digit(Bcd value, unsigned index){
	/*value <<= (sizeof(value)-(index*4)-4);
	value>>=(sizeof(value)-4); // gets the 4 wanted bits that make up a Bcd digit to the 4 right-most bits
	return value;
	*/
	return (value >> (4*index)) & (Bcd)(0xf);
}

Bcd
set_bcd_digit(Bcd value, Bcd set, unsigned index){
	/*for(int i = 0; i < 4; i++){
		if(set%2 == 1){
			*value &= (1<<((index*4)+i)); 	
		}
		else{
			*value &= (~(1<<((index*4)+i)));
		}
		set>>=1;
	}*/
	Bcd mask = (Bcd)(0xf);
	value &= ~(mask<<(index*4));
	value |= (set<<(index*4));
	return value;	
}
/** Return BCD encoding of binary (which has normal binary representation).
 *
 *  Examples: binary_to_bcd(0xc) => 0x12;
 *            binary_to_bcd(0xff) => 0x255
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd
binary_to_bcd(Binary value, BcdError *error)
{
  Bcd ret = 0;
  int i = 0;
  while(value > 0){
    ret = set_bcd_digit(ret, value%10, i);
    value/=10;
    i++;
  }  
  return ret;
}

/** Return binary encoding of BCD value bcd.
 *
 *  Examples: bcd_to_binary(0x12) => 0xc;
 *            bcd_to_binary(0x255) => 0xff
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR if bcd contains
 *  a bad BCD digit.
 *  Cannot overflow since Binary can represent larger values than Bcd
 */
Binary
bcd_to_binary(Bcd bcd, BcdError *error)
{
  Binary ret = 0;
  int i = 1;
  while(bcd > 0){
    ret	+= get_bcd_digit(bcd, 0)*(i);
    i*=10;
    bcd >>= 4;
  }
  return ret;
}

/** Return BCD encoding of decimal number corresponding to string s.
 *  Behavior undefined on overflow or if s contains a non-digit
 *  character.  Sets *p to point to first non-digit char in s.
 *  Rougly equivalent to strtol().
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd
str_to_bcd(const char *s, const char **p, BcdError *error)
{
  //@TODO
  return 0;
}

/** Convert bcd to a NUL-terminated string in buf[] without any
 *  non-significant leading zeros.  Never write more than bufSize
 *  characters into buf.  The return value is the number of characters
 *  written (excluding the NUL character used to terminate strings).
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR is bcd contains
 *  a BCD digit which is greater than 9, OVERFLOW_ERR if bufSize bytes
 *  is less than BCD_BUF_SIZE, otherwise *error is unchanged.
 */
int
bcd_to_str(Bcd bcd, char buf[], size_t bufSize, BcdError *error)
{
  //@TODO
  return 0;
}

/** Return the BCD representation of the sum of BCD int's x and y.
 *
 *  If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 *  contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 *  overflow, otherwise *error is unchanged.
 */
Bcd
bcd_add(Bcd x, Bcd y, BcdError *error)
{
  //@TODO
  return 0;
}

/** Return the BCD representation of the product of BCD int's x and y.
 *
 * If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 * contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 * overflow, otherwise *error is unchanged.
 */
Bcd
bcd_multiply(Bcd x, Bcd y, BcdError *error)
{
  //@TODO
  return 0;
}
