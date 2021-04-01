#include "test_runner.h"

#include <list>
using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() {
    cursor = text.begin();
  }
  void Left() {
    if (cursor != text.begin()) {
      --cursor;
    }
  }
  void Right() {
    if (cursor != text.end()) {
      ++cursor;

    }
  }
  void Insert(char token) {
    text.insert(cursor, token);
  }
  void Cut(size_t tokens = 1) {
    buffer.clear();
    auto current_cursor = cursor;
    cursor = GetLastAvailible(tokens);
    buffer.splice(buffer.begin(), text, current_cursor, cursor);
  }
  void Copy(size_t tokens = 1) {
    buffer.clear();
    copy(cursor, GetLastAvailible(tokens), back_inserter(buffer));
  }
  void Paste() {
    for (char ch : buffer) {
      Insert(ch);
    }
  }

  [[nodiscard]] string GetText() const {
    return {text.begin(), text.end()};
  }

private:
  list<char>::iterator cursor;
  list<char> text;
  list<char> buffer;

private:
  list<char>::iterator GetLastAvailible(size_t tokens) {
      auto it = cursor;
      while (tokens) {
        if (it == text.end()) {
          break;
        }
        std::advance(it, 1);
        --tokens;
    }
    return it;
  }
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestWriteText() {
  Editor editor;
  TypeText(editor, "example");
  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestRight() {
  Editor editor;
  editor.Insert('c');
  editor.Right();
  editor.Right();
  editor.Right();
  ASSERT_EQUAL(editor.GetText(), "c");
}

void TestCut() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    ASSERT_EQUAL(editor.GetText(), "world");
  }
}
int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCut);
  RUN_TEST(tr, TestRight);
  RUN_TEST(tr, TestWriteText);
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}
