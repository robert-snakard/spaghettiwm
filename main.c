#include <stdlib.h>
#include <stdio.h>
#include <wlc/wlc.h>

#include "logging.c"

static bool view_created(wlc_handle view) {
    wlc_handle output = wlc_view_get_output(view);
    uint32_t mask = wlc_output_get_mask(output);

    wlc_view_set_mask(view, mask);
    wlc_view_bring_to_front(view);
    wlc_view_focus(view);
    return true;
}

static void view_focus(wlc_handle view, bool focus) {
    wlc_view_set_state(view, WLC_BIT_ACTIVATED, focus);
}

static bool keyboard_key(wlc_handle view, uint32_t time, const struct wlc_modifiers *modifiers,
                         uint32_t key, enum wlc_key_state state) {
    const uint32_t sym = wlc_keyboard_get_keysym_for_key(key, NULL);

    if(state == WLC_KEY_STATE_RELEASED)
        return false;

    if(modifiers->mods & WLC_BIT_MOD_CTRL && modifiers->mods & WLC_BIT_MOD_SHIFT && sym == XKB_KEY_q) {
        wlc_terminate();
        return true;
    }

    if(modifiers->mods & WLC_BIT_MOD_CTRL && modifiers->mods & WLC_BIT_MOD_SHIFT && sym == XKB_KEY_c) {
        if(view)
            wlc_view_close(view);
        return true;
    }

    if(modifiers->mods & WLC_BIT_MOD_CTRL && sym == XKB_KEY_Return) {
        wlc_exec("weston-terminal", (char *const[]){"weston-terminal", NULL});
        return true;
    }
}

static bool pointer_motion(wlc_handle handle, uint32_t time, const struct wlc_point *position) {
    wlc_pointer_set_position(position);
    return false;
}

int main(int argc, char *argv[]) {
    logging_init("log.txt");
    wlc_log_set_handler(log_cb);

    wlc_set_view_created_cb(view_created);
    wlc_set_view_focus_cb(view_focus);
    wlc_set_keyboard_key_cb(keyboard_key);
    wlc_set_pointer_motion_cb(pointer_motion);

    if (!wlc_init())
        return EXIT_FAILURE;

    wlc_run();
    return EXIT_SUCCESS;
}
