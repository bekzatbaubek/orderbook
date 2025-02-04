#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <sys/types.h>

void printUsage() {
    std::cout << "Orderbook" << '\n';
    std::cout << "Options:" << '\n';
    std::cout << "p: Print ordebook" << '\n';
    std::cout << "o: Place order" << '\n';
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
            std::cout << "Adding order to bids at: " << order.price <<", quantity: " << order.quantity << '\n';
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
            std::cout << "Adding order to asks at: " << order.price <<", quantity: " << order.quantity << '\n';
        } break;
        }
    }

    void match(Order order, Side side) {
        switch (side) {
        case BUY: {
            // Find the largest ask not larger than order
            // Match from smallest until largest in FIFO order
            // Put remaining quantity as bid
            if (asks.empty()) {
                // No asks available, put order in bids
                addOrder(order, BUY);
                return;
            }
            std::vector<uint64_t> toRemove;
            toRemove.reserve(asks.size());

            auto it = asks.begin();

            for (auto it = asks.begin(); it != asks.end(); it++) {
                // Match price category
                if (order.price >= it->first) {
                    // Match FIFO, per order in queue
                    while (!it->second.orders.empty() && order.quantity > 0) {
                        Order ask = it->second.orders.front();
                        if (ask.quantity > order.quantity) {
                            // Partial fill
                            ask.quantity -= order.quantity;
                            it->second.totalQuantity -= order.quantity;
                            order.quantity = 0;
                            std::cout << "Order matched at price: " << ask.price << ", quantity: " << order.quantity << '\n';
                        } else {
                            // Full fill
                            order.quantity -= ask.quantity;
                            it->second.totalQuantity -= ask.quantity;
                            it->second.orders.pop();
                            std::cout << "Order matched at price: " << ask.price << ", quantity: " << ask.quantity << '\n';
                        }
                    }
                    if (it->second.totalQuantity == 0) {
                        // Remove price category
                        toRemove.push_back(it->first);
                    }
                } else {
                    // No more asks available
                    addOrder(order, BUY);
                    return;
                }
            }
            if (order.quantity > 0) {
                addOrder(order, BUY);
            }
            for (auto price : toRemove) {
                asks.erase(price);
            }
            return;
        } break;
        case SELL: {
            // Find the smallest bid
            return;
        } break;
        }
        return;
    }
    void printOrderbook() {
        std::cout << "Bids" << '\n';
        for (auto it = bids.rbegin(); it != bids.rend(); it++) {
            std::cout << it->first << " " << it->second.totalQuantity << '\n';
        }
        std::cout << "Asks" << '\n';
        for (auto it = asks.begin(); it != asks.end(); it++) {
            std::cout << it->first << " " << it->second.totalQuantity << '\n';
        }
    }
};

void fillTestData(Orderbook &orderbook) {
    for (int i = 9000; i <= 9500; i += 100) {
        Order order = Order(i, 20);
        orderbook.addOrder(order, BUY);
        Order nextOrder = Order(i, 30);
        orderbook.addOrder(nextOrder, BUY);
    }
    for (int i = 10100; i <= 10500; i += 100) {
        Order order = Order(i, 10);
        orderbook.addOrder(order, SELL);
        Order nextOrder = Order(i, 20);
        orderbook.addOrder(nextOrder, SELL);
    }
}

void placeOrder(const Orderbook &orderbook) {}

int main(int argc, char **argv) {
    bool running = true;

    Orderbook orderbook;
    fillTestData(orderbook);

    orderbook.printOrderbook();
    Order order = Order(10600, 78);
    orderbook.match(order, BUY);
    orderbook.printOrderbook();

    // while (running) {
    //     std::cout << "Simple Limit Orderbook" << '\n';
    //     std::cout << "Enter a command, \'h\' for help:" << '\n';
    //     char option;
    //     std::cin >> option;

    //     switch (option) {
    //     case 'p': {
    //         orderbook.printOrderbook();
    //     } break;
    //     case 'o': {
    //         std::cout << "Place order" << '\n';
    //         Order order = Order(10200, 20);
    //         orderbook.match(order, BUY);
    //     } break;
    //     case 'q': {
    //         std::cout << "Quit" << '\n';
    //         running = false;
    //     } break;
    //     case 'h': {
    //         printUsage();
    //     } break;
    //     }
    // }

    return 0;
}
