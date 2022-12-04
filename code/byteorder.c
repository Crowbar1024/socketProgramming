#include <stdio.h>

// 小端字节序：数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位地址。主机用
// 大端字节序：数据的低位字节存储在内存的高位地址，高位字节存储在内存的低位地址。网络用
// 通过代码检测当前主机的字节序
int main() {
    union {
        short value;    // 2字节
        char bytes[sizeof(short)];  // char[2]
    } test;

    test.value = 0x0102;
    if (test.bytes[0] == 1 && test.bytes[1] == 2) {
        printf("大端字节序\n");
    } else if ((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("小端字节序\n");
    } else {
        printf("未知\n");
    }
    return 0;
}