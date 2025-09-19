# CS 3503 Assignment 2 - Data Representation and Mapping

## Author
Dylan K.

## Description
Project for CS 3503. Converts numbers into different bases and displays the results.

## New Features
- specific functions for conversion from
  - octal to binary
  - octal to hex
  - hex to binary
- specific functions for converting decimal integers into
  - sign-magnitude binary
  - one's complement binary
  - two's complement binary
- new tests for new functions (although the old ones are still in there, just unused)

## Build Instructions
```Windows PowerShell
gcc -o BaseUtility main.c
.\BaseUtility.exe
```
Make sure you're not running it in a protected folder; it needs to make (and later delete) a temporary file in the middle of running.

## Test Results
51/51 tests passed

## Notes
Less conceptual overhead than the first time, thankfully. I think I am finally--finally--getting the barest handle on how to use pointers. I was also pleasantly surprised to see div_convert working out of the box on negative numbers. Something about the implicit conversion to being unsigned guarantees that it's equivalent to two's complement, which makes a lot of sense as far as why we use that one.
