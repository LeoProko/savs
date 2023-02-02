mkdir asan_build
cd asan_build
CXX=clang++-11 cmake -DCMAKE_BUILD_TYPE=ASAN ..
