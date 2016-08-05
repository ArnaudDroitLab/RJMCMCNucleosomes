/*
 * NucleoDirichletPA.h
 *
 *  Created on: Jul 29, 2016
 *      Author: belleau
 */

#ifndef NUCLEODIRICHLETPA_H_
#define NUCLEODIRICHLETPA_H_

#include "Nucleosome.h"
#include "NucleoDirichlet.h"

namespace space_process {

class NucleoDirichletPA: public NucleoDirichlet {
	public:
		NucleoDirichletPA(double mu, int df, std::vector<double> const fReads, std::vector<double> const rReads, long  sizeFReads, long sizeRReads);
		virtual ~NucleoDirichletPA();
		double testT();
		void testFRStart();

};

} /* namespace space_process */

#endif /* NUCLEODIRICHLETPA_H_ */
