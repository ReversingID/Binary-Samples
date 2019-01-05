/*
    Archive of Reversing.ID
    Sample Code for normal binary

    Code minimalis yang memuat semua konsep programming
    (percabangan, perulangan, fungsi, dsb).
    Tanpa pustaka (library) eksternal.

    Digunakan untuk membangun binary di FreeBSD, NetBSD, OpenBSD
*/

char code[] = "\x01\x12\x23\x34\x45\x56\x67\x78\x89\x9A\xAB\xBC\xCD\xDE\xEF\xF0";

int _xor(char * str, int len);
int sum(char * str, int len);

int _xor(char * str, int len)
{
    int i;
    for (i = 0; i < len; i++)
        str[i] ^= (i+1);

    return 0;
}

int sum(char * str, int len)
{
    int s = 0;
    int idx = len;
    while (idx--)
        s += str[idx];

    return s;
}


int main(int argc, char * argv[])
{
    int len;
    char code2[] = "\xAA\xBB\xCC\xDD\xEE\xFF\x11\x22";
    char * ptr;

    if (argc > 1)
    {
        len = 8;
        ptr = code2;
    }
    else
    {
        len = 16;
        ptr = code;
    }
    argv[0] = 0;

    _xor(ptr, len);
    return sum(ptr, len);
}