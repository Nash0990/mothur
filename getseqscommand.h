#ifndef GETSEQSCOMMAND_H
#define GETSEQSCOMMAND_H

/*
 *  getseqscommand.h
 *  Mothur
 *
 *  Created by Sarah Westcott on 7/8/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */
 
#include "command.hpp"

class GetSeqsCommand : public Command {
	
	public:
	
		GetSeqsCommand(string);
		~GetSeqsCommand(){};
		int execute();
		void help();	
		
	private:
		set<string> names;
		vector<string> outputNames;
		string accnosfile, fastafile, namefile, groupfile, alignfile, listfile, taxfile, outputDir;
		bool abort;
		
		int readFasta();
		int readName();
		int readGroup();
		int readAlign();
		int readAccnos();
		int readList();
		int readTax();
		
};

#endif

