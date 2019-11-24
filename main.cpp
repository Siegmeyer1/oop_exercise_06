#include <iostream>
#include <algorithm>
#include <map>

#include "octagon.h"
#include "containers/queue.h"
#include "allocators/allocator.h"

int main() {
    size_t N;
    char option = '0';
    containers::queue<Octagon<int>, allocators::my_allocator<Octagon<int>, 800>> q;
    Octagon<int> oct{};
    while (option != 'q') {
        std::cout << "choose option (m for man, q to quit)" << std::endl;
        std:: cin >> option;
        switch (option) {
            case 'q':
                break;
            case 'm':
                std::cout << "1) push new element into queue\n"
                          << "2) pop element from the queue\n"
                          << "3) delete element from the chosen position\n"
                          << "4) print queue\n"
                          << "5) option fore code testing (for example compatibility with std::map)\n"
                          << std::endl;
                break;
            case '1': {
                std::cout << "enter octagon (have to enter dots consequently): " << std::endl;
                oct = Octagon<int>(std::cin);
                q.push(oct);
                break;
            }
            case '2': {
                q.pop();
                break;
            }
            case '3': {
                std::cout << "enter position to delete: ";
                std::cin >> N;
                q.delete_by_number(N);
                break;
            }
            case '4': {
                std::for_each(q.begin(), q.end(), [](Octagon<int> &X) { X.Printout(std::cout); });
                break;
            }
            case '5': {
                std::map<int, int, std::less<>, allocators::my_allocator<std::pair<const int, int>, 100>> mp;
                for(int i = 0; i < 2; ++i){
                    mp[i] = i * i;
                }
                std::for_each(mp.begin(), mp.end(), [](std::pair<int, int> X) { std::cout << X.first << ' ' << X.second << ",  "; });
                std::cout << std::endl;
                for(int i = 2; i < 10; ++i){
                    mp.erase(i - 2);
                    mp[i] = i * i;
                }
                std::for_each(mp.begin(), mp.end(), [](std::pair<int, int> X) { std::cout << X.first << ' ' << X.second << ",  "; });
                std::cout << std::endl;
                break;
            }
            default:
                std::cout << "no such option. Try m for man" << std::endl;
                break;
        }
    }
    return 0;
}