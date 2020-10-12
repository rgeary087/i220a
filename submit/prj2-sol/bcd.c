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
  if(i > MAX_BCD_DIGITS && error != NULL){
    *error = OVERFLOW_ERR;
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
  Bcd i = 1;
  while(bcd > 0){
    Bcd temp = get_bcd_digit(bcd, 0);
    if(error != NULL && temp > 9){ *error = BAD_VALUE_ERR; break;}
    ret	+= temp*(i);
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
  Binary ret = 0;
  *p = s;
  unsigned i = 0;
  for(;s[i] != '\0' ; i++){	  
      if(s[i] >= '0' && s[i] <= '9'){   
	ret *= 10;
        ret += s[i] - '0';
	++*p;
      }else{
	break;
      }
  }
  if(i > MAX_BCD_DIGITS && error != NULL){ *error = OVERFLOW_ERR;}
  return binary_to_bcd(ret, error);
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
  for(int i = 0; i < bufSize; i++){
    if(get_bcd_digit(bcd, i) > 9 && error != NULL){*error = BAD_VALUE_ERR;}
  }
  if(bufSize < BCD_BUF_SIZE && error != NULL){
    *error = OVERFLOW_ERR;
  }
    
  	
  return snprintf(buf, bufSize, "%" BCD_FORMAT_MODIFIER "x", bcd);
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
  unsigned carry = 0;
  Bcd ret = 0;
  for(unsigned i = 0; i < MAX_BCD_DIGITS; i++){
    if(error != NULL && (get_bcd_digit(x, i) > 9 || get_bcd_digit(y,i) > 9)){ *error = BAD_VALUE_ERR;}
    Bcd temp = get_bcd_digit(x, i) + get_bcd_digit(y,i) + carry;
    ret = (temp >= 10 ? set_bcd_digit(ret, temp - 10 , i):set_bcd_digit(ret, temp, i));
    if(temp >= 10){carry = 1;}   
    else{carry = 0;}
  }
  if(carry > 0 && *error == 0){*error = OVERFLOW_ERR;}
  return ret;
}

/** Return the BCD representation of the product of BCD int's x and y.
 *
 * If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 * contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 * overflow, otherwise *error is unchanged.
 */

static Bcd
bcd_multiply_digit(Bcd multi, unsigned bcdDig, BcdError *error){
  Bcd ret = 0;
  for(int i = i; i < bcdDig; i++){
    ret = bcd_add(ret, multi, error);
  }
  return ret;
}
Bcd
bcd_multiply(Bcd x, Bcd y, BcdError *error)
{
  int counter = 0;
  Bcd ret = 0;
  Bcd ex = 1;
  for(int i = 0; i < MAX_BCD_DIGITS; i++){
    if(*error == 0 && (get_bcd_digit(x,i) > 9 || get_bcd_digit(y,i) > 9)){ *error = BAD_VALUE_ERR;} 
  }
  Bcd temp = y;
  while(counter < MAX_BCD_DIGITS){
    int val = get_bcd_digit(temp, counter);
    Bcd mult = bcd_multiply_digit(x, val, error);
    ret = bcd_add(ret, mult*ex, error);
    ex*=10;
    counter++;
  }
  return ret;
}
