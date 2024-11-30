#ifndef MASCOT_HEAD_H
#define MASCOT_HEAD_H

#include "MascotPart.h"

#define HEAD_RAD 2.0
#define HEAD_HEIGHT 4.0

// everything in gdextension is defined in this namespace
namespace godot {
class MascotHead : public MascotPart {
    // this macro sets up a few internal things
    GDCLASS(MascotHead, MascotPart);

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
    static void _bind_methods();

public:
    MascotHead();
    ~MascotHead();

    void _enter_tree ( ) override;
    void _process(double delta) override;
};

}

#endif