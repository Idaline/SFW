//
//  Group.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Group__
#define __BB__Group__

#include<map>
#include<iostream>

#include <boost/numeric/odeint.hpp>
const int SPECIES_MAX_NUM=250;
typedef boost::array< double , SPECIES_MAX_NUM> state_type;

class Group {
	
	public :
	Group();
	Group(int num, std::string name, double biomass, double bodymass, double CN, double loss);
	~Group();
    
    double getB();
    double getBmass();
    int getNum();
    double getN(double B);
    double getNC();
    double SetTest (std::string name, double B, double dB);
    std::string getName();
    double getLoss();

	protected :
	int m_num;
    std::string m_type;
    std::string m_name;
    double m_biomass;
    double m_bodymass;
    double m_CN;   
    double m_loss; 
};


#endif /* defined(__BB__Group__) */

