#ifndef EARTH_MARS_LT_H
#define EARTH_MARS_LT_H

#include <cmath>
#include <iostream>

#include "../../AstroToolbox/Pl_Eph_An.h"
#include "../../AstroToolbox/propagateKEP.h"
#include "GOproblem.h"

#define R 149597870.66
#define V std::sqrt(1.32712428e+11 / 149597870.66)
#define T (R / V)
#define A (R / (T * T))
#define F (1 * A)

class earth_mars_lt_problem: public GOProblem {
	public:
		// NOTE: parameters must be dimensional:
		// - thrust in Newton
		// - mass in kilograms
		earth_mars_lt_problem(int n_, const double &M_, const double &thrust_, const double &Isp_):GOProblem(n_ * 3 + 5),n(n_),M(M_),
			thrust((thrust_ / 1000) / F),Isp(Isp_)
		{
// std::cout << "thrust:\t" << thrust << '\n';
// std::cout << R << '\n';
// std::cout << V << '\n';
// std::cout << T << '\n';
// std::cout << A << '\n';
// std::cout << F << '\n';
// std::cout << M << '\n';
// std::cout << "-------" << '\n';
			// NOTE: sphere picking coord.
			// Dep. date.
			LB[0] = 0;
			UB[0] = 5000;
			// Vinf at departure.
			LB[1] = 0;
			UB[1] = 3;
			LB[2] = 0;
			UB[2] = 1;
			LB[3] = 0;
			UB[3] = 1;
			for (int i = 0; i < n; ++i) {
				// Segments' deltaVs.
				LB[3 * i + 4] = 0;
				UB[3 * i + 4] = 1;
				LB[3 * i + 5] = 0;
				UB[3 * i + 5] = 1;
				LB[3 * i + 6] = 0;
				UB[3 * i + 6] = 1;
			}
			// Transfer time.
			LB.back() = 0;
			UB.back() = 1000;
		}
		virtual earth_mars_lt_problem *clone() const {return new earth_mars_lt_problem(*this);}
		virtual std::string id_object() const {
			return "hippo's problem";
		}
		void human_readable(const std::vector<double> &x) const {
			std::cout <<
				"number of segments:\t\t" << n << '\n' <<
				"max thrust:\t\t" << thrust * F * 1000 << '\n' <<
				"max DV per segment (km/s):\t" << (thrust * F / M * x.back() * 86400. / n) << '\n' <<
				"mass:\t\t" << M << '\n' <<
				"dep. date (mjd2000):\t" << x[0] << '\n' <<
				"vinf at departure:\t" << x[1] << '\n';
			std::cout << "DV\t\tvx\tvy\tvz\t\n";
			double tmp_velocity[3];
			double dt = (x.back() / n) * 86400 / T;
			for (size_t i = 0; i < (size_t)n; ++i) {
				ruv2cart(tmp_velocity,&x[3 * i + 4]);
				std::cout << x[3 * i + 4] * thrust / M * dt * V << "\t\t";
				std::cout << tmp_velocity[0] * V * thrust / M * dt << '\t' << tmp_velocity[1] * V * thrust / M * dt <<
					'\t' << tmp_velocity[2] * V * thrust / M * dt << '\n';
			}
			std::cout << "duration (days):\t" << x.back() << '\n';
			double r_fwd[3], v_fwd[3], r_back[3], v_back[3];
			state_mismatch(x,r_fwd,v_fwd,r_back,v_back);
			std::cout << "pos mismatch:\t" << (r_fwd[0] - r_back[0]) << '\t' << (r_fwd[1] - r_back[1]) << '\t' << (r_fwd[2] - r_back[2]) << '\n';
			std::cout << "vel mismatch:\t" << (v_fwd[0] - v_back[0]) << '\t' << (v_fwd[1] - v_back[1]) << '\t' << (v_fwd[2] - v_back[2]) << '\n';
			std::cout << "total DV:\t" << main_objfun(x) << '\n';
			std::cout << "estimated final mass:\t" << M * std::exp(-main_objfun(x) * V * 1000. / (9.80665 * Isp)) << '\n';
			//std::cout << "DV_penalty:\t" << state_mismatch(x) * V << '\n';
		}
	private:
		virtual double objfun_(const std::vector<double> &x) const {
			double r_fwd[3], v_fwd[3], r_back[3], v_back[3];
			state_mismatch(x,r_fwd,v_fwd,r_back,v_back);
			const double s_mismatch = std::sqrt((r_back[0] - r_fwd[0]) * (r_back[0] - r_fwd[0]) + (r_back[1] - r_fwd[1]) * (r_back[1] - r_fwd[1]) +
				(r_back[2] - r_fwd[2]) * (r_back[2] - r_fwd[2])) +
				10 * std::sqrt((v_back[0] - v_fwd[0]) * (v_back[0] - v_fwd[0]) + (v_back[1] - v_fwd[1]) * (v_back[1] - v_fwd[1]) +
				(v_back[2] - v_fwd[2]) * (v_back[2] - v_fwd[2]));
				
			//std::cout << s_mismatch << '\n';
			return main_objfun(x) + s_mismatch;
		}
		double main_objfun(const std::vector<double> &x) const {
			double retval = 0;
			double dt = (x.back() / n) * 86400 / T;
			for (int i = 0; i < n; ++i) {
				retval += x[3 * i + 4] * thrust / M * dt ;
			}
			return retval;
		}
		void state_mismatch(const std::vector<double> &x, double *r_fwd, double *v_fwd, double *r_back, double *v_back) const {
// std::cout << "max DV:\t" << thrust / M * (x.back() / n) * 86400 / T << '\n';
			const int n_seg_fwd = (n + 1) / 2, n_seg_back = n / 2;
// std::cout << "n segments:\t" << n_seg_fwd << ',' << n_seg_back << '\n';
			const double dt = (x.back() / n) * 86400 / T;
// std::cout << "dt_initial:\t" << dt << '\n';
			// We define two virtual spacecraft, one travelling forward from the first endpoint and a second one travelling
			// backward from the second endpoint.
			//double r_fwd[3], v_fwd[3], r_back[3], v_back[3];
			earth_eph(x[0],r_fwd,v_fwd);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
			mars_eph(x[0] + x.back(),r_back,v_back);
			// Forward propagation.
			// Launcher velocity increment
			kick(v_fwd,&x[1]);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
			// First propagation
			propagate(r_fwd,v_fwd,dt / 2.);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
			// Other propagations
			for (int i = 0; i < n_seg_fwd; ++i) {
				punch(v_fwd,&x[3 * i + 4],thrust / M,dt);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
				if (i == n_seg_fwd - 1) {
					propagate(r_fwd,v_fwd,dt / 2.);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
				} else {
					propagate(r_fwd,v_fwd,dt);
// std::cout << "rs_fwd:\t" << r_fwd[0] << ',' << r_fwd[1] << ',' << r_fwd[2] << '\n';
// std::cout << "vs_fwd:\t" << v_fwd[0] << ',' << v_fwd[1] << ',' << v_fwd[2] << '\n';
				}
			}
			// Backward propagation.
			// First propagation
			back_propagate(r_back,v_back,dt/2.);
// std::cout << "rs_back:\t" << r_back[0] << ',' << r_back[1] << ',' << r_back[2] << '\n';
// std::cout << "vs_back:\t" << v_back[0] << ',' << v_back[1] << ',' << v_back[2] << '\n';
			// Other propagations
			for (int i = 0; i < n_seg_back; ++i) {
				back_punch(v_back,&x.back() - 3 - 3 * i,thrust / M,dt);
// std::cout << "rs_back:\t" << r_back[0] << ',' << r_back[1] << ',' << r_back[2] << '\n';
// std::cout << "vs_back:\t" << v_back[0] << ',' << v_back[1] << ',' << v_back[2] << '\n';
				if (i == n_seg_back - 1) {
					back_propagate(r_back,v_back,dt / 2.);
// std::cout << "rs_back:\t" << r_back[0] << ',' << r_back[1] << ',' << r_back[2] << '\n';
// std::cout << "vs_back:\t" << v_back[0] << ',' << v_back[1] << ',' << v_back[2] << '\n';
				} else {
					back_propagate(r_back,v_back,dt);
// std::cout << "rs_back:\t" << r_back[0] << ',' << r_back[1] << ',' << r_back[2] << '\n';
// std::cout << "vs_back:\t" << v_back[0] << ',' << v_back[1] << ',' << v_back[2] << '\n';
				}
			}
// 			return std::sqrt((r_back[0] - r_fwd[0]) * (r_back[0] - r_fwd[0]) + (r_back[1] - r_fwd[1]) * (r_back[1] - r_fwd[1]) +
// 				(r_back[2] - r_fwd[2]) * (r_back[2] - r_fwd[2])) +
// 				std::sqrt((v_back[0] - v_fwd[0]) * (v_back[0] - v_fwd[0]) + (v_back[1] - v_fwd[1]) * (v_back[1] - v_fwd[1]) +
// 					(v_back[2] - v_fwd[2]) * (v_back[2] - v_fwd[2]));
		}
		static void ruv2cart(double *output, const double *input) {
			double theta, phi, r = input[0];
			theta = 2 * M_PI * input[1];
			phi = std::acos(2 * input[2] - 1);
			output[0] = r * std::cos(theta) * std::sin(phi);
			output[1] = r * std::sin(theta) * std::sin(phi);
			output[2] = r * std::cos(phi);
		}
		static void earth_eph(const double &mjd2000, double *position, double *velocity) {
			// Non-dimensional axis!
			const double keplerian[6] = {149597891.091458 / R , 0.0167090220204814, 0, 0, -257.059521691888, -2.47457303222881};
			// Epoch must be in mjd.
			const double epoch = 51544;
			Custom_Eph(mjd2000 + 2.451544500000000e+06, epoch, keplerian, position, velocity);
			position[0] /= R;
			position[1] /= R;
			position[2] /= R;
			velocity[0] /= V;
			velocity[1] /= V;
			velocity[2] /= V;
// std::cout << "Earth pos:\t" << position[0] << ',' << position[1] << ',' << position[2] << '\n';
// std::cout << "Earth vel:\t" << velocity[0] << ',' << velocity[1] << ',' << velocity[2] << '\n';
		}
		static void mars_eph(const double &mjd2000, double *position, double *velocity) {
			// Non-dimensional axis!
			const double keplerian[6] =
				{227940515.511383 / R, 0.0934047954172235, 1.84967094444443, 49.5574266111111, -73.4983588723774,19.3881291190116};
			// Epoch must be in mjd.
			const double epoch = 51544;
			Custom_Eph(mjd2000 + 2.451544500000000e+06, epoch, keplerian, position,velocity);
			position[0] /= R;
			position[1] /= R;
			position[2] /= R;
			velocity[0] /= V;
			velocity[1] /= V;
			velocity[2] /= V;
// std::cout << "Mars pos:\t" << position[0] << ',' << position[1] << ',' << position[2] << '\n';
// std::cout << "Mars vel:\t" << velocity[0] << ',' << velocity[1] << ',' << velocity[2] << '\n';
		}
		static void kick(double *output, const double *input) {
			double input_cart[3], input_copy[3];
			input_copy[0] = input[0] / V;
			input_copy[1] = input[1];
			input_copy[2] = input[2];
			ruv2cart(input_cart,input_copy);
			output[0] += input_cart[0];
			output[1] += input_cart[1];
			output[2] += input_cart[2];
		}
		static void punch(double *output, const double *input, const double &Tmax_M, const double &dt) {
			double input_cart[3], input_copy[3];
			input_copy[0] = input[0] * dt * Tmax_M;
			input_copy[1] = input[1];
			input_copy[2] = input[2];
			ruv2cart(input_cart,input_copy);
			output[0] += input_cart[0];
			output[1] += input_cart[1];
			output[2] += input_cart[2];
		}
		static void back_punch(double *output, const double *input, const double &Tmax_M, const double &dt) {
			double input_cart[3], input_copy[3];
			input_copy[0] = input[0] * dt * Tmax_M;
			input_copy[1] = input[1];
			input_copy[2] = input[2];
			ruv2cart(input_cart,input_copy);
			output[0] -= input_cart[0];
			output[1] -= input_cart[1];
			output[2] -= input_cart[2];
		}
		static void propagate(double *r, double *v, const double &t) {
// std::cout << "dt:\t" << t << '\n';
			double dummy_r[3], dummy_v[3];
			propagateKEP(r,v,t,1,dummy_r,dummy_v);
			r[0] = dummy_r[0];
			r[1] = dummy_r[1];
			r[2] = dummy_r[2];
			v[0] = dummy_v[0];
			v[1] = dummy_v[1];
			v[2] = dummy_v[2];
		}
		static void back_propagate(double *r, double *v, const double &t) {
// std::cout << "dt:\t" << t << '\n';
			double dummy_r[3], dummy_v[3], back_v[3];
			back_v[0] = -v[0];
			back_v[1] = -v[1];
			back_v[2] = -v[2];
			propagateKEP(r,back_v,t,1,dummy_r,dummy_v);
			r[0] = dummy_r[0];
			r[1] = dummy_r[1];
			r[2] = dummy_r[2];
			v[0] = -dummy_v[0];
			v[1] = -dummy_v[1];
			v[2] = -dummy_v[2];
		}
	private:
		int 	n;
		double 	M;
		double 	thrust;
		double	Isp;
};

#undef R
#undef V
#undef T
#undef A
#undef F

#endif
