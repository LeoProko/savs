#include <emscripten.h>

#include <vector>

EMSCRIPTEN_KEEPALIVE
float add(float x, float y) {
  std::vector<float> v;
  v.push_back(x);
  v.push_back(y);
  return v.front() + v.back();
}
