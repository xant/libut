#include <ut.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

int main(int argc, char **argv)
{
    ut_init(basename(argv[0]));

    ut_section("Test Section");

    ut_testing("Testing integer");
    int a = 5;
    ut_validate_int(a, 5);

    ut_testing("Testing strings");
    char *string = "CIAO";
    ut_validate_string(string, "CIAO");

    ut_testing("Testing doubles");
    double b = 0.99;
    ut_validate_double(b, 0.99);

    ut_testing("Testing progress");
    int i;
    for (i = 0; i < 10000; i ++) {
        ut_progress(i/100);
    }
    ut_success();

    ut_summary();

    return ut_failed;
}

