#pragma once

#include "point.h"
#include "ray.h"
#include "object.h"

class space;

// physic classes
class mirror: public virtual object {
public:
    virtual ~mirror() {}

    void interact(ray& r, space&) const override;
    virtual void reflection(ray&) const = 0;
};

class screen: public virtual object {
public:
    virtual ~screen() {}

    // generic realisation
    void interact(ray& r, space&) const override;
};