#include "ray.hpp"

namespace SDL
{
	Ray::operator std::string() const { return "(" + (std::string)origin + " -> " + (std::string)dir + ")"; }

	static std::ostream& operator<<(std::ostream& os, const Ray& r) { return os << (std::string)r; }
}