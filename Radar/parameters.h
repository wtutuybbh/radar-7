#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <cmath>
#include <cstdint>
#include <QPainter>

constexpr uint8_t MARGIN{ 10 };
constexpr uint8_t DATA_SIZE{ 200 };
constexpr uint8_t MAX_XY{ 40 };
constexpr uint8_t GRID_NUMBER{ 10 };
const double PI{ std::acos(-1) };

constexpr QColor backgroundColor{ QColor(77, 77, 77) };
constexpr QColor gridColor      { QColor(45, 45, 45) };
constexpr QColor textColor      { QColor(255, 255, 255) };

#endif // PARAMETERS_H
