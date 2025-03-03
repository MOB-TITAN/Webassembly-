#include <emscripten.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<stdio.h>
#define PORT 6050
#define STBI_ASSERT(x)
extern unsigned int curTime();
extern void logProgress(float progress);
extern char *get_string();

EMSCRIPTEN_KEEPALIVE
unsigned char *randString(int len)
{
    unsigned char *str = malloc(len + 1);

    srand(curTime());

    for (int i = 0; i < len; i++)
    {
        // generate a printable character
        str[i] = rand() % (127 - 33) + 33;
        logProgress((float)(i + 1) / (float)len);
        for (int j = 0; j < 100000000; j++)
        {
        }
    }

    str[len] = 0;
    return str;
}EMSCRIPTEN_KEEPALIVE
void sendToServer(const char *k) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    ssize_t bytes_sent, total_bytes_sent = 0;
    size_t len = strlen(k);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        close(sock);
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return;
    }

    // Send the string to the server in chunks
    while (total_bytes_sent < len) {
        bytes_sent = send(sock, k + total_bytes_sent, len - total_bytes_sent, 0);
        if (bytes_sent < 0) {
            close(sock);
            return;
        }
        total_bytes_sent += bytes_sent;
    }

    close(sock);
}
// int main() {
//     const char *k = get_string();
//     sendToServer(k);
//     Optionally, you can generate a larger string for testing;
//     free(k); // Free the allocated memory after use
//     return 0;
// }
