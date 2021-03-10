#include<stdio.h>

int main(){
    int x = 0x80000000;
    int y = 0x00000001;
    unsigned ux = x;
    unsigned uy = y;

    // printf("%d\n",x+y);
    // printf("%u\n",ux+uy);
    // printf("%d\n",(x+y)==(ux+uy));
    // printf("%d\n",x*~y + uy*ux == -x);
    // printf("%d\n",(x>>2)<<2<=x);
    // printf("%d\n",(x+y)<<4+y-x);
    // printf("%d\n",17*y+15*x);
    printf("%d",ux-uy==-(y-x));
    return 0;
}