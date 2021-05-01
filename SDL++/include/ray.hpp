#pragma once

#include "rect.hpp"
#include <string>
#include <ostream>

namespace SDL {
	struct RayContact {
		bool contact = false;
		FPoint point = { 0,0 };
		FPoint normal = { 0,0 };
		float time = 0;
	};

	struct Ray {
		FPoint origin;
		FPoint dir;

		RayContact hit;

		Ray();
		Ray(FPoint origin, FPoint dir);

		operator std::string() const;

		bool intersectsRect(const Rect& rect) const;
		bool intersectsRect(const FRect& rect) const;
		bool intersectRect(const Rect& rect);
		bool intersectRect(const FRect& rect);

		friend std::ostream& operator<<(std::ostream& os, const Ray& r);
	};
}
