//
//  Group.cpp
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include "Group.h"

Group::Group () : m_type("consumer"), m_biomass(0), m_bodymass(1)
{
}

Group::Group (int num, std::string name, double biomass, double bodymass, double CN, double loss) : m_num(num), m_biomass(biomass), m_bodymass(bodymass), m_CN(CN), m_name(name), m_loss(loss)
{
}

Group::~Group()
{
}

double Group :: getB()
{
    return m_biomass;
}

double Group :: getNC()
{
	double NC;
	NC=1.0/m_CN;
	return NC;
}

double Group :: getN(double B)
{
	double N;
	N=B/m_CN;
    return N;
}

double Group :: getBmass()
{
    return m_bodymass;
}

int Group :: getNum()
{
    return m_num;
}

std::string Group :: getName()
{
    return m_name;
}

double Group :: getLoss()
{
    return m_loss;
}

double Group :: SetTest (std::string name, double B, double dB)
{
    dB=0;
    if (B<=0)
    {
        std::cout << "HALT" << "\t" << name << "\t" << B << std::endl;
    }
    return dB;
}

