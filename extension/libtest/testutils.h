#ifndef TEST_UTILS
#define TEST_UTILS

extern void testbool(bool condition, char *testname);
extern void testint(int result, int expected, char *testname);
extern void testlong(long result, long expected, char *testname);
extern void testdouble(double result, double expected, char *testname);
extern void teststring(char *result, char *expected, char *testname);
extern void testuint32_t(uint32_t result, uint32_t expected, char *testname);
extern void testchar(char result, char expected, char *testname);
extern void testuchar(unsigned char result, unsigned char expected,
                      char *testname);

#endif
