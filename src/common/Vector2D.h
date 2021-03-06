/*
 * Vector2D.h
 * This file is part of Astro Attack
 * Copyright (c) 2014 Christian Zommerfelds
 */

// 2D Vektor

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <boost/shared_ptr.hpp>

extern const float cPi;

float degToRad( float angleDeg );
float radToDeg( float angleRad );

struct b2Vec2;

/*
    Diese Klasse speichert Daten von einem zweidimensinaler Vektor.
    Einige Operationen für Vektoren sind auch implementiert.
*/
class Vector2D
{
public:
    // Default constructor, vector will be [0,0]
    Vector2D() : x ( 0.0f ), y ( 0.0f ) {}
    // Constructor with x and y components
    Vector2D( float x_comp, float y_comp ) : x ( x_comp ), y ( y_comp ) {}
    // Copy constructor
    Vector2D( const Vector2D& v ) : x ( v.x ), y ( v.y ) {}
    // Construct from a Box2D vector
    Vector2D( const b2Vec2& v );

    // The vector coordinates
    float x, y;

    // Convert to a Box2D vector
    boost::shared_ptr<b2Vec2> to_b2Vec2() const;

    // Set new values for x and y
    inline void set( float new_x, float new_y )
    {
        x = new_x;
        y = new_y;
    }

    // Set new x/y values from polar coordinates
    inline void setPolar( float lenght, float angle )
    {
        x = lenght * cos(angle);
        y = lenght * sin(angle);
    }

    // Skalarmultiplikation
    inline Vector2D operator * ( const float  s ) const
    {
        return Vector2D ( x*s, y*s );
    }

    // "Skalardivision"
    inline Vector2D operator / ( const float  s )            const
    {
        return Vector2D ( x/s, y/s );
    }

    // Vektoraddition
    inline Vector2D operator + ( const Vector2D &rV )    const
    {
        return Vector2D ( x+rV.x, y+rV.y );
    }

    // Vektorsubtraktion
    inline Vector2D operator - ( const Vector2D &rV )    const
    {
        return Vector2D ( x-rV.x, y-rV.y );
    }

    // Negativer Vektor
    inline Vector2D operator - ()    const
    {
        return Vector2D ( -x, -y );
    }

    // ==
    inline bool operator == ( const Vector2D &rV )    const
    {
        return ( x==rV.x && y==rV.y );
    }

    // !=
    inline bool operator != ( const Vector2D &rV )    const
    {
        return ( x!=rV.x || y!=rV.y );
    }

    // durch einen Skalar dividieren
    inline Vector2D &operator /= ( const float scalar )
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // mit einem Skalar multiplizieren
    inline Vector2D &operator *= ( const float scalar )
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // um rOther vergrössern
    inline Vector2D &operator += ( const Vector2D &rOther )
    {
        x += rOther.x;
        y += rOther.y;
        return *this;
    }

    // um rOther verkleinern
    inline Vector2D &operator -= ( const Vector2D &rOther )
    {
        x -= rOther.x;
        y -= rOther.y;
        return *this;
    }

    // Vektor festlegen
    inline Vector2D &operator = ( const Vector2D &rV )
    {
        x = rV.x;
        y = rV.y;
        return *this;
    }

    // Skalarprodukt
    inline float operator * ( const Vector2D &rV )    const
    {
        return ( x*rV.x ) + ( y*rV.y );    // dot product
    }

    // Perp Dot Product
    inline float perpDotProd(const Vector2D &v) const
    {
        return x * v.y - y * v.x;
    }

    // Betrag
    inline float length() const
    {
        return sqrt ( x*x + y*y );
    }

    // Betrag im Quadrat (is schneller als Lenght())
    inline float lengthSquared() const
    {
        return x*x + y*y ;
    }

    // Skaliert den Vektor auf Länge 1
    void normalize();

    // Gibt zurück ob rV sich Rechts von diesem Vektor befindet (=kleinster Winkel ist im Uhrzeigersinn)
    bool isRight( const Vector2D &rV ) const;

    // Winkel herausfinden, dass der Vektor mit der X-Achse schliesst ( [1,0] hat Winkel 0, im Gegenuhrzeigersinn +, im Uhrzeigersinn - )
    // Der erhaltene Winkel ist in Radian (zwischen -pi und +pi)
    float getAngle() const;

    // Vektor um Winkel angle (in Bogenmass) drehen im Gegenuhrzeigersinn
    void rotate( float angle );

    // Gibt einen Vektor zurück, der um Winkel angle (in Bogenmass) im Gegenuhrzeigersinn gedreht ist
    Vector2D rotated( float angle ) const;

    // Gibt die Richtung des Vektors als Einheitsvektor zurück
    inline Vector2D getUnitVector() const
    {
        float fLength = length();

        if ( fLength == 0.0f )
            return Vector2D(0.0f,0.0f);

        return ( *this ) * ( 1.0f / fLength );
    }
};

#endif
