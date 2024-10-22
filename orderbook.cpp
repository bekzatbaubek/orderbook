#include <cstdint>
#include <iostream>
#include <map>
#include <queue>

void printUsage() {
    std::cout << "Orderbook" << '\n';
    std::cout << "Options:" << '\n';
    std::cout << "p:  Print ordebook" << '\n';
    std::cout << "o:  Place order" << '\n';
    std::cout << "q:  Quit" << '\n';
}

struct Order {
    uint64_t id;
    uint64_t price; // in 100th of a cent -> $1.23 = 12300, to avoid using
                    // floating point numbers
    uint64_t quantity;
    Order(uint64_t iPrice, uint64_t iQuantity) {
        price = iPrice;
        quantity = iQuantity;
    }
};

struct OrderQueue {
    std::queue<Order> orders;
    uint64_t totalQuantity;
};

enum Side { BUY, SELL };

struct Orderbook {
    std::map<uint64_t, OrderQueue> bids;
    std::map<uint64_t, OrderQueue> asks;

    void addOrder(Order order, Side side) {
        switch (side) {
        case BUY: {
            if (bids.find(order.price) == bids.end()) {
                OrderQueue orderQueue;
                orderQueue.orders.push(order);
                orderQueue.totalQuantity = order.quantity;
                bids[order.price] = orderQueue;
            } else {
                bids[order.price].orders.push(order);
                bids[order.price].totalQuantity += order.quantity;
            }
        } break;
        case SELL: {
            if (asks.find(order.price) == asks.end()) {
                OrderQueue orderQueue;
                orderQueue.orders.push(order);
                orderQueue.totalQuantity = order.quantity;
                asks[order.price] = orderQueue;
            } else {
                asks[order.price].orders.push(order);
                asks[order.price].totalQuantity += order.quantity;
            }
        } break;
        }
    }

    bool match(Order order, Side side) {
        switch (side) {
        case BUY: {
            // Find the largest ask not larger than order
            // Match from smallest until largest in FIFO order
            // Put remaining quantity as bid
            return false;
        } break;
        case SELL: {
            // Find the smallest bid
            return false;
        } break;
        }
        return false;
    }
};

void fillTestData(Orderbook &orderbook) {
    for (int i = 90000; i <= 10000; i+=1000) {
        Order order = Order(i, 50);
        orderbook.addOrder(order, BUY);
    }
    for (int i = 10100; i <= 11000; i+=1000) {
        Order order = Order(i, 30);
        orderbook.addOrder(order, SELL);
    }
}

int main(int argc, char **argv) {
    bool running = true;

    Orderbook orderbook;
    fillTestData(orderbook);

    while (running) {
        std::cout << "Simple Limit Orderbook" << '\n';
        std::cout << "Enter a command, \'h\' for help:" << '\n';
        char option;
        std::cin >> option;

        switch (option) {
        case 'p': {
            std::cout << "Print orderbook" << '\n';
        } break;
        case 'o': {
            std::cout << "Place order" << '\n';
        } break;
        case 'q': {
            std::cout << "Quit" << '\n';
            running = false;
        } break;
        case 'h': {
            printUsage();
        } break;
        default: {
            printUsage();
        } break;
        }
    }

    return 0;
}
