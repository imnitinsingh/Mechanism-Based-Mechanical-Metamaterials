#ifndef VECTOR2_HPP
#define VECTOR2_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#include <ostream>
#include <iomanip>
#include <cmath>

// --------------------------------------------------------------------------------------------------------------------------------

struct Vector2
{
   // constructor(s)
   explicit Vector2() noexcept : x_(0), y_(0) { }
   Vector2(double x, double y) noexcept : x_(x), y_(y) { }

   // operator overloads with lhs is this
   Vector2 operator+(const Vector2& rhs) const { return add(*this, rhs); }
   Vector2 operator-(const Vector2& rhs) const { return substract(*this, rhs); }
   Vector2 operator*(double rhs) const { return multiply(*this, rhs); }
   Vector2 operator*(Vector2 rhs) const { return multiply(*this, rhs); }
   Vector2 operator/(double rhs) const { return divide(*this, rhs); }
   Vector2 operator/(Vector2 rhs) const { return divide(*this, rhs); }

   bool operator==(const Vector2& rhs) const { return x_ == rhs.x_ && y_ == rhs.y_; }
   bool operator!=(const Vector2& rhs) const { return !operator==(rhs); }
   Vector2 operator-() const { return Vector2(-x_, -y_); }

   // operator overloads with lhs and assign to this
   Vector2& operator+=(const Vector2& rhs) { *this = add(*this, rhs); return *this; }
   Vector2& operator-=(const Vector2& rhs) { *this = substract(*this, rhs); return *this; }
   Vector2& operator*=(double rhs) { *this = multiply(*this, rhs); return *this; }
   Vector2& operator/=(double rhs) { *this = divide(*this, rhs); return *this; }

   // utility
   double length() const { return length(*this); }
   double atan_mod() const { return atan_mod(*this); }
   Vector2 normal() const { return normal(*this); }
   Vector2 abs() const { return abs(*this); }

   // static methods
   static Vector2 add(const Vector2& u, const Vector2& v) { return Vector2(u.x_ + v.x_, u.y_ + v.y_); }
   static Vector2 substract(const Vector2& u, const Vector2& v) { return Vector2(u.x_ - v.x_, u.y_ - v.y_); }
   static Vector2 multiply(const Vector2& u, double s) { return Vector2(u.x_ * s, u.y_ *s); }
   static Vector2 multiply(const Vector2& u, const Vector2& v) { return Vector2(u.x_ * v.x_, u.y_ *v.y_); }
   static Vector2 divide(const Vector2& u, double s) { return Vector2(u.x_ / s, u.y_ / s); }
   static Vector2 divide(const Vector2& u, const Vector2& v) { return Vector2(u.x_ / v.x_, u.y_ / v.y_); }

   static double length(const Vector2& u) { return std::sqrt(dot(u, u)); }
   static double atan_mod(const Vector2& u) { auto angle = std::atan2(u.Y(), u.X()); return angle < 0 ? 8 * std::atan(1) + angle : angle; }
   static Vector2 normal(const Vector2& u) { return divide(u, u.length()); }
   static Vector2 abs(const Vector2& u) { return Vector2(std::abs(u.x_), std::abs(u.y_)); }

   static double dot(const Vector2& u, const Vector2& v) { return u.x_ * v.x_ + u.y_ * v.y_; }

   // --------------------------------------------------------------------------------------------------------------------------------

   double X() const { return x_; }
   double Y() const { return y_; }

private:
   double x_, y_;
};

// --------------------------------------------------------------------------------------------------------------------------------
// operator overloads with lhs double and rhs vector 

inline Vector2 operator*(double lhs, const Vector2& rhs)
{
   return Vector2::multiply(rhs, lhs);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector2& v)
{
   stream << std::setprecision(20) << v.X() << "\t" << v.Y();
   return stream;
}

namespace std
{
   template<> struct hash <Vector2>
   {
      size_t operator()(const Vector2& v) const { return hash<double>()(v.X()) + 3 * hash<double>()(v.Y()); }
   };
}

// --------------------------------------------------------------------------------------------------------------------------------

#endif