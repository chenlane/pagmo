/*****************************************************************************
 *   Copyright (C) 2004-2009 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#ifndef ASTRO_CONSTANTS_H
#define ASTRO_CONSTANTS_H

#include<boost/math/constants/constants.hpp>
#include<boost/array.hpp>

#define ASTRO_AU 149597870660.0
#define ASTRO_MU_SUN 1.32712428e20
#define ASTRO_EARTH_VELOCITY 29784.6905
#define ASTRO_DEG2RAD (boost::math::constants::pi<double>()/180.0)
#define ASTRO_RAD2DEG (180.0/boost::math::constants::pi<double>())
#define ASTRO_DAY2SEC 86400.0 //needs to be a double
#define ASTRO_SEC2DAY 1.157407407407407407407407407e-05
#define ASTRO_G0 9.80665

//This is used as a numerical proceure (e.g. newton-raphson or runge-kutta) stopping criteria
#define ASTRO_TOLERANCE 1e-12

//This needs to be set to the precision of the boost date library (microseconds is default,
//nanoseconds can be set when compiling boosts. Note that the code has not been checked in that case)
#define BOOST_DATE_PRECISION 1e-6

//Typedef for fixed size vectors
typedef boost::array<double,3> array3D;
typedef boost::array<double,6> array6D;
typedef boost::array<double,7> array7D;

#endif // ASTRO_CONSTANTS_H
