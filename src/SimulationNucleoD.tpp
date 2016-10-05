/*
 * SimulationNucleoD.tpp
 *
 *  Created on: Sep 27, 2016
 *      Author: belleau
 */

using namespace Rcpp;
using namespace std;

template< typename NucleoSpace>
SimulationNucleoD<NucleoSpace>::SimulationNucleoD(SegmentSeq const &segSeq,
		gsl_rng * rng, int kMax, long nbIteration):
	SimulationNucleo<NucleoSpace>(segSeq, rng, nbIteration),
	d_kMax(kMax), d_resStat(R_NilValue) {
	// TODO Auto-generated constructor stub

}

template< typename NucleoSpace>
SimulationNucleoD<NucleoSpace>::~SimulationNucleoD() {
	// TODO Auto-generated destructor stub
	//delete d_resStat;
}

template< typename NucleoSpace>
bool SimulationNucleoD<NucleoSpace>::initMu(int df){
	bool flag= true;
	NucleoSpace *tmp = new NucleoSpace(this->segSeq(), this->rng(), kMax());
	flag = (*tmp).initMu1(df);
	if(flag){
		(*tmp).prepSpace();
		(*tmp).addIteration();
		this->setCurrentState(tmp);
	}

	return(flag);
}

template< typename NucleoSpace>
int SimulationNucleoD<NucleoSpace>::kMax(){
	return(d_kMax);
}

template< typename NucleoSpace>
bool SimulationNucleoD<NucleoSpace>::sampler(){

	bool flag = false;
	double u = gsl_ran_flat (this->rng(), 0, 1);
	double rhoP1 = 1.0;
	//int typeMv = 0;

	this->currentClone();   // init mod

	if((*(this->currentState())).valK() > 1){

		if(u > (*(this->currentState())).dK()){
			if(u <= ((*(this->currentState())).dK() + (*(this->currentState())).bK()) ){

				flag = (*(this->mod())).birthR();

				if(flag){
					(*(this->mod())).prepSpace();
					rhoP1 = (*(this->mod())).rhoP2() / (*(this->currentState())).bK();
					rhoP1 *= (*(this->mod())).qalloc();
					//typeMv = 4;
				}
			}
			else{
				flag = (*(this->mod())).mhR();
				if(flag){
					(*(this->mod())).prepSpace();
					/* rhoP1 = 1.0; in the case mh */
					//typeMv = 5;
				}
			}
		}
		else{
			flag = (*(this->mod())).death();
			if(flag){
				(*(this->mod())).prepSpace();
				rhoP1 = (*(this->mod())).bK() / (*(this->currentState())).rhoP2();
				rhoP1 /= (*(this->mod())).qalloc();
				//typeMv = 3;
			}
		} /* End death */
	} /* End  case K > 1 */
	else{  /* case K == 1 */
		if(u <= 0.5){
			flag = (*(this->mod())).birthR();
			if(flag){
				(*(this->mod())).prepSpace();
				rhoP1 = (*(this->mod())).rhoP2() / (*(this->currentState())).bK(); // << (*mod).rhoP2()
				rhoP1 *= (*(this->mod())).qalloc();
				//typeMv = 1;
			}
		}
		else{
			flag = (*(this->mod())).mhR();
			if(flag){
				(*(this->mod())).prepSpace();
				/* rhoP1 = 1.0; in the case mh */
				//typeMv = 2;
			}
		}
	}
	this->setRhoP1(rhoP1);

	return(flag);
}

template< typename NucleoSpace>
double SimulationNucleoD<NucleoSpace>::computeRho(){

	double rho = this->rhoP1();
	rho *= exp(((*(this->mod())).kD() - (*(this->currentState())).kD()));
	rho *= ((*(this->mod())).priorMuDensity() / (*(this->currentState())).priorMuDensity());
	rho *= ((*(this->mod())).multinomial() / (*(this->currentState())).multinomial());
	rho = std::min(1.0, rho);
	return(rho);

}


template< typename NucleoSpace>
void SimulationNucleoD<NucleoSpace>::simulate(){
	for(long i = 0; i< this->nbIterations();i++){

		bool flag = false; // Generate a new valide (with enought read in the partition) move
		double rho = 1;
		double u = 0;

		flag = sampler();
		if(flag){
			rho = computeRho();

			u = gsl_ran_flat (this->rng(), 0, 1);


			if(rho > u){                   /* Accept mod */
				this->acceptMod();
				(*(this->currentState())).addIteration();

				this->pushState();

				this->setKMaxS( fmax( this->kMaxS(), (*(this->currentState())).valK()));
			}
			else{                          /* Reject mod */
				(*(this->currentState())).addIteration();
				(*(this->mod())).reject(); // delete nucleosome add in mod
				delete this->mod();
			}
		}
		else{               /* Enable to birth, mh or death in this iteration*/

			(*(this->currentState())).addIteration();
			delete this->mod();
		}
	}
}

/*template< typename NucleoSpace>
void SimulationNucleoD<NucleoSpace>::initResStat(){
	int nbState = this->sizeState();

	d_resStat = Rcpp::List( Rcpp::Named("k") = Rcpp::NumericVector( Rcpp::Dimension(nbState))
			, Rcpp::Named("k_max") = this->kMaxS()
			, Rcpp::Named("nbState") = this->sizeState()
			, Rcpp::Named("it") = Rcpp::IntegerVector( Rcpp::Dimension(nbState))
			, Rcpp::Named("mu") = Rcpp::NumericMatrix( Rcpp::Dimension(nbState, this->kMaxS()))
			, Rcpp::Named("muHat") = Rcpp::NumericVector(Rcpp::Dimension(this->kMaxS(), this->kMaxS()))
			, Rcpp::Named("nbK") = Rcpp::IntegerVector(this->kMaxS()));
}
*/

template< typename NucleoSpace>
Rcpp::List SimulationNucleoD<NucleoSpace>::simRapport(){
	return(d_resStat);

}
