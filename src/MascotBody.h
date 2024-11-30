#ifndef JACK_BODY_H
#define JACK_BODY_H

#include "MascotPart.h"

#define BODY_RAD 1.5
#define BODY_HEIGHT 4.0

// everything in gdextension is defined in this namespace
namespace godot {
class MascotBody : public MascotPart {
    // this macro sets up a few internal things
    GDCLASS(MascotBody, MascotPart);

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
    static void _bind_methods();

public:
    MascotBody();
    ~MascotBody();

    void _enter_tree ( ) override;
    void _process(double delta) override;
};

}

#endif