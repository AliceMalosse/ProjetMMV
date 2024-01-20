/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "perlin.h"
#include "simplexnoise1234.h"

perlin::perlin()
    :octave_data(5),persistency_data(0.3f)
{}

float perlin::operator()(Vector const& p) const
{
    float value=0.0f;
    double frequency=1.0f;
    double persistency=1.0;

    for(int k=0;k<octave_data;k++)
    {
        value += persistency*(0.5f+0.5f*snoise3(p[0]*frequency,
                                                p[1]*frequency,
                                                p[2]*frequency));
        frequency *= 2.0f;
        persistency *= persistency_data;
    }

    return value;
}

