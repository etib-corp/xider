#include <jni.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <openxr/openxr_platform.h>
#include <Software.hpp>
#include <iostream>

void hello_world() {
    std::cout << "Hello, World!" << std::endl;
}

extern "C" {

void android_main(struct android_app *android_app) {
    hello_world();
}
}
