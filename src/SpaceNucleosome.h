/*
 * SpaceNucleosome.h
 *
 *  Created on: Aug 5, 2016
 *      Author: belleau
 */

#ifndef SPACENUCLEOSOME_H_
#define SPACENUCLEOSOME_H_

#include <Rcpp.h>
#include <gsl/gsl_randist.h>
#include <unistd.h>

#include "Nucleosome.h"
#include "SegmentSeq.h"

namespace space_process {

template<typename NucleoClass>    /***** BEWARE NucleoClass Must inherit from Nucleosome *****/
class SpaceNucleosome {

	SegmentSeq const d_segSeq;

	std::list<NucleoClass> d_nucleosomes; /* List of nucleosomes */
	gsl_rng *d_rng;  // random number generator

public:
	SpaceNucleosome(std::vector<double> const  &fReads,
			std::vector<double> const &rReads, int zeta)
		:d_segSeq(fReads, rReads, zeta){
		setDefault();
	};

	SpaceNucleosome(std::vector<double> const  &fReads,
			std::vector<double> const &rReads, int zeta,
			long sizeFReads, long sizeRReads)
		:d_segSeq(fReads, rReads, zeta, sizeFReads, sizeRReads){
		setDefault();
	};
	virtual ~SpaceNucleosome(){};

	int size(){
		return(d_nucleosomes.size());
	};

	bool empty(){
		return(d_nucleosomes.empty());
	};

	void insert(NucleoClass &u){
		d_nucleosomes.push_back(u);
	};

	void setDeltaMin(int deltaMin){
		d_segSeq.setDeltaMin(deltaMin);
	};
	void setDeltaMax(int deltaMax){
		d_segSeq.setDeltaMax(deltaMax);
	};

	void setRng(gsl_rng *rng){
		d_rng = rng;
	};

	double minPos(){
		return(d_segSeq.minPos());
	};
	double maxPos(){
		return(d_segSeq.maxPos());
	}

protected:
	gsl_rng * rng(){
		return(d_rng);
	};
	SegmentSeq const &segSeq(){
		return(d_segSeq);
	}
private:

	void setDefault(){
		const gsl_rng_type * T;
		long seed;

		T = gsl_rng_default;

		d_rng = gsl_rng_alloc (T);     // pick random number generator
		seed = time (NULL) * getpid();
		gsl_rng_set (d_rng, seed);                  // set seed
	};

/*	long sizeFReads(){
		return(d_sizeFReads);
	};
	long sizeRReads(){
		return(d_sizeRReads);
	};
protected:
	std::vector<double> const  startFReads(){
		return(d_startFReads);
	};

	std::vector<double> const  startRReads(){
		return(d_startRReads);
	};*/

}; /* Class SpaceNucleosome */

} /* namespace space_process */

#endif /* SPACENUCLEOSOME_H_ */
