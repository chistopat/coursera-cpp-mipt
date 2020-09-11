#include "iostream"
#include "string"
#include "map"
#include "cinttypes"
#include "deque"
#include "algorithm"
#include "set"
#include "test_runner.h"
#include "profile.h"

typedef std::string THotelName;
typedef int TClientId;

using namespace std;

class BookingManager {
public:
  void Book(string hotel_name, int64_t time, int user_id, int rooms_count) {
    Accomodation accomodation({time, user_id, rooms_count, hotel_name});
    CheckOutHotel(time);
    CheckInHotel(accomodation, hotel_name);
  }

  const int Rooms(string hotel_name) const {
    if (_database.count(hotel_name) == 0) {
      return 0;
    }
    return _database.at(hotel_name).booked_rooms_count;
  }
  const int Clients(string hotel_name) const {
    if (_database.count(hotel_name) == 0) {
      return 0;
    }
    return _database.at(hotel_name).clients.size();
  }

private:
  struct Accomodation {
    int64_t time;
    int user_id;
    int rooms_count;
    THotelName hotel_name;
  };

  struct Hotel {
    int booked_rooms_count;
    set<TClientId> clients;
  };

  map<THotelName, Hotel> _database;
  deque<Accomodation> _accommodations;
  const int dayDurationSeconds = 86400;

  void CheckInHotel(Accomodation accomodation, const string& hotel_name) {
    _accommodations.push_front(accomodation);
    _database[hotel_name].clients.insert(accomodation.user_id);
    _database[hotel_name].booked_rooms_count += accomodation.rooms_count;
  }

  void CheckOutHotel(int64_t time) {
    auto Comp = [&](Accomodation entry){
      return (time - entry.time) < dayDurationSeconds;
    };
    auto firstExpiredIt = std::find_if(_accommodations.rbegin(), _accommodations.rend(), Comp);
    for (auto it = _accommodations.rbegin(); it != firstExpiredIt; it++) {
      _database[it->hotel_name].booked_rooms_count -= it->rooms_count;
      _database[it->hotel_name].clients.erase(it->user_id);
      _accommodations.pop_back();
    }
  }
};

void Test() {
  BookingManager bm;
  ASSERT_EQUAL(bm.Clients("Marriott"), 0);
  ASSERT_EQUAL(bm.Rooms("Marriott"), 0);
  bm.Book("FourSeasons", 10, 1, 2);
  bm.Book("Marriott", 10, 2, 1);
  bm.Book("FourSeasons", 86409, 2, 1);
  ASSERT_EQUAL(bm.Rooms("FourSeasons"), 3);
  ASSERT_EQUAL(bm.Clients("FourSeasons"), 2);
  ASSERT_EQUAL(bm.Clients("Marriott"), 1);
  bm.Book("Marriott", 86410, 2, 10);
  ASSERT_EQUAL(bm.Rooms("FourSeasons"), 1);
  ASSERT_EQUAL(bm.Rooms("Marriott"), 10);
}

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, Test);
}

enum class CommandTypes {
  Book,
  Clients,
  Rooms
};

int main() {
  TestAll();

  map<string, CommandTypes> commands_map{
      {"BOOK", CommandTypes::Book},
      {"CLIENTS", CommandTypes::Clients},
      {"ROOMS", CommandTypes::Rooms}
  };

  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n = 0;
  std::cin >> n;
  BookingManager bookingManager;
  while (n--) {
    string command;
    std::cin >> command;
    string hotel_name;
    switch (commands_map[command]) {
      case CommandTypes::Book: {
        int64_t time = 0;
        int client_id = 0;
        int room_count = 0;
        cin >> time >> hotel_name >> client_id >> room_count;
        bookingManager.Book(hotel_name, time, client_id, room_count);
        break;
      }
      case CommandTypes::Rooms:
        cin >> hotel_name;
        cout << bookingManager.Rooms(hotel_name) << '\n';
        break;
      case CommandTypes::Clients:
        cin >> hotel_name;
        cout << bookingManager.Clients(hotel_name) << '\n';
        break;
    }

  }
}
