//
//  Microbes.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Microbes__
#define __BB__Microbes__

#include <iostream>
#include "Group.h"
#include <math.h>


class Microbes : public Group
{
public:

	Microbes();
	Microbes(int num, std::string name, double biomass, double bodymass, double CN, double r, double K, double loss);
	~Microbes();
 
    double consume(double Micmass, double Detmass, int DetNum);
    double getr();
    double getK();   
    void getPref(state_type pref, int nbPC, int nbdet);

	private :
    double m_r;
    double m_K;
    state_type m_pref;
    int m_nbCP;
};


#endif /* defined(__BB__Microbes__) */