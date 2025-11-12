#pragma once
#include <iostream>

#include "vector2.h"

class Anchor {
    public:
        static constexpr float DIST_CONSTRAINT = 2;
        float radius;
        float angle;

        Vector2 position;
        Anchor *next;  // the next anchor of a chain

        Anchor();
        Anchor(Vector2 point, float angle);
        virtual void MoveTo(const Vector2 point);
        virtual void SetPosition(const Vector2 point);
};

class Head : public Anchor {
    public:
        Head();
        Head(const Vector2 point, const float angle);
        void MoveTo(const Vector2 point) override;
        void SetPosition(const Vector2 point) override;
};