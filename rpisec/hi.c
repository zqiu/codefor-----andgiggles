#include <stdio.h>
#include <string.h>

int main(){
  int i;
  char arr[] = "bOFZckG~XKZZONcDki_ZIKAOlKI^EXS";
  for(i = 0; i < strlen(arr); ++i){
    printf("%c",arr[i]^'\x2A');
  }
}
