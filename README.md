build library: https://developers.google.com/speed/webp/docs/compiling

make CXXFLAGS="-mmacosx-version-min=10.7"
]

node-gyp configure

node-gyp build
