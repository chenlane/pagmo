 /*****************************************************************************
 *   Copyright (C) 2004-2013 The PaGMO development team,                     *
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
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../src/pagmo.h"
#include "../src/keplerian_toolbox/planet_ss.h"
#include "../src/keplerian_toolbox/epoch.h"


//-------------------------------------------------------------------------------
// static data needed to test the non-default constructor in some of the problems.
#ifdef PAGMO_ENABLE_KEP_TOOLBOX 
//mga_1dsm
const std::vector<kep_toolbox::planet_ptr> construct_sequence() {
	std::vector<kep_toolbox::planet_ptr> retval;
	retval.push_back(kep_toolbox::planet_ss("earth").clone());
	retval.push_back(kep_toolbox::planet_ss("earth").clone());
	retval.push_back(kep_toolbox::planet_ss("earth").clone());
	return retval;
};
#endif

//knapsack
static std::vector<double> a(11,30), b(11,10);
static double c = 15;

//laplace
static const int default_sequence[5] = {3,2,2,1,5};
//--------------------------------------------------------------------------------

///The idea of this unit test is to serialize all pagmo::problems, deserialize them and check that
///the objective function and the constraint implementation return the same in the original and in the deserialized object

using namespace pagmo;
int main()
{
	unsigned int dimension = 24;

	// create two containers of pagmo::problems
	std::vector<problem::base_ptr> probs;
	std::vector<problem::base_ptr> probs_new;
	
	// fill it up with problems
	probs.push_back(problem::ackley(dimension).clone());
	probs_new.push_back(problem::ackley().clone());
	probs.push_back(problem::rosenbrock(dimension).clone());
	probs_new.push_back(problem::rosenbrock().clone());
	probs.push_back(problem::branin().clone());
	probs_new.push_back(problem::branin().clone());
	probs.push_back(problem::dejong(dimension).clone());
	probs_new.push_back(problem::dejong().clone());
	probs.push_back(problem::fon().clone());
	probs_new.push_back(problem::fon().clone());
	probs.push_back(problem::golomb_ruler(10,20).clone());
	probs_new.push_back(problem::golomb_ruler().clone());
	probs.push_back(problem::griewank(dimension).clone());
	probs_new.push_back(problem::griewank().clone());
	probs.push_back(problem::himmelblau().clone());
	probs_new.push_back(problem::himmelblau().clone());
	probs.push_back(problem::string_match("e dai dai dai.....portiamolo a casa!!").clone());
	probs_new.push_back(problem::string_match().clone());
	probs.push_back(problem::inventory(7,8,1234).clone());
	probs_new.push_back(problem::inventory().clone());
	probs.push_back(problem::knapsack(a,b,c).clone());
	probs_new.push_back(problem::knapsack().clone());
	probs.push_back(problem::kur(dimension).clone());
	probs_new.push_back(problem::kur().clone());
	probs.push_back(problem::lennard_jones(dimension).clone());
	probs_new.push_back(problem::lennard_jones().clone());
	probs.push_back(problem::levy5(dimension).clone());
	probs_new.push_back(problem::levy5().clone());
	probs.push_back(problem::luksan_vlcek_1(dimension).clone());
	probs_new.push_back(problem::luksan_vlcek_1().clone());
	probs.push_back(problem::luksan_vlcek_2(dimension).clone());
	probs_new.push_back(problem::luksan_vlcek_2().clone());
	probs.push_back(problem::luksan_vlcek_3(dimension).clone());
	probs_new.push_back(problem::luksan_vlcek_3().clone());
	probs.push_back(problem::michalewicz(dimension).clone());
	probs_new.push_back(problem::michalewicz().clone());
	probs.push_back(problem::pol().clone());
	probs_new.push_back(problem::pol().clone());
	probs.push_back(problem::rastrigin(dimension).clone());
	probs_new.push_back(problem::rastrigin().clone());
	probs.push_back(problem::sch().clone());
	probs_new.push_back(problem::sch().clone());
	probs.push_back(problem::schwefel(dimension).clone());
	probs_new.push_back(problem::schwefel().clone());
	probs.push_back(problem::snopt_toyprob().clone());
	probs_new.push_back(problem::snopt_toyprob().clone());
	probs.push_back(problem::zdt1(dimension).clone());
	probs_new.push_back(problem::zdt1().clone());
	probs.push_back(problem::zdt2(dimension).clone());
	probs_new.push_back(problem::zdt2().clone());
	probs.push_back(problem::zdt3(dimension).clone());
	probs_new.push_back(problem::zdt3().clone());
	probs.push_back(problem::zdt4(dimension).clone());
	probs_new.push_back(problem::zdt4().clone());
	probs.push_back(problem::zdt6(dimension).clone());
	probs_new.push_back(problem::zdt6().clone());
	probs.push_back(problem::dtlz1(dimension).clone());
	probs_new.push_back(problem::dtlz1().clone());
	probs.push_back(problem::dtlz2(dimension).clone());
	probs_new.push_back(problem::dtlz2().clone());
	probs.push_back(problem::dtlz3(dimension).clone());
	probs_new.push_back(problem::dtlz3().clone());
	probs.push_back(problem::dtlz4(dimension).clone());
	probs_new.push_back(problem::dtlz4().clone());
	probs.push_back(problem::dtlz5(dimension).clone());
	probs_new.push_back(problem::dtlz5().clone());
	probs.push_back(problem::dtlz6(dimension).clone());
	probs_new.push_back(problem::dtlz6().clone());
	probs.push_back(problem::dtlz7(dimension).clone());
	probs_new.push_back(problem::dtlz7().clone());
	probs.push_back(problem::tsp().clone()); //TODO: define the tsp using a non-default weight-matrix
	probs_new.push_back(problem::tsp().clone());
	
#ifdef PAGMO_ENABLE_KEP_TOOLBOX
	probs.push_back(problem::cassini_1(2).clone());
	probs_new.push_back(problem::cassini_1().clone());
	probs.push_back(problem::cassini_2().clone());
	probs_new.push_back(problem::cassini_2().clone());
	probs.push_back(problem::gtoc_1().clone());
	probs_new.push_back(problem::gtoc_1().clone());
	probs.push_back(problem::messenger().clone());
	probs_new.push_back(problem::messenger().clone());
	probs.push_back(problem::rosetta().clone());
	probs_new.push_back(problem::rosetta().clone());
	probs.push_back(problem::messenger_full().clone());
	probs_new.push_back(problem::messenger_full().clone());
	probs.push_back(problem::tandem(3,10).clone());
	probs_new.push_back(problem::tandem().clone());
	probs.push_back(problem::laplace(std::vector<int>(default_sequence,default_sequence + 5)).clone());
	probs_new.push_back(problem::laplace().clone());
	probs.push_back(problem::mga_1dsm_alpha(construct_sequence()).clone());
	probs_new.push_back(problem::mga_1dsm_alpha().clone());
	probs.push_back(problem::mga_1dsm_tof(construct_sequence()).clone());
	probs_new.push_back(problem::mga_1dsm_tof().clone());
#endif	

	
	//serialize probs and deserialize into probs_new checking they are then identical
	for (size_t i=0; i< probs.size(); ++i) {
		{
		// create and open a character archive for output
		std::ofstream ofs("test.ar");
		// save data to archive
		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa & probs[i];
		// archive and stream closed when destructors are called
		}
	
		{
		// create and open an archive for input
		std::ifstream ifs("test.ar");
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia & probs_new[i];
		// archive and stream closed when destructors are called
		}
		
		{
		decision_vector x(probs[i]->get_dimension(),0);
		fitness_vector f1(probs[i]->get_f_dimension(),0), f2(probs[i]->get_f_dimension(),0);
		constraint_vector c1(probs[i]->get_c_dimension(),0), c2(probs[i]->get_c_dimension());
		population pop(*probs[i],1);
		x = pop.champion().x;
		probs[i]->objfun(f1,x);
		probs_new[i]->objfun(f2,x);
		probs[i]->compute_constraints(c1,x);
		probs_new[i]->compute_constraints(c2,x);
		std::cout << std::endl << std::setw(40) << probs[i]->get_name();
		if (std::equal(f1.begin(),f1.end(),f2.begin())) {
			std::cout << ": Fitness pass,";
		} else { 
			std::cout << ": Fitness FAILED," << std::endl;
			return 1;
		}
		if (std::equal(c1.begin(),c1.end(),c2.begin())) {
			std::cout << " Constraints pass";
		} else {
			std::cout << " Constraints FAILED" << std::endl;
			return 1;
		}
		}
	
	}
	std::cout << std::endl;
	return 0;
}