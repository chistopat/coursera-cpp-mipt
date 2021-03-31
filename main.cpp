#include "test_runner.h"
#include <string>
#include <set>
#include <string_view>
#include <map>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target) {
    string_view source_view = *data.emplace(source).first;
    string_view target_view = *data.emplace(target).first;
    source_to_target[source_view] = target_view;
    target_to_source[target_view] = source_view;
  }
  string_view TranslateForward(string_view source) const {
    if (source_to_target.count(source)) {
      return source_to_target.at(source);
    }
    return {};
  }
  string_view TranslateBackward(string_view target) const {
    if (target_to_source.count(target)) {
      return target_to_source.at(target);
    }
    return {};
  }

private:
  map<string_view, string_view> source_to_target;
  map<string_view, string_view> target_to_source;
  set<string> data;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
