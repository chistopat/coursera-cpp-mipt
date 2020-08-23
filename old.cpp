//#include <iostream>
//#include "string"
//#include "vector"
//#include "sstream"
//#include "set"
//#include "map"
//#include "iomanip"
//
//using namespace std;
//
//vector<string> SplitString(const string& data, char delimiter = ' ') {
//    stringstream stream(data);
//    vector<string> tokens;
//    string token;
//    while (getline(stream, token, delimiter)) {
//        tokens.push_back(token);
//    }
//    return tokens;
//}
//
//class Date {
//public:
//    Date() {
//        _day = 1;
//        _month = 1;
//        _year = 1;
//    }
//    Date(const string& dateString) {
//        vector<string> dateTokens =  SplitString(dateString, '-');
//        if (dateTokens.size() == 4 and dateTokens[0].empty()) {
//            dateTokens.erase(dateTokens.begin());
//        }
//        if (dateTokens.size() == 3) {
//            int value = stoi(dateTokens[0]);
//            _year = value;
//
//            value = stoi(dateTokens[1]);
//            if (value >= 1 and value <= 12) {
//                _month = value;
//            } else {
//                throw invalid_argument("Month value is invalid: " + dateTokens[1]);
//            }
//
//            value = stoi(dateTokens[2]);
//            if (value >= 1 and value <= 31) {
//                _day = value;
//            } else {
//                throw invalid_argument("Day value is invalid: " + dateTokens[2]);
//            }
//        } else {
//            throw invalid_argument("Wrong date format: " + dateString);
//        }
//
//    }
//    int GetYear() const {
//        return _year;
//    }
//    int GetMonth() const {
//        return _month;
//    }
//    int GetDay() const{
//        return _day;
//    }
//
//    friend ostream& operator<<(ostream& stream, const Date& date) {
//        stream << setw(4) << setfill('0') << date._year << '-'
//                << setw(2) << date._month << '-'
//                << setw(2) << date._day;
//        return stream;
//    }
//    friend bool operator<(const Date& lhs, const Date& rhs);
//private:
//    int _year;
//    int _month;
//    int _day;
//};
//
//bool operator<(const Date& lhs, const Date& rhs) {
//
//    if (lhs.GetYear() == rhs.GetYear()) {
//        if (lhs.GetMonth() == rhs.GetMonth()) {
//            return (lhs.GetDay() < lhs.GetDay());
//        } else {
//            return (lhs.GetMonth() < rhs.GetMonth());
//        }
//    }
//    return (lhs.GetYear() < lhs.GetYear());
//}
//
//class Database {
//public:
//    void AddEvent(const Date& date, const string& event) {
//        _db[date] = {event};
//    }
//    bool DeleteEvent(const Date& date, const string& event){
//        if (!_db.empty() and _db.at(date).count(event)>0) {
//            cout << "Deleted successfully" << endl;
//            return true;
//        } else {
//            cout << "Event not found" << endl;
//            return false;
//        }
//        _db[date].erase(event);
//    }
//    int DeleteDate(const Date& date) {
//        int n = 0;
//        if (!_db.empty()) {
//            if (!_db.at(date).empty()) {
//                n = _db.at(date).size();
//            }
//            _db.erase(date);
//        }
//        cout << "Deleted " << n << " events" << endl;
//        return n;
//    }
//    void Find(const Date& date) const {
//        if (!_db.empty()) {
//            for (auto &e : _db.at(date)) {
//                cout << e << endl;
//            }
//        }
//    }
//    void Print() const {
//        for (auto& [date, events] : _db) {
//            cout << date << ' ';
//            for (auto& e : events) {
//                cout << e << ' ';
//            }
//            cout << endl;
//        }
//    }
//private:
//    map<Date, set<string>> _db;
//};
//
//struct Command {
//    string operation;
//    Date date;
//    string event;
//};
//
//Command ParseArguments(vector<string>& args) {
//    Command command;
//        command.operation = args[0];
//    if (args.size() >= 2) {
//        command.date = Date(args[2]);
//        command.event = args[2];
//    }
//    return command;
//
//}
//
//int main() {
//    Database db;
//    string userInput;
//
//    vector<Date> test = {Date{"2020-01-01"}, Date{"2020-02-01"}, Date{"1-1-1"}};
//    bool test1 = ( Date{"1-1-1"} < Date{"10-2-1"});
//    cout << test1;
//
//    while (getline(cin, userInput)) {
//        vector<string> arguments = SplitString(userInput);
//        if (arguments.empty()) {
//            continue;
//        }
//
//        try {
//            Command command = ParseArguments(arguments);
//            if (command.operation == "Add") {
//                db.AddEvent(command.date, command.event);
//            } else if (command.operation == "Del") {
//                if (command.event.empty()) {
//                    db.DeleteDate(command.date);
//                } else {
//                db.DeleteEvent(command.date, command.event);
//                }
//            } else if (command.operation == "Find") {
//                db.Find(command.date);
//            } else if (command.operation == "Print") {
//                db.Print();
//            } else {
//                throw invalid_argument("Unknown command: " + userInput);
//            }
//        } catch (const invalid_argument& ex ) {
//            cout << ex.what() << endl;
//        }
//    }
//
//    return 0;
//}


// Реализуйте функции и методы классов и при необходимости добавьте свои

#include <algorithm>
#include "iostream"
#include "set"
#include "string"
#include "vector"
#include "map"
#include "sstream"
#include "iomanip"

using namespace std;


//class Date {
//public:
//    Date() {
//        _year = 0;
//        _day = 0;
//        _month = 0;
//    }
//
//    }
//    [[nodiscard]] int GetYear() const {
//        return _year;
//    }
//    [[nodiscard]] int GetMonth() const {
//        return _month;
//    }
//    [[nodiscard]] int GetDay() const {
//        return _day;
//    }
//private:
//    int _year;
//    int _month;
//    int _day;
//};

bool operator<(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        } else {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    } else {
        return lhs.GetYear() < rhs.GetYear();
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event){
        if (!_db.empty() && _db.count(date) > 0) {
            _db[date].insert(event);
        } else {
            _db[date] = {event};
        }
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (!_db.empty() or _db.count(date)>0) {
            if (_db[date].count(event) > 0) {
                _db[date].erase(event);
                return true;
            }
        }
        return false;
    }

    int  DeleteDate(const Date& date) {
        int n = 0;
        if (!_db.empty() and _db.count(date)>0) {
            n = _db.at(date).size();
            _db.erase(date);
        }
        return n;
    }

    [[nodiscard]] set<string> Find(const Date& date) const {
        if (!_db.empty() and _db.count(date) > 0) {
            return _db.at(date);
        }
        return {};
    }

    void Print() const {
        for (auto& [date, events] : _db) {
            stringstream stream;
            stream << setw(4) << setfill('0') << date.GetYear() << '-'
                   << setw(2) << setfill('0') << date.GetMonth() << '-'
                   << setw(2) << setfill('0') << date.GetDay() << ' ';
            string sDate;
            stream >> sDate;
            for(auto& e : events) {
                cout<< sDate<< ' ' << e << endl;
            }

        }
    }

private:
    map<Date, set<string>> _db;
};

vector<string> SplitString(const string& str, char delimiter=' ') {
    stringstream stream(str);
    vector<string> results;
    string token;
    while (getline(stream, token, delimiter)) {
        if (!token.empty()) {
            results.push_back(token);
        }
    }
    return results;
}

bool CheckDisallowedChars(const string& dateString) {
    bool result = false;
    const set<char> allowedChars = {'+', '-', '\n', EOF};
    const set<char> allowedDigits = {'0', '1', '2', '3', '4', '5', '6', '7', '9'};
    for (auto& c : dateString) {
        if(allowedChars.count(c) == 0 and allowedDigits.count(c) == 0) {
            result = true;
        }
    }
    //Проверяем что последний символ цифра
    if (allowedDigits.count(dateString[dateString.size()-1]) == 0) {
        result = true;
    }
    return result;
}

Date ParseDate(const string& dateString){
    int year = 0;
    int month = 0;
    int day = 0;
    char dlm1 = '-';
    char dlm2 = '-';
    stringstream stream(dateString);
    stream >> year >> dlm1 >> month >> dlm2 >> day;

    if(!stream.fail()) {
        if (stream.peek() == EOF) {
            if (dlm1=='-' and dlm2=='-') {
                return Date(year, month, day);
            }
        }
    }
    throw invalid_argument("Wrong date format: " + dateString);
}

//void GetCurrentInt(stringstream& s, vector<int>& results) {
//    string current;
//    s >> current;
//    try {
//        int iCurrent = stoi(current);
//        results.push_back(iCurrent);
//    } catch (invalid_argument& ex) {
//        cout << "FAIL: " << current << endl;
//    }
//
//}
//
//bool EnsureIgnoreDelimiter(stringstream& s, char delimiter = '-') {
//    if (s.peek() == delimiter) {
//        s.ignore(1);
//        return true;
//    }
//    return false;
//}
//
//vector<int> ParseIntsFromString(const string& str, char delimiter = '-') {
//    vector<int> results = {};
//    bool isCorrect = true;
//    if (!str.empty()) {
//        stringstream stream(str);
//        while(stream and isCorrect) {
//            GetCurrentInt(stream, results);
//            isCorrect = EnsureIgnoreDelimiter(stream);
//        }
//    }
//    return results;
//    return {}; //ToDo: delete
//    throw invalid_argument("Wrong date format: " + str);
//
//}


bool RunCommand(const vector<string>& args, Database& database) {
    string command = args[0];
    if (command == "Add") {
        Date date = ParseDate(args[1]);
        string event = args[2];
        database.AddEvent(date, event);
    } else if (command == "Del") {
        if(args.size() == 2) {
            Date date = ParseDate(args[1]);
            int n = database.DeleteDate(date);
            cout  <<"Deleted " << n << " events" << endl;
        } else if (args.size() == 3) {
            Date date = ParseDate(args[1]);
            string event = args[2];
            bool result = database.DeleteEvent(date, event);
            if (result) {
                cout << "Deleted successfully"<< endl;
            } else {
                cout << "Event not found"<< endl;
            }
        }
    } else if (command == "Find") {
        Date date = ParseDate(args[1]);
        auto events = database.Find(date);
        for (auto& e : events) {
            cout << e<< endl;
        }
    } else if (command == "Print") {
        database.Print();
    } else {
        throw invalid_argument("Unknown command: " + command);
    }
    return true;
}

void PrintVector(const vector<string>& container){
    cout << '{';
    for (auto& item : container) {
        cout << '"' << item << "\",";
    }
    cout << '}';
}

void PrintVectorInt(const vector<int>& container){
    cout << '{';
    for (auto& item : container) {
        cout << '"' << item << "\",";
    }
    cout << '}';
}

void SplitTests() {
    map<string, vector<string>> splitTests = {
            {"", {}},
            {"foo",  {"foo"}},
            {"  bar", {"bar"}},
            {"Add 123 sdf", {"Add", "123", "sdf"}},
            {"1 1 1", {"1", "1", "1"}},
            {" foo bar buzz", {"foo", "bar", "buzz"}},
            {"foo  bar  buzz", {"foo", "bar", "buzz"}},
            {"foo bar buz  ", {"foo", "bar", "buz"}}

    };
    for (auto& [test, exp]: splitTests) {
        auto result = SplitString(test);
        if (result != exp) {
            cout << "[FAIL] Test: \"" << test << "\" \t\t\t";
            PrintVector(result);
            cout << endl;
        }
    }
}

void ParseData() {

}

void ParseDataTests() {
    map<string, vector<int>> tests = {
            {"", {}},
            {"foo",  {}},
            {"1", {}},
            {"1+", {}},
            {"1+1", {}},
            {"+++1", {}},
            {"1-1-1-", {}},
            {"1--1-1", {1, -1, 1}},
            {"1---1-1", {}},
            {"1--1--1", {1, -1, -1}},
            {"1-1-1", {1, 1, 1}},
            {"-1-1-1", {-1, 1, 1}},
            {"1-+1-+1", {1, 1, 1}},
            {"2020-10-11", {2020, 10, 11}},
            {"2020-10-01", {2020, 10, 1}},
            {"0-1-1", {0, 1, 1}},
            {"0-t-1", {0, 1, 1}},
            {"0-1-t", {0, 1, 1}},
            {"0-1-1)", {0, 1, 1}},
            {"0-1a-1", {0, 1, 1}},

    };
//    for (auto& [test, exp]: tests) {
//        auto result = ParseIntsFromString(test);
//
//        if (result != exp) {
//            cout << "[FAIL] Test: \"" << test << "\" \t\t\t";
//            PrintVectorInt(result);
//            cout << endl;
//        }
//    }
}

void ConstructDateTests() {
    vector<vector<int>> tests = {
            {1, 1, 1},
            {-1, 1, 1},
            {0, 0, 0},
            {1, 0, 1},
            {1, 1, 0},
            {1, 13, 1},
            {1, 13, 32},
            {1, -1, 1},
            {1, 1, -1},
    };
    vector<vector<int>> exp = {
            {1, 1, 1},
            {-1, 1, 1},
            {},
            {},
            {},
            {},
            {},
            {},
            {},
    };
    for (size_t i=0; i<tests.size(); ++i) {
        vector<int> result;
        try {
            Date date(tests[i][0], tests[i][1], tests[i][2]);
            result = {date.GetYear(), date.GetMonth(), date.GetDay()};
        } catch (invalid_argument& ex) {
            result = {};
        }
        if (result != exp[i]) {
            cout << "[FAIL] Test: ";
            PrintVectorInt(result);
            cout << "\t\t\t";
            PrintVectorInt(exp[i]);
            cout << endl;
        }
    }
}

void CompareLessTests() {
    if (Date(1, 1, 1) < Date(-1, 1, 1)) {
        cout << "Fail 1" << endl;
    }
    if (Date(1, 1, 2) < Date(1, 1, 1)) {
        cout << "Fail 2" << endl;
    }
    if (Date(1, 2, 1) < Date(1, 1, 1)) {
        cout << "Fail 3" << endl;
    }
    if (Date(2, 1, 1) < Date(1, 1, 1)) {
        cout << "Fail 4" << endl;
    }
    if (Date(2020, 10, 1) < Date(2020, 1, 1)) {
        cout << "Fail 5" << endl;
    }
}

void UnitTests() {
    SplitTests();
    ParseDataTests();
    ConstructDateTests();
    CompareLessTests();
}

//int main() {
////    UnitTests(); //Todo: delete before review
//    Database db;
//    string userInput;
//    while (getline(cin, userInput)) {
//        vector<string> args = SplitString(userInput);
//        if (args.empty()) {
//            continue;
//        }
//        try {
//            bool res = RunCommand(args, db);
//            if(!res) {
//                continue;
//            }
//        } catch (invalid_argument& ex) {
//            cout << ex.what();
//            break;
//        }
//        // Считайте команды с потока ввода и обработайте каждую
//    }
//
//    return 0;
//}