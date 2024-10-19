#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Order {
    uint32_t type;
    uint32_t order_id;
    uint32_t size;
    int32_t side;
    uint64_t price;
    uint64_t timestamp;
};

// Overload new operator to count memory allocations
static uint64_t GLOBAL_allocation_counter = 0;
void *operator new(std::size_t size) {
    GLOBAL_allocation_counter++;
    return malloc(size);
}

Order parse_csv_line(const std::string &line) {

    Order order;

    uint32_t l = 0;
    uint32_t r = line.find(','); // first comma
    std::string_view convert_str(line.c_str(), r);
    order.timestamp = std::strtoull(convert_str.data(), 0, 10);

    l = r + 1;
    r = line.find(',', l); // second comma
    convert_str = std::string_view(line.c_str() + l, r);
    order.type = std::strtol(convert_str.data(), 0, 10);

    l = r + 1;
    r = line.find(',', l); // third comma
    convert_str = std::string_view(line.c_str() + l, r);
    order.order_id = std::strtol(convert_str.data(), 0, 10);

    l = r + 1;
    r = line.find(',', l); // third comma
    convert_str = std::string_view(line.c_str() + l, r);
    order.size = std::strtol(convert_str.data(), 0, 10);

    l = r + 1;
    r = line.find(',', l); // third comma
    convert_str = std::string_view(line.c_str() + l, r);
    order.price = std::strtoull(convert_str.data(), 0, 10);

    l = r + 1;
    convert_str = std::string_view(line.c_str() + l, line.size());
    order.side = std::strtol(convert_str.data(), 0, 10);

    return order;
}

int main(int argc, char **argv) {

    const char *messages_file = argv[1];

    std::fstream fin;
    fin.open(messages_file, std::ios::in);
    std::string line;

    std::vector<Order> orders(1000000);

    while (getline(fin, line)) {
        Order order = parse_csv_line(line);
        orders.emplace_back(order);
    }
    fin.close();

    for (auto &order : orders) {
        switch (order.type) {
        // 1: Submission of a new limit order
        // 2: Cancellation (Partial deletion of a limit order)
        // 3: Deletion (Total deletion of a limit order)
        // 4: Execution of a visible limit order
        // 5: Execution of a hidden limit order
        // 7: Trading halt indicator

        // Note: Too many allocations when putting into map
        case 1: {
            if (order.side == 1) {
            } else {
            }
        } break;
        case 2: {
            if (order.side == 1) {
            } else {
            }
        } break;
        case 3: {
            if (order.side == 1) {
            } else {
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

    std::cout << "Allocation counter: " << GLOBAL_allocation_counter
              << std::endl;

    return 0;
}
