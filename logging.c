FILE *log_file;

int logging_init(const char* name) {
    log_file = fopen(name, "w");
}

void log_cb(enum wlc_log_type type, const char *str) {
    char *prefix;
    switch(type) {
    case WLC_LOG_INFO: prefix = "[INFO]: ";
        break;
    case WLC_LOG_WARN: prefix = "[WARN]: ";
        break;
    case WLC_LOG_ERROR: prefix = "[ERROR]: ";
        break;
    case WLC_LOG_WAYLAND: prefix = "[WAYLAND]: ";
        break;
    }

    fwrite(prefix, 1, strlen(prefix), log_file);
    fwrite(str, 1, strlen(str), log_file);
    fwrite("\n", 1, 1, log_file);
}
