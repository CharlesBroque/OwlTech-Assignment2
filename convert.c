#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* NOTICE
*  The assignment document is not very clear about how much of Assignment 1 to include in Assignment 2.
*  Erring on the side of caution, the entirety of Assignment 1's convert.c is reproduced here.
*  Scroll down or Ctrl+F "ASSIGNMENT 2" to find the new code.
*/

/* ASSIGNMENT 1 */
// provided debug macro
# define DEBUG(msg, val) printf("[DEBUG] %s = %u\n", msg, val)

void div_convert(uint32_t n, int base, char *out) {
   // beginning with provided skeleton
   char temp[65]; // buffer
   int pos = 0;

   // zero case
   if (n == 0) {
      strcpy(out, "0");
      return;
   }

   // collect digits in reverse order
   while (n > 0) {
      int remainder = n % base;
      n = n / base;

      // digit to character
      if (remainder < 10)
         temp[pos++] = '0' + remainder;
      else
         temp[pos++] = 'A' + (remainder - 10);
   }
   temp[pos] = '\0'; // saves a lot of headaches later

   // reverse the result (build string)
   // actually since we're using <string.h> anyway
   // i think strrev(temp) would have worked
   // whatever
   char result[33]; // empty string without terminator
   result[pos] = '\0'; // terminate string
   int pos2 = 0;
   while (pos > 0) {
      pos--;
      result[pos2] = temp[pos];
      pos2++;
   }
   strcpy(out, result);
}

void sub_convert(uint32_t n, int base, char *out) {
   // "find highest power, subtract multiples"
   
   if (n == 0) {
    strcpy(out, "0");
    return;
   }

   // populate powers table
   uint32_t p = 1;
   /* EXPLANATION
   *  this weird hack with the 1e-13 (see below) prevents a precision error.
   *  it's easy to see the problem. (int)(log(1000)/log(10)) = 2 instead of 3 like you would expect.
   *  the maximum error this introduces to recovering the argument is of 10^-4 order
   *  within the uint32_t limit for bases 2-16 and is still of just 10^-3 order for bases up to 256.
   *  the trick may be extendable with a smaller constant, e.g. 1e-14, but i am worried about
   *  pathological behavior near the precision limit.
   */
   double max_power = (log(4294967295) + 1e-13)/log(base);
   uint32_t powers[32]; // max uint32_t is 2 ** 32 - 1
   for (int i = 0; i < max_power; i++) {
      powers[i] = p;
      p *= base;
   }

   // algo time
   int pos = 0; // track position in number
   int pow = (int)((log(n) + 1e-13)/log(base)); // opposite of position, kind of. will decrement to 0
   int bigpower = pow; // the largest power less than n
   int subcount = 0; // counts multiples subtracted
   char result[33]; // will store result
   // prefill result with 0 (except terminator)
   for (int i = 0; i < 33; i++) result[i] = '0';
   result[33] = '\0'; // just in case
   while (n != 0) {
      // find biggest power less than n
      bigpower = (int)((log(n) + 1e-13)/log(base));
      // if we would do no subtractions, skip ahead
      if (bigpower < pow) {
         pos += (pow - bigpower);
         pow = bigpower;
      }
      // repeatedly subtract biggest power
      subcount = 0;
      while (n >= powers[pow]) {
         n -= powers[pow];
         subcount++;
      }
      // digit to character
      if (subcount < 10)
         result[pos++] = '0' + subcount;
      else
         result[pos++] = 'A' + (subcount - 10);
      // decrement pow
      pow--;
   }
   // is pow still bigger than 0? add some place value padding
   while (pow >= 0) {
      result[pos++] = '0';
      pow--;
   }
   result[pos] = '\0'; // terminate result string
   strcpy(out, result);
}

void print_tables(uint32_t n) {
   /*
   # Expected format for each operation (3 lines total):
   # Original: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
   # Left Shift by 3: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
   # AND with 0xFF: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
   */

   // there's no *out. i guess we're doing printf().
   uint32_t original = n;
   uint32_t lshift3 = n << 3;
   uint32_t and0xFF = n & 0xFF;
   char* mybin = malloc(32+1);
   char* myoct = malloc(32+1);
   char* mydec = malloc(32+1);
   char* myhex = malloc(32+1);
   // i suspect div_convert is faster.
   div_convert(original, 2, mybin); div_convert(original, 8, myoct); div_convert(original, 10, mydec); div_convert(original, 16, myhex);
   printf("Original: Binary=%s Octal=%s Decimal=%s Hex=%s\n", mybin, myoct, mydec, myhex);
   div_convert(lshift3, 2, mybin); div_convert(lshift3, 8, myoct); div_convert(lshift3, 10, mydec); div_convert(lshift3, 16, myhex);
   printf("Left Shift by 3: Binary=%s Octal=%s Decimal=%s Hex=%s\n", mybin, myoct, mydec, myhex);
   div_convert(and0xFF, 2, mybin); div_convert(and0xFF, 8, myoct); div_convert(and0xFF, 10, mydec); div_convert(and0xFF, 16, myhex);
   printf("AND with 0xFF: Binary=%s Octal=%s Decimal=%s Hex=%s", mybin, myoct, mydec, myhex);
}

/* ASSIGNMENT 2 */
void oct_to_bin(const char *oct, char *out) {
   // "Convert octal to binary using 3-bit mapping"

   // input is at most 11 characters for 32-bit number, need 3 times this for binary
   char buffer[40] = ""; // i THINK this is safe.

   // for each octal digit, put three binary digits in the buffer
   for (int i = 0; i < strlen(oct); i++) {
      char bin_digits[] = "000";
      int octal_digit = oct[i] - '0'; // from Section 3.3
      if ((octal_digit & 4) == 4) bin_digits[0] = '1'; // & is lower priority than == ???
      if ((octal_digit & 2) == 2) bin_digits[1] = '1';
      if ((octal_digit & 1) == 1) bin_digits[2] = '1';
      strcat(buffer, bin_digits);
   }

   strcpy(out, buffer);
}

void oct_to_hex(const char *oct, char *out) {
   // "Convert octal to hexadecimal via binary"
   // because we need binary as an intermediary, i think it is fair game to invoke oct_to_bin here.
   char * tempbin2 = malloc(41);
   oct_to_bin(oct, tempbin2); // now tempbin holds the binary representation of oct.
   char tempbin[41] = "";
   if (strchr(tempbin2, '1') == NULL) { // if it's all zeroes...
      strcpy(out, "0"); // just output 0.
      return;
   }
   strcpy(tempbin, strchr(tempbin2, '1')); // otherwise, shave off the leading zeroes.
   
   char buffer[12] = ""; // hex representation is at most 8 characters for 32-bit number, extra padding included just in case
   
   // group by 4
   int h = 0;
   int bonus = strlen(tempbin) % 4; // number of "extra" digits at head of tempbin
   // group loop
   for (int i = 0; i < strlen(tempbin); i++) {
      int power = ((-1 * i) - (4 - bonus)) % 4 + 3; // counts down from 3 to 0 repeatedly, beginning at bonus - 1 if bonus is positive
      if (tempbin[i] == '1') h += pow(2., power);
      // printf("h = %d, power = %d\n", h, power); // sanity check
      if (power == 0) {
         // add digit to buffer
         char hex_digit[2] = "";
         if (h < 10) hex_digit[0] = '0' + h;
         else hex_digit[0] = 'A' + h - 10;
         strcat(buffer, hex_digit);
         h = 0; // reset h
      }
   }

   strcpy(out, buffer);
}

void hex_to_bin(const char *hex, char *out) {
   // "Convert hexadecimal to binary using 4-bit mapping"
   // this is a lot like oct_to_bin, but with a little more processing involved.
   char buffer[64] = ""; // output will be at most 32 characters for a 32-bit integer, but you never know.

   // for each hex digit, put four binary digits in the buffer
   for (int i = 0; i < strlen(hex); i++) {
      char bin_digits[] = "0000";
      int hex_digit = 0;
      if (hex[i] - '0' < 10) hex_digit = hex[i] - '0';
      else if (hex[i] - 'A' < 6) hex_digit = hex[i] - 'A' + 10;
      else if (hex[i] - 'a' < 6) hex_digit = hex[i] - 'a' + 10;
      else {
         strcat(buffer, "x__x"); // will look obviously wrong in output if it happens (it should not happen)
         continue;
      }
      if ((hex_digit & 8) == 8) bin_digits[0] = '1'; // & is lower priority than == (i guess because of boolean comparison)
      if ((hex_digit & 4) == 4) bin_digits[1] = '1';
      if ((hex_digit & 2) == 2) bin_digits[2] = '1';
      if ((hex_digit & 1) == 1) bin_digits[3] = '1';
      strcat(buffer, bin_digits);
   }

   strcpy(out, buffer);
}

void to_sign_magnitude(int32_t *n, char *out) {
   // there is no skeleton provided for these next functions.
   // given that i have to convert decimal to binary,
   // i am going to invoke div_convert rather than redo that whole mess from scratch.
   char * bin = malloc(65);

   if (*n == -2147483648) { // although this is an acceptable int32_t, it's not welcome here
      strcpy(out, "this number isn't representable in 32 bits of sign-magnitude form."); // it'd be 33 bits long
      return;
   }

   char buffer[33] = ""; // 32 bit register + terminator
   if (*n >= 0) {
      div_convert(*n, 2, bin); // put binary conversion in bin
      // front-pad with extra bits
      for (int i = 0; i < 32 - strlen(bin); i++) buffer[i] = '0';
      strcat(buffer, bin);
   }
   else {
      div_convert(*n * -1, 2, bin); // put binary conversion in bin
      buffer[0] = '1'; // sign bit
      // front-pad with extra bits
      for (int i = 1; i < 32 - strlen(bin); i++) buffer[i] = '0';
      strcat(buffer, bin);
   }

   strcpy(out, buffer);
}

void to_ones_complement(int32_t *n, char *out) {
   char * bin = malloc(65);

   if (*n == -2147483648) { // although this is an acceptable int32_t, it's not welcome here
      strcpy(out, "this number isn't representable in 32 bits of one's complement form."); // it'd be 33 bits long
      return;
   }

   char buffer[33] = ""; // 32 bit register + terminator
   if (*n >= 0) div_convert(*n, 2, bin); // put positive binary conversion in bin
   else div_convert(*n * -1, 2, bin); // either way, the same way
   // front-pad with extra bits
   for (int i = 0; i < 32 - strlen(bin); i++) buffer[i] = '0';
   strcat(buffer, bin);
   if (*n < 0) { // negative case
      for (int i = 0; i < 32; i++) { // for each bit
         if (buffer[i] == '0') buffer[i] = '1'; // if it's 0, make it 1
         else buffer[i] = '0'; // if it's not 0, it's 1, so make it 0
      }
   }

   strcpy(out, buffer);
}

void to_twos_complement(int32_t *n, char *out) {
   char * bin = malloc(65);

   // no issues with -2^31 this time

   char buffer[33] = ""; // 32 bit register + terminator
   div_convert(*n, 2, bin); // perhaps ironically this needs no additional help whatsoever to get the right answer
   // front-pad with extra bits
   for (int i = 0; i < 32 - strlen(bin); i++) buffer[i] = '0';
   strcat(buffer, bin);

   strcpy(out, buffer);
}