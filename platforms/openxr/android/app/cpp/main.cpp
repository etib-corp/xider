/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <jni.h>

#define LOG_NDEBUG 0

#define LOG_TAG	  "XiderEngine"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#include <android/log.h>
#include <android/looper.h>

#include <android_native_app_glue.h>
#include <vulkan/vulkan.h>
#include <openxr/openxr_platform.h>
#include <evan/Engine.hpp>
#include <iostream>

#include <evan/openxr/platform/AndroidXrPlatform.hpp>

#include <xider/xider.hpp>

#include <iostream>
#include <streambuf>
#include <string>

class AndroidLogBuffer: public std::streambuf
{
	public:
	enum AndroidLogPriority {
		ANDROID_LOG_VERBOSE = 2,
		ANDROID_LOG_DEBUG	= 3,
		ANDROID_LOG_INFO	= 4,
		ANDROID_LOG_WARN	= 5,
		ANDROID_LOG_ERROR	= 6,
	};

	AndroidLogBuffer(AndroidLogPriority priority, const char *tag)
		: priority_(priority)
		, tag_(tag)
	{
		buffer_[0] = '\0';
	}

	protected:
	virtual int_type overflow(int_type c) override
	{
		if (c != traits_type::eof()) {
			char ch = c;
			if (ch == '\n') {
				buffer_[pos_] = '\0';
				__android_log_write(priority_, tag_, buffer_);
				pos_ = 0;
			} else {
				if (pos_ < sizeof(buffer_) - 1) {
					buffer_[pos_++] = ch;
				}
			}
		}
		return c;
	}

	virtual int sync() override
	{
		if (pos_ > 0) {
			buffer_[pos_] = '\0';
			__android_log_write(priority_, tag_, buffer_);
			pos_ = 0;
		}
		return 0;
	}

	private:
	AndroidLogPriority priority_;
	const char *tag_;
	char buffer_[512];
	size_t pos_ = 0;
};

void redirectStdoutToLogcat()
{
	static AndroidLogBuffer coutBuffer(AndroidLogBuffer::ANDROID_LOG_INFO,
									   LOG_TAG);
	static AndroidLogBuffer cerrBuffer(AndroidLogBuffer::ANDROID_LOG_ERROR,
									   LOG_TAG);
	std::cout.rdbuf(&coutBuffer);
	std::cerr.rdbuf(&cerrBuffer);
	std::clog.rdbuf(&coutBuffer);
}

extern "C" {

static void AppHandleCmd(struct android_app *app, int32_t cmd)
{
	auto *app_state =
		reinterpret_cast<evan::AndroidXrPlatform::AndroidAppState *>(
			app->userData);
	switch (cmd) {
		case APP_CMD_START: {
			LOGD("APP_CMD_START onStart()");
			break;
		}
		case APP_CMD_RESUME: {
			LOGD("APP_CMD_RESUME onResume()");
			app_state->_resumed = true;
			app_state->_paused	= false;
			break;
		}
		case APP_CMD_PAUSE: {
			LOGD("APP_CMD_PAUSE onPause()");
			app_state->_resumed = false;
			app_state->_paused	= true;
			break;
		}
		case APP_CMD_STOP: {
			LOGD("APP_CMD_STOP onStop()");
			break;
		}
		case APP_CMD_DESTROY: {
			LOGD("APP_CMD_DESTROY onDestroy()");
			break;
		}
		case APP_CMD_INIT_WINDOW: {
			LOGD("APP_CMD_INIT_WINDOW surfaceCreated()");
			break;
		}
		case APP_CMD_TERM_WINDOW: {
			LOGD("APP_CMD_TERM_WINDOW surfaceDestroyed()");
			break;
		}
	}
}

void android_main(struct android_app *android_app)
{
	redirectStdoutToLogcat();
	std::cout << "XIDER Android Application Started" << std::endl;

	JNIEnv *env;
	android_app->activity->vm->AttachCurrentThread(&env, nullptr);

	android_app->onAppCmd = AppHandleCmd;
	evan::Engine::initializeAssetManager(android_app->activity->assetManager);

	g_assetManager->add(std::string("./texture1.png"));

	// Initialize XR platform data
	evan::AndroidXrPlatform::AndroidPlatformData platformData {
		.applicationVM		 = android_app->activity->vm,
		.applicationActivity = android_app->activity->clazz,
		.androidApp			 = android_app
	};

	// Create XR platform instance
	auto xrPlatform = std::shared_ptr<evan::AndroidXrPlatform>(
		new evan::AndroidXrPlatform(platformData));
	android_app->userData = &xrPlatform->_appState;

	// Initialize XIDER application with Evan engine
	xider::XIDER app(xrPlatform);

	std::cout << "XIDER Application initialized successfully" << std::endl;
	std::cout << "Entering main application loop..." << std::endl;

	std::vector<std::string> texturePaths = {
		"./texture1.png",
	};

	while (!android_app->destroyRequested) {
		// Process Android events
		for (;;) {
			int events;
			struct android_poll_source *source;
			const int kTimeoutMilliseconds =
				(!xrPlatform->_appState._resumed
				 && android_app->destroyRequested == 0)
				? -1
				: 0;
			if (ALooper_pollOnce(kTimeoutMilliseconds, nullptr, &events,
								 (void **)&source)
				< 0) {
				break;
			}
			if (source != nullptr) {
				source->process(android_app, source);
			}
		}

		// Application lifecycle
		app.pollEvents();
		if (app.gotNewEvents())
			continue;
		app.clear();
		app.routine();
		app.present();
	}
}
}
