/*
 *  parsefastaqcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 9/30/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "parsefastaqcommand.h"
#include "sequence.hpp"

//**********************************************************************************************************************
vector<string> ParseFastaQCommand::setParameters(){	
	try {
        CommandParameter pfile("file", "InputTypes", "", "", "fastqFile", "fastqFile", "none","",false,false,true); parameters.push_back(pfile);
		CommandParameter pfastq("fastq", "InputTypes", "", "", "fastqFile", "fastqFile", "none","",false,false,true); parameters.push_back(pfastq);
        CommandParameter poligos("oligos", "InputTypes", "", "", "oligosGroup", "none", "none","",false,false); parameters.push_back(poligos);
        CommandParameter pgroup("group", "InputTypes", "", "", "oligosGroup", "none", "none","",false,false); parameters.push_back(pgroup);
        CommandParameter preorient("checkorient", "Boolean", "", "F", "", "", "","",false,false,true); parameters.push_back(preorient);
        CommandParameter ppdiffs("pdiffs", "Number", "", "0", "", "", "","",false,false); parameters.push_back(ppdiffs);
		CommandParameter pbdiffs("bdiffs", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pbdiffs);
        CommandParameter pldiffs("ldiffs", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pldiffs);
		CommandParameter psdiffs("sdiffs", "Number", "", "0", "", "", "","",false,false); parameters.push_back(psdiffs);
        CommandParameter ptdiffs("tdiffs", "Number", "", "0", "", "", "","",false,false); parameters.push_back(ptdiffs);
		CommandParameter pfasta("fasta", "Boolean", "", "T", "", "", "","fasta",false,false); parameters.push_back(pfasta);
		CommandParameter pqual("qfile", "Boolean", "", "T", "", "", "","qfile",false,false); parameters.push_back(pqual);
        CommandParameter ppacbio("pacbio", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(ppacbio);
 		CommandParameter pformat("format", "Multiple", "sanger-illumina-solexa-illumina1.8+", "sanger", "", "", "","",false,false,true); parameters.push_back(pformat);
        CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string ParseFastaQCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The fastq.info command reads a fastq file and creates a fasta and quality file or can be used to parse fastq files by sample.\n";
		helpString += "The fastq.info command parameters are file, fastq, fasta, qfile, oligos, group and format; file or fastq is required.\n";
        helpString += "The fastq.info command should be in the following format: fastq.info(fastaq=yourFastaQFile).\n";
        helpString += "The oligos parameter allows you to provide an oligos file to split your fastq file into separate fastq files by barcode and primers. \n";
        helpString += "The group parameter allows you to provide a group file to split your fastq file into separate fastq files by group. \n";
        helpString += "The tdiffs parameter is used to specify the total number of differences allowed in the reads. The default is pdiffs + bdiffs + sdiffs + ldiffs.\n";
		helpString += "The bdiffs parameter is used to specify the number of differences allowed in the barcode. The default is 0.\n";
		helpString += "The pdiffs parameter is used to specify the number of differences allowed in the primer. The default is 0.\n";
        helpString += "The ldiffs parameter is used to specify the number of differences allowed in the linker. The default is 0.\n";
		helpString += "The sdiffs parameter is used to specify the number of differences allowed in the spacer. The default is 0.\n";
        helpString += "The checkorient parameter will check look for the reverse compliment of the barcode or primer in the sequence. If found the sequence is flipped. The default is false.\n";
		helpString += "The format parameter is used to indicate whether your sequences are sanger, solexa, illumina1.8+ or illumina, default=illumina1.8+.\n";
        helpString += "The fasta parameter allows you to indicate whether you want a fasta file generated. Default=T.\n";
        helpString += "The qfile parameter allows you to indicate whether you want a quality file generated. Default=T.\n";
        helpString += "The pacbio parameter allows you to indicate .... When set to true, quality scores of 0 will results in a corresponding base of N. Default=F.\n";
		helpString += "Example fastq.info(fastaq=test.fastaq).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string ParseFastaQCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "fasta") {  pattern = "[filename],fasta-[filename],[group],[tag],fasta-[filename],[group],fasta"; }
        else if (type == "qfile") {  pattern = "[filename],qual-[filename],[group],[tag],qual-[filename],[group],qual"; }
        else if (type == "fastq") {  pattern = "[filename],[group],fastq-[filename],[group],[tag],fastq"; } //make.sra assumes the [filename],[group],[tag],fastq format for the 4 column file option. If this changes, may have to modify fixMap function. 
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "ParseFastaQCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
ParseFastaQCommand::ParseFastaQCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["fasta"] = tempOutNames;
		outputTypes["qfile"] = tempOutNames;
        outputTypes["fastq"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "ParseFastaQCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
ParseFastaQCommand::ParseFastaQCommand(string option){
	try {
        abort = false; calledHelp = false; fileOption = 0; createFileGroup = false; hasIndex = false;
        split = 1;
		
		if(option == "help") {	help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string,string>::iterator it;

			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["fasta"] = tempOutNames;
			outputTypes["qfile"] = tempOutNames;
            outputTypes["fastq"] = tempOutNames;
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("fastq");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fastq"] = inputDir + it->second;		}
				}
                
                it = parameters.find("file");
				//user has given a template file
				if(it != parameters.end()){
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["file"] = inputDir + it->second;		}
				}
                
                it = parameters.find("oligos");
				//user has given a template file
				if(it != parameters.end()){
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["oligos"] = inputDir + it->second;		}
				}
                
                it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
			}
			
			//check for required parameters
			fastaQFile = validParameter.validFile(parameters, "fastq");
			if (fastaQFile == "not found") {	fastaQFile= "";	}
			else if (fastaQFile == "not open")	{	fastaQFile = ""; abort = true;	}
            else { inputfile = fastaQFile; }
            
            file = validParameter.validFile(parameters, "file");
			if (file == "not found") {	file = "";	}
			else if (file == "not open")	{	file = ""; abort = true;	}
            else { inputfile = file; fileOption = true; }
            
            if ((file == "") && (fastaQFile == "")) {  m->mothurOut("You must provide a file or fastq option."); m->mothurOutEndLine(); abort = true;  }

            
            oligosfile = validParameter.validFile(parameters, "oligos");
			if (oligosfile == "not found") {	oligosfile = "";	}
			else if (oligosfile == "not open")	{	oligosfile = ""; abort = true;	}
            else { current->setOligosFile(oligosfile); split = 2; }
            
            groupfile = validParameter.validFile(parameters, "group");
			if (groupfile == "not found") {	groupfile = "";	}
			else if (groupfile == "not open")	{	groupfile = ""; abort = true;	}
            else { current->setGroupFile(groupfile); split = 2; }
            
            if ((groupfile != "") && (oligosfile != "")) { m->mothurOut("You must enter ONLY ONE of the following: oligos or group."); m->mothurOutEndLine(); abort = true;  }
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");	if (outputDir == "not found"){	outputDir = util.hasPath(inputfile); 	}
			
			string temp;
			temp = validParameter.valid(parameters, "fasta");	if(temp == "not found"){	temp = "T";	}
			fasta = util.isTrue(temp); 

			temp = validParameter.valid(parameters, "qfile");	if(temp == "not found"){	temp = "T";	}
			qual = util.isTrue(temp);
            
            temp = validParameter.valid(parameters, "pacbio");	if(temp == "not found"){	temp = "F";	}
			pacbio = util.isTrue(temp);

            temp = validParameter.valid(parameters, "bdiffs");		if (temp == "not found") { temp = "0"; }
			util.mothurConvert(temp, bdiffs);
			
			temp = validParameter.valid(parameters, "pdiffs");		if (temp == "not found") { temp = "0"; }
			util.mothurConvert(temp, pdiffs);
            
            temp = validParameter.valid(parameters, "ldiffs");		if (temp == "not found") { temp = "0"; }
			util.mothurConvert(temp, ldiffs);
            
            temp = validParameter.valid(parameters, "sdiffs");		if (temp == "not found") { temp = "0"; }
			util.mothurConvert(temp, sdiffs);
			
			temp = validParameter.valid(parameters, "tdiffs");		if (temp == "not found") { int tempTotal = pdiffs + bdiffs + ldiffs + sdiffs;  temp = toString(tempTotal); }
			util.mothurConvert(temp, tdiffs);
			
			if(tdiffs == 0){	tdiffs = bdiffs + pdiffs + ldiffs + sdiffs;	}

			
            format = validParameter.valid(parameters, "format");		if (format == "not found"){	format = "illumina1.8+";	}
            
            if ((format != "sanger") && (format != "illumina") && (format != "illumina1.8+") && (format != "solexa"))  { 
				m->mothurOut(format + " is not a valid format. Your format choices are sanger, solexa, illumina1.8+ and illumina, aborting." ); m->mothurOutEndLine();
				abort=true;
			}

            if ((!fasta) && (!qual) && (file == "") && (fastaQFile == "") && (oligosfile == "")) { m->mothurOut("[ERROR]: no outputs selected. Aborting."); m->mothurOutEndLine(); abort=true; }
            temp = validParameter.valid(parameters, "checkorient");		if (temp == "not found") { temp = "F"; }
			reorient = util.isTrue(temp);

		}		
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "ParseFastaQCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int ParseFastaQCommand::execute(){
	try {
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
        vector< vector<string> > files;
        if (file != "") {
            //read file
            files = readFile();
        }
        
        if (m->getControl_pressed()) { return 0; }
        
        TrimOligos* trimOligos = NULL; TrimOligos* rtrimOligos = NULL;
        pairedOligos = false; numBarcodes = 0; numPrimers= 0; numLinkers= 0; numSpacers = 0; numRPrimers = 0;
        if (oligosfile != "")       {
            readOligos(oligosfile);
            //find group read belongs to
            if (pairedOligos)   {   trimOligos = new TrimOligos(pdiffs, bdiffs, 0, 0, oligos.getPairedPrimers(), oligos.getPairedBarcodes(), hasIndex); numBarcodes = oligos.getPairedBarcodes().size(); numPrimers = oligos.getPairedPrimers().size(); }
            else                {   trimOligos = new TrimOligos(pdiffs, bdiffs, ldiffs, sdiffs, oligos.getPrimers(), oligos.getBarcodes(), oligos.getReversePrimers(), oligos.getLinkers(), oligos.getSpacers());  numPrimers = oligos.getPrimers().size(); numBarcodes = oligos.getBarcodes().size();  }
            
            if (reorient) {
                rtrimOligos = new TrimOligos(pdiffs, bdiffs, 0, 0, oligos.getReorientedPairedPrimers(), oligos.getReorientedPairedBarcodes(), hasIndex); numBarcodes = oligos.getReorientedPairedBarcodes().size();
            }
            
        }else if (groupfile != "")   { readGroup(groupfile);     }
        
        if (file != "") {
            if (m->getControl_pressed()) { return 0; }
            
            for (int i = 0; i < files.size(); i++) { //process each pair
                
                if (m->getControl_pressed()) { break; }
                
                if ((fileOption == 2) || (fileOption == 4))  {  processFile(files[i], trimOligos, rtrimOligos);  }
                else if (fileOption == 3) {
                    if (current->getMothurCalling()) {
                        //add group names to fastq files and make copies
                        ofstream temp, temp2;
                        map<string, string> variables;
                        variables["[filename]"] = util.getRootName(files[i][0]);
                        variables["[group]"] = file2Group[i];
                        variables["[tag]"] = "forward";
                        string newffqFile = getOutputFileName("fastq", variables);
                        util.openOutputFile(newffqFile, temp);		temp.close();
                        util.appendFiles(files[i][0], newffqFile);
                        outputNames.push_back(newffqFile); outputTypes["fastq"].push_back(newffqFile);
                        
                        variables["[filename]"] = util.getRootName(files[i][1]);
                        variables["[group]"] = file2Group[i];
                        variables["[tag]"] = "reverse";
                        string newfrqFile = getOutputFileName("fastq", variables);
                        util.openOutputFile(newfrqFile, temp2);		temp2.close();
                        util.appendFiles(files[i][1], newfrqFile);
                        outputNames.push_back(newfrqFile); outputTypes["fastq"].push_back(newfrqFile);
                    }
                    
                    //if requested, make fasta and qual
                    if (fasta || qual) {  processFile(files[i], trimOligos, rtrimOligos); }  //split = 1, so no parseing by group will be done.
                }
            }
        }else {
            processFile(fastaQFile, trimOligos, rtrimOligos);
        }
		
        if (split > 1) {
            map<string, string>::iterator it;
            set<string> namesToRemove;
            for(int i=0;i<fastqFileNames.size();i++){
                for(int j=0;j<fastqFileNames[0].size();j++){
                    if (fastqFileNames[i][j] != "") {
                        if (namesToRemove.count(fastqFileNames[i][j]) == 0) {
                            if(util.isBlank(fastqFileNames[i][j])){
                                util.mothurRemove(fastqFileNames[i][j]);
                                namesToRemove.insert(fastqFileNames[i][j]);
                                
                                if (pairedOligos) { if (fileOption) { util.mothurRemove(rfastqFileNames[i][j]); namesToRemove.insert(rfastqFileNames[i][j]); } }
                                
                                if(fasta){
                                    util.mothurRemove(fastaFileNames[i][j]);
                                    namesToRemove.insert(fastaFileNames[i][j]);
                                    
                                    if (pairedOligos) { if (fileOption) { util.mothurRemove(rfastaFileNames[i][j]); namesToRemove.insert(rfastaFileNames[i][j]); } }
                                }
                                
                                if(qual){
                                    util.mothurRemove(qualFileNames[i][j]);
                                    namesToRemove.insert(qualFileNames[i][j]);
                                    
                                    if (pairedOligos) { if (fileOption) { util.mothurRemove(rqualFileNames[i][j]); namesToRemove.insert(rqualFileNames[i][j]); } }
                                }
                            }
                        }
                    }
                }
            }
            
            //remove names for outputFileNames, just cleans up the output
            outputTypes.clear();
            for(int i = 0; i < outputNames.size(); i++) {
                if (namesToRemove.count(outputNames[i]) != 0) {
                    outputNames.erase(outputNames.begin()+i);
                    i--;
                }else {
                    string ending = outputNames[i].substr(outputNames[i].length()-5);
                    if (ending == "fastq") { outputTypes["fastq"].push_back(outputNames[i]); }
                    else if (ending == "fasta") { outputTypes["fasta"].push_back(outputNames[i]); }
                    else if (ending == ".qfile") { outputTypes["qfile"].push_back(outputNames[i]); }
                }
            }
            
            //ffqnoMatchFile, rfqnoMatchFile, ffnoMatchFile, rfnoMatchFile, fqnoMatchFile, rqnoMatchFile
            if(util.isBlank(ffqnoMatchFile)){  util.mothurRemove(ffqnoMatchFile); }
            else { outputNames.push_back(ffqnoMatchFile); outputTypes["fastq"].push_back(ffqnoMatchFile); }
            
            if(fasta){
                if(util.isBlank(ffnoMatchFile)){  util.mothurRemove(ffnoMatchFile); }
                else { outputNames.push_back(ffnoMatchFile); outputTypes["fasta"].push_back(ffnoMatchFile); }
            }
            
            if(qual){
                if(util.isBlank(fqnoMatchFile)){  util.mothurRemove(fqnoMatchFile); }
                else { outputNames.push_back(fqnoMatchFile); outputTypes["qfile"].push_back(fqnoMatchFile); }
            }
            
            if (pairedOligos) {
                if (fileOption) {
                    if(util.isBlank(rfqnoMatchFile)){  util.mothurRemove(rfqnoMatchFile); }
                    else { outputNames.push_back(rfqnoMatchFile); outputTypes["fastq"].push_back(rfqnoMatchFile); }
                    
                    if(fasta){
                        if(util.isBlank(rfnoMatchFile)){  util.mothurRemove(rfnoMatchFile); }
                        else { outputNames.push_back(rfnoMatchFile); outputTypes["fasta"].push_back(rfnoMatchFile); }
                    }
                    
                    if(qual){
                        if(util.isBlank(rqnoMatchFile)){  util.mothurRemove(rqnoMatchFile); }
                        else { outputNames.push_back(rqnoMatchFile); outputTypes["qfile"].push_back(rqnoMatchFile); }
                    }
                }
            }
        }

        
        if (groupfile != "")        { delete groupMap;      }
        else if (oligosfile != "")  { delete trimOligos; if (reorient) { delete rtrimOligos; }   }
        
        
        if (m->getControl_pressed()) { for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); }  outputTypes.clear(); outputNames.clear();  return 0; }
		
		//set fasta file as new current fastafile
		string currentName = "";
		itTypes = outputTypes.find("fasta");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setFastaFile(currentName); }
		}
		
		itTypes = outputTypes.find("qfile");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setQualFile(currentName); }
		}		
		
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i] +"\n"); 	} m->mothurOutEndLine();

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
//assumes file option was used
int ParseFastaQCommand::processFile(vector<string> files, TrimOligos*& trimOligos, TrimOligos*& rtrimOligos){
    try {
        string inputfile = files[0]; string inputReverse = files[1];
        
        //open Output Files
        map<string, string> variables;
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
        string ffastaFile = getOutputFileName("fasta",variables);
        string fqualFile = getOutputFileName("qfile",variables);
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputReverse));
        string rfastaFile = getOutputFileName("fasta",variables);
        string rqualFile = getOutputFileName("qfile",variables);
        ofstream outfFasta, outfQual, outrFasta, outrQual;
        
        if (fasta) { util.openOutputFile(ffastaFile, outfFasta);  outputNames.push_back(ffastaFile); outputTypes["fasta"].push_back(ffastaFile);	util.openOutputFile(rfastaFile, outrFasta);  outputNames.push_back(rfastaFile); outputTypes["fasta"].push_back(rfastaFile);}
        if (qual) { util.openOutputFile(fqualFile, outfQual);	outputNames.push_back(fqualFile);  outputTypes["qfile"].push_back(fqualFile);	util.openOutputFile(rqualFile, outrQual);	outputNames.push_back(rqualFile);  outputTypes["qfile"].push_back(rqualFile);	}
        
        ifstream inf;
        util.openInputFile(inputfile, inf);
        
        ifstream inr;
        util.openInputFile(inputReverse, inr);
        
        ifstream inFIndex, inRIndex;
        if (files[2] != "") { util.openInputFile(files[2], inFIndex);  }
        if (files[3] != "") { util.openInputFile(files[3], inRIndex);  }
        
        int count = 0;
        while (!inf.eof() && !inr.eof()) {
            
            if (m->getControl_pressed()) { break; }
            
            bool ignoref, ignorer;
            FastqRead thisfRead(inf, ignoref, format);
            FastqRead thisrRead(inr, ignorer, format);
            
            if (!ignoref && ! ignorer) {
                if (qual) {
                    thisfRead.getQuality().printQScores(outfQual);
                    thisrRead.getQuality().printQScores(outrQual);
                }
                
                if (pacbio) { //change sequence bases with 0 quality scores to N
                    vector<int> fqual = thisfRead.getScores();
                    vector<int> rqual = thisrRead.getScores();
                    string fseq = thisfRead.getSeq();
                    string rseq = thisrRead.getSeq();
                    
                    for (int i = 0; i < fqual.size(); i++) { if (fqual[i] == 0){ fseq[i] = 'N'; } }
                    thisfRead.setSeq(fseq);
                
                    for (int i = 0; i < rqual.size(); i++) { if (rqual[i] == 0){ rseq[i] = 'N'; } }
                    thisrRead.setSeq(rseq);
                }
                FastqRead copyForward = thisfRead;
                FastqRead copyReverse = thisrRead;
                
                //print sequence info to files
                if (fasta) {
                    thisfRead.getSequence().printSequence(outfFasta);
                    thisrRead.getSequence().printSequence(outrFasta);
                }
                
                if (m->getControl_pressed()) { break; }
                if (split > 1) {
                    
                    Sequence findexBarcode("findex", "NONE");  Sequence rindexBarcode("rindex", "NONE");
                    if (fileOption == 4) {
                        bool ignorefi, ignoreri;
                    
                        if (files[2] != "") {
                            FastqRead thisfiRead(inFIndex, ignorefi, format);
                            if (!ignorefi) {  findexBarcode.setAligned(thisfiRead.getSequence().getAligned());  }
                        }
                        
                        if (files[3] != "") {
                            FastqRead thisriRead(inRIndex, ignoreri, format);
                            if (!ignoreri) {  rindexBarcode.setAligned(thisriRead.getSequence().getAligned());  }
                        }
                    }
                    
                    int barcodeIndex, primerIndex, trashCodeLength;
                    if (oligosfile != "") {
                        QualityScores tempF = thisfRead.getQuality();
                        QualityScores tempR = thisrRead.getQuality();
                        if ((files[2] != "") || (files[3] != "")) {
                            //barcode already removed so no need to reset sequence to trimmed version
                            trashCodeLength = findGroup(findexBarcode, tempF, rindexBarcode, tempR, barcodeIndex, primerIndex, trimOligos, rtrimOligos, numBarcodes, numPrimers);
                        }else {
                            Sequence tempSeqF = thisfRead.getSequence();
                            Sequence tempSeqR = thisrRead.getSequence();
                            trashCodeLength = findGroup(tempSeqF, tempF, tempSeqR, tempR, barcodeIndex, primerIndex, trimOligos, rtrimOligos, numBarcodes, numPrimers);
                            thisfRead.setSeq(tempSeqF.getUnaligned());
                            thisrRead.setSeq(tempSeqR.getUnaligned());
                        }
                        thisfRead.setScores(tempF.getScores()); //set to trimmed scores
                        thisrRead.setScores(tempR.getScores());
                    }else if (groupfile != "")  {  trashCodeLength = findGroup(thisfRead.getSequence(), barcodeIndex, primerIndex, "groupMode");   }
                    else {  m->mothurOut("[ERROR]: uh oh, we shouldn't be here...\n"); }
                    
                    if(trashCodeLength == 0){
                        ofstream out;
                        util.openOutputFileAppend(fastqFileNames[barcodeIndex][primerIndex], out);
                        copyForward.printFastq(out);
                        out.close();
                        
                        ofstream out2;
                        util.openOutputFileAppend(rfastqFileNames[barcodeIndex][primerIndex], out2);
                        copyReverse.printFastq(out2);
                        out2.close();
                        
                        //print no match fasta, if wanted
                        if (fasta) {
                            ofstream outf, outr;
                            util.openOutputFileAppend(fastaFileNames[barcodeIndex][primerIndex], outf);
                            thisfRead.getSequence().printSequence(outf);
                            outf.close();
                            
                            util.openOutputFileAppend(rfastaFileNames[barcodeIndex][primerIndex], outr);
                            thisrRead.getSequence().printSequence(outr);
                            outr.close();
                        }
                        
                        //print no match quality parse, if wanted
                        if (qual) {
                            ofstream outq, outq2;
                            util.openOutputFileAppend(qualFileNames[barcodeIndex][primerIndex], outq);
                            thisfRead.getQuality().printQScores(outq);
                            outq.close();
                            
                            util.openOutputFileAppend(rqualFileNames[barcodeIndex][primerIndex], outq2);
                            thisrRead.getQuality().printQScores(outq2);
                            outq2.close();
                        }

                    }else{
                        //print no match fastq
                        ofstream out, out2;
                        util.openOutputFileAppend(ffqnoMatchFile, out);
                        copyForward.printFastq(out);
                        out.close();
                        
                        util.openOutputFileAppend(rfqnoMatchFile, out2);
                        copyReverse.printFastq(out2);
                        out2.close();
                        
                        //print no match fasta, if wanted
                        if (fasta) {
                            ofstream outf, outr;
                            util.openOutputFileAppend(ffnoMatchFile, outf);
                            thisfRead.getSequence().printSequence(outf);
                            outf.close();
                            
                            util.openOutputFileAppend(rfnoMatchFile, outr);
                            thisrRead.getSequence().printSequence(outr);
                            outr.close();
                        }
                        
                        //print no match quality parse, if wanted
                        if (qual) {
                            ofstream outq, outq2;
                            util.openOutputFileAppend(fqnoMatchFile, outq);
                            thisfRead.getQuality().printQScores(outq);
                            outq.close();
                            
                            util.openOutputFileAppend(rqnoMatchFile, outq2);
                            thisrRead.getQuality().printQScores(outq2);
                            outq2.close();
                        }
                    }
                }
                //report progress
                if((count+1) % 10000 == 0){	m->mothurOut(toString(count+1)); m->mothurOutEndLine();		}
                count++;
            }
        }
        
        inf.close(); inr.close();
        if (files[2] != "") { inFIndex.close();  }
        if (files[3] != "") { inRIndex.close();  }
        
        if (fasta)	{ outfFasta.close(); outrFasta.close();	}
        if (qual)	{ outfQual.close();	outrQual.close();   }
        
        //report progress
        if (!m->getControl_pressed()) {   if((count) % 10000 != 0){	m->mothurOut(toString(count)); m->mothurOutEndLine();		}  }
        
        
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "processFile");
		exit(1);
	}
}
//**********************************************************************************************************************
int ParseFastaQCommand::processFile(string inputfile, TrimOligos*& trimOligos, TrimOligos*& rtrimOligos){
    try {
        
        //open Output Files
        map<string, string> variables;
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
        string fastaFile = getOutputFileName("fasta",variables);
        string qualFile = getOutputFileName("qfile",variables);
        ofstream outFasta, outQual;
        
        //fasta and quality files for whole input file
        if (fasta) { util.openOutputFile(fastaFile, outFasta);  outputNames.push_back(fastaFile); outputTypes["fasta"].push_back(fastaFile);	}
        if (qual) { util.openOutputFile(qualFile, outQual);	outputNames.push_back(qualFile);  outputTypes["qfile"].push_back(qualFile);		}
        
        ifstream in;
        util.openInputFile(inputfile, in);
        
        //fill convert table - goes from solexa to sanger. Used fq_all2std.pl as a reference.
        for (int i = -64; i < 65; i++) {
            char temp = (char) ((int)(33 + 10*log(1+pow(10,(i/10.0)))/log(10)+0.499));
            convertTable.push_back(temp);
        }
        
        
        int count = 0;
        while (!in.eof()) {
            
            if (m->getControl_pressed()) { break; }
            
            bool ignore;
            FastqRead thisRead(in, ignore, format);
            
            if (!ignore) {
                if (qual) {  thisRead.getQuality().printQScores(outQual); }
                
                if (pacbio) {
                    vector<int> qual = thisRead.getScores();
                    string seq = thisRead.getSeq();
                    
                    for (int i = 0; i < qual.size(); i++) { if (qual[i] == 0){ seq[i] = 'N'; } }
                    thisRead.setSeq(seq);
                }
                
                FastqRead copy = thisRead;
                
                //print sequence info to files
                if (fasta) { thisRead.getSequence().printSequence(outFasta); }
                
                if (m->getControl_pressed()) { break; }
                
                if (split > 1) {
                    int barcodeIndex, primerIndex, trashCodeLength;
                    if (oligosfile != "")      {
                        Sequence tempSeq = thisRead.getSequence();
                        QualityScores tempQual = thisRead.getQuality();
                        trashCodeLength = findGroup(tempSeq, tempQual, barcodeIndex, primerIndex, trimOligos, rtrimOligos, numBarcodes, numPrimers);
                        thisRead.setSeq(tempSeq.getUnaligned());
                        thisRead.setScores(tempQual.getScores());
                    }
                    else if (groupfile != "")  {  trashCodeLength = findGroup(thisRead.getSequence(), barcodeIndex, primerIndex, "groupMode");   }
                    else {  m->mothurOut("[ERROR]: uh oh, we shouldn't be here...\n"); }
                    
                    if(trashCodeLength == 0){ //files in here are per group
                        //print fastq to barcode and primer match
                        ofstream out;
                        util.openOutputFileAppend(fastqFileNames[barcodeIndex][primerIndex], out);
                        copy.printFastq(out);
                        out.close();
                        
                        //print fasta match if wanted
                        if (fasta) {
                            ofstream outf;
                            util.openOutputFileAppend(fastaFileNames[barcodeIndex][primerIndex], outf);
                            thisRead.getSequence().printSequence(outf);
                            outf.close();
                        }
                        
                        //print qual match, if wanted
                        if (qual) {
                            ofstream outq;
                            util.openOutputFileAppend(qualFileNames[barcodeIndex][primerIndex], outq);
                            thisRead.getQuality().printQScores(outq);
                            outq.close();
                        }
                    }else{
                        //print no match fastq
                        ofstream out;
                        util.openOutputFileAppend(ffqnoMatchFile, out);
                        copy.printFastq(out);
                        out.close();
                        
                        //print no match fasta, if wanted
                        if (fasta) {
                            ofstream outf;
                            util.openOutputFileAppend(ffnoMatchFile, outf);
                            thisRead.getSequence().printSequence(outf);
                            outf.close();
                        }
                        
                        //print no match quality parse, if wanted
                        if (qual) {
                            ofstream outq;
                            util.openOutputFileAppend(fqnoMatchFile, outq);
                            thisRead.getQuality().printQScores(outq);
                            outq.close();
                        }
                    }
                }
                //report progress
                if((count+1) % 10000 == 0){	m->mothurOut(toString(count+1)); m->mothurOutEndLine();		}
                count++;
            }
        }
        
        in.close();
        if (fasta)	{ outFasta.close();	}
        if (qual)	{ outQual.close();	}
        
        //report progress
        if (!m->getControl_pressed()){   if((count) % 10000 != 0){	m->mothurOut(toString(count)); m->mothurOutEndLine();		}  }
        
        
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "processFile");
		exit(1);
	}
}
//**********************************************************************************************************************
int ParseFastaQCommand::findGroup(Sequence& currSeq, QualityScores& currQual, int& barcode, int& primer, TrimOligos*& trimOligos, TrimOligos*& rtrimOligos, int numBarcodes, int numPrimers) {
	try {
        int success = 1;
        string trashCode = "";
        int currentSeqsDiffs = 0;
        
        //Sequence currSeq(thisRead.seq.getName(), thisRead.seq.getAligned());
        //QualityScores currQual; currQual.setScores(convertQual(thisRead.quality));
        
        //for reorient
        Sequence savedSeq(currSeq.getName(), currSeq.getAligned());
        QualityScores savedQual(currQual.getName(), currQual.getScores());
        
        if(numLinkers != 0){
            success = trimOligos->stripLinker(currSeq, currQual);
            if(success > ldiffs)		{	trashCode += 'k';	}
            else{ currentSeqsDiffs += success;  }
            
        }
        
        if(numBarcodes != 0){
            vector<int> results = trimOligos->stripBarcode(currSeq, currQual, barcode);
            if (pairedOligos)   {  success = results[0] + results[2];   }
            else                { success = results[0];                 }
            if(success > bdiffs)		{	trashCode += 'b';	}
            else{ currentSeqsDiffs += success;  }
        }
        
        if(numSpacers != 0){
            success = trimOligos->stripSpacer(currSeq, currQual);
            if(success > sdiffs)		{	trashCode += 's';	}
            else{ currentSeqsDiffs += success;  }
            
        }
        
        if(numPrimers != 0){
            vector<int> results = trimOligos->stripForward(currSeq, currQual, primer, true);
            if (pairedOligos)   {  success = results[0] + results[2];   }
            else                {  success = results[0];                }
            if(success > pdiffs)		{	trashCode += 'f';	}
            else{ currentSeqsDiffs += success;  }
        }
        
        if(numRPrimers != 0){
            vector<int> results = trimOligos->stripReverse(currSeq, currQual);
            success = results[0];
            if(success > pdiffs)		{	trashCode += 'r';	}
            else{ currentSeqsDiffs += success;  }
        }
        
        if (currentSeqsDiffs > tdiffs)	{	trashCode += 't';   }
        
        if (reorient && (trashCode != "")) { //if you failed and want to check the reverse
            int thisSuccess = 0;
            string thisTrashCode = "";
            int thisCurrentSeqsDiffs = 0;
            
            int thisBarcodeIndex = 0;
            int thisPrimerIndex = 0;
            //cout << currSeq.getName() << '\t' << savedSeq.getUnaligned() << endl;
            if(numBarcodes != 0){
                vector<int> results = rtrimOligos->stripBarcode(savedSeq, savedQual, thisBarcodeIndex);
                if (pairedOligos)   {  thisSuccess = results[0] + results[2];   }
                else                {  thisSuccess = results[0];                }
                if(thisSuccess > bdiffs)		{ thisTrashCode += "b"; }
                else{ thisCurrentSeqsDiffs += thisSuccess;  }
            }
            //cout << currSeq.getName() << '\t' << savedSeq.getUnaligned() << endl;
            if(numPrimers != 0){
                vector<int> results = rtrimOligos->stripForward(savedSeq, savedQual, thisPrimerIndex, true);
                if (pairedOligos)   {  thisSuccess = results[0] + results[2];   }
                else                {  thisSuccess = results[0];                }
                if(thisSuccess > pdiffs)		{ thisTrashCode += "f"; }
                else{ thisCurrentSeqsDiffs += thisSuccess;  }
            }
            
            if (thisCurrentSeqsDiffs > tdiffs)	{	thisTrashCode += 't';   }
            
            if (thisTrashCode == "") {
                trashCode = thisTrashCode;
                success = thisSuccess;
                currentSeqsDiffs = thisCurrentSeqsDiffs;
                barcode = thisBarcodeIndex;
                primer = thisPrimerIndex;
                //savedSeq.reverseComplement();
                //currSeq.setAligned(savedSeq.getAligned());
                //savedQual.flipQScores();
                //currQual.setScores(savedQual.getScores());
            }else { trashCode += "(" + thisTrashCode + ")";  }
        }
        
        if (trashCode.length() == 0) { //is this sequence in the ignore group
            string thisGroup = oligos.getGroupName(barcode, primer);
            
            int pos = thisGroup.find("ignore");
            if (pos != string::npos) {  trashCode += "i"; }
        }

        
        return trashCode.length();
    }
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "findGroup");
		exit(1);
	}
}
//**********************************************************************************************************************
int ParseFastaQCommand::findGroup(Sequence seq, int& barcode, int& primer, string groupMode) {
	try {
        string trashCode = "";
        primer = 0;
        
        string group = groupMap->getGroup(seq.getName());
        if (group == "not found") {     trashCode += "g";   } //scrap for group
        else {  barcode = GroupToFile[group]; }
    
        return trashCode.length();
    }
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "findGroup");
		exit(1);
	}
}
//**********************************************************************************************************************
int ParseFastaQCommand::findGroup(Sequence& fcurrSeq, QualityScores& fcurrQual, Sequence& rcurrSeq, QualityScores& rcurrQual, int& barcode, int& primer, TrimOligos*& trimOligos, TrimOligos*& rtrimOligos, int numBarcodes, int numPrimers) {
	try {
        int success = 1;
        string trashCode = "";
        int currentSeqsDiffs = 0;
        
        //Sequence fcurrSeq(thisfRead.seq.getName(), thisfRead.seq.getAligned());
        //QualityScores fcurrQual; fcurrQual.setScores(convertQual(thisfRead.quality));
        //Sequence rcurrSeq(thisrRead.seq.getName(), thisrRead.seq.getAligned());
        //QualityScores rcurrQual; rcurrQual.setScores(convertQual(thisrRead.quality));
        
        //for reorient
        Sequence fsavedSeq(fcurrSeq.getName(), fcurrSeq.getAligned());
        QualityScores fsavedQual(fcurrQual.getName(), fcurrQual.getScores());
        Sequence rsavedSeq(rcurrSeq.getName(), rcurrSeq.getAligned());
        QualityScores rsavedQual(rcurrQual.getName(), rcurrQual.getScores());
        
        if(numBarcodes != 0){
            vector<int> results = trimOligos->stripBarcode(fcurrSeq, rcurrSeq, fcurrQual, rcurrQual, barcode);
            if (pairedOligos)   {  success = results[0] + results[2];   }
            else                { success = results[0];                 }
            if(success > bdiffs)		{	trashCode += 'b';	}
            else{ currentSeqsDiffs += success;  }
        }
        
        if(numPrimers != 0){
            vector<int> results = trimOligos->stripForward(fcurrSeq, rcurrSeq, fcurrQual, rcurrQual, primer);
            if (pairedOligos)   {  success = results[0] + results[2];   }
            else                { success = results[0];                 }
            if(success > pdiffs)		{	trashCode += 'f';	}
            else{ currentSeqsDiffs += success;  }
        }
        
        if (currentSeqsDiffs > tdiffs)	{	trashCode += 't';   }
        
        if (reorient && (trashCode != "")) { //if you failed and want to check the reverse
            int thisSuccess = 0;
            string thisTrashCode = "";
            int thisCurrentSeqsDiffs = 0;
            
            int thisBarcodeIndex = 0;
            int thisPrimerIndex = 0;
            
            if(numBarcodes != 0){
                
                vector<int> results = rtrimOligos->stripBarcode(fsavedSeq, rsavedSeq, fsavedQual, rsavedQual, thisBarcodeIndex);
                if (pairedOligos)   {  thisSuccess = results[0] + results[2];   }
                else                {  thisSuccess = results[0];                 }
                
                if(thisSuccess > bdiffs)		{	thisTrashCode += 'b';	}
                else{ thisCurrentSeqsDiffs += thisSuccess;  }
            }
            
            if(numPrimers != 0){
                
                vector<int> results = rtrimOligos->stripForward(fsavedSeq, rsavedSeq, fsavedQual, rsavedQual, thisPrimerIndex);
                if (pairedOligos)   {  thisSuccess = results[0] + results[2];   }
                else                {  thisSuccess = results[0];                 }
                
                if(thisSuccess > pdiffs)		{	thisTrashCode += 'f';	}
                else{ thisCurrentSeqsDiffs += thisSuccess;  }
            }
            
            if (thisCurrentSeqsDiffs > tdiffs)	{	thisTrashCode += 't';   }
            
            if (thisTrashCode == "") {
                trashCode = thisTrashCode;
                success = thisSuccess;
                currentSeqsDiffs = thisCurrentSeqsDiffs;
                barcode = thisBarcodeIndex;
                primer = thisPrimerIndex;
                //fsavedSeq.reverseComplement();
                //rsavedSeq.reverseComplement();
                //fcurrSeq.setAligned(fsavedSeq.getAligned());
                //rcurrSeq.setAligned(rsavedSeq.getAligned());
                //fsavedQual.flipQScores(); rsavedQual.flipQScores();
               // fcurrQual.setScores(fsavedQual.getScores()); rcurrQual.setScores(rsavedQual.getScores());
                
            }else { trashCode += "(" + thisTrashCode + ")";  }
        }
        
        if (trashCode.length() == 0) { //is this sequence in the ignore group
            string thisGroup = oligos.getGroupName(barcode, primer);
            
            int pos = thisGroup.find("ignore");
            if (pos != string::npos) {  trashCode += "i"; }
        }
        
        return trashCode.length();
    }
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "findGroup");
		exit(1);
	}
}
//**********************************************************************************************************************
/*
 
 file option 1
 
 ffastqfile1 rfastqfile1
 ffastqfile2 rfastqfile2
 ...
 
 file option 2
 
 group ffastqfile  rfastqfile
 group ffastqfile  rfastqfile
 group ffastqfile  rfastqfile
 ...
 
 file option 3 
 
 My.forward.fastq My.reverse.fastq none My.rindex.fastq //none is an option is no forward or reverse index file
 
 */

//lines can be 2, 3, or 4 columns
// forward.fastq reverse.fastq -> 2 column
// groupName forward.fastq reverse.fastq -> 3 column
// forward.fastq reverse.fastq forward.index.fastq  reverse.index.fastq  -> 4 column
// forward.fastq reverse.fastq none  reverse.index.fastq  -> 4 column
// forward.fastq reverse.fastq forward.index.fastq  none  -> 4 column
vector< vector<string> > ParseFastaQCommand::readFile(){
	try {
        vector< vector<string> > files;
        string forward, reverse, findex, rindex;
        
        ifstream in;
        util.openInputFile(inputfile, in);
        
        while(!in.eof()) {
            
            if (m->getControl_pressed()) { return files; }
            
            string line = util.getline(in);  util.gobble(in);
            vector<string> pieces = util.splitWhiteSpace(line);
            
            string group = "";
            if (pieces.size() == 2) {
                forward = pieces[0];
                reverse = pieces[1];
                group = "";
                findex = "";
                rindex = "";
                fileOption = 2;
            }else if (pieces.size() == 3) {
                if (oligosfile != "") { m->mothurOut("[ERROR]: You cannot have an oligosfile and 3 column file option at the same time. Aborting. \n"); m->setControl_pressed(true); }
                if (groupfile != "") { m->mothurOut("[ERROR]: You cannot have an groupfile and 3 column file option at the same time. Aborting. \n"); m->setControl_pressed(true); }
                group = pieces[0];
                util.checkGroupName(group);
                forward = pieces[1];
                reverse = pieces[2];
                findex = "";
                rindex = "";
                createFileGroup = true;
                fileOption = 3;
            }else if (pieces.size() == 4) {
                if (oligosfile == "") { m->mothurOut("[ERROR]: You must have an oligosfile with the index file option. Aborting. \n"); m->setControl_pressed(true); }
                forward = pieces[0];
                reverse = pieces[1];
                findex = pieces[2];
                rindex = pieces[3];
                fileOption = 4;
                hasIndex = true;
                if ((findex == "none") || (findex == "NONE")){ findex = ""; }
                if ((rindex == "none") || (rindex == "NONE")){ rindex = ""; }
            }else {
                m->mothurOut("[ERROR]: file lines can be 2, 3, or 4 columns. The forward fastq files in the first column and their matching reverse fastq files in the second column, or a groupName then forward fastq file and reverse fastq file, or forward fastq file then reverse fastq then forward index and reverse index file.  If you only have one index file add 'none' for the other one. \n"); m->setControl_pressed(true);
            }
            
            if (m->getDebug()) { m->mothurOut("[DEBUG]: group = " + group + ", forward = " + forward + ", reverse = " + reverse + ", forwardIndex = " + findex + ", reverseIndex = " + rindex + ".\n"); }
            
            if (inputDir != "") {
                string path = util.hasPath(forward);
                if (path == "") {  forward = inputDir + forward;  }
                
                path = util.hasPath(reverse);
                if (path == "") {  reverse = inputDir + reverse;  }
                
                if (findex != "") {
                    path = util.hasPath(findex);
                    if (path == "") {  findex = inputDir + findex;  }
                }
                
                if (rindex != "") {
                    path = util.hasPath(rindex);
                    if (path == "") {  rindex = inputDir + rindex;  }
                }
            }
            
            //check to make sure both are able to be opened
            ifstream in2;
            bool openForward = util.openInputFile(forward, in2, "noerror");
            
            //if you can't open it, try default location
            if (!openForward) {
                if (current->getDefaultPath() != "") { //default path is set
                    string tryPath = current->getDefaultPath() + util.getSimpleName(forward);
                    m->mothurOut("Unable to open " + forward + ". Trying default " + tryPath); m->mothurOutEndLine();
                    ifstream in3;
                    openForward = util.openInputFile(tryPath, in3, "noerror");
                    in3.close();
                    forward = tryPath;
                }
            }
            
            //if you can't open it, try output location
            if (!openForward) {
                if (current->getOutputDir() != "") { //default path is set
                    string tryPath = current->getOutputDir() + util.getSimpleName(forward);
                    m->mothurOut("Unable to open " + forward + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                    ifstream in4;
                    openForward = util.openInputFile(tryPath, in4, "noerror");
                    forward = tryPath;
                    in4.close();
                }
            }
            
            if (!openForward) { //can't find it
                m->mothurOut("[WARNING]: can't find " + forward + ", ignoring pair.\n");
            }else{  in2.close();  }
            
            ifstream in3;
            bool openReverse = util.openInputFile(reverse, in3, "noerror");
            
            //if you can't open it, try default location
            if (!openReverse) {
                if (current->getDefaultPath() != "") { //default path is set
                    string tryPath = current->getDefaultPath() + util.getSimpleName(reverse);
                    m->mothurOut("Unable to open " + reverse + ". Trying default " + tryPath); m->mothurOutEndLine();
                    ifstream in3;
                    openReverse = util.openInputFile(tryPath, in3, "noerror");
                    in3.close();
                    reverse = tryPath;
                }
            }
            
            //if you can't open it, try output location
            if (!openReverse) {
                if (current->getOutputDir() != "") { //default path is set
                    string tryPath = current->getOutputDir() + util.getSimpleName(reverse);
                    m->mothurOut("Unable to open " + reverse + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                    ifstream in4;
                    openReverse = util.openInputFile(tryPath, in4, "noerror");
                    reverse = tryPath;
                    in4.close();
                }
            }
            
            if (!openReverse) { //can't find it
                m->mothurOut("[WARNING]: can't find " + reverse + ", ignoring pair.\n");
            }else{  in3.close();  }
            
            bool openFindex = true;
            if (findex != "") {
                ifstream in4;
                openFindex = util.openInputFile(findex, in4, "noerror"); in4.close();
                
                //if you can't open it, try default location
                if (!openFindex) {
                    if (current->getDefaultPath() != "") { //default path is set
                        string tryPath = current->getDefaultPath() + util.getSimpleName(findex);
                        m->mothurOut("Unable to open " + findex + ". Trying default " + tryPath); m->mothurOutEndLine();
                        ifstream in5;
                        openFindex = util.openInputFile(tryPath, in5, "noerror");
                        in5.close();
                        findex = tryPath;
                    }
                }
                
                //if you can't open it, try output location
                if (!openFindex) {
                    if (current->getOutputDir() != "") { //default path is set
                        string tryPath = current->getOutputDir() + util.getSimpleName(findex);
                        m->mothurOut("Unable to open " + findex + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                        ifstream in6;
                        openFindex = util.openInputFile(tryPath, in6, "noerror");
                        findex = tryPath;
                        in6.close();
                    }
                }
                
                if (!openFindex) { //can't find it
                    m->mothurOut("[WARNING]: can't find " + findex + ", ignoring pair.\n");
                }
            }
            
            bool openRindex = true;
            if (rindex != "") {
                ifstream in7;
                openRindex = util.openInputFile(rindex, in7, "noerror"); in7.close();
                
                //if you can't open it, try default location
                if (!openRindex) {
                    if (current->getDefaultPath() != "") { //default path is set
                        string tryPath = current->getDefaultPath() + util.getSimpleName(rindex);
                        m->mothurOut("Unable to open " + rindex + ". Trying default " + tryPath); m->mothurOutEndLine();
                        ifstream in8;
                        openRindex = util.openInputFile(tryPath, in8, "noerror");
                        in8.close();
                        rindex = tryPath;
                    }
                }
                
                //if you can't open it, try output location
                if (!openRindex) {
                    if (current->getOutputDir() != "") { //default path is set
                        string tryPath = current->getOutputDir() + util.getSimpleName(rindex);
                        m->mothurOut("Unable to open " + rindex + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                        ifstream in9;
                        openRindex = util.openInputFile(tryPath, in9, "noerror");
                        rindex = tryPath;
                        in9.close();
                    }
                }
                
                if (!openRindex) { //can't find it
                    m->mothurOut("[WARNING]: can't find " + rindex + ", ignoring pair.\n");
                }
            }
            
            
            if ((openForward) && (openReverse) && (openFindex) && (openRindex)) { //good pair
                file2Group[files.size()] = group;
                vector<string> pair;
                pair.push_back(forward);
                pair.push_back(reverse);
                pair.push_back(findex);
                pair.push_back(rindex);
                if (((findex != "") || (rindex != "")) && (oligosfile == "")) { m->mothurOut("[ERROR]: You need to provide an oligos file if you are going to use an index file.\n"); m->setControl_pressed(true);  }
                files.push_back(pair);
            }
        }
        in.close();
        
        return files;
    }
    catch(exception& e) {
        m->errorOut(e, "ParseFastaQCommand", "readFileNames");
        exit(1);
    }
}
//***************************************************************************************************************

bool ParseFastaQCommand::readOligos(string oligoFile){
	try {
        bool allBlank = false;
        
        if (fileOption) { oligos.read(oligosfile, false);  } // like make.contigs
        else {  oligos.read(oligosfile);  }
        
        if (m->getControl_pressed()) { return false; } //error in reading oligos
        
        if (oligos.hasPairedPrimers() || oligos.hasPairedBarcodes()) {
            pairedOligos = true;
            numPrimers = oligos.getPairedPrimers().size();
            numBarcodes = oligos.getPairedBarcodes().size();
        }else {
            pairedOligos = false;
            numPrimers = oligos.getPrimers().size();
            numBarcodes = oligos.getBarcodes().size();
        }
        
        numLinkers = oligos.getLinkers().size();
        numSpacers = oligos.getSpacers().size();
        numRPrimers = oligos.getReversePrimers().size();
        
        vector<string> groupNames = oligos.getGroupNames();
        if (groupNames.size() == 0) { allBlank = true;  }
        
        if (m->getControl_pressed()) { return false; }
        
        fastqFileNames.resize(oligos.getBarcodeNames().size());
		for(int i=0;i<fastqFileNames.size();i++){
            for(int j=0;j<oligos.getPrimerNames().size();j++){  fastqFileNames[i].push_back(""); }
		}
        if (pairedOligos) { rfastqFileNames = fastqFileNames; }
        
        if(qual)	{	qualFileNames = fastqFileNames;	 if (pairedOligos) { rqualFileNames = fastqFileNames; }    }
        if(fasta)	{	fastaFileNames = fastqFileNames; if (pairedOligos) { rfastaFileNames = fastqFileNames; }	}
        
        set<string> uniqueNames; //used to cleanup outputFileNames
        if (pairedOligos) {
            map<int, oligosPair> barcodes = oligos.getPairedBarcodes();
            map<int, oligosPair> primers = oligos.getPairedPrimers();
            for(map<int, oligosPair>::iterator itBar = barcodes.begin();itBar != barcodes.end();itBar++){
                for(map<int, oligosPair>::iterator itPrimer = primers.begin();itPrimer != primers.end(); itPrimer++){
                    
                    string primerName = oligos.getPrimerName(itPrimer->first);
                    string barcodeName = oligos.getBarcodeName(itBar->first);
                    
                    if ((primerName == "ignore") || (barcodeName == "ignore")) { } //do nothing
                    else if ((primerName == "") && (barcodeName == "")) { } //do nothing
                    else {
                        string comboGroupName = "";
                        string comboName = "";
                        
                        if(primerName == ""){
                            comboGroupName = barcodeName;
                        }else{
                            if(barcodeName == ""){
                                comboGroupName = primerName;
                            }
                            else{
                                comboGroupName = barcodeName + "." + primerName;
                            }
                        }
                        
                        if(((itPrimer->second).forward+(itPrimer->second).reverse) == ""){
                            if ((itBar->second).forward != "NONE") { comboName += (itBar->second).forward; }
                            if ((itBar->second).reverse != "NONE") {
                                if (comboName == "") {  comboName += (itBar->second).reverse; }
                                else {  comboName += ("."+(itBar->second).reverse);  }
                            }
                        }else{
                            if(((itBar->second).forward+(itBar->second).reverse) == ""){
                                if ((itPrimer->second).forward != "NONE") { comboName += (itPrimer->second).forward; }
                                if ((itPrimer->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).reverse; }
                                    else {  comboName += ("."+(itPrimer->second).reverse);  }
                                }
                            }
                            else{
                                if ((itBar->second).forward != "NONE") { comboName += (itBar->second).forward; }
                                if ((itBar->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itBar->second).reverse; }
                                    else {  comboName += ("."+(itBar->second).reverse);  }
                                }
                                if ((itPrimer->second).forward != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).forward; }
                                    else {  comboName += ("."+(itPrimer->second).forward);  }
                                }
                                if ((itPrimer->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).reverse; }
                                    else {  comboName += ("."+(itPrimer->second).reverse);  }
                                }
                            }
                        }

                        
                        if (comboName != "") {  comboGroupName +=  "_" + comboName;  }
                        ofstream temp;
                        map<string, string> variables;
                        if (fileOption) {   variables["[tag]"] = "forward";     }
                        
                        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                        variables["[group]"] = comboGroupName;
                        string fastqFileName = getOutputFileName("fastq", variables);
                        if (uniqueNames.count(fastqFileName) == 0) {
                            outputNames.push_back(fastqFileName);
                            uniqueNames.insert(fastqFileName);
                        }
                        
                        fastqFileNames[itBar->first][itPrimer->first] = fastqFileName;
                        util.openOutputFile(fastqFileName, temp);		temp.close();
                        
                        if (fileOption) {
                            variables["[tag]"] = "reverse";
                            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                            string rfastqFileName = getOutputFileName("fastq", variables);
                            if (uniqueNames.count(rfastqFileName) == 0) {
                                outputNames.push_back(rfastqFileName);
                                uniqueNames.insert(rfastqFileName);
                            }
                            
                            ofstream temp2;
                            rfastqFileNames[itBar->first][itPrimer->first] = rfastqFileName;
                            util.openOutputFile(rfastqFileName, temp2);		temp2.close();
                        }
                        
                        if(fasta){
                            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                            variables["[group]"] = comboGroupName;
                            
                            if (fileOption) {   variables["[tag]"] = "forward";     }
                            string fastaFileName = getOutputFileName("fasta", variables);
                            if (uniqueNames.count(fastaFileName) == 0) {
                                outputNames.push_back(fastaFileName);
                                outputTypes["fasta"].push_back(fastaFileName);
                            }
                            
                            ofstream temp3;
                            fastaFileNames[itBar->first][itPrimer->first] = fastaFileName;
                            util.openOutputFile(fastaFileName, temp3);		temp3.close();
                            
                            if (fileOption) {
                                variables["[tag]"] = "reverse";
                                string fastaFileName2 = getOutputFileName("fasta", variables);
                                if (uniqueNames.count(fastaFileName2) == 0) {
                                    outputNames.push_back(fastaFileName2);
                                    outputTypes["fasta"].push_back(fastaFileName2);
                                }
                                
                                ofstream temp4;
                                rfastaFileNames[itBar->first][itPrimer->first] = fastaFileName2;
                                util.openOutputFile(fastaFileName2, temp4);		temp4.close();
                            }
                        }
                        
                        if(qual){
                            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                            variables["[group]"] = comboGroupName;
                            if (fileOption) {   variables["[tag]"] = "forward";     }
                            string qualFileName = getOutputFileName("qfile", variables);
                            if (uniqueNames.count(qualFileName) == 0) {
                                outputNames.push_back(qualFileName);
                                outputTypes["qfile"].push_back(qualFileName);
                            }
                            
                            ofstream temp4;
                            qualFileNames[itBar->first][itPrimer->first] = qualFileName;
                            util.openOutputFile(qualFileName, temp4);		temp4.close();
                            
                            if (fileOption) {
                                variables["[tag]"] = "reverse";
                                string qualFileName2 = getOutputFileName("qfile", variables);
                                if (uniqueNames.count(qualFileName2) == 0) {
                                    outputNames.push_back(qualFileName2);
                                    outputTypes["qfile"].push_back(qualFileName2);
                                }
                            
                                ofstream temp5;
                                rqualFileNames[itBar->first][itPrimer->first] = qualFileName2;
                                util.openOutputFile(qualFileName2, temp5);		temp5.close();
                            }
                        }

                    }
                }
            }
            
            //make blank files for no matches
            ofstream temp, tempff, tempfq, rtemp, temprf, temprq;
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
            variables["[group]"] = "scrap";
            if (fileOption) {  variables["[tag]"] = "forward"; }
            ffqnoMatchFile = getOutputFileName("fastq", variables);
            util.openOutputFile(ffqnoMatchFile, temp);		temp.close();
            
            if (fileOption) {
                variables["[tag]"] = "reverse";
                rfqnoMatchFile = getOutputFileName("fastq", variables);
                util.openOutputFile(rfqnoMatchFile, rtemp);		rtemp.close();
            }
            
            if (fasta) {
                if (fileOption) {  variables["[tag]"] = "forward"; }
                ffnoMatchFile = getOutputFileName("fasta", variables);
                util.openOutputFile(ffnoMatchFile, tempff);		tempff.close();
                
                if (fileOption) {
                    variables["[tag]"] = "reverse";
                    rfnoMatchFile = getOutputFileName("fasta", variables);
                    util.openOutputFile(rfnoMatchFile, temprf);		temprf.close();
                }
            }
            
            if (qual) {
                if (fileOption) {  variables["[tag]"] = "forward"; }
                fqnoMatchFile = getOutputFileName("qfile", variables);
                util.openOutputFile(fqnoMatchFile, tempfq);		tempfq.close();
                
                if (fileOption) {
                    variables["[tag]"] = "reverse";
                    rqnoMatchFile = getOutputFileName("qfile", variables);
                    util.openOutputFile(rqnoMatchFile, temprq);		temprq.close();
                }
            }

        }else {
            map<string, int> barcodes = oligos.getBarcodes() ;
            map<string, int> primers = oligos.getPrimers();
            for(map<string, int>::iterator itBar = barcodes.begin();itBar != barcodes.end();itBar++){
                for(map<string, int>::iterator itPrimer = primers.begin();itPrimer != primers.end(); itPrimer++){
                    
                    string primerName = oligos.getPrimerName(itPrimer->second);
                    string barcodeName = oligos.getBarcodeName(itBar->second);
                   
                    if ((primerName == "ignore") || (barcodeName == "ignore")) { } //do nothing
                    else if ((primerName == "") && (barcodeName == "")) { } //do nothing
                    else {
                        string comboGroupName = "";
                        string comboName = "";
                        
                        if(primerName == ""){
                            comboGroupName = barcodeName;
                        }else{
                            if(barcodeName == ""){
                                comboGroupName = primerName;
                            }
                            else{
                                comboGroupName = barcodeName + "." + primerName;
                            }
                        }
                        
                        if(itPrimer->first == ""){
                            comboName = itBar->first;
                        }else{
                            if(itBar->first == ""){
                                comboName = itPrimer->first;
                            }
                            else{
                                comboName = itBar->first + "." + itPrimer->first;
                            }
                        }
                        
                        if (comboName != "") {  comboGroupName +=  "_" + comboName;  }
                        
                        
                        ofstream temp;
                        map<string, string> variables;
                        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                        variables["[group]"] = comboGroupName;
                        string fastqFileName = getOutputFileName("fastq", variables);
                        if (uniqueNames.count(fastqFileName) == 0) {
                            outputNames.push_back(fastqFileName);
                            uniqueNames.insert(fastqFileName);
                        }
                        
                        fastqFileNames[itBar->second][itPrimer->second] = fastqFileName;
                        util.openOutputFile(fastqFileName, temp);		temp.close();
                        
                        if(fasta){
                            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                            variables["[group]"] = comboGroupName;
                            string fastaFileName = getOutputFileName("fasta", variables);
                            if (uniqueNames.count(fastaFileName) == 0) {
                                outputNames.push_back(fastaFileName);
                                outputTypes["fasta"].push_back(fastaFileName);
                            }
                            
                            ofstream temp3;
                            fastaFileNames[itBar->second][itPrimer->second] = fastaFileName;
                            util.openOutputFile(fastaFileName, temp3);		temp3.close();
                        }
                        
                        if(qual){
                            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
                            variables["[group]"] = comboGroupName;
                            string qualFileName = getOutputFileName("qfile", variables);
                            if (uniqueNames.count(qualFileName) == 0) {
                                outputNames.push_back(qualFileName);
                                outputTypes["qfile"].push_back(qualFileName);
                            }
                            
                            ofstream temp4;
                            qualFileNames[itBar->second][itPrimer->second] = qualFileName;
                            util.openOutputFile(qualFileName, temp4);		temp4.close();
                        }
                    }
                }
            }
       
            //make blank files for no matches
            ofstream temp, tempff, tempfq;
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
            variables["[group]"] = "scrap";
            ffqnoMatchFile = getOutputFileName("fastq", variables);
            util.openOutputFile(ffqnoMatchFile, temp);		temp.close();
            
            if (fasta) {
                ffnoMatchFile = getOutputFileName("fasta", variables);
                util.openOutputFile(ffqnoMatchFile, tempff);		tempff.close();
            }
            
            if (qual) {
                fqnoMatchFile = getOutputFileName("qfile", variables);
                util.openOutputFile(fqnoMatchFile, tempfq);		tempfq.close();
            }
        }
        
        if (allBlank) {
            m->mothurOut("[WARNING]: your oligos file does not contain any group names.  mothur will not create a groupfile."); m->mothurOutEndLine();
            return false;
        }
       
		return true;
		
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "getOligos");
		exit(1);
	}
}
//***************************************************************************************************************
bool ParseFastaQCommand::readGroup(string groupfile){
	try {
        fastqFileNames.clear();
        
        groupMap = new GroupMap();
        groupMap->readMap(groupfile);
        
        //like barcodeNameVector - no primer names
        vector<string> groups = groupMap->getNamesOfGroups();
		
		fastqFileNames.resize(groups.size());
        for (int i = 0; i < fastqFileNames.size(); i++) {
            for (int j = 0; j < 1; j++) {
                
                map<string, string> variables;
                variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(fastaQFile));
                variables["[group]"] = groups[i];
                string thisFilename = getOutputFileName("fastq",variables);
                outputNames.push_back(thisFilename);
                
                ofstream temp;
                util.openOutputFileBinary(thisFilename, temp); temp.close();
                fastqFileNames[i].push_back(thisFilename);
                GroupToFile[groups[i]] = i;
            }
        }
        
        //make blank files for no matches
        ofstream temp, tempff, tempfq;
        map<string, string> variables;
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputfile));
        variables["[group]"] = "scrap";
        ffqnoMatchFile = getOutputFileName("fastq", variables);
        util.openOutputFile(ffqnoMatchFile, temp);		temp.close();
        
        if (fasta) {
            ffnoMatchFile = getOutputFileName("fasta", variables);
            util.openOutputFile(ffqnoMatchFile, tempff);		tempff.close();
        }
        
        if (qual) {
            fqnoMatchFile = getOutputFileName("qfile", variables);
            util.openOutputFile(fqnoMatchFile, tempfq);		tempfq.close();
        }
		
		return true;
		
	}
	catch(exception& e) {
		m->errorOut(e, "ParseFastaQCommand", "readGroup");
		exit(1);
	}
}
//**********************************************************************************************************************



