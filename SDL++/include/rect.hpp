#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_rect_hpp_
#define SDL_rect_hpp_
#pragma once

#include <SDL_rect.h>

#include "container.hpp"

#include <algorithm>
#include <cmath>
#include <ostream>
#include <string>

namespace SDL
{
	struct Point;
	struct FPoint;
	struct Rect;
	struct FRect;

	struct FPoint
	{
		union
		{
			struct { float x, y; };
			struct { float w, h; };
			SDL_FPoint point;
		};

		inline constexpr FPoint() : x(0), y(0) {}
		inline constexpr FPoint(float x, float y) : x(x), y(y) {}
		inline constexpr FPoint(const FPoint& v) : x(v.x), y(v.y) {}
		inline constexpr FPoint(const SDL_FPoint& point) : x(point.x), y(point.y) {}

		inline static FPoint FromAngle(float angle, float mag = 1.0f);

		operator std::string() const;

		inline constexpr float sqrMag() const { return x * x + y * y; }
		inline constexpr float min() const { return std::min(x, y); }
		inline constexpr float max() const { return std::max(x, y); }
		float mag() const;

		inline constexpr bool nonZero() const { return x != 0 || y != 0; }

		inline constexpr FPoint rotate(float rotx, float roty) const { return FPoint(x * rotx - y * roty, x * roty + y * rotx); }
		inline constexpr FPoint rotateAround(FPoint center, float rotx, float roty) const { return (*this - center).rotate(rotx, roty) + center; }
		FPoint rotate(float a) const;
		FPoint rotateAround(FPoint center, float a) const;
		inline constexpr FPoint perp() const { return FPoint(-y, x); }
		inline constexpr FPoint abs() const { return FPoint(x < 0 ? -x : x, y < 0 ? -y : y); }
		FPoint norm() const;

		Point floor() const;
		Point round() const;
		Point ceil()  const;

		inline constexpr FPoint clamp(float x1, float x2, float y1, float y2) const { return FPoint(std::clamp(x, x1, x2), std::clamp(y, y1, y2)); }
		inline constexpr FPoint clampX(float x1, float x2) const { return FPoint(std::clamp(x, x1, x2), y); }
		inline constexpr FPoint clampY(float y1, float y2) const { return FPoint(x, std::clamp(y, y1, y2)); }
		FPoint clampR(float r1, float r2) const;

		inline static constexpr float  dot(const FPoint& v1, const FPoint& v2) { return v1.x * v2.x + v1.y * v2.y; }
		inline static constexpr float  cross(const FPoint& v1, const FPoint& v2) { return v1.y * v2.x - v1.x * v2.y; }
		inline static constexpr FPoint min(const FPoint& v1, const FPoint& v2) { return FPoint(std::min(v1.x, v2.x), std::min(v1.y, v2.y)); }
		inline static constexpr FPoint max(const FPoint& v1, const FPoint& v2) { return FPoint(std::max(v1.x, v2.x), std::max(v1.y, v2.y)); }

		inline constexpr FPoint operator+() const { return FPoint(+x, +y); }
		inline constexpr FPoint operator-() const { return FPoint(-x, -y); }

		inline constexpr FPoint operator+(const FPoint& v) const { return FPoint(x + v.x, y + v.y); }
		inline constexpr FPoint operator-(const FPoint& v) const { return FPoint(x - v.x, y - v.y); }
		inline constexpr FPoint operator*(const FPoint& v) const { return FPoint(x * v.x, y * v.y); }
		inline constexpr FPoint operator/(const FPoint& v) const { return FPoint(x / v.x, y / v.y); }

		inline constexpr FPoint operator*(double m) const { return FPoint(float(x * m), float(y * m)); }
		inline constexpr FPoint operator/(double m) const { return FPoint(float(x / m), float(y / m)); }
		inline constexpr FPoint operator*(float m) const { return FPoint(x * m, y * m); }
		inline constexpr FPoint operator/(float m) const { return FPoint(x / m, y / m); }
		inline constexpr FPoint operator*(int m) const { return FPoint(x * m, y * m); }
		inline constexpr FPoint operator/(int m) const { return FPoint(x / m, y / m); }

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

		inline constexpr bool operator==(const FPoint& v) const { return x == v.x && y == v.y; }
		inline constexpr bool operator!=(const FPoint& v) const { return x != v.x || y != v.y; }
	};

	struct Point
	{
		union
		{
			struct { int x, y; };
			struct { int w, h; };
			SDL_Point point;
		};

		inline constexpr Point() : x(0), y(0) {}
		inline constexpr Point(int x, int y) : x(x), y(y) {}
		inline constexpr Point(const Point& v) : x(v.x), y(v.y) {}
		inline constexpr Point(const SDL_Point& point) : x(point.x), y(point.y) {}

		inline constexpr operator FPoint() const { return { float(x), float(y) }; }
		operator std::string() const;

		inline constexpr int sqrMag() const { return x * x + y * y; }
		inline constexpr int min() const { return std::min(x, y); }
		inline constexpr int max() const { return std::max(x, y); }
		float mag() const;

		inline constexpr bool nonZero() const { return x != 0 || y != 0; }

		inline constexpr FPoint rotate(float rotx, float roty) const { return { x * rotx - y * roty, x * roty + y * rotx }; }
		inline constexpr FPoint rotateAround(Point center, float rotx, float roty) const { return (*this - center).rotate(rotx, roty) + center; }
		FPoint rotate(float a) const;
		FPoint rotateAround(Point center, float a) const;
		inline constexpr Point perp() const { return { -y, x }; }
		inline constexpr Point abs() const { return { x < 0 ? -x : x, y < 0 ? -y : y }; }
		FPoint norm() const;

		inline constexpr Point clamp(int x1, int x2, int y1, int y2) const { return { std::clamp(x, x1, x2), std::clamp(y, y1, y2) }; }
		inline constexpr Point clampX(int x1, int x2) const { return { std::clamp(x, x1, x2), y }; }
		inline constexpr Point clampY(int y1, int y2) const { return Point(x, std::clamp(y, y1, y2)); }
		FPoint clampR(float r1, float r2) const;

		inline static constexpr int   dot(const Point& v1, const Point& v2) { return v1.x * v2.x + v2.y * v1.y; }
		inline static constexpr int cross(const Point& v1, const Point& v2) { return v2.x * v1.y - v2.y * v1.x; }
		inline static constexpr Point min(const Point& v1, const Point& v2) { return { std::min(v1.x, v2.x), std::min(v1.y, v2.y) }; }
		inline static constexpr Point max(const Point& v1, const Point& v2) { return { std::max(v1.x, v2.x), std::max(v1.y, v2.y) }; }

		inline constexpr Point operator+ () const { return { +x, +y }; };
		inline constexpr Point operator- () const { return { -x, -y }; };

		inline constexpr Point operator+(const Point& v) const { return { x + v.x, y + v.y }; }
		inline constexpr Point operator-(const Point& v) const { return { x - v.x, y - v.y }; }
		inline constexpr Point operator*(const Point& v) const { return { x * v.x, y * v.y }; }
		inline constexpr Point operator/(const Point& v) const { return { x / v.x, y / v.y }; }

		inline constexpr FPoint operator*(double m) const { return { float(x * m), float(y * m) }; }
		inline constexpr FPoint operator/(double m) const { return { float(x / m), float(y / m) }; }
		inline constexpr FPoint operator*(float m) const { return { x * m, y * m }; }
		inline constexpr FPoint operator/(float m) const { return { x / m, y / m }; }
		inline constexpr Point  operator*(int m) const { return { x * m, y * m }; };
		inline constexpr Point  operator/(int m) const { return { x / m, y / m }; };

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

		inline constexpr bool operator==(const Point& v) const { return x == v.x && y == v.y; }
		inline constexpr bool operator!=(const Point& v) const { return x != v.x || y != v.y; }
	};

	inline constexpr FPoint operator+(const FPoint& v1, const Point& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	inline constexpr FPoint operator-(const FPoint& v1, const Point& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
	inline constexpr FPoint operator*(const FPoint& v1, const Point& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
	inline constexpr FPoint operator/(const FPoint& v1, const Point& v2) { return { v1.x / v2.x, v1.y / v2.y }; }

	inline constexpr FPoint operator+(const Point& v1, const FPoint& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	inline constexpr FPoint operator-(const Point& v1, const FPoint& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
	inline constexpr FPoint operator*(const Point& v1, const FPoint& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
	inline constexpr FPoint operator/(const Point& v1, const FPoint& v2) { return { v1.x / v2.x, v1.y / v2.y }; }

	inline constexpr FPoint operator*(double m, const FPoint& v) { return { float(m * v.x), float(m * v.y) }; }
	inline constexpr FPoint operator/(double m, const FPoint& v) { return { float(m / v.x), float(m / v.y) }; }
	inline constexpr FPoint operator*(float m, const FPoint& v) { return { m * v.x, m * v.y }; }
	inline constexpr FPoint operator/(float m, const FPoint& v) { return { m / v.x, m / v.y }; }
	inline constexpr FPoint operator*(int m, const FPoint& v) { return { m * v.x, m * v.y }; }
	inline constexpr FPoint operator/(int m, const FPoint& v) { return { m / v.x, m / v.y }; }

	inline constexpr FPoint operator*(double m, const Point& v) { return { float(m * v.x), float(m * v.y) }; }
	inline constexpr FPoint operator/(double m, const Point& v) { return { float(m / v.x), float(m / v.y) }; }
	inline constexpr FPoint operator*(float m, const Point& v) { return { m * v.x, m * v.y }; }
	inline constexpr FPoint operator/(float m, const Point& v) { return { m / v.x, m / v.y }; }
	inline constexpr  Point operator*(int m, const Point& v) { return { m * v.x, m * v.y }; }
	inline constexpr  Point operator/(int m, const Point& v) { return { m / v.x, m / v.y }; }

	struct FRect
	{
		union
		{
			struct
			{
				union
				{
					struct { float x, y; };
					FPoint pos;
				};
				union
				{
					struct { float w, h; };
					FPoint size;
				};
			};
			SDL_FRect rect;
		};

		inline constexpr FRect() : pos(0.f, 0.f), size(0.f, 0.f) {}
		inline constexpr FRect(float x, float y, float w, float h) : pos(x, y), size(w, h) {}
		inline constexpr FRect(float x, float y, const FPoint& size) : pos(x, y), size(size) {}
		inline constexpr FRect(const FPoint& pos, float w, float h) : pos(pos), size(w, h) {}
		inline constexpr FRect(const FPoint& pos, const FPoint& size) : pos(pos), size(size) {}
		inline constexpr FRect(const FRect& rect) : pos(rect.pos), size(rect.size) {}
		inline constexpr FRect(const SDL_FRect& rect) : pos(rect.x, rect.y), size(rect.x, rect.y) {}

		operator std::string() const;

		inline constexpr bool  empty() const { return size.x <= 0 || size.y <= 0; }
		inline constexpr float area() const { return size.x * size.y; }
		inline constexpr float perimeter() const { return 2 * (size.x + size.y); }
		float diagonal() const;

		Rect floor() const;
		Rect round() const;
		Rect ceil() const;

		inline constexpr FPoint normToPoint(FPoint v) const { return (v * size) + pos; }
		inline constexpr FPoint pointToNorm(FPoint v) const { return (v - pos) / size; }

		inline constexpr FPoint topLeft() const { return pos; }
		inline constexpr FPoint topRight() const { return pos + FPoint(size.y, 0); }
		inline constexpr FPoint bottomLeft() const { return pos + FPoint(0, size.x); }
		inline constexpr FPoint bottomRight() const { return pos + size; }
		inline constexpr FPoint middle() const { return pos + size / 2.0f; }

		inline constexpr FPoint clamp(const  Point& v) const { return { std::clamp(float(v.x), pos.x, pos.x + size.x), std::clamp(float(v.y), pos.y, pos.y + size.y) }; }
		inline constexpr FPoint clamp(const FPoint& v) const { return { std::clamp(v.x , pos.x, pos.x + size.x), std::clamp(v.y , pos.y, pos.y + size.y) }; }

#if SDL_VERSION_ATLEAST(2, 0, 22)
		inline bool enclosePoints(const FPoint* points, int count, const FRect& clip) { return SDL_EncloseFPoints((const SDL_FPoint*)points, count, &clip.rect, &rect) == SDL_TRUE; }
		inline bool enclosePoints(const FPoint* points, int count) { return SDL_EncloseFPoints((const SDL_FPoint*)points, count, NULL, &rect) == SDL_TRUE; }

		template <const int length>
		inline bool enclosePoints(const FPoint(&points)[length], const FRect& clip) { return enclosePoints(points, length, clip); }

		template <const int length>
		inline bool enclosePoints(const FPoint(&points)[length]) { return enclosePoints(points, length); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FPoint, T>::is_continuous_container>>
		inline bool enclosePoints(const T& points, const FRect& clip)
			{ return enclosePoints(points.data(), (int)points.size(), clip); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<FPoint, T>::is_continuous_container>>
		inline bool enclosePoints(const T& points)
			{ return enclosePoints(points.data(), (int)points.size()); }
#endif

		inline constexpr bool contains(const  Point& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		inline constexpr bool contains(const FPoint& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }

		inline constexpr FRect operator+(const FPoint& v) const { return { pos + v, size }; }
		inline constexpr FRect operator-(const FPoint& v) const { return { pos - v, size }; }
		inline constexpr FRect operator+(const  Point& v) const { return { pos + v, size }; }
		inline constexpr FRect operator-(const  Point& v) const { return { pos - v, size }; }

		inline constexpr FRect operator* (const FPoint& v) const { return { pos, size * v }; }
		inline constexpr FRect operator/ (const FPoint& v) const { return { pos, size / v }; }
		inline constexpr FRect operator* (const  Point& v) const { return { pos, size * v }; }
		inline constexpr FRect operator/ (const  Point& v) const { return { pos, size / v }; }

		inline constexpr FRect operator*(double m) const { return { pos, size * m }; }
		inline constexpr FRect operator/(double m) const { return { pos, size / m }; }
		inline constexpr FRect operator*(float m) const { return { pos, size * m }; }
		inline constexpr FRect operator/(float m) const { return { pos, size / m }; }
		inline constexpr FRect operator*(int m) const { return { pos, size * m }; }
		inline constexpr FRect operator/(int m) const { return { pos, size / m }; }

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

		inline constexpr bool operator==(const FRect& v) const { return x == v.x && y == v.y && w == v.w && h == v.h; }
		inline constexpr bool operator!=(const FRect& v) const { return x != v.x || y != v.y || w != v.w || h != v.h; }
	};

	struct Rect
	{
		union
		{
			struct
			{
				union
				{
					struct { int x, y; };
					Point pos;
				};
				union
				{
					struct { int w, h; };
					Point size;
				};
			};
			SDL_Rect rect;
		};

		inline constexpr Rect() : pos(0, 0), size(0, 0) {}
		inline constexpr Rect(int x, int y, int w, int h) : pos(x, y), size(w, h) {}
		inline constexpr Rect(int x, int y, const Point& size) : pos(x, y), size(size) {}
		inline constexpr Rect(const Point& pos, int w, int h) : pos(pos), size(w, h) {}
		inline constexpr Rect(const Point& pos, const Point& size) : pos(pos), size(size) {}
		inline constexpr Rect(const Rect& rect) : pos(rect.pos), size(rect.size) {}
		inline constexpr Rect(const SDL_Rect& rect) : pos(rect.x, rect.y), size(rect.w, rect.h) {}

		inline constexpr operator FRect() const { return { pos, size }; }
		operator std::string() const;

		inline constexpr bool empty() const { return size.x <= 0 || size.y <= 0; }
		inline constexpr int  area() const { return size.x * size.y; }
		inline constexpr int  perimeter() const { return 2 * (size.x + size.y); }
		float diagonal() const;

		inline constexpr FPoint normToPoint(FPoint v) const { return (v * size) + pos; }
		inline constexpr FPoint pointToNorm(FPoint v) const { return (v - pos) / size; }

		inline constexpr  Point topLeft() const { return pos; }
		inline constexpr  Point topRight() const { return pos + Point(size.x, 0); }
		inline constexpr  Point bottomLeft() const { return pos + Point(0, size.y); }
		inline constexpr  Point bottomRight() const { return pos + size; }
		inline constexpr FPoint middle() const { return pos + size / 2.0f; }

		inline constexpr  Point clamp(const  Point& v) const { return v.clamp(pos.x, pos.x + size.x, pos.y, pos.y + size.y); }
		inline constexpr FPoint clamp(const FPoint& v) const { return v.clamp(float(pos.x), float(pos.x + size.x), float(pos.y), float(pos.y + size.y)); }

		inline bool enclosePoints(const Point* points, int count, const Rect& clip) { return SDL_EnclosePoints((const SDL_Point*)points, count, &clip.rect, &rect) == SDL_TRUE; }
		inline bool enclosePoints(const Point* points, int count) { return SDL_EnclosePoints((const SDL_Point*)points, count, NULL, &rect) == SDL_TRUE; }

		template <const int length>
		inline bool enclosePoints(const Point(&points)[length], const Rect& clip) { return SDL_EnclosePoints((const SDL_Point*)points, length, &clip.rect, &rect) == SDL_TRUE; }
		template <const int length>
		inline bool enclosePoints(const Point(&points)[length]) { return SDL_EnclosePoints((const SDL_Point*)points, length, NULL, &rect) == SDL_TRUE; }
		
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Point, T>::is_continuous_container>>
		inline bool enclosePoints(const T& points, const Rect& clip)
			{ return enclosePoints(points.data(), (int)points.size(), clip); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Point, T>::is_continuous_container>>
		inline bool enclosePoints(const T& points)
			{ return enclosePoints(points.data(), (int)points.size()); }

		inline constexpr bool contains(const  Point& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		inline constexpr bool contains(const FPoint& v) const { return v.x > pos.x && v.y > pos.y && v.x < pos.x + size.x && v.y < pos.y + size.y; }
		bool intersectsLine(const Point& P1, const Point& P2);

		bool intersectLine(Point& P1, Point& P2) const;

		inline constexpr FRect operator+(const FPoint& v) const { return { pos + v, size }; }
		inline constexpr FRect operator-(const FPoint& v) const { return { pos - v, size }; }
		inline constexpr Rect  operator+(const  Point& v) const { return { pos + v, size }; }
		inline constexpr Rect  operator-(const  Point& v) const { return { pos - v, size }; }

		inline constexpr FRect operator* (const FPoint& v) const { return { pos, size * v }; }
		inline constexpr FRect operator/ (const FPoint& v) const { return { pos, size / v }; }
		inline constexpr Rect  operator* (const Point& v)  const { return { pos, size * v }; }
		inline constexpr Rect  operator/ (const Point& v)  const { return { pos, size / v }; }

		inline constexpr FRect operator*(double m) const { return { pos, size * m }; }
		inline constexpr FRect operator/(double m) const { return { pos, size / m }; }
		inline constexpr FRect operator*(float m)  const { return { pos, size * m }; }
		inline constexpr FRect operator/(float m)  const { return { pos, size / m }; }
		inline constexpr Rect  operator*(int m)    const { return { pos, size * m }; }
		inline constexpr Rect  operator/(int m)    const { return { pos, size / m }; }

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

		inline constexpr bool operator==(const Rect& r) const { return pos.x == r.pos.x && pos.y == r.pos.y && size.x == r.size.x && size.y == r.size.y; }
		inline constexpr bool operator!=(const Rect& r) const { return pos.x != r.pos.x || pos.y != r.pos.y || size.x != r.size.x || size.y != r.size.y; }
	};

	inline constexpr bool RectsIntersect(const  Rect& A, const  Rect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	inline constexpr bool RectsIntersect(const  Rect& A, const FRect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	inline constexpr bool RectsIntersect(const FRect& A, const  Rect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }
	inline constexpr bool RectsIntersect(const FRect& A, const FRect& B) { return !A.empty() && !B.empty() && A.pos.x + A.size.x > B.pos.x && A.pos.x < B.pos.x + B.size.x && A.pos.y + A.size.y > B.pos.y && A.pos.y < B.pos.y + B.size.y; }

	inline constexpr  Rect RectIntersection(const  Rect& A, const  Rect& B)
	{
		if (!RectsIntersect(A, B)) return {};

		const Point top_left = Point::max(A.topLeft(), B.topLeft());
		const Point bottom_right = Point::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	inline constexpr FRect RectIntersection(const FRect& A, const  Rect& B)
	{
		if (!RectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	inline constexpr FRect RectIntersection(const  Rect& A, const FRect& B)
	{
		if (!RectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return { top_left, bottom_right - top_left };
	}
	inline constexpr FRect RectIntersection(const FRect& A, const FRect& B)
	{
		if (!RectsIntersect(A, B)) return {};

		const FPoint top_left = FPoint::max(A.topLeft(), B.topLeft());
		const FPoint bottom_right = FPoint::min(A.bottomRight(), B.bottomRight());

		return FRect(top_left, bottom_right - top_left);
	}

	inline constexpr  Rect RectUnion(const  Rect& A, const Rect& B)
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
	inline constexpr FRect RectUnion(const FRect& A, const Rect& B)
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
	inline constexpr FRect RectUnion(const  Rect& A, const FRect& B)
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
	inline constexpr FRect RectUnion(const FRect& A, const FRect& B)
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

#if SDL_VERSION_ATLEAST(2, 0, 22)
	inline void UnionFRect(const FRect& A, const FRect& B, FRect& out) { SDL_UnionFRect(&A.rect, &B.rect, &out.rect); }
	inline FRect UnionFRect(const FRect& A, const FRect& B)
	{
		FRect result;
		UnionFRect(A, B, result);
		return result;
	}
	// Returns true if the two rectangles are equal, within some given epsilon.
	inline bool FRectEqualsEpsilon(const FRect& A, const FRect& B, const float epsilon)
	{
		// This is directly taken from the header
		return (&A == &B) || (
			(SDL_fabsf(A.x - B.x) <= epsilon) &&
			(SDL_fabsf(A.y - B.y) <= epsilon) &&
			(SDL_fabsf(A.w - B.w) <= epsilon) &&
			(SDL_fabsf(A.h - B.h) <= epsilon)
		);
	}

	// Returns true if the two rectangles are equal, using a default epsilon.
	inline bool FRectEquals(const FRect& A, const FRect& B) { return FRectEqualsEpsilon(A, B, SDL_FLT_EPSILON); }

	/**
	 * Determine whether two rectangles intersect with float precision.
	 *
	 * If either pointer is NULL the function will return false.
	 *
	 * \param A an FRect structure representing the first rectangle
	 * \param B an FRect structure representing the second rectangle
	 * \returns true if there is an intersection, false otherwise.
	 */
	inline bool HasIntersectionF(const FRect& A, const FRect& B) { return SDL_HasIntersectionF(&A.rect, &B.rect) == SDL_TRUE; }

	/**
	 * Calculate the intersection of two rectangles with float precision.
	 *
	 * \param A an FRect structure representing the first rectangle
	 * \param B an FRect structure representing the second rectangle
	 * \param result an FRect structure filled in with the intersection of
	 *               rectangles `A` and `B`
	 * \returns true if there is an intersection, false otherwise.
	 */
	inline bool IntersectFRect(const FRect& A, const FRect& B, FRect& result) { return SDL_IntersectFRect(&A.rect, &B.rect, &result.rect) == SDL_TRUE; }
#endif

	inline constexpr  FRect transformToWorld(const FRect& reference, const   Rect& target) { return  FRect( target.pos * reference.size + reference.pos,  target.size * reference.size); }
	inline constexpr FPoint transformToWorld(const FRect& reference, const  Point& target) { return FPoint( target     * reference.size + reference.pos                               ); }
	inline constexpr  FRect transformToWorld(const FRect& reference, const  FRect& target) { return  FRect( target.pos * reference.size + reference.pos,  target.size * reference.size); }
	inline constexpr FPoint transformToWorld(const FRect& reference, const FPoint& target) { return FPoint( target     * reference.size + reference.pos                               ); }

	inline constexpr   Rect transformToWorld(const  Rect& reference, const   Rect& target) { return   Rect( target.pos * reference.size + reference.pos,  target.size * reference.size); }
	inline constexpr  Point transformToWorld(const  Rect& reference, const  Point& target) { return  Point( target     * reference.size + reference.pos                               ); }
	inline constexpr  FRect transformToWorld(const  Rect& reference, const  FRect& target) { return  FRect( target.pos * reference.size + reference.pos,  target.size * reference.size); }
	inline constexpr FPoint transformToWorld(const  Rect& reference, const FPoint& target) { return FPoint( target     * reference.size + reference.pos                               ); }

	inline constexpr  FRect transformToLocal(const FRect& reference, const   Rect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	inline constexpr FPoint transformToLocal(const FRect& reference, const  Point& target) { return FPoint((target     - reference.pos) / reference.size                              ); }
	inline constexpr  FRect transformToLocal(const FRect& reference, const  FRect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	inline constexpr FPoint transformToLocal(const FRect& reference, const FPoint& target) { return FPoint((target     - reference.pos) / reference.size                              ); }

	inline constexpr   Rect transformToLocal(const  Rect& reference, const   Rect& target) { return   Rect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	inline constexpr  Point transformToLocal(const  Rect& reference, const  Point& target) { return  Point((target     - reference.pos) / reference.size                              ); }
	inline constexpr  FRect transformToLocal(const  Rect& reference, const  FRect& target) { return  FRect((target.pos - reference.pos) / reference.size, target.size * reference.size); }
	inline constexpr FPoint transformToLocal(const  Rect& reference, const FPoint& target) { return FPoint((target     - reference.pos) / reference.size                              ); }

	inline std::ostream& operator<<(std::ostream& os, const FPoint& p) { return os << (std::string)p; }
	inline std::ostream& operator<<(std::ostream& os, const  Point& p) { return os << (std::string)p; }
	inline std::ostream& operator<<(std::ostream& os, const  FRect& r) { return os << (std::string)r; }
	inline std::ostream& operator<<(std::ostream& os, const   Rect& r) { return os << (std::string)r; }
}

#endif
#endif