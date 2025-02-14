#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <jni.h>
#include <android/log.h>

#include "frida-gumjs.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "VPID", __VA_ARGS__)

static void on_message(const gchar *message, GBytes *data, gpointer user_data) {
    // LOGI("on_message: %p", message);
    // JsonParser *parser;
    // JsonObject *root;
    // const gchar *type;

    // parser = json_parser_new();
    // json_parser_load_from_data(parser, message, -1, NULL);
    // root = json_node_get_object(json_parser_get_root(parser));

    // type = json_object_get_string_member(root, "type");
    // if(strcmp(type, "log") == 0) {
    //     const gchar *log_message;

    //     log_message = json_object_get_string_member(root, "payload");
    //     g_print("%s\n", log_message);
    // }
    // else {
    //     g_print("on_message: %s\n", message);
    // }

    // g_object_unref(parser);
}


void *main_thread(void *) {
    std::string myScript = "";
    FILE *f = fopen("/data/local/tmp/script.js", "r");
    if (!f) {
        return 0;
    }

    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        myScript += buf;
    }
    
    fclose(f);

    GumScriptBackend *backend;
    GError *error = NULL;
    GumScript *script;
    GMainContext *context;

    gum_init();

    backend = gum_script_backend_obtain_qjs();

    script = gum_script_backend_create_sync(backend, "script", myScript.c_str(), 0, 0, &error);
    g_assert(error == NULL);

    gum_script_set_message_handler(script, on_message, NULL, NULL);

    gum_script_load_sync(script, 0);

    LOGI("Script loaded!");

    context = g_main_context_get_thread_default();

    // Change this to while(true){} incase crash.
    while(g_main_context_pending(context))
        g_main_context_iteration(context, FALSE);

    gum_script_unload_sync(script, 0);

    LOGI("Script unloaded!");

    g_object_unref(script);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*vm, void*reserved) {
    LOGI("Hello from JNI_OnLoad :)");
    pthread_t t;
    pthread_create(&t, NULL, main_thread, NULL);
    return JNI_VERSION_1_6;
}