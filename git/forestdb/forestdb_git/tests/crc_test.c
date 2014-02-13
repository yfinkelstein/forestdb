#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "crc32.h"
#include "hash_functions.h"

void basic_test()
{
    TEST_INIT();

    size_t len = 1024*1024*1024 + 7;
    void *dummy;
    size_t i;
    uint32_t r1, r2, r3;

    dummy = (void *)malloc(len);
    for (i=0;i<len/sizeof(size_t); i+=sizeof(size_t)) {
        memcpy(dummy+i, &i, sizeof(size_t));
    }

    TEST_TIME();

    r1 = crc32_8(dummy, len, 0);

    TEST_TIME();

    r2 = crc32_1(dummy, len, 0);

    TEST_TIME();

    r3 = hash_djb2(dummy, len);

    TEST_TIME();

    DBG("crc value: %u %u\n", r1, r2);

    free(dummy);

    TEST_RESULT("crc32 slicing-8 test");
}

void endian_test()
{
    uint32_t a, b, c;
    char buf[12], str[256];
    uint32_t r1, r2;
    a = 0xabcddef0;
    b = 0x13576420;
    c = 0x01234567;
    memcpy(buf, &a, sizeof(uint32_t));
    memcpy(buf + 4, &b, sizeof(uint32_t));
    memcpy(buf + 8, &c, sizeof(uint32_t));

    r1 = crc32_8(buf, 12, 0);
    r2 = crc32_8(&a, 4, 0);
    r2 = crc32_8(&b, 4, r2);
    r2 = crc32_8(&c, 4, r2);

    DBG("%u %u\n", r1,r2);

    sprintf(str, "1234aaaaaaaa");
    a = crc32_8_last8(str, strlen(str), 0);
    sprintf(str, "5678aaaaaaaa");
    b = crc32_8_last8(str, strlen(str), 0);
    DBG("%u %u\n", a, b);

    sprintf(str, "./dummy12");
    a = crc32_8_last8(str, strlen(str), 0);
    sprintf(str, ";/dummy21");
    b = crc32_8_last8(str, strlen(str), 0);
    DBG("%u %u\n", a, b);

}

int main()
{
    //basic_test();
    endian_test();
    return 0;
}
