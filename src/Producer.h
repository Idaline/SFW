//
//  Producer.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Producer__
#define __BB__Producer__

#include <iostream>
#include "Group.h"


class Producer : public Group
{
public:
	Producer();
	Producer(int num, std::string name, double biomass, double bodymass, double r, double m_K, double m_CN, double loss);
	~Producer();
    
    double getr();
    double getK();
    
	private :
    double m_r;
	double m_K;	
};

#endif /* defined(__BB__Producer__) */

