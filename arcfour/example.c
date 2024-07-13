/* example.c */
#include "arcfour.h"
#define F fflush(stdout);

void printbin(int8 *input, const int16 size) {
    int16 i;
    int8 *p;

    assert(size > 0);

    for (i = size, p = input; i; i--, p++) {
        if (!(i % 2))
            printf(" ");
        printf("%02x", *p);
    }
    printf("\n");
}

int main() {
    Arcfour *rc4;
    int16 skey, stext;
    char *key, *from;
    int8 *encrypted, *decrypted;

    key = "tomatoes"; // 8 bytes to 28 bytes
    skey = strlen(key);
    from = "hello world"; // 8 bytes to 28 bytes
    stext = strlen(from);

    printf("Initialisation\n");
    rc4 = rc4init((int8 *)key, skey);
    printf("Done\n");

    printf("'%s'\n ->", from);
    encrypted = rc4encrypt(rc4, (int8 *)from, stext);
    printbin(encrypted, stext);

    rc4unint(rc4);

    printf("Initialisation\n");
    rc4 = rc4init((int8 *)key, skey);
    printf("Done\n");

    decrypted = rc4decrypt(rc4, encrypted, stext);
    printf("  -> '%s'\n ->", decrypted);
    printbin((int8 *)decrypted, stext);

    rc4unint(rc4);
    free(encrypted);
    free(decrypted);

    return 0;
}