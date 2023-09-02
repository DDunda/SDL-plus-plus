#pragma once

#ifndef SDLpp_ray_h_
#define SDLpp_ray_h_

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

		constexpr Ray(                         ) : origin(0,0   ), dir(0,0) {}
		constexpr Ray(FPoint origin, FPoint dir) : origin(origin), dir(dir) {}

		operator std::string() const;

#undef min

		constexpr RayContact intersectRect(const Rect& rect) const
		{
			// Broad collision check
			if (!rectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return { false };

			if (dir.x == 0 && dir.y == 0) return { false }; // No ray
			if (dir.y == 0) { // Horizontal
				float invX = 1.f / dir.x;
				float nearX = (rect.pos.x - origin.x) * invX;
				float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
				float time = std::min(nearX, farX);

				return {
					true,
					origin + time * dir,
					dir.x < 0 ? FPoint(1,0) : FPoint(-1,0),
					time,
				};
			}
			if (dir.x == 0) { // Vertical
				float invY = 1.f / dir.y;
				float nearY = (rect.pos.y - origin.y) * invY;
				float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
				float time = std::min(nearY, farY);

				return {
					true,
					origin + time * dir,
					dir.y < 0 ? FPoint(0,1) : FPoint(0,-1),
					time,
				};
			}

			FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return { false };

			RayContact hit{
				true,
				{ 0, 0 },
				{ 0, 0 },
				nearNorm.max()
			};

			hit.point = origin + hit.time * dir;

			if (nearNorm.x >= nearNorm.y) hit.normal.x = dir.x < 0 ? 1.f : -1.f;
			if (nearNorm.x <= nearNorm.y) hit.normal.y = dir.y < 0 ? 1.f : -1.f;

			return hit;
		}

		constexpr RayContact intersectRect(const FRect& rect) const
		{
			// Broad collision check
			if (!rectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return { false };
			
			if (dir.x == 0 && dir.y == 0) return { false }; // No ray
			if (dir.y == 0) { // Horizontal
				float invX = 1.f / dir.x;
				float nearX = (rect.pos.x - origin.x) * invX;
				float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
				float time = std::min(nearX, farX);

				return {
					true,
					origin + time * dir,
					dir.x < 0 ? FPoint(1.f,0.f) : FPoint(-1.f,0.f),
					time,
				};
			}
			if (dir.x == 0) { // Vertical
				float invY = 1.f / dir.y;
				float nearY = (rect.pos.y - origin.y) * invY;
				float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
				float time = std::min(nearY, farY);

				return {
					true,
					origin + time * dir,
					dir.y < 0 ? FPoint(0.f,1.f) : FPoint(0.f,-1.f),
					time,
				};
			}

			FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return { false };

			RayContact hit {
				true,
				{ 0, 0 },
				{ 0, 0 },
				nearNorm.max()
			};

			hit.point = origin + hit.time * dir;

			if (nearNorm.x >= nearNorm.y) hit.normal.x = dir.x < 0 ? 1.f : -1.f;
			if (nearNorm.x <= nearNorm.y) hit.normal.y = dir.y < 0 ? 1.f : -1.f;

			return hit;
			return { true };
		}

		constexpr bool intersectsRect(const Rect& rect) const
		{
			// Broad collision check
			if (!rectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

			if (dir.x == 0 && dir.y == 0) return false; // No ray
			if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

			FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			return nearNorm.x < farNorm.y && nearNorm.y > farNorm.x && nearNorm.max() <= 1;
		}

		constexpr bool intersectsRect(const FRect& rect) const
		{
			// Broad collision check
			if (!rectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

			if (dir.x == 0 && dir.y == 0) return false; // No ray
			if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

			FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			return nearNorm.x < farNorm.y && nearNorm.y > farNorm.x && nearNorm.max() <= 1;
		}

		friend std::ostream& operator<<(std::ostream& os, const Ray& r);
	};
}

#endif