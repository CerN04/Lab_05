#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "customMemoryResource.hpp"
#include "forwardList.hpp"

struct Test {
    double a;
    char b;
    bool c;

    friend std::ostream &operator<<(std::ostream &os, const Test &t) {
        os << "RandomStruct {a = " << t.a << ", b = '" << t.b << "', c = " << (t.c ? "true" : "false") << "}";
        return os;
    }
};

int main() {
    try {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        std::cout << "Welcome to Lab5_OOP_8Var_Worker!\n";
        std::cout << "Choose input type: simple(1) or complex(2)?" << std::endl;
        int test_type = 0;
        std::cin >> test_type;
        std::string menu = "0 - Quit;\n1 - Add element;\n2 - Delete element;\n3 - Show forward list;\n4 - Delete forward "
                           "list;\n5 - Menu;\n";

        CustomMemResource resource(1000);

        if (test_type == 1) {
            std::cout << "Thanks! We will test forward list at simple type: int\n";
            ForwardList<int> spis(&resource);
            std::cout << "So, let's start! Choose an option: \n";
            std::cout << menu << std::endl;
            int option;
            std::cin >> option;
            while (option != 0) {
                switch (option) {
                case 1: {
                    int val = rand() % 100;
                    spis.push_front(val);
                    std::cout << "Added: " << val << std::endl;
                    break;
                }
                case 2: {
                    if (!spis.empty()) {
                        spis.pop_front();
                        std::cout << "Deleted first element\n";
                    } else {
                        std::cout << "List is empty!\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "Forward list contents:\n";
                    for (auto iter = spis.begin(); iter != spis.end(); ++iter) {
                        std::cout << *iter << " ";
                    }
                    std::cout << std::endl;
                    break;
                }
                case 4: {
                    spis.clear();
                    std::cout << "Forward list cleared\n";
                    break;
                }
                case 5: {
                    std::cout << menu << std::endl;
                    break;
                }
                }
                std::cin >> option;
            }
        } else if (test_type == 2) {
            std::cout << "Thanks! We will test forward list at complex type: struct\n";
            ForwardList<Test> spis(&resource);
            std::cout << "So, let's start! Choose an option: \n";
            std::cout << menu << std::endl;
            int option;
            std::cin >> option;
            while (option != 0) {
                switch (option) {
                case 1: {
                    Test test;
                    test.a = (rand() % 10000) / 100.0;
                    test.b = static_cast<char>(32 + rand() % 95);
                    test.c = static_cast<bool>(rand() % 2);
                    spis.push_front(test);
                    std::cout << "Added: " << test << std::endl;
                    break;
                }
                case 2: {
                    if (!spis.empty()) {
                        spis.pop_front();
                        std::cout << "Deleted first element\n";
                    } else {
                        std::cout << "List is empty!\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "Forward list contents:\n";
                    for (auto iter = spis.begin(); iter != spis.end(); ++iter) {
                        std::cout << *iter << std::endl;
                    }
                    std::cout << std::endl;
                    break;
                }
                case 4: {
                    spis.clear();
                    std::cout << "Forward list cleared\n";
                    break;
                }
                case 5: {
                    std::cout << menu << std::endl;
                    break;
                }
                }
                std::cin >> option;
            }
        } else {
            throw std::invalid_argument("Bad type choice");
        }

        std::cout << "Bye!" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}