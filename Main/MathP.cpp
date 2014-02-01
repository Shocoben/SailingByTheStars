#include "MathP.h"

using namespace Ogre;

template<typename T>
bool solveQuadratic(const T &a, const T &b, const T &c, T &x0, T &x1)
{
    T discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        T q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}


std::pair<bool, Ogre::Real> MathP::rayIntersectSphere(Ogre::Ray& ray, const Ogre::Sphere& s)
{
		Ogre::Vector3 c = s.getCenter();
		bool OriginInsideTheSpere = (ray.getOrigin().distance( c ) < s.getRadius() )? true : false;

		if (OriginInsideTheSpere)
		{
			
			Vector3 L = ray.getOrigin() - c;
			Ogre::Real a = ray.getDirection().dotProduct(ray.getDirection());
			Ogre::Real b = 2 * ray.getDirection().dotProduct(L);
			Ogre::Real c = L.dotProduct(L) - (s.getRadius() * s.getRadius());

			Ogre::Real t0 = 0;
			Ogre::Real t1 = 0;

			solveQuadratic<Ogre::Real>(a, b, c, t0, t1);
			std::pair <bool,Real> result = std::make_pair(true,t1);
			return result;
		}
		else
		{
			return Math::intersects(ray, s);
		}
}