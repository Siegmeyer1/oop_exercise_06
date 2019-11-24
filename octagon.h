#ifndef OOP_LAB5_OCTAGON_H
#define OOP_LAB5_OCTAGON_H

#include "vertex.h"

template <class T>
class Octagon {
public:
    vertex<T> dots[8];

    explicit Octagon<T>(std::istream& is) {
        for (auto & dot : dots) {
            is >> dot;
        }
    }

    Octagon<T>() = default;

    double Area() {
        double res = 0;
        for (size_t i = 0; i < 7; i++) {
            res += (dots[i].x * dots[i+1].y) - (dots[i+1].x * dots[i].y);
        }
        res = res + (dots[7].x * dots[0].y) - (dots[0].x * dots[7].y);
        return std::abs(res)/ 2;
    }

    void Printout(std::ostream& os) {
        for (int i = 0; i < 8; ++i) {
            os << this->dots[i];
            if (i != 7) {
                os << ", ";
            }
        }
        os << std::endl;
    }

    void operator<< (std::ostream& os) {
        for (int i = 0; i < 8; ++i) {
            os << this->dots[i];
            if (i != 7) {
                os << ", ";
            }
        }
    }
};

#endif //OOP_LAB5_OCTAGON_H
