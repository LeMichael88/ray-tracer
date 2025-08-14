/**
 * Name: Material.hpp
 * Description: Base abstract base class for materials. This class serves as a base for
 * all material types in the rendering engine to derive from.
 */

#pragma once

class Material {
public:
    virtual ~Material();

protected:
    Material();
};
