#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include "Player.h"

class Weapon: virtual public Player
{
    protected:

    int gunAngle = 0;

    public:

        getAngle ();
        gunRotate ();
};

#endif // WEAPON_H_INCLUDED
