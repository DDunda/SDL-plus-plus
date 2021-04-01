#include "rect.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

using namespace SDL;

#pragma region FPoint
FPoint::FPoint() : x(0), y(0) {}
FPoint::FPoint(float x, float y) : x(x), y(y) {}
FPoint::FPoint(const FPoint& v) : point(v.point) {}
FPoint::FPoint(const SDL_FPoint& point) : point(point) {}

FPoint FPoint::FromAngle(float angle, float mag) { return { cosf(angle) * mag, sinf(angle) * mag }; }

FPoint::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

float FPoint::sqrMag() const { return x * x + y * y; }
float FPoint::mag() const { return std::sqrt(x * x + y * y); }
float FPoint::min() const { return std::min(x, y); }
float FPoint::max() const { return std::max(x, y); }

bool FPoint::nonZero() const { return x != 0 || y != 0; }
bool FPoint::inRect(const FRect& r) const { return x >= r.x && y >= r.y && x < r.x + r.w && y < r.y + r.h; }

FPoint FPoint::turn(float a) const { return { x * cos(a) - y * sin(a), x * sin(a) + y * cos(a) }; }
FPoint FPoint::perp() const { return { -y, x }; }
FPoint FPoint::norm() const { float size = mag(); return size == 0.0f ? FPoint(0, 0) : FPoint(x / size, y / size); }
FPoint FPoint::abs() const { return { std::abs(x), std::abs(y) }; }

Point FPoint::floor() const { return { (int)std::floor(x), (int)std::floor(y) }; }
Point FPoint::round() const { return { (int)std::round(x), (int)std::round(y) }; }
Point FPoint::ceil()  const { return { (int)std::ceil(x),  (int)std::ceil(y) }; }

FPoint FPoint::clampX(float x1, float x2) const { return { std::clamp(x, x1, x2), y }; }
FPoint FPoint::clampY(float y1, float y2) const { return { x, std::clamp(y, y1, y2) }; }
FPoint FPoint::clampR(float r1, float r2) const { float size = mag(); float r = std::min(0.0f, std::clamp(size, r1, r2)); if (size == r) return FPoint(*this); else return *this * (r / size); }

float FPoint::dot(const FPoint& v1, const FPoint& v2) { return v1.x * v2.x + v1.y * v2.y; }
float FPoint::cross(const FPoint& v1, const FPoint& v2) { return v1.y * v2.x - v1.x * v2.y; }
FPoint FPoint::min(const FPoint& v1, const FPoint& v2) { return { std::min(v1.x, v2.x), std::min(v1.y, v2.y) }; }
FPoint FPoint::max(const FPoint& v1, const FPoint& v2) { return { std::max(v1.x, v2.x), std::max(v1.y, v2.y) }; }

#pragma region FPoint arithmetic
FPoint FPoint::operator+() const { return { +x, +y }; }
FPoint FPoint::operator-() const { return { -x, -y }; }

FPoint FPoint::operator+(const FPoint& v) const { return { x + v.x, y + v.y }; }
FPoint FPoint::operator-(const FPoint& v) const { return { x - v.x, y - v.y }; }
FPoint FPoint::operator*(const FPoint& v) const { return { x * v.x, y * v.y }; }
FPoint FPoint::operator/(const FPoint& v) const { return { x / v.x, y / v.y }; }

FPoint FPoint::operator+(const Point& v) const { return { x + v.x, y + v.y }; }
FPoint FPoint::operator-(const Point& v) const { return { x - v.x, y - v.y }; }
FPoint FPoint::operator*(const Point& v) const { return { x * v.x, y * v.y }; }
FPoint FPoint::operator/(const Point& v) const { return { x / v.x, y / v.y }; }

FPoint& FPoint::operator+=(const FPoint& v) { x += v.x; y += v.y; return *this; }
FPoint& FPoint::operator-=(const FPoint& v) { x -= v.x; y -= v.y; return *this; }
FPoint& FPoint::operator*=(const FPoint& v) { x *= v.x; y *= v.y; return *this; }
FPoint& FPoint::operator/=(const FPoint& v) { x /= v.x; y /= v.y; return *this; }

FPoint& FPoint::operator+=(const Point& v) { x += v.x; y += v.y; return *this; }
FPoint& FPoint::operator-=(const Point& v) { x -= v.x; y -= v.y; return *this; }
FPoint& FPoint::operator*=(const Point& v) { x *= v.x; y *= v.y; return *this; }
FPoint& FPoint::operator/=(const Point& v) { x /= v.x; y /= v.y; return *this; }

FPoint FPoint::operator*(double m) const { return { float(x * m), float(y * m) }; }
FPoint FPoint::operator/(double m) const { return { float(x / m), float(y / m) }; }
FPoint FPoint::operator*(float m ) const { return { x * m, y * m }; }
FPoint FPoint::operator/(float m ) const { return { x / m, y / m }; }
FPoint FPoint::operator*(int m   ) const { return { x * m, y * m }; }
FPoint FPoint::operator/(int m   ) const { return { x / m, y / m }; }

FPoint& FPoint::operator*=(double m) { x *= m; y *= m; return *this; }
FPoint& FPoint::operator/=(double m) { x /= m; y /= m; return *this; }
FPoint& FPoint::operator*=(float m ) { x *= m; y *= m; return *this; }
FPoint& FPoint::operator/=(float m ) { x /= m; y /= m; return *this; }
FPoint& FPoint::operator*=(int m   ) { x *= m; y *= m; return *this; }
FPoint& FPoint::operator/=(int m   ) { x /= m; y /= m; return *this; }

FPoint SDL::operator*(double m, const FPoint& v) { return { float(m * v.x), float(m * v.y) }; }
FPoint SDL::operator/(double m, const FPoint& v) { return { float(m / v.x), float(m / v.y) }; }
FPoint SDL::operator*(float m,  const FPoint& v) { return { m * v.x, m * v.y }; }
FPoint SDL::operator/(float m,  const FPoint& v) { return { m / v.x, m / v.y }; }
FPoint SDL::operator*(int m,    const FPoint& v) { return { m * v.x, m * v.y }; }
FPoint SDL::operator/(int m,    const FPoint& v) { return { m / v.x, m / v.y }; }
#pragma endregion

bool FPoint::operator==(const FPoint& v) const { return x == v.x && y == v.y; }
bool FPoint::operator!=(const FPoint& v) const { return x != v.x || y != v.y; }

std::ostream& SDL::operator<<(std::ostream& os, const FPoint& v) { return os << (std::string)v; }
#pragma endregion

#pragma region Point
Point::Point() : x(0), y(0) {}
Point::Point(int x, int y) : x(x), y(y) {}
Point::Point(const Point& v) : point(v.point) {}
Point::Point(const SDL_Point& point) : point(point) {}

Point::operator FPoint() const { return { float(x), float(y) }; }
Point::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

int Point::sqrMag() const { return x * x + y * y; }
float Point::mag() const { return std::sqrt(x * x + y * y); }
int Point::min() const { return std::min(x, y); }
int Point::max() const { return std::max(x, y); }

bool Point::nonZero() const { return x != 0 || y != 0; }
bool Point::inRect(const Rect& r) const { return x >= r.x && y >= r.y && x < r.x + r.w && y < r.y + r.h; };

FPoint Point::turn(float a) const { return { x * cos(a) - y * sin(a), x * sin(a) + y * cos(a) }; }
Point Point::perp() const { return { -y, x }; }
FPoint Point::norm() const { float size = mag(); return size == 0 ? FPoint(0, 0) : FPoint(x / size, y / size); }
Point Point::abs() const { return { std::abs(x), std::abs(y) }; }

Point  Point::clampX(int x1, int x2) const { return { std::clamp(x, x1, x2), y }; }
Point  Point::clampY(int y1, int y2) const { return { x, std::clamp(y, y1, y2) }; }
FPoint Point::clampR(float r1, float r2) const { float size = mag(); float r = std::min(0.0f, std::clamp(size, r1, r2)); if (size == r) return FPoint(*this); else return *this * (r / size); };

int Point::dot(const Point& v1, const Point& v2) { return v1.x * v2.x + v2.y * v1.y; }
int Point::cross(const Point& v1, const Point& v2) { return v2.x * v1.y - v2.y * v1.x; }
Point Point::min(const Point& v1, const Point& v2) { return { std::min(v1.x, v2.x), std::min(v1.y, v2.y) }; }
Point Point::max(const Point& v1, const Point& v2) { return { std::max(v1.x, v2.x), std::max(v1.y, v2.y) }; }

#pragma region Point arithmetic
Point Point::operator+() const { return { +x, +y }; }
Point Point::operator-() const { return { -x, -y }; }

FPoint Point::operator+(const FPoint& v) const { return { x + v.x, y + v.y }; }
FPoint Point::operator-(const FPoint& v) const { return { x - v.x, y - v.y }; }
FPoint Point::operator*(const FPoint& v) const { return { x * v.x, y * v.y }; }
FPoint Point::operator/(const FPoint& v) const { return { x / v.x, y / v.y }; }

Point Point::operator+(const Point& v) const { return { x + v.x, y + v.y }; }
Point Point::operator-(const Point& v) const { return { x - v.x, y - v.y }; }
Point Point::operator*(const Point& v) const { return { x * v.x, y * v.y }; }
Point Point::operator/(const Point& v) const { return { x / v.x, y / v.y }; }

Point& Point::operator+=(const FPoint& v) { x += v.x; y += v.y; return *this; }
Point& Point::operator-=(const FPoint& v) { x -= v.x; y -= v.y; return *this; }
Point& Point::operator*=(const FPoint& v) { x *= v.x; y *= v.y; return *this; }
Point& Point::operator/=(const FPoint& v) { x /= v.x; y /= v.y; return *this; }

Point& Point::operator+=(const Point& v) { x += v.x; y += v.y; return *this; }
Point& Point::operator-=(const Point& v) { x -= v.x; y -= v.y; return *this; }
Point& Point::operator*=(const Point& v) { x *= v.x; y *= v.y; return *this; }
Point& Point::operator/=(const Point& v) { x /= v.x; y /= v.y; return *this; }

FPoint Point::operator*(double m) const { return { float(x * m), float(y * m) }; }
FPoint Point::operator/(double m) const { return { float(x / m), float(y / m) }; }
FPoint Point::operator*(float m) const { return { x * m, y * m }; }
FPoint Point::operator/(float m) const { return { x / m, y / m }; }
Point  Point::operator*(int m) const { return { x * m, y * m }; };
Point  Point::operator/(int m) const { return { x / m, y / m }; };

Point& Point::operator*=(double m) { x *= m; y *= m; return *this; }
Point& Point::operator/=(double m) { x /= m; y /= m; return *this; }
Point& Point::operator*=(float m) { x *= m; y *= m; return *this; }
Point& Point::operator/=(float m) { x /= m; y /= m; return *this; }
Point& Point::operator*=(int m) { x *= m; y *= m; return *this; }
Point& Point::operator/=(int m) { x /= m; y /= m; return *this; }

FPoint SDL::operator*(double m, const Point& v) { return { float(m * v.x), float(m * v.y) }; }
FPoint SDL::operator/(double m, const Point& v) { return { float(m / v.x), float(m / v.y) }; }
FPoint SDL::operator*(float m,  const Point& v) { return { v.x * m, v.y * m }; };
FPoint SDL::operator/(float m,  const Point& v) { return { v.x / m, v.y / m }; };
Point  SDL::operator*(int m,    const Point& v) { return { v.x * m, v.y * m }; };
Point  SDL::operator/(int m,    const Point& v) { return { v.x / m, v.y / m }; };
#pragma endregion

bool Point::operator==(const Point& v) const { return x == v.x && y == v.y; };
bool Point::operator!=(const Point& v) const { return x != v.x || y != v.y; };

std::ostream& SDL::operator<<(std::ostream& os, const Point& v) { return os << (std::string)v; }
#pragma endregion

#pragma region FRect
FRect::FRect() : pos(0, 0), size(0, 0) {}
FRect::FRect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
FRect::FRect(const FPoint& pos, const FPoint& size) : pos(pos), size(size) {}
FRect::FRect(const FRect& rect) : rect(rect.rect) {}
FRect::FRect(const SDL_FRect& rect) : rect(rect) {}

FRect::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")"; }

bool  FRect::empty() const { return w <= 0 || h <= 0; }
float FRect::area() const { return w * h; }
float FRect::perimeter() const { return 2 * (w + h); }
float FRect::diagonal() const { return size.mag(); }

Rect FRect::floor() const { return { pos.floor(), (pos + size).floor() - pos.floor() }; }
Rect FRect::round() const { return { pos.round(), (pos + size).round() - pos.round() }; }
Rect FRect::ceil()  const { return { pos.ceil(),  (pos + size).ceil()  - pos.ceil()  }; }

FPoint FRect::topLeft() const { return FPoint(pos); }
FPoint FRect::topRight() const { return { pos.x + w, pos.y }; }
FPoint FRect::bottomLeft() const { return { pos.x, pos.y + h }; }
FPoint FRect::bottomRight() const { return pos + size; }
FPoint FRect::middle() const { return pos + size / 2.0f; }

FPoint FRect::clamp(const Point& v) const { return { std::clamp<float>(v.x, x, x + w), std::clamp<float>(v.y, y, y + h) }; }
FPoint FRect::clamp(const FPoint& v) const { return { std::clamp<float>(v.x, x, x + w), std::clamp<float>(v.y, y, y + h) }; }

FRect  FRect::transform(const Rect& target) const { return { (target.x - x) * w, (target.y - y) * h, target.w * w, target.h * h }; }
FPoint FRect::transform(const Point& target) const { return { (target.x - x) * w, (target.y - y) * h }; }
FRect  FRect::transform(const FRect& target) const { return { (target.x - x) * w, (target.y - y) * h, target.w * w, target.h * h }; }
FPoint FRect::transform(const FPoint& target) const { return { (target.x - x) * w, (target.y - y) * h }; }

bool FRect::intersectsPoint(const Point& v) const { return v.x > x && v.y > y && v.x < x + w && v.y < y + h; }
bool FRect::intersectsPoint(const FPoint& v) const { return v.x > x && v.y > y && v.x < x + w && v.y < y + h; }
bool FRect::intersectsRect(const  Rect& r) const { return x < r.x + r.w && y < r.y + r.h && x + w > r.x && y + h > r.y; }
bool FRect::intersectsRect(const FRect& r) const { return x < r.x + r.w && y < r.y + r.h && x + w > r.x && y + h > r.y; }

bool FRect::intersectRect(const Rect& rect, FRect& result) const {
	if (!intersectsRect(rect)) return false;

	FPoint topLeft = FPoint::max(this->topLeft(), (FPoint)rect.topLeft());
	FPoint bottomRight = FPoint::min(this->bottomRight(), (FPoint)rect.bottomRight());
	result = { topLeft, bottomRight - topLeft };

	return true;
}
bool FRect::intersectRect(const FRect& rect, FRect& result) const {
	if (!intersectsRect(rect)) return false;

	FPoint topLeft = FPoint::max(this->topLeft(), rect.topLeft());
	FPoint bottomRight = FPoint::min(this->bottomRight(), rect.bottomRight());
	result = { topLeft, bottomRight - topLeft };

	return true;
}

#pragma region FRect arithmetic
FRect FRect::operator+(const FPoint& v) const { return { pos + v, size }; }
FRect FRect::operator-(const FPoint& v) const { return { pos - v, size }; }
FRect FRect::operator+(const Point& v) const { return { pos + v, size }; }
FRect FRect::operator-(const Point& v) const { return { pos - v, size }; }

FRect& FRect::operator+=(const FPoint& v) { pos += v; return *this; }
FRect& FRect::operator-=(const FPoint& v) { pos -= v; return *this; }
FRect& FRect::operator+=(const Point& v) { pos += v; return *this; }
FRect& FRect::operator-=(const Point& v) { pos -= v; return *this; }

FRect FRect::operator* (const FPoint& v) const { return { pos, size * v }; }
FRect FRect::operator/ (const FPoint& v) const { return { pos, size / v }; }
FRect FRect::operator* (const Point& v) const { return { pos, size * v }; }
FRect FRect::operator/ (const Point& v) const { return { pos, size / v }; }

FRect& FRect::operator*=(const FPoint& v) { size *= v; return *this; }
FRect& FRect::operator/=(const FPoint& v) { size /= v; return *this; }
FRect& FRect::operator*=(const Point& v) { size *= v; return *this; }
FRect& FRect::operator/=(const Point& v) { size /= v; return *this; }

FRect FRect::operator*(double m) const { return { pos, size * m }; }
FRect FRect::operator/(double m) const { return { pos, size / m }; }
FRect FRect::operator*(float m) const { return { pos, size * m }; }
FRect FRect::operator/(float m) const { return { pos, size / m }; }
FRect FRect::operator*(int m) const { return { pos, size * m }; }
FRect FRect::operator/(int m) const { return { pos, size / m }; }

FRect& FRect::operator*=(double m) { size *= m; return *this; }
FRect& FRect::operator/=(double m) { size /= m; return *this; }
FRect& FRect::operator*=(float m) { size *= m; return *this; }
FRect& FRect::operator/=(float m) { size /= m; return *this; }
FRect& FRect::operator*=(int m) { size *= m; return *this; }
FRect& FRect::operator/=(int m) { size /= m; return *this; }
#pragma endregion

bool FRect::operator==(const FRect& v) const { return x == v.x && y == v.y && w == v.w && h == v.h; }
bool FRect::operator!=(const FRect& v) const { return x != v.x || y != v.y || w != v.w || h != v.h; }

std::ostream& SDL::operator<<(std::ostream& os, const FRect& r) { return os << (std::string)r; }
#pragma endregion

#pragma region Rect
Rect::Rect() : pos(), size() {}
Rect::Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
Rect::Rect(const Point& pos, const Point& size) : pos(pos), size(size) {}
Rect::Rect(const Rect& rect) : rect(rect.rect) {}
Rect::Rect(const SDL_Rect& rect) : rect(rect) {}

Rect::operator FRect() const { return { pos, size }; }
Rect::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")"; }

bool  Rect::empty() const { return w <= 0 || h <= 0; }
int   Rect::area() const { return w * h; }
int   Rect::perimeter() const { return 2 * (w + h); }
float Rect::diagonal() const { return size.mag(); }

Point  Rect::topLeft() const { return Point(pos); }
Point  Rect::topRight() const { return { pos.x + w, pos.y }; }
Point  Rect::bottomLeft() const { return { pos.x, pos.y + h }; }
Point  Rect::bottomRight() const { return pos + size; }
FPoint Rect::middle() const { return pos + size / 2.0f; }

Point  Rect::clamp(const Point& v) const { return v.clampX(x, x + h).clampY(y, y + h); }
FPoint Rect::clamp(const FPoint& v) const { return v.clampX(x, x + h).clampY(y, y + h); }

FPoint Rect::percent(const FPoint& p) const { return pos + size * p; }

template <typename iterator>
bool Rect::enclosePoints(iterator begin, iterator end, const Rect& clip) {
	std::vector<SDL_Point> points;
	points.insert(points.begin(), begin, end);
	return SDL_EnclosePoints(points.data(), points.size(), &clip.rect, &rect);
}
template <typename iterator>
bool Rect::enclosePoints(iterator begin, iterator end) {
	std::vector<SDL_Point> points;
	points.insert(points.begin(), begin, end);
	return SDL_EnclosePoints(points.data(), points.size(), NULL, &rect);
}

Rect   Rect::transform(const Rect& target) const { return { (target.x - x) * w, (target.y - y) * h, target.w * w, target.h * h }; }
Point  Rect::transform(const Point& target) const { return { (target.x - x) * w, (target.y - y) * h }; }
FRect  Rect::transform(const FRect& target) const { return { (target.x - x) * w, (target.y - y) * h, target.w * w, target.h * h }; }
FPoint Rect::transform(const FPoint& target) const { return { (target.x - x) * w, (target.y - y) * h }; }

bool Rect::intersectsPoint(const Point& v) const { return v.x > x && v.y > y && v.x < x + w && v.y < y + h; }
bool Rect::intersectsPoint(const FPoint& v) const { return v.x > x && v.y > y && v.x < x + w && v.y < y + h; }
bool Rect::intersectsRect(const Rect& r) const { return x < r.x + r.w && y < r.y + r.h && x + w > r.x && y + h > r.y; }
bool Rect::intersectsRect(const FRect& r) const { return x < r.x + r.w && y < r.y + r.h && x + w > r.x && y + h > r.y; }
bool Rect::intersectsLine(const Point& P1, const Point& P2) {
	Rect result(P1, P2);
	return SDL_IntersectRectAndLine(&rect, &result.x, &result.y, &result.w, &result.h);
}

void Rect::rectUnion(const Rect& rect, Rect& result) const {
	SDL_UnionRect(&this->rect, &rect.rect, &result.rect);
}

bool Rect::intersectRect(const Rect& rect, Rect& result) const {
	return SDL_IntersectRect(&this->rect, &rect.rect, &result.rect);
}
bool Rect::intersectRect(const FRect& rect, FRect& result) const {
	if (!intersectsRect(rect)) return false;

	FPoint topLeft = FPoint::min(this->topLeft(), rect.topLeft());
	FPoint bottomRight = FPoint::max(this->bottomRight(), rect.bottomRight());
	result = { topLeft, bottomRight - topLeft };

	return true;
}
bool Rect::intersectLine(Point& P1, Point& P2) const {
	return SDL_IntersectRectAndLine(&rect, &P1.x, &P1.y, &P2.w, &P2.h);
}

#pragma region Rect arithmetic
FRect Rect::operator+ (const FPoint& v) const { return { pos + v, size }; }
FRect Rect::operator- (const FPoint& v) const { return { pos - v, size }; }
Rect Rect::operator+ (const Point& v) const { return { pos + v, size }; }
Rect Rect::operator- (const Point& v) const { return { pos - v, size }; }

Rect& Rect::operator+=(const FPoint& v) { pos += v; return *this; }
Rect& Rect::operator-=(const FPoint& v) { pos -= v; return *this; }
Rect& Rect::operator+=(const Point& v) { pos += v; return *this; }
Rect& Rect::operator-=(const Point& v) { pos -= v; return *this; }

FRect Rect::operator* (const FPoint& v) const { return { pos, size * v }; }
FRect Rect::operator/ (const FPoint& v) const { return { pos, size / v }; }
Rect Rect::operator* (const Point& v) const { return { pos, size * v }; }
Rect Rect::operator/ (const Point& v) const { return { pos, size / v }; }

Rect& Rect::operator*=(const FPoint& v) { size *= v; return *this; }
Rect& Rect::operator/=(const FPoint& v) { size /= v; return *this; }
Rect& Rect::operator*=(const Point& v) { size *= v; return *this; }
Rect& Rect::operator/=(const Point& v) { size /= v; return *this; }

FRect Rect::operator*(double m) const { return { pos, size * m }; }
FRect Rect::operator/(double m) const { return { pos, size / m }; }
FRect Rect::operator*(float m)  const { return { pos, size * m }; }
FRect Rect::operator/(float m)  const { return { pos, size / m }; }
Rect  Rect::operator*(int m  )  const { return { pos, size * m }; }
Rect  Rect::operator/(int m  )  const { return { pos, size / m }; }

Rect& Rect::operator*=(double m) { size *= m; return *this; }
Rect& Rect::operator/=(double m) { size /= m; return *this; }
Rect& Rect::operator*=(float m)  { size *= m; return *this; }
Rect& Rect::operator/=(float m)  { size /= m; return *this; }
Rect& Rect::operator*=(int m  )  { size *= m; return *this; }
Rect& Rect::operator/=(int m  )  { size /= m; return *this; }
#pragma endregion

bool Rect::operator==(const Rect& v) const { return x == v.x && y == v.y && w == v.w && h == v.h; }
bool Rect::operator!=(const Rect& v) const { return x != v.x || y != v.y || w != v.w || h != v.h; }

std::ostream& SDL::operator<<(std::ostream& os, const Rect& r) { return os << (std::string)r; }
#pragma endregion