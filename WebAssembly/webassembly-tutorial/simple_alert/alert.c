#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void show_alert() {
    emscripten_run_script("alert(document.cookie);");
}
