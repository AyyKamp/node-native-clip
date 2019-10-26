// hello.cc
#include <node.h>
#include "clip.h"

namespace NodeClip {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Exception;

    void SetText(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() < 1 || !args[0]->IsString()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument provided. Must be of type string.")));
            return;
        }

        std::string value(*String::Utf8Value(args[0]->ToString()));
        clip::set_text(value);
    }

    void GetText(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        std::string value;
        clip::get_text(value);
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, value.c_str()));
    }

    void SetImage(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() < 4 || !args[0]->IsBuffer() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument provided. Must be of type string.")));
            return;
        }

        Local<Object> bufferObj = args[0]->ToObject();
        uint32_t* bufferData = Buffer::Data(bufferObj);
        size_t bufferLength = Buffer::Length(bufferObj);

        clip::image_spec spec;
        spec.width = Number::UInt32Value(args[2]->ToNumber());
        spec.height = Number::UInt32Value(args[3]->ToNumber());
        spec.bits_per_pixel = 32;
        spec.bytes_per_row = spec.width*4;
        spec.red_mask   = 0xff000000;
        spec.green_mask = 0xff0000;
        spec.blue_mask  = 0xff00;
        spec.alpha_mask = 0xff;
        spec.red_shift    = 24;
        spec.green_shift  = 16;
        spec.blue_shift   = 8;
        spec.alpha_shift  = 0;

        clip::image img(bufferData, spec);
        clip::set_image(img);
    }

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "setText", SetText);
        NODE_SET_METHOD(exports, "getText", GetText);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} 