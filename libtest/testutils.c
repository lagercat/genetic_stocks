#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "testutils.h"

/*
 * FOR ALL METHODS BELOW:
 * ----------------------
 * These methods are utils for the tests. They are given a condition as
 * parameter, in the case of testbool, or two parameters (result and expected)
 * that will cause the test to pass when they are equal, and fail when they are
 * not.
 *
 * The purpose of having these functions is to standardise the format of local
 * testing in a way that both makes it easier to understand for the tester and
 * also reduces time taken to write the testsuites.
 */

void testbool(bool condition, char *testname) {
    printf("Test, '%s': %s\n", testname, condition ? "PASS" : "FAIL");
}

void testint(int result, int expected, char *testname) {
    printf("Test, '%s', (Expected='%d', Got='%d'): %s\n", testname, expected,
           result, (result == expected) ? "PASS" : "FAIL");
}

void testlong(long result, long expected, char *testname) {
    printf("Test, '%s', (Expected='%ld', Got='%ld'): %s\n", testname, expected,
           result, (result == expected) ? "PASS" : "FAIL");
}

void testdouble(double result, double expected, char *testname) {
    printf("Test, '%s', (Expected='%g', Got='%g'): %s\n", testname, expected,
           result, (result == expected) ? "PASS" : "FAIL");
}

void teststring(char *result, char *expected, char *testname) {
    printf("Test, '%s', (Expected='%s', Got='%s'): %s\n", testname, expected,
           result, (strcmp(result, expected) == 0) ? "PASS" : "FAIL");
}

void testuint32_t(uint32_t result, uint32_t expected, char *testname) {
    printf("Test, '%s', (Expected='%" PRIu32 "', Got='%" PRIu32 "'): %s\n",
           testname, expected, result, (result == expected) ? "PASS" : "FAIL");
}

void testchar(char result, char expected, char *testname) {
    printf("Test, '%s', (Expected='%c', Got='%c'): %s\n", testname, expected,
           result, (result == expected) ? "PASS" : "FAIL");
}

void testuchar(unsigned char result, unsigned char expected, char *testname) {
    printf("Test, '%s', (Expected='%c', Got='%c'): %s\n", testname, expected,
           result, (result == expected) ? "PASS" : "FAIL");
}
