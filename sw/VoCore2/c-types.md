# C Types

```
int main(int argc, char** argv)
{
    printf("sizeof types:\n");
    printf("\n");
    printf("char:        %u\n", sizeof(char));
    printf("short:       %u\n", sizeof(short));
    printf("int:         %u\n", sizeof(int));
    printf("long:        %u\n", sizeof(long));
    printf("long long:   %u\n", sizeof(long long));
    printf("float:       %u\n", sizeof(float));
    printf("double:      %u\n", sizeof(double));
    printf("long double: %u\n", sizeof(long double));
    printf("\n");
    printf("int8_t:      %u\n", sizeof(int8_t));
    printf("int16_t:     %u\n", sizeof(int16_t));
    printf("int32_t:     %u\n", sizeof(int32_t));
    printf("int64_t:     %u\n", sizeof(int64_t));
    printf("uint8_t:     %u\n", sizeof(uint8_t));
    printf("uint16_t:    %u\n", sizeof(uint16_t));
    printf("uint32_t:    %u\n", sizeof(uint32_t));
    printf("uint64_t:    %u\n", sizeof(uint64_t));
    
    return 0;
}
```
Output:
```
char:        1
short:       2
int:         4
long:        4
long long:   8
float:       4
double:      8
long double: 8

int8_t:      1
int16_t:     2
int32_t:     4
int64_t:     8
uint8_t:     1
uint16_t:    2
uint32_t:    4
uint64_t:    8
```
