cmd_Release/obj.target/webp-js-native.node := g++ -o Release/obj.target/webp-js-native.node -shared -pthread -rdynamic -m64  -Wl,-soname=webp-js-native.node -Wl,--start-group Release/obj.target/webp-js-native/src/webp_js.o Release/obj.target/node_modules/node-addon-api/src/nothing.a -Wl,--end-group /usr/local/lib/libwebp.a
