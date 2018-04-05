//
//  Consumer.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Consumer__
#define __BB__Consumer__

#include <iostream>
#include "Group.h"
#include <math.h>


class Consumer : public Group
{
public:
	Consumer();
	Consumer(int num, std::string name, double biomass, double bodymass, double CN, double ax, double y, double z, double BMtotprey, int totprey, double loss);
	~Consumer();
    
    void calcZ();
    double getY();
    double getZ();
    double getBMtotprey(state_type Btemp);
    state_type getPref();
    void SetPref(state_type, int);
    double getBM();
    
	private :
    double m_ax;
    double m_y;
    double m_z;
    double m_BMtotprey;
    int m_totprey;
    int m_nbtot;
    state_type m_pref;
};


#endif /* defined(__BB__Consumer__) */

