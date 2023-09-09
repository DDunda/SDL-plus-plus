#ifndef SDL_ray_hpp_
#define SDL_ray_hpp_
#pragma once

#include "rect.hpp"

#include <ostream>
#include <string>

namespace SDL
{
	struct Ray
	{
		struct Contact
		{
			bool contact = false;
			FPoint point = { 0,0 };
			FPoint normal = { 0,0 };
			float time = 0;
		};

		FPoint origin;
		FPoint dir;

		inline constexpr Ray(                         ) : origin(0,0   ), dir(0,0) {}
		inline constexpr Ray(FPoint origin, FPoint dir) : origin(origin), dir(dir) {}

		inline operator std::string() const { return "(" + (std::string)origin + " -> " + (std::string)dir + ")"; }

		inline constexpr Contact IntersectRect(const Rect& rect) const
		{
			// Broad collision check
			if (!RectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return { false };

			if (dir.x == 0 && dir.y == 0) return { false }; // No ray

			if (dir.y == 0) // Horizontal
			{
				const float invX = 1.f / dir.x;
				const float nearX = (rect.pos.x - origin.x) * invX;
				const float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
				const float time = std::min(nearX, farX);

				return Contact
				{
					true,
					origin + time * dir,
					dir.x < 0 ? FPoint(1,0) : FPoint(-1,0),
					time,
				};
			}
			if (dir.x == 0) // Vertical
			{
				const float invY = 1.f / dir.y;
				const float nearY = (rect.pos.y - origin.y) * invY;
				const float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
				const float time = std::min(nearY, farY);

				return Contact
				{
					true,
					origin + time * dir,
					dir.y < 0 ? FPoint(0,1) : FPoint(0,-1),
					time,
				};
			}

			const FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return { false };

			Contact hit
			{
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

		inline constexpr Contact IntersectRect(const FRect& rect) const
		{
			// Broad collision check
			if (!RectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return { false };
			
			if (dir.x == 0 && dir.y == 0) return { false }; // No ray

			if (dir.y == 0) // Horizontal
			{
				const float invX = 1.f / dir.x;
				const float nearX = (rect.pos.x - origin.x) * invX;
				const float farX = (rect.pos.x + rect.size.x - origin.x) * invX;
				const float time = std::min(nearX, farX);

				return Contact
				{
					true,
					origin + time * dir,
					dir.x < 0 ? FPoint(1.f,0.f) : FPoint(-1.f,0.f),
					time,
				};
			}
			if (dir.x == 0) // Vertical
			{
				const float invY = 1.f / dir.y;
				const float nearY = (rect.pos.y - origin.y) * invY;
				const float farY = (rect.pos.y + rect.size.y - origin.y) * invY;
				const float time = std::min(nearY, farY);

				return Contact
				{
					true,
					origin + time * dir,
					dir.y < 0 ? FPoint(0.f,1.f) : FPoint(0.f,-1.f),
					time,
				};
			}

			const FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			if (nearNorm.x > farNorm.y || nearNorm.y > farNorm.x || nearNorm.max() > 1) return { false };

			Contact hit
			{
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

		inline bool IntersectsRect(const Rect& rect) const
		{
			// Broad collision check
			if (!RectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

			if (dir.x == 0 && dir.y == 0) return false; // No ray
			if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

			const FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			return nearNorm.x < farNorm.y && nearNorm.y > farNorm.x && nearNorm.max() <= 1;
		}

		inline bool IntersectsRect(const FRect& rect) const
		{
			// Broad collision check
			if (!RectsIntersect(rect, FRect(FPoint::min(origin, origin + dir), dir.abs()))) return false;

			if (dir.x == 0 && dir.y == 0) return false; // No ray
			if (dir.x == 0 || dir.y == 0) return true; // Horizontal or vertical

			const FPoint invDir = 1.f / dir;

			// Normalised proportion of ray
			FPoint nearNorm = (rect.pos - origin) * invDir;
			FPoint farNorm = (rect.pos + rect.size - origin) * invDir;

			if (nearNorm.x > farNorm.x) std::swap(nearNorm.x, farNorm.x);
			if (nearNorm.y > farNorm.y) std::swap(nearNorm.y, farNorm.y);

			return nearNorm.x < farNorm.y && nearNorm.y > farNorm.x && nearNorm.max() <= 1;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Ray& r)
	{
		return os << (std::string)r;
	}
}

#endif