#ifndef MASCOT_ARM_H
#define MASCOT_ARM_H

#include "MascotPart.h"


// everything in gdextension is defined in this namespace
namespace godot {
class MascotArm : public MascotPart {
    // this macro sets up a few internal things
    GDCLASS(MascotArm, MascotPart);

private:
    float time_passed;

protected:
    // a static function that Godot will call to find out which methods can be called and which properties it exposes
    static void _bind_methods();

public:
    MascotArm();
    ~MascotArm();

    void _enter_tree ( ) override;
    void _process(double delta) override;
};

}

#endif