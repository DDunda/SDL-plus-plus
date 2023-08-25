#pragma once

#ifndef SDLpp_rect_h_
#define SDLpp_rect_h_

#include <SDL_rect.h>
#include <string>
#include <ostream>

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

		FPoint();
		FPoint(float x, float y);
		FPoint(const FPoint& v);
		FPoint(const SDL_FPoint& point);

		static FPoint FromAngle(float angle, float mag = 1.0f);

		operator std::string() const;

		float sqrMag() const;
		float mag() const;
		float min() const;
		float max() const;

		bool nonZero() const;
		bool inRect(const FRect& r) const;

		FPoint rotate(float rotx, float roty) const;
		FPoint rotate(float a) const;
		FPoint rotateAround(FPoint center, float rotx, float roty) const;
		FPoint rotateAround(FPoint center, float a) const;
		FPoint perp() const;
		FPoint norm() const;
		FPoint abs()  const;

		Point floor() const;
		Point round() const;
		Point ceil()  const;

		FPoint clamp (float x1, float x2, float y1, float y2) const;
		FPoint clampX(float x1, float x2) const;
		FPoint clampY(float y1, float y2) const;
		FPoint clampR(float r1, float r2) const;

		static float  dot  (const FPoint& v1, const FPoint& v2);
		static float  cross(const FPoint& v1, const FPoint& v2);
		static FPoint min  (const FPoint& v1, const FPoint& v2);
		static FPoint max  (const FPoint& v1, const FPoint& v2);

		FPoint operator+() const;
		FPoint operator-() const;

		FPoint operator+(const FPoint& v) const;
		FPoint operator-(const FPoint& v) const;
		FPoint operator*(const FPoint& v) const;
		FPoint operator/(const FPoint& v) const;

		FPoint operator+(const Point& v) const;
		FPoint operator-(const Point& v) const;
		FPoint operator*(const Point& v) const;
		FPoint operator/(const Point& v) const;

		FPoint& operator+=(const FPoint& v);
		FPoint& operator-=(const FPoint& v);
		FPoint& operator*=(const FPoint& v);
		FPoint& operator/=(const FPoint& v);

		FPoint& operator+=(const Point& v);
		FPoint& operator-=(const Point& v);
		FPoint& operator*=(const Point& v);
		FPoint& operator/=(const Point& v);

		FPoint operator*(double m) const;
		FPoint operator/(double m) const;
		FPoint operator*(float m) const;
		FPoint operator/(float m) const;
		FPoint operator*(int m) const;
		FPoint operator/(int m) const;

		FPoint& operator*=(double m);
		FPoint& operator/=(double m);
		FPoint& operator*=(float m);
		FPoint& operator/=(float m);
		FPoint& operator*=(int m);
		FPoint& operator/=(int m);

		bool operator==(const FPoint& v) const;
		bool operator!=(const FPoint& v) const;
	};

	FPoint operator*(double a, const FPoint& b);
	FPoint operator/(double a, const FPoint& b);
	FPoint operator*(float a, const FPoint& b);
	FPoint operator/(float a, const FPoint& b);
	FPoint operator*(int a, const FPoint& b);
	FPoint operator/(int a, const FPoint& b);

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

		Point();
		Point(int x, int y);
		Point(const Point& v);
		Point(const SDL_Point& point);

		operator FPoint() const;
		operator std::string() const;

		int sqrMag() const;
		float mag() const;
		int min() const;
		int max() const;

		bool nonZero() const;
		bool inRect(const Rect& r) const;

		FPoint rotate(float rotx, float roty) const;
		FPoint rotate(float a) const;
		FPoint rotateAround(Point center, float rotx, float roty) const;
		FPoint rotateAround(Point center, float a) const;
		Point perp() const;
		FPoint norm() const;
		Point abs() const;

		Point  clamp (int x1, int x2, int y1, int y2) const;
		Point  clampX(int x1, int x2) const;
		Point  clampY(int y1, int y2) const;
		FPoint clampR(float r1, float r2) const;

		static int dot(const Point& v1, const Point& v2);
		static int cross(const Point& v1, const Point& v2);
		static Point min(const Point& v1, const Point& v2);
		static Point max(const Point& v1, const Point& v2);

		Point operator+ () const;
		Point operator- () const;

		FPoint operator+(const FPoint& v) const;
		FPoint operator-(const FPoint& v) const;
		FPoint operator*(const FPoint& v) const;
		FPoint operator/(const FPoint& v) const;

		Point operator+(const Point& v) const;
		Point operator-(const Point& v) const;
		Point operator*(const Point& v) const;
		Point operator/(const Point& v) const;

		Point& operator+=(const FPoint& v);
		Point& operator-=(const FPoint& v);
		Point& operator*=(const FPoint& v);
		Point& operator/=(const FPoint& v);

		Point& operator+=(const Point& v);
		Point& operator-=(const Point& v);
		Point& operator*=(const Point& v);
		Point& operator/=(const Point& v);

		FPoint operator*(double m) const;
		FPoint operator/(double m) const;
		FPoint operator*(float m) const;
		FPoint operator/(float m) const;
		Point  operator*(int m) const;
		Point  operator/(int m) const;

		Point& operator*=(double m);
		Point& operator/=(double m);
		Point& operator*=(float m);
		Point& operator/=(float m);
		Point& operator*=(int m);
		Point& operator/=(int m);

		bool operator==(const Point& v) const;
		bool operator!=(const Point& v) const;
	};

	FPoint operator*(double a, const Point& b);
	FPoint operator/(double a, const Point& b);
	FPoint operator*(float a, const Point& b);
	FPoint operator/(float a, const Point& b);
	Point operator*(int a, const Point& b);
	Point operator/(int a, const Point& b);

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

		FRect();
		FRect(float x, float y, float w, float h);
		FRect(float x, float y, const FPoint& size);
		FRect(const FPoint& pos, float w, float h);
		FRect(const FPoint& pos, const FPoint& size);
		FRect(const FRect& rect);
		FRect(const SDL_FRect& rect);

		operator std::string() const;

		bool  empty() const;
		float area() const;
		float perimeter() const;
		float diagonal() const;

		Rect floor() const;
		Rect round() const;
		Rect ceil() const;

		FPoint topLeft() const;
		FPoint topRight() const;
		FPoint bottomLeft() const;
		FPoint bottomRight() const;
		FPoint middle() const;

		FPoint clamp(const Point& v) const;
		FPoint clamp(const FPoint& v) const;

		FRect transform(const Rect& target) const;
		FPoint transform(const Point& target) const;
		FRect transform(const FRect& target) const;
		FPoint transform(const FPoint& target) const;

		bool contains(const Point& v) const;
		bool contains(const FPoint& v) const;

		bool intersectsRect(const Rect& r) const;
		bool intersectsRect(const FRect& r) const;

		bool intersectRect(const Rect& rect, FRect& result) const;
		bool intersectRect(const FRect& rect, FRect& result) const;

		FRect operator+ (const FPoint& v) const;
		FRect operator- (const FPoint& v) const;
		FRect operator+ (const Point& v) const;
		FRect operator- (const Point& v) const;

		FRect& operator+=(const FPoint& v);
		FRect& operator-=(const FPoint& v);
		FRect& operator+=(const Point& v);
		FRect& operator-=(const Point& v);

		FRect operator*(const FPoint& v) const;
		FRect operator/(const FPoint& v) const;
		FRect operator*(const Point& v) const;
		FRect operator/(const Point& v) const;

		FRect& operator*=(const FPoint& v);
		FRect& operator/=(const FPoint& v);
		FRect& operator*=(const Point& v);
		FRect& operator/=(const Point& v);

		FRect operator*(double m) const;
		FRect operator/(double m) const;
		FRect operator*(float m) const;
		FRect operator/(float m) const;
		FRect operator*(int m) const;
		FRect operator/(int m) const;

		FRect& operator*=(double m);
		FRect& operator/=(double m);
		FRect& operator*=(float m);
		FRect& operator/=(float m);
		FRect& operator*=(int m);
		FRect& operator/=(int m);

		bool operator==(const FRect& v) const;
		bool operator!=(const FRect& v) const;
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

		Rect();
		Rect(int x, int y, int w, int h);
		Rect(int x, int y, const Point& size);
		Rect(const Point& pos, int w, int h);
		Rect(const Point& pos, const Point& size);
		Rect(const Rect& rect);
		Rect(const SDL_Rect& rect);

		operator FRect() const;
		operator std::string() const;

		bool empty() const;
		int area() const;
		int perimeter() const;
		float diagonal() const;

		Point topLeft() const;
		Point topRight() const;
		Point bottomLeft() const;
		Point bottomRight() const;
		FPoint middle() const;

		Point clamp(const Point& v) const;
		FPoint clamp(const FPoint& v) const;

		FPoint percent(const FPoint& p) const;

		template <typename iterator>
		bool enclosePoints(iterator begin, iterator end, const Rect& clip);
		template <typename iterator>
		bool enclosePoints(iterator begin, iterator end);

		Rect transform(const Rect& target) const;
		Point transform(const Point& target) const;
		FRect transform(const FRect& target) const;
		FPoint transform(const FPoint& target) const;

		bool contains(const Point& v) const;
		bool contains(const FPoint& v) const;

		bool intersectsRect(const Rect& r) const;
		bool intersectsRect(const FRect& r) const;
		bool intersectsLine(const Point& P1, const Point& P2);

		void rectUnion(const Rect& rect, Rect& result) const;

		bool intersectRect(const Rect& rect, Rect& result) const;
		bool intersectRect(const FRect& rect, FRect& result) const;
		bool intersectLine(Point& P1, Point& P2) const;

		FRect operator+(const FPoint& v) const;
		FRect operator-(const FPoint& v) const;
		Rect  operator+(const Point& v) const;
		Rect  operator-(const Point& v) const;

		Rect& operator+=(const FPoint& v);
		Rect& operator-=(const FPoint& v);
		Rect& operator+=(const Point& v);
		Rect& operator-=(const Point& v);

		FRect operator*(const FPoint& v) const;
		FRect operator/(const FPoint& v) const;
		Rect  operator*(const Point& v) const;
		Rect  operator/(const Point& v) const;

		Rect& operator*=(const FPoint& v);
		Rect& operator/=(const FPoint& v);
		Rect& operator*=(const Point& v);
		Rect& operator/=(const Point& v);

		FRect operator*(double m) const;
		FRect operator/(double m) const;
		FRect operator*(float m) const;
		FRect operator/(float m) const;
		Rect  operator*(int m) const;
		Rect  operator/(int m) const;

		Rect& operator*=(double m);
		Rect& operator/=(double m);
		Rect& operator*=(float m);
		Rect& operator/=(float m);
		Rect& operator*=(int m);
		Rect& operator/=(int m);

		bool operator==(const Rect& v) const;
		bool operator!=(const Rect& v) const;
	};

	std::ostream& operator<<(std::ostream& os, const FPoint& v);
	std::ostream& operator<<(std::ostream& os, const Point& v);
	std::ostream& operator<<(std::ostream& os, const FRect& r);
	std::ostream& operator<<(std::ostream& os, const Rect& r);
}

#endif