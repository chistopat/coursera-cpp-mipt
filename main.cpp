#include "iostream"
#include "string"
#include "map"
#include "cinttypes"
#include "deque"
#include "algorithm"
#include "set"
#include "test_runner.h"

typedef std::string THotelName;
typedef int TClientId;

using namespace std;

struct Accomodation {
  int64_t time;
  int user_id;
  int rooms_count;
};

struct Hotel {
  deque<Accomodation> accommodations;
  int booked_rooms_count;
  set<TClientId> clients;
};

class BookingManager {
public:
  void Book(string hotel_name, int64_t time, int user_id, int rooms_count) {
    Accomodation accomodation({time, user_id, rooms_count});
    _CheckOut(time);
    _CheckIn(accomodation, hotel_name);

  }
  const int Rooms(string hotel_name) const {
    int count = 0;
    if (_database.count(hotel_name) == 0) {
      return 0;
    }
    return _database.at(hotel_name).booked_rooms_count;
  }
  const int Clients(string hotel_name) const {
    set<int> users;
    if (_database.count(hotel_name) == 0) {
      return 0;
    }
    return _database.at(hotel_name).clients.size();
  }

private:
  map<THotelName, Hotel> _database;
  const int dayDurationSeconds = 86400;

  void _CheckIn(Accomodation accomodation, const string& hotel_name) {
    _database[hotel_name].accommodations.emplace_back(accomodation);
    _database[hotel_name].clients.insert(accomodation.user_id);
    _database[hotel_name].booked_rooms_count += accomodation.rooms_count;
  }

  void _CheckOut(int64_t time) {
    auto Predicate = [&](Accomodation entry){
      return (time - entry.time) < dayDurationSeconds;
    };
    //iterate by hotels
    for (auto& [hotel_name, hotel] : _database) {
      //move all expired accomodations to tail
      auto firstExpiredIt = std::find_if(begin(hotel.accommodations),
                                      end(hotel.accommodations),
                                      Predicate);
      for (auto it = hotel.accommodations.begin(); it != firstExpiredIt; it++) {
        hotel.booked_rooms_count -= it->rooms_count;
        hotel.clients.erase(it->user_id);
      }
      hotel.accommodations.erase( begin(hotel.accommodations), firstExpiredIt);
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

map<string, CommandTypes> commands_map{
    {"BOOK", CommandTypes::Book},
    {"CLIENTS", CommandTypes::Clients},
    {"ROOMS", CommandTypes::Rooms}
};

int main() {
  TestAll();
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
