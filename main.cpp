#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
#include <string>

using namespace std;

struct Order {
    uint64_t timestamp;
    uint32_t type;
    uint32_t order_id;
    uint32_t size;
    uint64_t price;
    int32_t side;
};

Order parse_csv_line(string line) {

    Order order;

    uint32_t l = 0;
    uint32_t r = line.find(','); // first comma

    string convert_str = line.substr(l, r - l);

    // remove decimal dot
    // and convert to seconds.fraction of second
    uint32_t n = convert_str.find('.');

    string seconds = convert_str.substr(0, n);
    string fraction = convert_str.substr(n + 1, r);
    order.timestamp = stoull(convert_str);

    l = r + 1;
    r = line.find(',', l); // second comma

    convert_str = line.substr(l, r - l);
    order.type = stoi(convert_str);
    l = r + 1;
    r = line.find(',', l); // third comma

    convert_str = line.substr(l, r - l);
    order.order_id = stoi(convert_str);
    l = r + 1;
    r = line.find(',', l); // fourth comma

    convert_str = line.substr(l, r - l);
    order.size = stoi(convert_str);
    l = r + 1;
    r = line.find(',', l); // fifth comma

    convert_str = line.substr(l, r - l);
    order.price = stoull(line.substr(l, r - l));

    convert_str = line.substr(r + 1);
    order.side = stoi(convert_str);

    return order;
}

int main(int argc, char **argv) {

    string messages_file = argv[1];

    fstream fin;
    fin.open(messages_file, ios::in);
    string line;

    vector<Order> orders;
    orders.reserve(1000000);

    while (getline(fin, line)) {
        Order order = parse_csv_line(line);
        orders.emplace_back(order);
    }
    fin.close();

    // cout << orders.size() << endl;

    // track bids and asks
    //
    //

    // Map by price?
    map<uint64_t, Order> bids;
    map<uint64_t, Order> asks;

    for (auto &order : orders) {
        switch (order.type) {
        // 1: Submission of a new limit order
        // 2: Cancellation (Partial deletion of a limit order)
        // 3: Deletion (Total deletion of a limit order)
        // 4: Execution of a visible limit order
        // 5: Execution of a hidden limit order
        // 7: Trading halt indicator
        case 1: {
            if (order.side == 1) {
                bids[order.price] = order;
            } else {
                asks[order.price] = order;
            }
        } break;
        case 2: {
            if (order.side == 1) {
                bids.erase(order.price);
            } else {
                asks.erase(order.price);
            }
        } break;
        case 3: {
            if (order.side == 1) {
                bids.erase(order.price);
            } else {
                asks.erase(order.price);
            }
        } break;
        case 4: {

        } break;
        case 5: {

        } break;
        case 6: {

        } break;
        case 7: {

        } break;
        }
    }

    return 0;
}
