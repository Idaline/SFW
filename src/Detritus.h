//
//  Detritus.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Detritus__
#define __BB__Detritus__

#include <iostream>
#include "Group.h"
#include <boost/numeric/odeint.hpp>


class Detritus : public Group
{
public:

    public :

	Detritus();
	Detritus(int num, std::string name, double biomass, double bodymass, double CN, double loss, double CNmin, double CNmax);
	~Detritus();

    void calcCN(double C1,double C2, double N2);
	int SetClassDet (double CNdet);

private:
    double m_CNmin;
    double m_CNmax;
};

#endif /* defined(__BB__Detritus__) */

