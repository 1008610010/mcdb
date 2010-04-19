/* License: GPLv3 */

#ifndef MCDB_UINT32_H
#define MCDB_UINT32_H

#include <stdint.h>
#include <unistd.h>

uint32_t
mcdb_uint32_unpack(const char s[4]);

uint32_t
mcdb_uint32_unpack_big(const char s[4]);

void
mcdb_uint32_pack(char s[4], uint32_t);

void
mcdb_uint32_pack_big(char s[4], uint32_t);

/*(use macros only with simple args, or else better to call subroutines)*/

/* mcdb_uint32_unpack_macro(): string 's' must be unsigned char */
#define mcdb_uint32_unpack_macro(s) \
  ( (s)[0] | ((s)[1]<<8) | ((s)[2]<<16) | ((s)[3]<<24) )

/* mcdb_uint32_unpack_big_macro(): string 's' must be unsigned char */
#define mcdb_uint32_unpack_big_macro(s) \
  ( ((s)[0]<<24) | ((s)[1]<<16) | ((s)[2]<<8) | (s)[3] )

#define mcdb_uint32_pack_macro(s,u) \
  (s)[0]=u & 0xFF; (s)[1]=(u>>8) & 0xFF; (s)[2]=(u>>16) & 0xFF; (s)[3]=(u>>24)

#define mcdb_uint32_pack_big_macro(s,u) \
  (s)[0]=(u>>24); (s)[1]=(u>>16) & 0xFF; (s)[2]=(u>>8) & 0xFF; (s)[3]=u & 0xFF

#endif
