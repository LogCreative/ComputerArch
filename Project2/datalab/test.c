#include <stdio.h>

int main(){
    // unsigned int uf = 0x7FC00000;
    // int index = uf & 0x7F800000;
    // unsigned int tail = uf & 0x007FFFFF;
    // if(!tail & !(~((index<<1)>>24))) return uf;
    // // t = t<<1>>24;
    // // t = !(~t);
    // printf("%d\n",!!tail);
    unsigned mask = (1<<31 + ((unsigned)(-1)>>9));
    printf("%d\n", mask==0x807FFFFF);
    return 0;
}