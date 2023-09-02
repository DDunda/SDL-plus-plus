#pragma once

#ifndef SDLpp_rect_h_
#define SDLpp_rect_h_

#include <SDL_rect.h>
#include <algorithm>
#include <cmath>
#include <ostream>
#include <string>
#include <assert.h>

namespace SDL {
	struct Point;
	struct FPoint;
	struct Rect;
	struct FRect;

	struct FPoint {
	public:
		union {
			struct {
				float x, y;
			};
			struct {
				float w, h;
			};
			SDL_FPoint point;
		};


		constexpr FPoint() : x(0), y(0) {}
		constexpr FPoint(float x, float y) : x(x), y(y) {}
		constexpr FPoint(const FPoint& v) : x(v.x), y(v.y) {}
		constexpr FPoint(const SDL_FPoint& point) : x(point.x), y(point.y) {}

		static FPoint FromAngle(float angle, float mag = 1.0f);

		operator std::string() const;

		constexpr float sqrMag() const { return x * x + y * y; }
		constexpr float min() const { return std::min(x, y); }
		constexpr float max() const { return std::max(x, y); }
		float mag() const;

		constexpr bool nonZero() const { return x != 0 || y != 0; }

		constexpr FPoint rotate(float rotx, float roty) const { return FPoint(x * rotx - y * roty, x * roty + y * rotx); }
		constexpr FPoint rotateAround(FPoint center, float rotx, float roty) const { return (*this - center).rotate(rotx, roty) + center; }
		FPoint rotate(float a) const;
		FPoint rotateAround(FPoint center, float a) const;
		constexpr FPoint perp() const { return FPoint(-y, x); }
		constexpr FPoint abs () const { return FPoint(x < 0 ? -x : x, y < 0 ? -y : y); }
		FPoint norm() const;

		Point floor() const;
		Point round() const;
		Point ceil()  const;

		constexpr FPoint clamp(float x1, float x2, float y1, float y2) const { return FPoint(std::clamp(x, x1, x2), std::clamp(y, y1, y2)); }
		constexpr FPoint clampX(float x1, float x2) const { return FPoint(std::clamp(x, x1, x2), y); }
		constexpr FPoint clampY(float y1, float y2) const { return FPoint(x, std::clamp(y, y1, y2)); }
		FPoint clampR(float r1, float r2) const;

		static constexpr float  dot  (const FPoint& v1, const FPoint& v2) { return v1.x * v2.x + v1.y * v2.y; }
		static constexpr float  cross(const FPoint& v1, const FPoint& v2) { return v1.y * v2.x - v1.x * v2.y; }
		static constexpr FPoint min  (const FPoint& v1, const FPoint& v2) { return FPoint(std::min(v1.x, v2.x), std::min(v1.y, v2.y)); }
		static constexpr FPoint max  (const FPoint& v1, const FPoint& v2) { return FPoint(std::max(v1.x, v2.x), std::max(v1.y, v2.y)); }

		constexpr FPoint operator+() const { return FPoint(+x, +y); }
		constexpr FPoint operator-() const { return FPoint(-x, -y); }

		constexpr FPoint operator+(const FPoint& v) const { return FPoint(x + v.x, y + v.y); }
		constexpr FPoint operator-(const FPoint& v) const { return FPoint(x - v.x, y - v.y); }
		constexpr FPoint operator*(const FPoint& v) const { return FPoint(x * v.x, y * v.y); }
		constexpr FPoint operator/(const FPoint& v) const { return FPoint(x / v.x, y / v.y); }

		constexpr FPoint operator*(double m) const { return FPoint(float(x * m), float(y * m)); }
		constexpr FPoint operator/(double m) const { return FPoint(float(x / m), float(y / m)); }
		constexpr FPoint operator*( float m) const { return FPoint(      x * m,        y * m ); }
		constexpr FPoint operator/( float m) const { return FPoint(      x / m,        y / m ); }
		constexpr FPoint operator*(   int m) const { return FPoint(      x * m,        y * m ); }
		constexpr FPoint operator/(   int m) const { return FPoint(      x / m,        y / m ); }

		FPoint& operator+=(const FPoint& v);
		FPoint& operator-=(const FPoint& v);
		FPoint& operator*=(const FPoint& v);
		FPoint& operator/=(const FPoint& v);

		FPoint& operator+=(const Point& v);
		FPoint& operator-=(const Point& v);
		FPoint& operator*=(const Point& v);
		FPoint& operator/=(const Point& v);

		FPoint& operator*=(double m);
		FPoint& operator/=(double m);
		FPoint& operator*=(float m);
		FPoint& operator/=(float m);
		FPoint& operator*=(int m);
		FPoint& operator/=(int m);

		constexpr bool operator==(const FPoint& v) const { return x == v.x && y == v.y; }
		constexpr bool operator!=(const FPoint& v) const { return x != v.x || y != v.y; }
	};

	struct Point {
	public:
		union {
			struct {
				int x, y;
			};
			struct {
				int w, h;
			};
			SDL_Point point;
		};

		constexpr Point(                      ) : x(      0), y(      0) {}
		constexpr Point(int x, int y          ) : x(      x), y(      y) {}
		constexpr Point(const Point& v        ) : x(    v.x), y(    v.y) {}
		constexpr Point(const SDL_Point& point) : x(point.x), y(point.y) {}

		constexpr operator FPoint() const { return { float(x), float(y) }; }
		operator std::string() const;

		constexpr int sqrMag() const { return x * x + y * y; }
		constexpr int min() const { return std::min(x, y); }
		constexpr int max() const { return std::max(x, y); }
		float mag() const;

		constexpr bool nonZero() const { return x != 0 || y != 0; }

		constexpr FPoint rotate(float rotx, float roty) const { return { x * rotx - y * roty, x * roty + y * rotx }; }
		constexpr FPoint rotateAround(Point center, float rotx, float roty) const { return (*this - center).rotate(rotx, roty) + center; }
		FPoint rotate(float a) const;
		FPoint rotateAround(Point center, float a) const;
		constexpr Point perp() const { return { -y, x }; }
		constexpr Point abs() const { return { x < 0 ? -x : x, y < 0 ? -y : y }; }
		FPoint norm() const;

		constexpr Point clamp(int x1, int x2, int y1, int y2) const { return { std::clamp(x, x1, x2), std::clamp(y, y1, y2) }; }
		constexpr Point clampX(int x1, int x2) const { return { std::clamp(x, x1, x2), y }; }
		constexpr Point clampY(int y1, int y2) const { return Point(x, std::clamp(y, y1, y2)); }
		FPoint clampR(float r1, float r2) const;

		static constexpr int   dot(const Point& v1, const Point& v2) { return v1.x * v2.x + v2.y * v1.y; }
		static constexpr int cross(const Point& v1, const Point& v2) { return v2.x * v1.y - v2.y * v1.x; }
		static constexpr Point min(const Point& v1, const Point& v2) { return { std::min(v1.x, v2.x), std::min(v1.y, v2.y) }; }
		static constexpr Point max(const Point& v1, const Point& v2) { return { std::max(v1.x, v2.x), std::max(v1.y, v2.y) }; }

		constexpr Point operator+ () const { return { +x, +y }; };
		constexpr Point operator- () const { return { -x, -y }; };

		constexpr Point operator+(const Point& v) const { return { x + v.x, y + v.y }; }
		constexpr Point operator-(const Point& v) const { return { x - v.x, y - v.y }; }
		constexpr Point operator*(const Point& v) const { return { x * v.x, y * v.y }; }
		constexpr Point operator/(const Point& v) const { return { x / v.x, y / v.y }; }

		constexpr FPoint operator*(double m) const { return { float(x * m), float(y * m) }; }
		constexpr FPoint operator/(double m) const { return { float(x / m), float(y / m) }; }
		constexpr FPoint operator*( float m) const { return { x * m, y * m }; }
		constexpr FPoint operator/( float m) const { return { x / m, y / m }; }
		constexpr Point  operator*(   int m) const { return { x * m, y * m }; };
		constexpr Point  operator/(   int m) const { return { x / m, y / m }; };

		Point& operator+=(const FPoint& v);
		Point& operator-=(const FPoint& v);
		Point& operator*=(const FPoint& v);
		Point& operator/=(const FPoint& v);

		Point& operator+=(const Point& v);
		Point& operator-=(const Point& v);
		Point& operator*=(const Point& v);
		Point& operator/=(const Point& v);

		Point& operator*=(double m);
		Point& operator/=(double m);
		Point& operator*=(float m);
		Point& operator/=(float m);
		Point& operator*=(int m);
		Point& operator/=(int m);

		constexpr bool operator==(const Point& v) const { return x == v.x && y == v.y; }
		constexpr bool operator!=(const Point& v) const { return x != v.x || y != v.y; }
	};

	static constexpr FPoint operator+(const FPoint& v1, const Point& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	static constexpr FPoint operator-(const FPoint& v1, const Point& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
	static constexpr FPoint operator*(const FPoint& v1, const Point& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
	static constexpr FPoint operator/(const FPoint& v1, const Point& v2) { return { v1.x / v2.x, v1.y / v2.y }; }

	static constexpr FPoint operator+(const Point& v1, const FPoint& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	static constexpr FPoint operator-(const Point& v1, const FPoint& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
	static constexpr FPoint operator*(const Point& v1, const FPoint& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
	static constexpr FPoint operator/(const Point& v1, const FPoint& v2) { return { v1.x / v2.x, v1.y / v2.y }; }

	static constexpr FPoint operator*(double m, const FPoint& v) { return { float(m * v.x), float(m * v.y) }; }
	static constexpr FPoint operator/(double m, const FPoint& v) { return { float(m / v.x), float(m / v.y) }; }
	static constexpr FPoint operator*(float m,  const FPoint& v) { return { m * v.x, m * v.y }; }
	static constexpr FPoint operator/(float m,  const FPoint& v) { return { m / v.x, m / v.y }; }
	static constexpr FPoint operator*(int m,    const FPoint& v) { return { m * v.x, m * v.y }; }
	static constexpr FPoint operator/(int m,    const FPoint& v) { return { m / v.x, m / v.y }; }

	static constexpr FPoint operator*(double m, const Point& v) { return { float(m * v.x), float(m * v.y) }; }
	static constexpr FPoint operator/(double m, const Point& v) { return { float(m / v.x), float(m / v.y) }; }
	static constexpr FPoint operator*(float m,  const Point& v) { return { m * v.x, m * v.y }; }
	static constexpr FPoint operator/(float m,  const Point& v) { return { m / v.x, m / v.y }; }
	static constexpr  Point operator*(int m,    const Point& v) { return { m * v.x, m * v.y }; }
	static constexpr  Point operator/(int m,    const Point& v) { return { m / v.x, m / v.y }; }

	struct FRect {
		union {
			struct {
				union {
					struct {
						float x, y;
					};
					FPoint pos;
				};
				union {
					struct {
						float w, h;
					};
					FPoint size;
				};
			};
			SDL_FRect rect;
		};

		constexpr FRect(                                     ) : pos(   0.f,    0.f), size(   0.f,    0.f) {}
		constexpr FRect( float x, float y,   float w, float h) : pos(     x,      y), size(     w,      h) {}
		constexpr FRect( float x, float y, const FPoint& size) : pos(     x,      y), size(     size     ) {}
		constexpr FRect(const FPoint& pos,   float w, float h) : pos(     pos      ), size(     w,      h) {}
		constexpr FRect(const FPoint& pos, const FPoint& size) : pos(     pos      ), size(     size     ) {}
		constexpr FRect(const FRect& rect                    ) : pos(rect.pos      ), size(rect.size     ) {}
		constexpr FRect(const SDL_FRect& rect                ) : pos(rect.x, rect.y), size(rect.x, rect.y) {}

		operator std::string() const;
		
		constexpr bool empty() const { return size.x <= 0 || size.y <= 0; }
		constexpr float area() const { return size.x * size.y; }
		constexpr float perimeter() const { return 2 * (size.x + size.y); }
		float diagonal() const;

		Rect floor() const;
		Rect round() const;
		Rect ceil() const;

		constexpr FPoint normToPoint(FPoint v) const { return (v * size) + pos; }
		constexpr FPoint pointToNorm(FPoint v) const { return (v - pos) / size; }

		constexpr FPoint topLeft    () const { return pos;                     }
		constexpr FPoint topRight   () const { return pos + FPoint(size.y, 0); }
		constexpr FPoint bottomLeft () const { return pos + FPoint(0, size.x); }
		constexpr FPoint bottomRight() const { return pos + size;              }
		constexpr FPoint middle     () const { return pos + size / 2.0f;       }

		constexpr FPoint clamp(const  Point& v) const { return { std::clamp(float(v.x), pos.x, pos.x + size.x), std::clamp(float(v.y), pos.y, pos.y + size.y) }; }
		constexpr FPoint clamp(const FPoint& v) const { return { std::clamp(      v.x , pos.x, pos.x + size.x), std::clamp(      v.y , pos.y, pos.y + size.y) }; }

		constexpr bool contains(const  Point& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		constexpr bool contains(const FPoint& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }

		constexpr FRect operator+(const FPoint& v) const { return { pos + v, size }; }
		constexpr FRect operator-(const FPoint& v) const { return { pos - v, size }; }
		constexpr FRect operator+(const  Point& v) const { return { pos + v, size }; }
		constexpr FRect operator-(const  Point& v) const { return { pos - v, size }; }

		constexpr FRect operator* (const FPoint& v) const { return { pos, size * v }; }
		constexpr FRect operator/ (const FPoint& v) const { return { pos, size / v }; }
		constexpr FRect operator* (const  Point& v) const { return { pos, size * v }; }
		constexpr FRect operator/ (const  Point& v) const { return { pos, size / v }; }

		constexpr FRect operator*(double m) const { return { pos, size * m }; }
		constexpr FRect operator/(double m) const { return { pos, size / m }; }
		constexpr FRect operator*( float m) const { return { pos, size * m }; }
		constexpr FRect operator/( float m) const { return { pos, size / m }; }
		constexpr FRect operator*(   int m) const { return { pos, size * m }; }
		constexpr FRect operator/(   int m) const { return { pos, size / m }; }

		FRect& operator+=(const FPoint& v);
		FRect& operator-=(const FPoint& v);
		FRect& operator+=(const Point& v);
		FRect& operator-=(const Point& v);

		FRect& operator*=(const FPoint& v);
		FRect& operator/=(const FPoint& v);
		FRect& operator*=(const Point& v);
		FRect& operator/=(const Point& v);

		FRect& operator*=(double m);
		FRect& operator/=(double m);
		FRect& operator*=(float m);
		FRect& operator/=(float m);
		FRect& operator*=(int m);
		FRect& operator/=(int m);

		constexpr bool operator==(const FRect& v) const { return x == v.x && y == v.y && w == v.w && h == v.h; }
		constexpr bool operator!=(const FRect& v) const { return x != v.x || y != v.y || w != v.w || h != v.h; }
	};

	struct Rect {
		union {
			struct {
				union {
					struct {
						int x, y;
					};
					Point pos;
				};
				union {
					struct {
						int w, h;
					};
					Point size;
				};
			};
			SDL_Rect rect;
		};

		constexpr Rect(                                   ) : pos(     0,     0), size(     0,      0) {}
		constexpr Rect(    int x, int y,      int w, int h) : pos(     x,     y), size(     w,      h) {}
		constexpr Rect(    int x, int y, const Point& size) : pos(     x,     y), size(     size     ) {}
		constexpr Rect(const Point& pos,      int w, int h) : pos(     pos     ), size(     w,      h) {}
		constexpr Rect(const Point& pos, const Point& size) : pos(     pos     ), size(     size     ) {}
		constexpr Rect(const Rect& rect                   ) : pos(rect.pos     ), size(rect.size     ) {}
		constexpr Rect(const SDL_Rect& rect               ) : pos(rect.x,rect.y), size(rect.w, rect.h) {}

		constexpr operator FRect() const { return { pos, size }; }
		operator std::string() const;

		constexpr bool empty() const { return size.x <= 0 || size.y <= 0; }
		constexpr int  area() const { return size.x * size.y; }
		constexpr int  perimeter() const { return 2 * (size.x + size.y); }
		float diagonal() const;

		constexpr FPoint normToPoint(FPoint v) const { return (v * size) + pos; }
		constexpr FPoint pointToNorm(FPoint v) const { return (v - pos) / size; }

		constexpr  Point topLeft    () const { return pos;                    }
		constexpr  Point topRight   () const { return pos + Point(size.x, 0); }
		constexpr  Point bottomLeft () const { return pos + Point(0, size.y); }
		constexpr  Point bottomRight() const { return pos + size;             }
		constexpr FPoint middle     () const { return pos + size / 2.0f;      }

		constexpr  Point clamp(const  Point& v) const { return v.clamp(      pos.x,        pos.x + size.x,        pos.y,        pos.y + size.y ); }
		constexpr FPoint clamp(const FPoint& v) const { return v.clamp(float(pos.x), float(pos.x + size.x), float(pos.y), float(pos.y + size.y)); }

		template <typename iterator>
		bool enclosePoints(iterator begin, iterator end, const Rect& clip);
		template <typename iterator>
		bool enclosePoints(iterator begin, iterator end);

		constexpr bool contains(const  Point& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		constexpr bool contains(const FPoint& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		bool intersectsLine(const Point& P1, const Point& P2);

		bool intersectLine(Point& P1, Point& P2) const;

		constexpr FRect operator+(const FPoint& v) const { return { pos + v, size }; }
		constexpr FRect operator-(const FPoint& v) const { return { pos - v, size }; }
		constexpr Rect  operator+(const  Point& v) const { return { pos + v, size }; }
		constexpr Rect  operator-(const  Point& v) const { return { pos - v, size }; }

		constexpr FRect operator* (const FPoint& v) const { return { pos, size * v }; }
		constexpr FRect operator/ (const FPoint& v) const { return { pos, size / v }; }
		constexpr Rect  operator* (const Point& v)  const { return { pos, size * v }; }
		constexpr Rect  operator/ (const Point& v)  const { return { pos, size / v }; }

		constexpr FRect operator*(double m) const { return { pos, size * m }; }
		constexpr FRect operator/(double m) const { return { pos, size / m }; }
		constexpr FRect operator*(float m)  const { return { pos, size * m }; }
		constexpr FRect operator/(float m)  const { return { pos, size / m }; }
		constexpr Rect  operator*(int m)    const { return { pos, size * m }; }
		constexpr Rect  operator/(int m)    const { return { pos, size / m }; }

		Rect& operator+=(const FPoint& v);
		Rect& operator-=(const FPoint& v);
		Rect& operator+=(const Point& v);
		Rect& operator-=(const Point& v);

		Rect& operator*=(const FPoint& v);
		Rect& operator/=(const FPoint& v);
		Rect& operator*=(const Point& v);
		Rect& operator/=(const Point& v);

		Rect& operator*=(double m);
		Rect& operator/=(double m);
		Rect& operator*=(float m);
		Rect& operator/=(float m);
		Rect& operator*=(int m);
		Rect& operator/=(int m);

		constexpr bool operator==(const Rect& r) const { return pos.x == r.pos.x && pos.y == r.pos.y && size.x == r.size.x && size.y == r.size.y; }
		constexpr bool operator!=(const Rect& r) const { return pos.x != r.pos.x || pos.y != r.pos.y || size.x != r.size.x || size.y != r.size.y; }
	};

	static constexpr bool rectsIntersect(const  Rect& A, const  Rect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	static constexpr bool rectsIntersect(const  Rect& A, const FRect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	static constexpr bool rectsIntersect(const FRect& A, const  Rect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	static constexpr bool rectsIntersect(const FRect& A, const FRect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }

	static constexpr  Rect intersectionRect(const  Rect& A, const  Rect& B)
	{
		if (!rectsIntersect(A, B)) return {};

		const Point top_left = Point::max(A.topLeft(), B.topLeft());
		const Point bottom_right = Point::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect intersectionRect(const FRect& A, const  Rect& B)
	{
		if (!rectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect intersectionRect(const  Rect& A, const FRect& B)
	{
		if (!rectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect intersectionRect(const FRect& A, const FRect& B)
	{
		if (!rectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return FRect(top_left, bottom_right - top_left);
	}

	static constexpr  Rect unionRect(const  Rect& A,  const Rect& B)
	{
		if (A.empty()) {
			if (B.empty()) return {};
			else return B;
		}
		else if (B.empty()) return A;

		const Point top_left = Point::min(A.topLeft(), B.topLeft());
		const Point bottom_right = Point::max(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect unionRect(const FRect& A,  const Rect& B)
	{
		if (A.empty()) {
			if (B.empty()) return {};
			else return B;
		}
		else if (B.empty()) return A;

		const FPoint top_left = FPoint::min(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::max(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect unionRect(const  Rect& A, const FRect& B)
	{
		if (A.empty()) {
			if (B.empty()) return {};
			else return B;
		}
		else if (B.empty()) return A;

		const FPoint top_left = FPoint::min(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::max(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	static constexpr FRect unionRect(const FRect& A, const FRect& B)
	{
		if (A.empty()) {
			if (B.empty()) return {};
			else return B;
		}
		else if (B.empty()) return A;

		const FPoint top_left = FPoint::min(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::max(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}

	static constexpr  FRect transformToWorld(const  FRect& reference, const   Rect& target) { return  FRect(target.pos * reference.size + reference.pos, target.size * reference.size); }
	static constexpr FPoint transformToWorld(const  FRect& reference, const  Point& target) { return FPoint(target     * reference.size + reference.pos);                               }
	static constexpr  FRect transformToWorld(const  FRect& reference, const  FRect& target) { return  FRect(target.pos * reference.size + reference.pos, target.size * reference.size); }
	static constexpr FPoint transformToWorld(const  FRect& reference, const FPoint& target) { return FPoint(target     * reference.size + reference.pos);                               }

	static constexpr   Rect transformToWorld(const   Rect& reference, const   Rect& target) { return   Rect(target.pos * reference.size + reference.pos, target.size * reference.size); }
	static constexpr  Point transformToWorld(const   Rect& reference, const  Point& target) { return  Point(target     * reference.size + reference.pos);                               }
	static constexpr  FRect transformToWorld(const   Rect& reference, const  FRect& target) { return  FRect(target.pos * reference.size + reference.pos, target.size * reference.size); }
	static constexpr FPoint transformToWorld(const   Rect& reference, const FPoint& target) { return FPoint(target     * reference.size + reference.pos);                               }

	static constexpr  FRect transformToLocal(const  FRect& reference, const   Rect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	static constexpr FPoint transformToLocal(const  FRect& reference, const  Point& target) { return FPoint((target     - reference.pos) / reference.size);                               }
	static constexpr  FRect transformToLocal(const  FRect& reference, const  FRect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	static constexpr FPoint transformToLocal(const  FRect& reference, const FPoint& target) { return FPoint((target     - reference.pos) / reference.size);                               }

	static constexpr   Rect transformToLocal(const   Rect& reference, const   Rect& target) { return   Rect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	static constexpr  Point transformToLocal(const   Rect& reference, const  Point& target) { return  Point((target     - reference.pos) / reference.size);                               }
	static constexpr  FRect transformToLocal(const   Rect& reference, const  FRect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	static constexpr FPoint transformToLocal(const   Rect& reference, const FPoint& target) { return FPoint((target     - reference.pos) / reference.size);                               }
	
	std::ostream& operator<<(std::ostream& os, const FPoint& v);
	std::ostream& operator<<(std::ostream& os, const  Point& v);
	std::ostream& operator<<(std::ostream& os, const  FRect& r);
	std::ostream& operator<<(std::ostream& os, const   Rect& r);
}

#endif