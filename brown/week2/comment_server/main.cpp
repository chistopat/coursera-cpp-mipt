#include "test_runner.h"

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <optional>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct HttpHeader {
  string name;
  string value;

};

enum class HttpCode {
  Ok = 200,
  NotFound = 404,
  Found = 302,
};

class HttpResponse {
public:
  const std::string kHttp = "HTTP/1.1"s;

public:
  explicit HttpResponse(HttpCode code)
      : code_(static_cast<int>(code))
      , comment_(kComments.at(code))
      , protocol_(kHttp) {

  }

  HttpResponse& AddHeader(string name, string value) {
      headers_.push_back({move(name), move(value)});
      return *this;
  }

  HttpResponse& SetContent(string a_content) {
      string content_header = "Content-Length";
      auto old_content_header = std::find_if(headers_.begin(), headers_.end(),
                                             [&content_header](const HttpHeader& h)
                                             {return h.name == content_header;});
      if (old_content_header != headers_.end()) {
          headers_.erase(old_content_header);
      }
      AddHeader(move(content_header), to_string(a_content.size()));
      body_ = move(a_content);
      return *this;
  }

  HttpResponse& SetCode(HttpCode a_code) {
      code_ = static_cast<int>(a_code);
      comment_ = kComments.at(a_code);
      return *this;
  }

  friend ostream& operator << (ostream& output, const HttpResponse& resp) {
      output << resp.protocol_ << ' ' << resp.code_ << ' ' << resp.comment_ << '\n';
      for (const auto& [name, value] : resp.headers_) {
          output << name << ": " << value << '\n';
      }
      output << '\n' << resp.body_;
      return output;
  }

private:
  const std::map<HttpCode, const std::string> kComments = {
      {HttpCode::Ok, "OK"},
      {HttpCode::NotFound, "Not found"},
      {HttpCode::Found, "Found"},
  };

private:
  int code_;
  vector<HttpHeader> headers_;
  std::string body_;
  std::string comment_;
  std::string protocol_;
};


struct HttpRequest {
  string method;
  string path;
  string body;
  map<string, string> get_params;
};

pair<string, string> SplitBy(const string& what, const string& by) {
    size_t pos = what.find(by);
    if (by.size() < what.size() && pos < what.size() - by.size()) {
        return {what.substr(0, pos), what.substr(pos + by.size())};
    } else {
        return {what, {}};
    }
}

template<typename T>
T FromString(const string& s) {
    T x;
    istringstream is(s);
    is >> x;
    return x;
}

pair<size_t, string> ParseIdAndContent(const string& body) {
    auto [id_string, content] = SplitBy(body, " ");
    return {FromString<size_t>(id_string), content};
}

struct LastCommentInfo {
  size_t user_id, consecutive_count;
};


class CommentServer {
private:
  vector<vector<string>> comments_;
  std::optional<LastCommentInfo> last_comment;
  unordered_set<size_t> banned_users;

private:
  enum class ServerRoutes {
    AddUserPost,
    AddCommentPost,
    CheckCaptchaPost,
    UserCommentsGet,
    CaptchaGet,
    NONE,
  };

  const map<std::string, ServerRoutes> kStringToRoute {
      {"/add_user", ServerRoutes::AddUserPost},
      {"/add_comment", ServerRoutes::AddCommentPost,},
      {"/checkcaptcha", ServerRoutes::CheckCaptchaPost},
      {"/user_comments", ServerRoutes::UserCommentsGet},
      {"/captcha", ServerRoutes::CaptchaGet},
      {"", ServerRoutes::NONE},
  };

public:
  HttpResponse ServeRequest(const HttpRequest& req) {
      auto route = ServerRoutes::NONE;
      auto path = SplitBy(req.path, "?").first;
      if (kStringToRoute.count(path)) {
          route = kStringToRoute.at(path);
      }
      switch (route) {
          case (ServerRoutes::AddUserPost):
              return AddUserHandler();

          case (ServerRoutes::AddCommentPost):
              return AddCommentHandler(req);

          case (ServerRoutes::CheckCaptchaPost):
              return CheckCaptchaHandler(req);

          case (ServerRoutes::UserCommentsGet):
              return UserCommentHandler(req);

          case (ServerRoutes::CaptchaGet):
              return CaptchaHandler();

          case (ServerRoutes::NONE):
              return HttpResponse(HttpCode::NotFound);
      }
  }

private:
  HttpResponse AddUserHandler() {
      comments_.emplace_back();
      HttpResponse response_obj(HttpCode::Ok);
      response_obj.SetContent(to_string(comments_.size() - 1));
      return response_obj;
  }

  HttpResponse AddCommentHandler(const HttpRequest& req) {
      auto[user_id, comment] = ParseIdAndContent(req.body);

      if (!last_comment || last_comment->user_id != user_id) {
          last_comment = LastCommentInfo{user_id, 1};
      } else if (++last_comment->consecutive_count > 3) {
          banned_users.insert(user_id);
      }

      if (banned_users.count(user_id) == 0) {
          comments_[user_id].push_back(string(comment));
          HttpResponse response_obj(HttpCode::Ok);
          return response_obj;
      }

      HttpResponse response_obj(HttpCode::Found);
      response_obj.AddHeader("Location", "/captcha");
      return response_obj;
  }

  HttpResponse CheckCaptchaHandler(const HttpRequest& req) {
      if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
          banned_users.erase(id);
          if (last_comment && last_comment->user_id == id) {
              last_comment.reset();
          }
          HttpResponse response_obj(HttpCode::Ok);
          return response_obj;
      }
      HttpResponse response_obj(HttpCode::Found);
      response_obj.AddHeader("Location", "/captcha");
      return response_obj;
  }

  HttpResponse UserCommentHandler(const HttpRequest& req) {
      auto user_id = FromString<size_t>(req.get_params.at("user_id"));
      string response;
      for (const string& c : comments_[user_id]) {
          response += c + '\n';
      }
      HttpResponse response_obj(HttpCode::Ok);
      response_obj.SetContent(response);
      return response_obj;
  }

  HttpResponse CaptchaHandler() {
      HttpResponse response(HttpCode::Ok);
      response.SetContent("What's the answer for The Ultimate Question of Life, the Universe, and Everything?");
      return response;
  }

};



ostream& operator<<(ostream& output, const HttpHeader& h) {
    return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct ParsedResponse {
  int code;
  vector<HttpHeader> headers;
  string content;
};

istream& operator >>(istream& input, ParsedResponse& r) {
    string line;
    getline(input, line);

    {
        istringstream code_input(line);
        string dummy;
        code_input >> dummy >> r.code;
    }

    size_t content_length = 0;

    r.headers.clear();
    while (getline(input, line) && !line.empty()) {
        if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length") {
            istringstream length_input(value);
            length_input >> content_length;
        } else {
            r.headers.push_back( {std::move(name), std::move(value)});
        }
    }

    r.content.resize(content_length);
    input.read(r.content.data(), r.content.size());
    return input;
}

void Test(CommentServer& srv, const HttpRequest& request, const ParsedResponse& expected) {
    stringstream ss;
    ss << srv.ServeRequest(request);
//    std::cout << ss.str();
    ParsedResponse resp;
    ss >> resp;
    ASSERT_EQUAL(resp.code, expected.code);
    ASSERT_EQUAL(resp.headers, expected.headers);
    ASSERT_EQUAL(resp.content, expected.content);
}

template <typename CommentServer>
void TestServer() {
    CommentServer cs;

    const ParsedResponse ok{200};
    const ParsedResponse redirect_to_captcha{302, {{"Location", "/captcha"}}};
    const ParsedResponse not_found{404};

    Test(cs, {"POST", "/add_user"}, {200, {}, "0"});
    Test(cs, {"POST", "/add_user"}, {200, {}, "1"});
    Test(cs, {"POST", "/add_comment", "0 Hello"}, ok);
    Test(cs, {"POST", "/add_comment", "1 Hi"}, ok);
    Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, ok);
    Test(cs, {"POST", "/add_comment", "1 Enlarge"}, ok);
    Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, redirect_to_captcha);
    Test(cs, {"POST", "/add_comment", "0 What are you selling?"}, ok);
    Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, redirect_to_captcha);
    Test(
        cs,
        {"GET", "/user_comments", "", {{"user_id", "0"}}},
        {200, {}, "Hello\nWhat are you selling?\n"});
    Test(
        cs,
        {"GET", "/user_comments", "", {{"user_id", "1"}}},
        {200, {}, "Hi\nBuy my goods\nEnlarge\n"});
    Test(
        cs,
        {"GET", "/captcha", {}, {}},
        {200, {}, {"What's the answer for The Ultimate Question of Life, the Universe, and Everything?"}}
        );
    Test(cs, {"POST", "/checkcaptcha", "1 24", {}}, redirect_to_captcha);
    Test(cs, {"POST", "/checkcaptcha", "1 42", {}}, ok);
    Test(cs, {"POST", "/add_comment", "1 Sorry! No spam any more", {}}, ok);
    Test(
        cs,
        {"GET", "/user_comments", "", {{"user_id", "1"}}},
        {200, {}, "Hi\nBuy my goods\nEnlarge\nSorry! No spam any more\n"});

    Test(cs, {"GET", "/user_commntes", {}, {}}, not_found);
    Test(cs, {"POST", "/add_uesr", {}, {}}, not_found);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestServer<CommentServer>);
}
