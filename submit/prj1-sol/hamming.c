#include "hamming.h"

#include <assert.h>
#include <stdio.h>

/**
  All bitIndex'es are numbered starting at the LSB which is given index 1

  ** denotes exponentiation; note that 2**n == (1 << n)
*/

/** Return bit at bitIndex from word. */
static inline unsigned
get_bit(HammingWord word, int bitIndex)
{
  assert(bitIndex > 0);
  //@TODO
  return (word >> (bitIndex-1))%2;
}

/** Return word with bit at bitIndex in word set to bitValue. */
static inline HammingWord
set_bit(HammingWord word, int bitIndex, unsigned bitValue)
{
  assert(bitIndex > 0);
  assert(bitValue == 0 || bitValue == 1);
  //TODO
  if(bitValue != get_bit(word, bitIndex)){ word ^= 1 << (bitIndex-1);}
  return word;
}

/** Given a Hamming code with nParityBits, return 2**nParityBits - 1,
 *  i.e. the max # of bits in an encoded word (# data bits + # parity
 *  bits).
 */
static inline unsigned
get_n_encoded_bits(unsigned nParityBits)
{
  //@TODO
  return (1 << nParityBits)-1;
}

/** Return non-zero if bitIndex indexes a bit which will be used for a
 *  Hamming parity bit; i.e. the bit representation of bitIndex
 *  contains only a single 1.
 */
static inline int
is_parity_position(int bitIndex)
{
  assert(bitIndex > 0);
  //@TODO
  for(unsigned power = 1; power > 0; power <<= 1){
	if(power == bitIndex){return 1;}
	if(power > bitIndex){return 0;}
  }
  return 0;
}

/** Return the parity over the data bits in word specified by the
 *  parity bit bitIndex.  The word contains a total of nBits bits.
 *  Equivalently, return parity over all data bits whose bit-index has
 *  a 1 in the same position as in bitIndex.
 */
static int
compute_parity(HammingWord word, int bitIndex, unsigned nBits)
{
  assert(bitIndex > 0);
  //@TODO
  int ret = 0; 
  for(unsigned i = 1; i <= nBits; i++){
    if(!is_parity_position(i) && (i | bitIndex) == i){
	ret ^= get_bit(word, i);	
    }
  }
  return ret;
}

/** Encode data using nParityBits Hamming code parity bits.
 *  Assumes data is within range of values which can be encoded using
 *  nParityBits.
 */
HammingWord
hamming_encode(HammingWord data, unsigned nParityBits)	// 111 3
{
  //@TODO
  HammingWord encoded = 0;
  unsigned size = get_n_encoded_bits(nParityBits); //7
  int counter = 1;
  for(unsigned i = 1; i <= size; i++){
    if(is_parity_position(i) == 0){
        encoded = set_bit(encoded, i, get_bit(data, counter));
	counter++;
    }
  }
  for(unsigned i = 1; i <= size; i++){
    if(is_parity_position(i) == 1){
	encoded = set_bit(encoded, i, compute_parity(encoded, i, size));
    }
  }
  return encoded;
}

/** Decode encoded using nParityBits Hamming code parity bits.
 *  Set *hasError if an error was corrected.
 *  Assumes that data is within range of values which can be decoded
 *  using nParityBits.
 */
HammingWord
hamming_decode(HammingWord encoded, unsigned nParityBits,
                           int *hasError)
{
  //@TODO
  unsigned syndrome = 0;
  HammingWord data = 0;
  unsigned counter = 1;
  unsigned size = get_n_encoded_bits(nParityBits);
 	 //printf("%llu, ", encoded); 
 	 for(int i = 0; i < nParityBits; i++){
		unsigned parityBit = 1<<i;
			if(get_bit(encoded, parityBit) != compute_parity(encoded, parityBit, size + nParityBits)){
				syndrome |= parityBit;
			}	
		//printf(" syn: %u @ bit: %u", syndrome, i);
  	}
	printf("%u, ", syndrome);
  	if(syndrome > 0){
		*hasError = 1; 
		unsigned errorBit = get_bit(encoded, syndrome);
		if(errorBit == 0) encoded = set_bit(encoded, syndrome, 1);
		else encoded = set_bit(encoded, syndrome, 0);
		syndrome = 0;
	}
//	printf("%llu, ", encoded);
	for(int i = 1; i <= size; i++){
		if(!is_parity_position(i)){
			data = set_bit(data, counter, get_bit(encoded, i));
			counter++;
		}	
	}
  
  return data;
}
