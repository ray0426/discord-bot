#include <napi.h>
#include <string.h>
//#include "calc.h"

float calc(std::string strin);
std::string rmspace(std::string str);
float rec(std::string str);

void RunCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function cb = info[0].As<Napi::Function>();
  cb.Call(env.Global(), {Napi::String::New(env, "hello world")});
}

/*Napi::Value Calc(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::string str0 = "The string is: ";
    std::string str1 = info[0].As<Napi::String>();
    Napi::String str = Napi::String::New(env, str0 + str1);

    return str;
}*/

float calc(std::string strin)
{
    float ans;

    strin = rmspace(strin);
	ans = rec(strin);

    return ans;
}

std::string rmspace(std::string str)
{
    std::string str1;
    unsigned int i;

    for (i = 0; i < str.length(); i++) {
        if (str[i] != ' ') {
            str1.push_back(str[i]);
        }
    }
    return str1;
}

float rec(std::string str)
{
    int i;
    int isnum = 1;
    int brace_count = 0;

    for (i = str.length() - 1; i >= 0; i--) {
        if (brace_count >= 1) {
            if (str[i] == '(') {
                brace_count -= 1;
            }
        } else {
            switch (str[i]) {
                case ')': {
                    isnum = 0;
                    brace_count += 1;
                    break;
                }
                case '+': {
                    isnum = 0;
                    return rec(str.substr(0, i)) +
                        rec(str.substr(i + 1, str.length() - i - 1));
                    break;
                }
                case '-': {
                    isnum = 0;
                    return rec(str.substr(0, i)) -
                        rec(str.substr(i + 1, str.length() - i - 1));
                    break;
                }
            }
        }
    }
	for (i = str.length() - 1; i >= 0; i--) {
        if (brace_count >= 1) {
            if (str[i] == '(') {
                brace_count -= 1;
            }
        } else {
            switch (str[i]) {
                case ')': {
                    isnum = 0;
                    brace_count += 1;
                    break;
                }
                case '*': {
                    isnum = 0;
                    return rec(str.substr(0, i)) *
                        rec(str.substr(i + 1, str.length() - i - 1));
                    break;
                }
                case '/': {
                    isnum = 0;
                    return rec(str.substr(0, i)) /
                        rec(str.substr(i + 1, str.length() - i - 1));
                    break;
                }
            }
        }
    }
    if (isnum == 1) {
        return std::stof(str);
    }
    if (str[0] == '(' && str[str.length() - 1] == ')') {
        return rec(str.substr(1, str.length() - 2));
    }
    return 0;
}


Napi::Value Calc(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::string str = info[0].As<Napi::String>();
    //std::string str0 = "The string is: ";
    //std::string str1 = info[0].As<Napi::String>();
    Napi::Number num = Napi::Number::New(env, calc(str));

    return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set(Napi::String::New(env, "msg"),
				Napi::Function::New(env, RunCallback));
	exports.Set(Napi::String::New(env, "calc"),
				Napi::Function::New(env, Calc));
    return exports;
}

NODE_API_MODULE(addon, Init)
