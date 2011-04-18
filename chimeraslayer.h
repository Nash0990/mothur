#ifndef CHIMERASLAYER_H
#define CHIMERASLAYER_H

/*
 *  chimeraslayer.h
 *  Mothur
 *
 *  Created by westcott on 9/25/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */


#include "chimera.h"
#include "maligner.h"
#include "slayer.h"

//***********************************************************************/
//This class was modeled after the chimeraSlayer written by the Broad Institute
/***********************************************************************/

class ChimeraSlayer : public Chimera {
	
	public:
		ChimeraSlayer(string, string, bool, string, int, int, int, int, float, int, int, int, int, int, int, int, int, bool);
		ChimeraSlayer(string, string, bool, string, string, int, int, int, int, float, int, int, int, int, int, int, int, int, bool);

		~ChimeraSlayer();
		
		int getChimeras(Sequence*);
		Sequence* print(ostream&, ostream&);
		Sequence* print(ostream&, ostream&, data_results, data_results);
		void printHeader(ostream&);
		int doPrep();
		data_results getResults() { return printResults; }
		
		#ifdef USE_MPI
		Sequence* print(MPI_File&, MPI_File&);
		Sequence* print(MPI_File&, MPI_File&, data_results, data_results);
		#endif
		
	private:
		Sequence* querySeq;
		Sequence trimQuery;
		DeCalculator* decalc;
		map<int, int>  spotMap;
		Database* databaseRight;
		Database* databaseLeft;
		vector<Sequence*> userTemplate;  //when template=self, the query file is sorted from most abundance to least abundant
										 //userTemplate grows as the query file is processed by adding sequences that are not chimeric
		set<string> namesOfChimericSeqs; //only used when template=self
		
		vector<data_struct>  chimeraResults;
		data_results printResults;
		string chimeraFlags, searchMethod, fastafile;
		bool realign, trimChimera;
		int window, numWanted, kmerSize, match, misMatch, minSim, minCov, minBS, minSNP, parents, iters, increment;
		float divR;
	
		void printBlock(data_struct, string, ostream&);
		void printBlock(data_results, data_results, bool, bool, string, ostream&);
		string getBlock(data_struct, string);
		string getBlock(data_results, data_results, bool, bool, string);
		//int readNameFile(string);
		int getTemplate(Sequence*);
		
};

/************************************************************************/

#endif


