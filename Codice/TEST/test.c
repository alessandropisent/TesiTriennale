#include <string.h>
#include <stdio.h>

#define FORMATO_STAMPA_INTERI(n) %nd

void concatena(char* s){
    char b[100];
    strcat(s,"HEllo");
    sprintf(b,"GIO");
    strcat(s,b);

}

int main(int argc, char const *argv[])
{
    char s[100]="";
    int i;
    for(i=0;i<3;i++)
        concatena(s);
    printf("%s\n",s);
    printf("FORMATO_STAMPA_INTERI(3)",3);
    return 0;
}

