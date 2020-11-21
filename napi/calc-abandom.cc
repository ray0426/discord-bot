#include <napi.h>
#include <string.h>

Napi::Value Calc(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	std::string str0 = "The string is: ";
	std::string str1 = info[0].As<Napi::String>();
	Napi::String str = Napi::String::New(env, str0 + str1);

	return str;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set(Napi::String::New(env, "calc"), 
				Napi::Function::New(env, Calc));
	return exports;
}

NODE_API_MODULE(calc1, Init)
