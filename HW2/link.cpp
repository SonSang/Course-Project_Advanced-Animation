#include "link.hpp"

void link::setTR(const transformation &TR) {
    this->linkTR = TR;
}
const transformation& link::getTR() const noexcept {
    return this->linkTR;
}
void link::translate(double x, double y, double z) {
    this->linkTR.translate(x, y, z);
}