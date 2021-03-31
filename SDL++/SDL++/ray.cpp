#include "ray.hpp"

using namespace SDL;

Ray::Ray() : origin(), dir() {}
Ray::Ray(FPoint origin, FPoint dir) : origin(origin), dir(dir) {}

Ray::operator std::string() const { return "(" + (std::string)origin + " -> " + (std::string)dir + ")"; }

bool Ray::intersectRect(const Rect& rect) {
	hit.contact = false;

	// Broad collision check
	if (!rect.intersectsRect({ FPoint::min(origin, origin + dir), dir.abs() })) return false;

	if (dir.x == 0 && dir.y == 0) return false; // No ray
	if (dir.y == 0) { // Horizontal
		float invX = 1.0 / dir.x;
		float nearX = (rect.pos.x - origin.x) * invX;
		float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
		float time = std::min(nearX, farX);

		hit = {
			true,
			origin + time * dir,
			dir.x < 0 ? FPoint(1,0) : FPoint(-1,0),
			time,
		};

		return true;
	}
	if (dir.x == 0) { // Vertical
		float invY = 1.0 / dir.y;
		float nearY = (rect.pos.y - origin.y) * invY;
		float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
		float time = std::min(nearY, farY);

		hit = {
			true,
			origin + time * dir,
			dir.x < 0 ? FPoint(0,1) : FPoint(0,-1),
			time,
		};

		return true;
	}

	FPoint invDir = 1.0f / dir;

	// Normalised proportion of ray
	FPoint nearNorm = (rect.pos - origin) * invDir;
	FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

	if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
	if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

	if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return false;

	hit.contact = true;
	hit.time = nearNorm.max();
	hit.point = origin + hit.time * dir;

	hit.normal = { 0, 0 };
	if (nearNorm.x >= nearNorm.y) hit.normal.x = dir.x < 0 ? 1 : -1;
	if (nearNorm.x <= nearNorm.y) hit.normal.y = dir.y < 0 ? 1 : -1;

	return true;
}
bool Ray::intersectRect(const FRect& rect) {
	hit.contact = false;

	// Broad collision check
	if (!rect.intersectsRect(FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

	if (dir.x == 0 && dir.y == 0) return false; // No ray
	if (dir.y == 0) { // Horizontal
		float invX = 1.0 / dir.x;
		float nearX = (rect.pos.x - origin.x) * invX;
		float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
		float time = std::min(nearX, farX);

		hit = {
			true,
			origin + time * dir,
			dir.x < 0 ? FPoint(1,0) : FPoint(-1,0),
			time,
		};

		return true;
	}
	if (dir.x == 0) { // Vertical
		float invY = 1.0 / dir.y;
		float nearY = (rect.pos.y - origin.y) * invY;
		float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
		float time = std::min(nearY, farY);

		hit = {
			true,
			origin + time * dir,
			dir.x < 0 ? FPoint(0,1) : FPoint(0,-1),
			time,
		};

		return true;
	}

	FPoint invDir = 1.0f / dir;

	// Normalised proportion of ray
	FPoint nearNorm = (rect.pos - origin) * invDir;
	FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

	if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
	if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

	if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return false;

	hit.contact = true;
	hit.time = nearNorm.max();
	hit.point = origin + hit.time * dir;

	hit.normal = { 0, 0 };
	if (nearNorm.x >= nearNorm.y) hit.normal.x = dir.x < 0 ? 1 : -1;
	if (nearNorm.x <= nearNorm.y) hit.normal.y = dir.y < 0 ? 1 : -1;

	return true;
}

bool Ray::intersectsRect(const Rect& rect) const {
	// Broad collision check
	if (!rect.intersectsRect(FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

	if (dir.x == 0 && dir.y == 0) return false; // No ray
	if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

	FPoint invDir = 1.0f / (FPoint)dir;

	// Normalised proportion of ray
	FPoint nearNorm = (rect.pos - origin) * invDir;
	FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

	if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
	if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

	return nearNorm.x < farNorm.y&& nearNorm.y > farNorm.x && nearNorm.max() <= 1;
}
bool Ray::intersectsRect(const FRect& rect) const {
	// Broad collision check
	if (!rect.intersectsRect(FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

	if (dir.x == 0 && dir.y == 0) return false; // No ray
	if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

	FPoint invDir = 1.0f / dir;

	// Normalised proportion of ray
	FPoint nearNorm = (rect.pos - origin) * invDir;
	FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

	if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
	if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

	return nearNorm.x < farNorm.y&& nearNorm.y > farNorm.x && nearNorm.max() <= 1;
}

static std::ostream& operator<<(std::ostream& os, const Ray& r) { return os << (std::string)r; }