#include <iostream>
#include "query.h"
#include "responses.h"
#include "bus_manager.h"

using namespace std;


int main() {
    // main code
    int query_count;
    Query q;
    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}
// No buses
// No stop
// No bus
// 32 32K
// Stop Vnukovo: 32 32K 950
// Stop Moskovsky: no interchange
// Stop Rumyantsevo: no interchange
// Stop Troparyovo: 950
// Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
// Bus 32: Tolstopaltsevo Marushkino Vnukovo
// Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
// Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
//
//10
//ALL_BUSES
//BUSES_FOR_STOP Marushkino
//STOPS_FOR_BUS 32K
//NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo
//NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
//BUSES_FOR_STOP Vnukovo
//NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
//NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo
//STOPS_FOR_BUS 272
//ALL_BUSES
