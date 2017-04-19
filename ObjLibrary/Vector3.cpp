//
//  Vector3.cpp
//
//  This file is part of the ObjLibrary, by Richard Hamilton,
//    which is copyright Hamilton 2009-2014.
//
//  You may use these files for any purpose as long as you do
//    not explicitly claim them as your own work or object to
//    other people using them.  If you are in a position of
//    authority, you may forbid others ffrom using them in areas
//    that fall under your authority.  For example, a professor
//    could forbid students from using them for a class project,
//    or an employer could forbid employees using for a company
//    project.
//
//  If you are destributing the source files, you must not
//    remove this notice.  If you are only destributing compiled
//    code, no credit is required.
//
//  A (theoretically) up-to-date version of the ObjLibrary can
//    be found at:
//  http://infiniplix.ca/resources/obj_library/
//

#include <iostream>
#include <cstdlib>	// for rand()
#include <cassert>

#include "Vector3.h"

using namespace std;
namespace
{
	//
	//  random2 (adapted from Random.h/.cpp)
	//
	//  Purpose: To generate a psudorandom double in the
	//	     specified range.
	//  Parameter(s):
	//	<1> min: The inclusive minimum
	//	<2> max: The exclusive maximum
	//  Precondition:
	//	<1> min <= max
	//	<2> The random number generator must have been
	//	    initialized
	//  Returns: A psuedorandom value in the range
	//	     [min, max).
	//  Side Effect: N/A
	//  Note: Calls the rand() function
	//

	inline double random2 (double min, double max)
	{
		assert(min <= max);

		double random0to1 = (double)rand() / ((double) (RAND_MAX) + 1.0);
		return min + random0to1 * (max - min);
	}



	const unsigned int MATRIX3X3_COLUMNS = 9;
	const unsigned int MATRIX3X3_ROWS = 9;
	const unsigned int MATRIX3X3_ENTRIES = MATRIX3X3_COLUMNS * MATRIX3X3_ROWS;

	//
	//  Matrix3x3
	//
	//  A minimal class to encapsulate a 3x3 matrix.  This
	//    class is used when rotating a Vector3 around an
	//    arbitrary axis.
	//

	class Matrix3x3
	{
	public:
		Matrix3x3(double e11, double e12, double e13,
		          double e21, double e22, double e23,
		          double e31, double e32, double e33)
		{
			ma_entries[0] = e11;
			ma_entries[1] = e12;
			ma_entries[2] = e13;
			ma_entries[3] = e21;
			ma_entries[4] = e22;
			ma_entries[5] = e23;
			ma_entries[6] = e31;
			ma_entries[7] = e32;
			ma_entries[8] = e33;
		}

		Matrix3x3(const Matrix3x3& original)
		{
			for(unsigned int i = 0; i < MATRIX3X3_ENTRIES; i++)
				ma_entries[i] = original.ma_entries[i];
		}

		Matrix3x3 operator+ (const Matrix3x3& other) const
		{
			Matrix3x3 result(*this);

			for(unsigned int i = 0; i < MATRIX3X3_ENTRIES; i++)
				result.ma_entries[i] += other.ma_entries[i];

			return result;
		}

		Matrix3x3 operator- (const Matrix3x3& other) const
		{
			Matrix3x3 result(*this);

			for(unsigned int i = 0; i < MATRIX3X3_ENTRIES; i++)
				result.ma_entries[i] -= other.ma_entries[i];

			return result;
		}

		Matrix3x3 operator* (double constant) const
		{
			Matrix3x3 result(*this);

			for(unsigned int i = 0; i < MATRIX3X3_ENTRIES; i++)
				result.ma_entries[i] *= constant;

			return result;
		}

		Vector3 operator* (const Vector3& right) const
		{
			return Vector3(right.x * ma_entries[0] + right.y * ma_entries[1] + right.z * ma_entries[2],
			               right.x * ma_entries[3] + right.y * ma_entries[4] + right.z * ma_entries[5],
			               right.x * ma_entries[6] + right.y * ma_entries[7] + right.z * ma_entries[8]);
		}

		inline double getEntry (unsigned int row, unsigned int column) const
		{
			assert(row < MATRIX3X3_ROWS);
			assert(column < MATRIX3X3_COLUMNS);

			return ma_entries[row * MATRIX3X3_COLUMNS + column];
		}

	private:
		double ma_entries[MATRIX3X3_ENTRIES];
	};

	const Matrix3x3 INDENTITY_MATRIX(1, 0, 0, 0, 1, 0, 0, 0, 1);

}	// end of anonymous namespace



const Vector3 Vector3 :: ZERO(0.0, 0.0, 0.0);
const Vector3 Vector3 :: ONE (1.0, 1.0, 1.0);
const Vector3 Vector3 :: UNIT_X_PLUS ( 1.0,  0.0,  0.0);
const Vector3 Vector3 :: UNIT_X_MINUS(-1.0,  0.0,  0.0);
const Vector3 Vector3 :: UNIT_Y_PLUS ( 0.0,  1.0,  0.0);
const Vector3 Vector3 :: UNIT_Y_MINUS( 0.0, -1.0,  0.0);
const Vector3 Vector3 :: UNIT_Z_PLUS ( 0.0,  0.0,  1.0);
const Vector3 Vector3 :: UNIT_Z_MINUS( 0.0,  0.0, -1.0);



double Vector3 :: getCosAngle (const Vector3& other) const
{
	assert(isFinite());
	assert(!isZero());
	assert(other.isFinite());
	assert(!other.isZero());

	double ratio = dotProduct(other) / (getNorm() * other.getNorm());

	//  In theory, ratio should always be in the range [-1, 1].
	//    Sadly, in reality there are floating point errors.
	return (ratio < -1.0) ? -1.0 : ((ratio > 1.0) ? 1.0 : ratio);
}

double Vector3 :: getCosAngleSafe (const Vector3& other) const
{
	assert(isFinite());
	assert(other.isFinite());

	if(isZero() ||other.isZero())
		return 1.0;

	double ratio = dotProduct(other) / (getNorm() * other.getNorm());

	//  In theory, ratio should always be in the range [-1, 1].
	//    Sadly, in reality there are floating point errors.
	return (ratio < -1.0) ? -1.0 : ((ratio > 1.0) ? 1.0 : ratio);
}

double Vector3 :: getAngle (const Vector3& other) const
{
	assert(isFinite());
	assert(!isZero());
	assert(other.isFinite());
	assert(!other.isZero());

	static const double PI = 3.1415926535897932384626433832795;

	double ratio = dotProduct(other) / (getNorm() * other.getNorm());

	//  In theory, ratio should always be in the range [-1, 1].
	//    Sadly, in reality there are floating point errors.
	return (ratio < -1.0) ? PI : ((ratio > 1.0) ? 0.0 : acos(ratio));
}

double Vector3 :: getAngleSafe (const Vector3& other) const
{
	assert(isFinite());
	assert(other.isFinite());

	static const double PI = 3.1415926535897932384626433832795;

	if(isZero() ||other.isZero())
		return 1.0;

	double ratio = dotProduct(other) / (getNorm() * other.getNorm());

	//  In theory, ratio should always be in the range [-1, 1].
	//    Sadly, in reality there are floating point errors.
	return (ratio < -1.0) ? PI : ((ratio > 1.0) ? 0.0 : acos(ratio));
}



Vector3 Vector3 :: getRotatedArbitrary (const Vector3& axis, double radians) const
{
	assert(isFinite());
	assert(axis.isFinite());
	assert(axis.isNormal());

	// www2.cs.uregina.ca/~anima/408/Notes/ObjectModels/Rotation.htm

	double aa = axis.x * axis.x;
	double bb = axis.y * axis.y;
	double cc = axis.z * axis.z;
	double ab = axis.x * axis.y;
	double ac = axis.x * axis.z;
	double bc = axis.y * axis.z;

	Matrix3x3 A_hat(aa, ab, ac, ab, bb, bc, ac, bc, cc);
	Matrix3x3 A_star( 0,      -axis.z,  axis.y,
	                  axis.z,  0,      -axis.x,
	                 -axis.y,  axis.x,  0);

	Matrix3x3 M = A_hat + (INDENTITY_MATRIX - A_hat) * cos(radians) + A_star * sin(radians);

	return M * (*this);
}

void Vector3 :: rotateArbitrary (const Vector3& axis, double radians)
{
	assert(isFinite());
	assert(axis.isFinite());
	assert(axis.isNormal());

	// www2.cs.uregina.ca/~anima/408/Notes/ObjectModels/Rotation.htm

	double aa = axis.x * axis.x;
	double bb = axis.y * axis.y;
	double cc = axis.z * axis.z;
	double ab = axis.x * axis.y;
	double ac = axis.x * axis.z;
	double bc = axis.y * axis.z;

	Matrix3x3 A_hat(aa, ab, ac, ab, bb, bc, ac, bc, cc);
	Matrix3x3 A_star( 0,      -axis.z,  axis.y,
	                  axis.z,  0,      -axis.x,
	                 -axis.y,  axis.x,  0);

	Matrix3x3 M = A_hat + (INDENTITY_MATRIX - A_hat) * cos(radians) + A_star * sin(radians);

	operator=(M * (*this));
}

Vector3 Vector3 :: getMatrixProduct (double e11, double e12, double e13,
				     double e21, double e22, double e23,
				     double e31, double e32, double e33) const
{
	assert(isFinite());

	return Matrix3x3(e11, e12, e13, e21, e22, e23, e31, e32, e33) * (*this);
}

Vector3 Vector3 :: getRandomUnitVector ()
{
	//
	//  The following WDL script is included because it was
	//    used to derive the algorithm for this function.
	//

/*
From Spheretest.wdl (with variable names added)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// This function creates a hollow sphere of "equally" spaced balls
ACTION Spheremaker
{
	z_spacing = 360/around_z;

	z_current = z_spacing;
	while(z_current < 180)
	{
		xy_radius = sin(z_current)*radius;
		z_radius = cos(z_current)*radius;

		around_count = int(sin(z_current)*360/z_spacing);

		around_current = 0;
		while(around_current < around_count)
		{
			glow_pos.x = sin(around_current/around_count*360)*xy_radius;
			glow_pos.y = cos(around_current/around_count*360)*xy_radius;
			glow_pos.z = z_radius;
			ent_create(ball_mdl,glow_pos,Spherepoint);

			around_current++;
		}

		z_current += z_spacing;
	}
}
*/

	//
	//  Generate a random unit vector.
	//
	//  1. pick a random direction on the xy plane
	//  2. pick a random angle towards to z-axis, weighted
	//     so that the values on the xy plane are the most
	//     likely.
	//

	static const double TWO_PI = 6.283185307179586476925286766559;

	double xy_angle = random2(0.0, TWO_PI);
	double vertical_angle = asin(random2(-1.0, 1.0));

	double radius_xy = cos(vertical_angle);

	double x = radius_xy * cos(xy_angle);
	double y = radius_xy * sin(xy_angle);
	double z = sin(vertical_angle);

	return Vector3(x, y, z);
}



Vector3 Vector3 :: getClosestPointOnLine (const Vector3& l1, const Vector3& l2, const Vector3& p, bool bounded)
{
	assert(l1.isFinite());
	assert(l2.isFinite());
	assert(p.isFinite());
	assert(l1 != l2);

	//
	//         + p
	//       /
	//     /
	//   +-----+-------+
	//  l1     s       l2
	//     +
	//     O
	//
	//  O: The origin (0, 0, 0)
	//  l1, l2: The two ends of the line segment
	//  p: The not-on-line-point
	//  s: The point on the line segment closest to p
	//

	Vector3 line_direction = l2 - l1;
	Vector3 p_direction = p - l1;
	Vector3 s_minus_l1 = p_direction.projection(line_direction);

	if(bounded)
	{
		if(s_minus_l1.dotProduct(line_direction) <= 0)
			return l1;
		else if(s_minus_l1.getNormSquared() > line_direction.getNormSquared())
			return l2;
	}

	return s_minus_l1 + l1;
}



ostream& operator<< (ostream& r_os, const Vector3& vector)
{
	r_os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return r_os;
}
