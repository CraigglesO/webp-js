#include <napi.h>

#include "./webp/encode.h"
#include "./webp/decode.h"

using namespace Napi;

Napi::Object GetInfo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // create object
  Napi::Object obj = Napi::Object::New(env);
  // check input
  if (info.Length() != 1) {
    Napi::Error::New(env, "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return obj;
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return obj;
  }

  // prepare variables
  size_t length = 0;
  int width, height;
  uint8_t* data;

  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  data = reinterpret_cast<uint8_t*>(buf.Data());
  length = buf.ByteLength() / sizeof(uint8_t);

  // get the data
  int res = WebPGetInfo(data, length, &width, &height);

  obj.Set(Napi::String::New(env, "data"), Napi::Number::New(env, res));
  obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
  obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, height));

  return obj;
}

Napi::Object DecodeRGB(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // create object
  Napi::Object obj = Napi::Object::New(env);
  // check input
  if (info.Length() != 1) {
    Napi::Error::New(env, "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return obj;
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return obj;
  }

  // prepare variables
  size_t length = 0;
  int width, height;
  uint8_t* data;
  uint8_t* webpData;

  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  data = reinterpret_cast<uint8_t*>(buf.Data());
  length = buf.ByteLength() / sizeof(uint8_t);

  // decode and get width & height
  webpData = WebPDecodeRGB(data, length, &width, &height);
  // set new length
  length = width * height * 4;

  if (webpData == NULL) {
    obj.Set(Napi::String::New(env, "buffer"), NULL);
  } else {
    obj.Set(Napi::String::New(env, "buffer"), Napi::ArrayBuffer::New(env, webpData, length, [](Env /*env*/, void* finalizeData) {
      WebPFree(finalizeData);
    }));
  }
  obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
  obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, height));

  return obj;
}

Napi::Object DecodeRGBA(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // create object
  Napi::Object obj = Napi::Object::New(env);
  // check input
  if (info.Length() != 1) {
    Napi::Error::New(env, "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return obj;
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return obj;
  }

  // prepare variables
  size_t length = 0;
  int width, height;
  uint8_t* data;
  uint8_t* webpData;

  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  data = reinterpret_cast<uint8_t*>(buf.Data());
  length = buf.ByteLength() / sizeof(uint8_t);

  // decode and get width & height
  webpData = WebPDecodeRGBA(data, length, &width, &height);
  // set new length
  length = width * height * 4;

  if (webpData == NULL) {
    obj.Set(Napi::String::New(env, "buffer"), NULL);
  } else {
    obj.Set(Napi::String::New(env, "buffer"), Napi::ArrayBuffer::New(env, webpData, length, [](Env /*env*/, void* finalizeData) {
      WebPFree(finalizeData);
    }));
  }
  obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
  obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, height));

  return obj;
}

Napi::ArrayBuffer EncodeRGB(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // check input (ArrayBuffer input, int width, int height, float quality_factor)
  if (info.Length() != 4) {
    Napi::Error::New(env, "Expected exactly four arguments")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected first argument to be an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[1].IsNumber()) {
    Napi::Error::New(env, "Expected second argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[2].IsNumber()) {
    Napi::Error::New(env, "Expected third argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[3].IsNumber()) {
    Napi::Error::New(env, "Expected fourth argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }

  uint8_t* rgb;
  uint8_t* output;
  size_t encodedSize;

  // get the input data
  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  rgb = reinterpret_cast<uint8_t*>(buf.Data());
  // grab width, height, and quality_factor
  int width = info[1].As<Napi::Number>().Int32Value();
  int height = info[2].As<Napi::Number>().Int32Value();
  float quality_factor = info[3].As<Napi::Number>().FloatValue();

  encodedSize = WebPEncodeRGB(rgb, width, height, width * 3, quality_factor, &output);

  return Napi::ArrayBuffer::New(env, output, encodedSize, [](Env /*env*/, void* finalizeData) {
    WebPFree(finalizeData);
  });
}


Napi::ArrayBuffer EncodeRGBA(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // check input (ArrayBuffer input, int width, int height, float quality_factor)
  if (info.Length() != 4) {
    Napi::Error::New(env, "Expected exactly four arguments")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected first argument to be an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[1].IsNumber()) {
    Napi::Error::New(env, "Expected second argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[2].IsNumber()) {
    Napi::Error::New(env, "Expected third argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[3].IsNumber()) {
    Napi::Error::New(env, "Expected fourth argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }

  uint8_t* rgba;
  uint8_t* output;
  size_t encodedSize;

  // get the input data
  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  rgba = reinterpret_cast<uint8_t*>(buf.Data());
  // grab width, height, and quality_factor
  int width = info[1].As<Napi::Number>().Int32Value();
  int height = info[2].As<Napi::Number>().Int32Value();
  float quality_factor = info[3].As<Napi::Number>().FloatValue();

  encodedSize = WebPEncodeRGBA(rgba, width, height, width * 4, quality_factor, &output);

  return Napi::ArrayBuffer::New(env, output, encodedSize, [](Env /*env*/, void* finalizeData) {
    WebPFree(finalizeData);
  });
}

Napi::ArrayBuffer EncodeLosslessRGB(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // check input (ArrayBuffer input, int width, int height)
  if (info.Length() != 3) {
    Napi::Error::New(env, "Expected exactly four arguments")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected first argument to be an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[1].IsNumber()) {
    Napi::Error::New(env, "Expected second argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[2].IsNumber()) {
    Napi::Error::New(env, "Expected third argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }

  uint8_t* rgb;
  uint8_t* output;
  size_t encodedSize;

  // get the input data
  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  rgb = reinterpret_cast<uint8_t*>(buf.Data());
  // grab width, height, and quality_factor
  int width = info[1].As<Napi::Number>().Int32Value();
  int height = info[2].As<Napi::Number>().Int32Value();

  encodedSize = WebPEncodeLosslessRGB(rgb, width, height, width * 3, &output);

  return Napi::ArrayBuffer::New(env, output, encodedSize, [](Env /*env*/, void* finalizeData) {
    WebPFree(finalizeData);
  });
}

Napi::ArrayBuffer EncodeLosslessRGBA(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  // check input (ArrayBuffer input, int width, int height)
  if (info.Length() != 3) {
    Napi::Error::New(env, "Expected exactly four arguments")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(env, "Expected first argument to be an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[1].IsNumber()) {
    Napi::Error::New(env, "Expected second argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }
  if (!info[2].IsNumber()) {
    Napi::Error::New(env, "Expected third argument to be a number")
        .ThrowAsJavaScriptException();
    return Napi::ArrayBuffer::New(env, nullptr, 0);
  }

  uint8_t* rgba;
  uint8_t* output;
  size_t encodedSize;

  // get the input data
  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  rgba = reinterpret_cast<uint8_t*>(buf.Data());
  // grab width, height, and quality_factor
  int width = info[1].As<Napi::Number>().Int32Value();
  int height = info[2].As<Napi::Number>().Int32Value();

  encodedSize = WebPEncodeLosslessRGBA(rgba, width, height, width * 4, &output);

  return Napi::ArrayBuffer::New(env, output, encodedSize, [](Env /*env*/, void* finalizeData) {
    WebPFree(finalizeData);
  });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getInfo"),
              Napi::Function::New(env, GetInfo));
  exports.Set(Napi::String::New(env, "decodeRGB"),
              Napi::Function::New(env, DecodeRGB));
  exports.Set(Napi::String::New(env, "decodeRGBA"),
              Napi::Function::New(env, DecodeRGBA));
  exports.Set(Napi::String::New(env, "encodeRGB"),
              Napi::Function::New(env, EncodeRGB));
  exports.Set(Napi::String::New(env, "encodeRGBA"),
              Napi::Function::New(env, EncodeRGBA));
  exports.Set(Napi::String::New(env, "encodeLosslessRGB"),
              Napi::Function::New(env, EncodeLosslessRGB));
  exports.Set(Napi::String::New(env, "encodeLosslessRGBA"),
              Napi::Function::New(env, EncodeLosslessRGBA));
  return exports;
}

NODE_API_MODULE(addon, Init)
