#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h> // for ioctl() on Darwin
#include <string.h>

#include "ut.h"
#include <stdarg.h>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define DOUBLE_EPSILON          0x00000000001   //!< epsilon for comparing doubles

static int cols = 0;
static int rows = 0;
static int left_width;
static int right_width;
static int backspace = 0;

int ut_tests = 0;
int ut_sections = 0;
int ut_succeeded = 0;
int ut_skipped = 0;
int ut_failed = 0;

static char *hex_escape(const char *buf, int len) {
    int i;
    static char *str = NULL;

    str = realloc(str, (len*2)+4);
    strcpy(str, "0x");
    char *p = str+2;
    
    for (i = 0; i < len; i++) {
        sprintf(p, "%02x", buf[i]);
        p+=2;
    }
    return str;
}

//Initialise cols and rows variables.
static void
ut_windowsize(void)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
        cols = ws.ws_col;
        rows = ws.ws_row;
    } else {
        char *s;
        if ((s = getenv("COLUMNS")) == NULL || (cols = strtol(s, NULL, 0)) == 0
             || (s = getenv("LINES")) == NULL || (rows = strtol(s, NULL, 0)) == 0)
        {
            cols = 80;      // default values as a fallback option
            rows = 25;
        }
    }

    left_width = cols*0.60;
    right_width = cols - left_width;
}

void
ut_init(char *programname)
{
    ut_windowsize();

    printf("==> Testing %s\n", programname);
}

void
ut_section(const char *subtitle)
{
    if (ut_sections > 0)
        printf("\n");

    ut_sections++;
    printf("==> Section %d: %s\n", ut_sections, subtitle);
}

void
ut_testing(const char *fmt, ...)
{
    va_list args;
    int n = left_width;

    ut_tests++;
    printf("% 3d ", ut_tests);
    n -= 3;

    va_start(args, fmt);

    n -= vprintf(fmt, args);

    for (; n > 0; n--)
        putchar(' ');

    fflush (stdout);

    va_end(args);
}

// print \c backspace \c \\b characters.
static void
ut_backspace(void)
{
    for (; backspace > 0; backspace--)
        printf("\b \b");
}

void
ut_progress(int percentage)
{
    ut_backspace();

    backspace = printf("%d%%", percentage);
    fflush(stdout);
}

int
ut_success(void)
{
    ut_backspace();

    printf("ok\n");
    fflush (stdout);

    ut_succeeded++;

    return 1;
}

int
ut_skip(void)
{
    ut_backspace();

    printf("skipped\n");
    fflush(stdout);

    ut_skipped++;

    return 1;
}

int
ut_failure(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    ut_vfailure(fmt, args);

    va_end(args);

    return 0;
}

int
ut_vfailure(const char *fmt, va_list args)
{
    ut_backspace();

    printf("FAILED ");
    vprintf(fmt, args);
    putchar('\n');
    fflush(stdout);

    ut_failed++;

    return 0;
}

int
ut_result(int r, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    ut_vresult(r, fmt, args);
    va_end(args);

    return r;
}

int
ut_vresult(int r, const char *fmt, va_list args)
{
    if (r)
        ut_success();
    else
        ut_vfailure(fmt, args);

    return r;
}

void
ut_summary(void)
{
    int r = ut_succeeded+ut_failed+ut_skipped;

    if (ut_tests == 0) {
        ut_failure("no tests");
        exit(99);
    }

    if (r != ut_tests) {
        ut_failure("number of tests (%d) does not match number of results (%d)", ut_tests, r);
        exit(99);
    }

    printf("==> Summary: %d tests, %d succeeded", ut_tests, ut_succeeded);

    if (ut_failed)
        printf(", %d failed", ut_failed);

    if (ut_skipped)
        printf(", %d skipped", ut_skipped);

    printf(".\n");
}

int
ut_validate_buffer(const char *result, int result_len, const char *orig, int orig_len)
{
    if (orig == NULL && result != NULL) {
        return ut_failure("'%s' should be NULL", hex_escape(result, result_len));
    } else if (result == NULL && orig != NULL) {
        return ut_failure("should not be NULL");
    } else if (result == NULL && result_len != 0) {
        return ut_failure("NULL but len is not 0");
    } else if (result_len != orig_len) {
        return ut_failure("result len == %d but should be %d", result_len, orig_len);
    } else if (result == NULL && orig == NULL) {
        return ut_success();
    } else if (memcmp(result, orig, orig_len) != 0) {
        char *buf1 = strdup(hex_escape(result, result_len));
        char *buf2 = strdup(hex_escape(orig, orig_len));
        ut_failure("'%s' should be '%s'", buf1, buf2);
        free(buf1);
        free(buf2);
        return 0;
    } else {
        return ut_success();
    }
}

int
ut_validate_string(const char *result, const char *orig)
{
    if (orig == NULL && result != NULL) {
        return ut_failure("should be NULL");
    } else if (result == NULL && orig != NULL) {
        return ut_failure("should not be NULL");
    } else if (result == NULL && orig == NULL) {
        return ut_success();
    } else if (strcmp(result, orig) != 0) {
        char *result2 = strdup(result);        // is result from *_escape functions in escape_test.c, so strdup it first.
        ut_failure("'%s' should be '%s'", result2, orig);
        free(result2);
        return 0;
    } else {
        return ut_success();
    }
}

int
ut_validate_double(double result, double orig)
{
    if (fabs(result - orig) > DOUBLE_EPSILON) {
        return ut_failure("%f should be %f", result, orig);
    } else {
        return ut_success();
    }
}

int
ut_validate_int(int result, int orig)
{
    if (result != orig) { 
        return ut_failure("%d should be %d", result, orig);
    } else {
        return ut_success();
    }
}
