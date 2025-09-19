#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "convert.c"

/* NOTICE
*  The assignment document is not very clear about how much of Assignment 1 to include in Assignment 2.
*  Erring on the side of caution, the entirety of Assignment 1's main.c is reproduced here (with some modifications).
*  The new tests are located above the old ones. Ctrl+F "ASSIGNMENT 2" for quick reference.
*/

int main() {
   // based on A1 Appendix A.1
   FILE *file = fopen("a2_test.txt", "r"); // open test file
   if (file == NULL) {
      printf("Error: Could not open file\n");
      return 1;
   }
   
   char line[256];
   int test_number = 0;
   int tests_passed = 0;
   while (fgets(line, sizeof(line), file) != NULL) { // read lines
      if (line[0] == '#') continue; // ignore comments
      if (line[0] == '\n') continue; // ignore empty lines
      line[strcspn(line, "\n")] = '\0'; // strip the newline character
      char func[64]; char arg1[64]; char arg2[64]; char arg3[64];
      strcpy(func, strtok(line, " ")); // first word is the function name
      strcpy(arg1, strtok(NULL, " ")); // second word is the first argument
      strcpy(arg2, strtok(NULL, " ")); // third word is the expected output (for a2_test.txt anyhow)
      char *token = strtok(NULL, " ");
      if (token != NULL) strcpy(arg3, token); // fourth word is the desired output (for a1's converts) or NULL (for a1's print_tables) // leaving this in just in case
      else arg3[0] = '\0'; // empty string
      // printf("got %s, %s, %s, %s\n", func, arg1, arg2, arg3); // sanity check
      
      // test time
      test_number++;
      char pass[] = "PASS";
      char fail[] = "FAIL";
      char test_result[5];

      // whole strings cannot be used in switches in C. for the sake of future-proofing, i replaced it with an if-else ladder.

      // every function i could make use myout i made use myout.
      char * myout = malloc(32+1); // prevents segmentation fault

      /* ASSIGNMENT 2 */
      if (strcmp(func, "oct_to_bin") == 0) {
         oct_to_bin(arg1, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: oct_to_bin(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      else if (strcmp(func, "oct_to_hex") == 0) {
         oct_to_hex(arg1, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: oct_to_hex(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      else if (strcmp(func, "hex_to_bin") == 0) {
         hex_to_bin(arg1, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: hex_to_bin(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      else if (strcmp(func, "to_sign_magnitude") == 0) {
         int32_t * myin = malloc(65);
         *myin = atoi(arg1); // another one of those magic functions that was hidden to me until i happened to see it
         to_sign_magnitude(myin, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: to_sign_magnitude(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      else if (strcmp(func, "to_ones_complement") == 0) {
         int32_t * myin = malloc(65);
         *myin = atoi(arg1);
         to_ones_complement(myin, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: to_ones_complement(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      else if (strcmp(func, "to_twos_complement") == 0) {
         int32_t * myin = malloc(65);
         *myin = atoi(arg1);
         to_twos_complement(myin, myout);
         if (strcmp(arg2, myout) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: to_twos_complement(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, arg1, arg2, myout, test_result);
      }
      
      /* ASSIGNMENT 1 */
      else if (strcmp(func, "sub_convert") == 0) {
         uint32_t sub_n = (uint32_t)strtoul(arg1, 0L, 10); // magic (converts string to uint32_t)
         uint32_t sub_base = (uint32_t)strtoul(arg2, 0L, 10);
         sub_convert(sub_n, sub_base, myout);
         if (strcmp(myout, arg3) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: sub_convert(%u, %d) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, sub_n, sub_base, arg3, myout, test_result);
      }
      else if (strcmp(func, "div_convert") == 0) {
         uint32_t div_n = (uint32_t)strtoul(arg1, 0L, 10);
         uint32_t div_base = (uint32_t)strtoul(arg2, 0L, 10);
         myout = malloc(32+1);
         div_convert(div_n, div_base, myout);
         if (strcmp(myout, arg3) == 0) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: div_convert(%u, %d) -> Expected: \"%s\", Got: \"%s\" [%s]\n", test_number, div_n, div_base, arg3, myout, test_result);
      }
      else if (strcmp(func, "print_tables") == 0) { // crucially this test does not need to verify the numbers... just the formatting
         uint32_t print_n = (uint32_t)strtoul(arg1, 0L, 10);
         freopen("temp.txt", "w", stdout); // direct stdout to temporary file so we don't print_tables() to terminal (and can do comparisons)
         print_tables(print_n);
         freopen("CON", "w", stdout); // incantation that restores terminal output
         // fopen within fopen
         FILE *tempfile = fopen("temp.txt", "r");
         if (tempfile == NULL) {
            printf("Error: Could not open temp.txt\n");
            return 1;
         }
         char templine[256]; // should be big enough
         int goodlines = 0; // the lines have to be in a specific order... need 3
         while (fgets(templine, sizeof(templine), tempfile) != NULL) {
            /*
            # Expected format for each operation (3 lines total):
            # Original: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
            # Left Shift by 3: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
            # AND with 0xFF: Binary=<binary> Octal=<octal> Decimal=<decimal> Hex=<hex>
            */
            
            // here's where i'd put my regex... if i had access to any
            // seriously i have no idea how to do that. do you know how many times i've typed #include <...> only for it to not exist
            templine[strcspn(templine, "\n")] = '\0'; // strip the newline character
            char header[64]; char binary[64]; char octal[64]; char decimal[64]; char hex[64];
            char *temptoken = strtok(templine, ":");
            if (temptoken == NULL) break; // no header, no pass
            strcpy(header, temptoken);
            temptoken = strtok(NULL, " ");
            if (temptoken == NULL) break; // no binary word, no pass
            strcpy(binary, temptoken);
            temptoken = strtok(NULL, " ");
            if (temptoken == NULL) break; // no octal word, no pass
            strcpy(octal, temptoken);
            temptoken = strtok(NULL, " ");
            if (temptoken == NULL) break; // no decimal word, no pass
            strcpy(decimal, temptoken);
            temptoken = strtok(NULL, " ");
            if (temptoken == NULL) break; // no hex word, no pass
            strcpy(hex, temptoken);
            
            // printf("got %s, %s, %s, %s, %s\n", header, binary, octal, decimal, hex); // sanity check
            
            // validate words
            bool header_sat = false; bool binary_sat = false; bool octal_sat = false; bool decimal_sat = false; bool hex_sat = false;
            if (strcmp(header, "Original") == 0 || strcmp(header, "Left Shift by 3") == 0 || strcmp(header, "AND with 0xFF") == 0) header_sat = true;
            if (strncmp(binary, "Binary=", 7) == 0) binary_sat = true;
            if (strncmp(octal, "Octal=", 6) == 0) octal_sat = true;
            if (strncmp(decimal, "Decimal=", 8) == 0) decimal_sat = true;
            if (strncmp(hex, "Hex=", 4) == 0) hex_sat = true;
            if (header_sat && binary_sat && octal_sat && decimal_sat && hex_sat) goodlines++;
            else break;
         }
         fclose(tempfile); // close temp.txt
         if (goodlines == 3) { strcpy(test_result, pass); tests_passed++; }
         else strcpy(test_result, fail);
         printf("Test %d: print_tables(%u) [%s]\n", test_number, print_n, test_result);
      }
   }
   fclose(file);
   remove("temp.txt"); // clean up temporary file
   printf("Summary: %d/%d tests passed", tests_passed, test_number);

   return 0;
}