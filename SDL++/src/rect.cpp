#include "rect.hpp"

namespace SDL
{
#pragma region FPoint

#pragma region Convenience Functions
	FPoint FPoint::FromAngle(float angle, float mag) { return { cosf(angle) * mag, sinf(angle) * mag }; }

	FPoint::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

	float FPoint::mag() const { return std::sqrt(x * x + y * y); }

	FPoint FPoint::rotate(float a) const { return rotate(cosf(a), sinf(a)); }
	FPoint FPoint::rotateAround(FPoint center, float a) const { return (*this - center).rotate(a) + center; }
	FPoint FPoint::norm() const { const float size = mag(); return size == 0.0f ? FPoint(0, 0) : FPoint(x / size, y / size); }

	Point FPoint::floor() const { return { (int)std::floor(x), (int)std::floor(y) }; }
	Point FPoint::round() const { return { (int)std::round(x), (int)std::round(y) }; }
	Point FPoint::ceil()  const { return { (int)std::ceil(x),  (int)std::ceil(y) }; }

	FPoint FPoint::clampR(float r1, float r2) const { float size = mag(); float r = std::min(0.0f, std::clamp(size, r1, r2)); if (size == r) return FPoint(*this); else return *this * (r / size); }
#pragma endregion

#pragma region Arithmetic
	FPoint& FPoint::operator+=(const FPoint& v) { x += v.x; y += v.y; return *this; }
	FPoint& FPoint::operator-=(const FPoint& v) { x -= v.x; y -= v.y; return *this; }
	FPoint& FPoint::operator*=(const FPoint& v) { x *= v.x; y *= v.y; return *this; }
	FPoint& FPoint::operator/=(const FPoint& v) { x /= v.x; y /= v.y; return *this; }

	FPoint& FPoint::operator+=(const Point& v) { x += v.x; y += v.y; return *this; }
	FPoint& FPoint::operator-=(const Point& v) { x -= v.x; y -= v.y; return *this; }
	FPoint& FPoint::operator*=(const Point& v) { x *= v.x; y *= v.y; return *this; }
	FPoint& FPoint::operator/=(const Point& v) { x /= v.x; y /= v.y; return *this; }

	FPoint& FPoint::operator*=(double m) { x *= m; y *= m; return *this; }
	FPoint& FPoint::operator/=(double m) { x /= m; y /= m; return *this; }
	FPoint& FPoint::operator*=(float m ) { x *= m; y *= m; return *this; }
	FPoint& FPoint::operator/=(float m ) { x /= m; y /= m; return *this; }
	FPoint& FPoint::operator*=(int m   ) { x *= m; y *= m; return *this; }
	FPoint& FPoint::operator/=(int m   ) { x /= m; y /= m; return *this; }
#pragma endregion

#pragma endregion

#pragma region Point

#pragma region Convenience Functions
	Point::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

	float Point::mag() const { return float(std::sqrt(x * x + y * y)); }

	FPoint Point::rotate(float a) const { return rotate(cosf(a), sinf(a)); }
	FPoint Point::rotateAround(Point center, float a) const { return (*this - center).rotate(a) + center; }
	FPoint Point::norm() const { float size = mag(); return size == 0 ? FPoint(0, 0) : FPoint(x / size, y / size); }

	FPoint Point::clampR(float r1, float r2) const { float size = mag(); float r = std::min(0.0f, std::clamp(size, r1, r2)); if (size == r) return FPoint(*this); else return *this * (r / size); };
#pragma endregion

#pragma region Arithmetic
	Point& Point::operator+=(const FPoint& v) { x += v.x; y += v.y; return *this; }
	Point& Point::operator-=(const FPoint& v) { x -= v.x; y -= v.y; return *this; }
	Point& Point::operator*=(const FPoint& v) { x *= v.x; y *= v.y; return *this; }
	Point& Point::operator/=(const FPoint& v) { x /= v.x; y /= v.y; return *this; }

	Point& Point::operator+=(const Point& v) { x += v.x; y += v.y; return *this; }
	Point& Point::operator-=(const Point& v) { x -= v.x; y -= v.y; return *this; }
	Point& Point::operator*=(const Point& v) { x *= v.x; y *= v.y; return *this; }
	Point& Point::operator/=(const Point& v) { x /= v.x; y /= v.y; return *this; }

	Point& Point::operator*=(double m) { x *= m; y *= m; return *this; }
	Point& Point::operator/=(double m) { x /= m; y /= m; return *this; }
	Point& Point::operator*=(float m ) { x *= m; y *= m; return *this; }
	Point& Point::operator/=(float m ) { x /= m; y /= m; return *this; }
	Point& Point::operator/=(int m   ) { x /= m; y /= m; return *this; }
	Point& Point::operator*=(int m   ) { x *= m; y *= m; return *this; }
#pragma endregion

#pragma endregion

#pragma region FRect

#pragma region Convenience Functions
	FRect::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")"; }

	float FRect::diagonal() const { return size.mag(); }

	Rect FRect::floor() const { return { pos.floor(), (pos + size).floor() - pos.floor() }; }
	Rect FRect::round() const { return { pos.round(), (pos + size).round() - pos.round() }; }
	Rect FRect::ceil()  const { return { pos.ceil(),  (pos + size).ceil() - pos.ceil() }; }
#pragma endregion

#pragma region Arithmetic
	FRect& FRect::operator+=(const FPoint& v) { pos += v; return *this; }
	FRect& FRect::operator-=(const FPoint& v) { pos -= v; return *this; }
	FRect& FRect::operator+=(const Point& v) { pos += v; return *this; }
	FRect& FRect::operator-=(const Point& v) { pos -= v; return *this; }

	FRect& FRect::operator*=(const FPoint& v) { size *= v; return *this; }
	FRect& FRect::operator/=(const FPoint& v) { size /= v; return *this; }
	FRect& FRect::operator*=(const Point& v) { size *= v; return *this; }
	FRect& FRect::operator/=(const Point& v) { size /= v; return *this; }

	FRect& FRect::operator*=(double m) { size *= m; return *this; }
	FRect& FRect::operator/=(double m) { size /= m; return *this; }
	FRect& FRect::operator*=(float m) { size *= m; return *this; }
	FRect& FRect::operator/=(float m) { size /= m; return *this; }
	FRect& FRect::operator*=(int m) { size *= m; return *this; }
	FRect& FRect::operator/=(int m) { size /= m; return *this; }
#pragma endregion

#pragma endregion

#pragma region Rect

#pragma region Convenience Functions
	Rect::operator std::string() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")"; }

	float Rect::diagonal() const { return size.mag(); }

	bool Rect::intersectsLine(const Point& P1, const Point& P2) {
		Rect result(P1, P2);
		return SDL_IntersectRectAndLine(&rect, &result.x, &result.y, &result.w, &result.h);
	}

	bool Rect::intersectLine(Point& P1, Point& P2) const {
		return SDL_IntersectRectAndLine(&rect, &P1.x, &P1.y, &P2.x, &P2.y);
	}
#pragma endregion

#pragma region Arithmetic
	Rect& Rect::operator+=(const FPoint& v) { pos += v; return *this; }
	Rect& Rect::operator-=(const FPoint& v) { pos -= v; return *this; }
	Rect& Rect::operator+=(const  Point& v) { pos += v; return *this; }
	Rect& Rect::operator-=(const  Point& v) { pos -= v; return *this; }

	Rect& Rect::operator*=(const FPoint& v) { size *= v; return *this; }
	Rect& Rect::operator/=(const FPoint& v) { size /= v; return *this; }
	Rect& Rect::operator*=(const  Point& v) { size *= v; return *this; }
	Rect& Rect::operator/=(const  Point& v) { size /= v; return *this; }

	Rect& Rect::operator*=(double m) { size *= m; return *this; }
	Rect& Rect::operator/=(double m) { size /= m; return *this; }
	Rect& Rect::operator*=(float m) { size *= m; return *this; }
	Rect& Rect::operator/=(float m) { size /= m; return *this; }
	Rect& Rect::operator*=(int m) { size *= m; return *this; }
	Rect& Rect::operator/=(int m) { size /= m; return *this; }
#pragma endregion

#pragma endregion
}