#include <stdarg.h>

/**
 * @file
 *
 * Unit Testing Framework
 *
 * General usage:
 * @code
 * #include "unit_testing.h"
 *
 * int
 * main(int argc, char **argv)
 * {
 * ...
 *
 *     ut_init();
 * ...
 *     ut_testing("function1(args1)");
 *     char *val = function1(args1);
 *     ut_validate_string(val, "expected_value");
 * ...
 *     ut_testing("function2(args3)");
 *     int rc = function2(args3);
 *     ut_validate_int(rc, expected_rc);
 * ...
 *     ut_summary(); // print a summary and validate the test counters
 * ...
 *     return ut_failed; // return the error count
 * }
 * @endcode
 */

 
extern int ut_tests;     //!< Number of tests run
extern int ut_skipped;   //!< Number of skipped tests
extern int ut_failed;    //!< Number of failed tests
extern int ut_succeeded; //!< Number of succesfull tests

/**
 * @brief Initialise the lib.
 * @param progname The program name to announce
 */
void ut_init(char *progname);

/*!
 * \brief Insert a section header.
 */
void ut_section(const char *subtitle);

/**
 * @brief announce what we are going to test.
 * @param fmt printf style format string
 * @param ... printf style arguments
 *
 * @note stdout is flushed after writing the message and indenting up to the
 * right column.
 */
void ut_testing(const char *fmt, ...);

/**
 * @brief Print a percentage value, including backspace chars.
 * @param percentage percentage
 */
void ut_progress(int percentage);

/**
 * @brief Announce a succesfull test.
 * @returns 1.
 */
int ut_success(void);

/**
 * @brief Announce a skipped test.
 */
int ut_skip(void);

/**
 * @brief Announce a failed test, including an error message.
 * @param fmt printf style format string
 * @param ... printf style arguments
 * @returns 0.
 */
int ut_failure(const char *fmt, ...);

/**
 * @brief Announce a failed test, including an error message.
 * @param fmt vprintf style format string
 * @param args vprintf style arguments
 * @returns 0.
 */
int ut_vfailure(const char *fmt, va_list args);

/**
 * @brief Call ut_success or ut_vfailure depending on the boolean r.
 * @param r boolean result, indicating success if true or failure otherwise
 * @param fmt printf style format string
 * @param ... printf style arguments
 * @returns r.
 */
int ut_result(int r, const char *fmt, ...);

/**
 * @brief Call ut_success or ut_vfailure depending on the boolean r.
 * @param r boolean result, indicating success if true or failure otherwise
 * @param fmt vprintf style format string
 * @param args vprintf style arguments
 * @returns r.
 */
int ut_vresult(int r, const char *fmt, va_list args);

/**
 * @brief Display a summary of the test.
 */
void ut_summary(void);

/**
 * @brief Compare two buffers.
 * @param result result buffer
 * @param result_len length of result buffer
 * @param orig original buffer
 * @param orig_len length of original buffer
 * @returns 1 on success, 0 otherwise.
 */
int ut_validate_buffer(const char *result, int result_len, const char *orig, int orig_len);

/**
 * @brief Compare two strings.
 * @param result result string
 * @param orig original string
 * @returns 1 on success, 0 otherwise.
 */
int ut_validate_string(const char *result, const char *orig);

/**
 * @brief Compare two doubles.
 * @param result result double
 * @param orig original double
 * @returns 1 on success, 0 otherwise.
 * @note Compares the doubles with DOUBLE_EPSILON precision.
 */
int ut_validate_double(double result, double orig);

/**
 * @brief Compare two integers.
 * @param result result integer
 * @param orig original integer
 * @returns 1 on success, 0 otherwise.
 */
int ut_validate_int(int result, int orig);
