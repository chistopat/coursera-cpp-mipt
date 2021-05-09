#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_worker) {
            next_worker->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_worker = move(next);
    }
    unique_ptr<Worker> next_worker;
};


class Reader : public Worker {
public:
    Reader(istream& in) : _in(in) {
    }

    void Run() override {
        while(_in) {
            auto email = make_unique<Email>();
            getline(_in, email->from);
            getline(_in, email->to);
            getline(_in, email->body);
            if (_in) {
                PassOn(move(email));
            }
        }
    }

    void Process(unique_ptr<Email> email) override {}

private:
    istream& _in;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

public:
    Filter(const Filter::Function& filter) : filter_(filter) {

    }
    void Process(unique_ptr<Email> email) override {
        if (filter_(*email) && next_worker) {
            PassOn(move(email));
        }
    }
private:
    Function filter_;
};


class Copier : public Worker {
public:
    explicit Copier(string recipient) : copy_to(recipient) {

    }

    void Process(unique_ptr<Email> email) override {
        if (!next_worker) {
            return void();
        }

        if (email->to != copy_to) {
            Email duplicate {email->from, copy_to, email->body};
            PassOn(move(email));
            PassOn(make_unique<Email>(duplicate));
        } else {
            PassOn(move(email));
        }


    }
private:
    std::string copy_to;
};


class Sender : public Worker {
public:
    explicit Sender(ostream& out) : out_(out) {

    }

    void Process(unique_ptr<Email> email) override {
        out_ << email->from << '\n'
          << email->to << '\n'
          << email->body << '\n';
        if (next_worker) {
            PassOn(move(email));
        }

    }

private:
    ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        chain.push_back(make_unique<Reader>(in));
        last_worker = chain.front().get();
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        last_worker->next_worker = make_unique<Filter>(filter);
        last_worker = last_worker->next_worker.get();
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        last_worker->next_worker = make_unique<Copier>(recipient);
        last_worker = last_worker->next_worker.get();
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        last_worker->next_worker = make_unique<Sender>(out);
        last_worker = last_worker->next_worker.get();
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(chain.front());
    }


private:
    Worker* last_worker;
    vector<unique_ptr<Worker>> chain;
};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
      return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

void TestReader() {
    string input = "\n\n\n\n";
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = "\n\n\n";

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    RUN_TEST(tr, TestReader);
    return 0;
}
