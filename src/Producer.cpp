//
//  Producer.cpp
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include "Producer.h"


using namespace std;


Producer::Producer () : Group(), m_r(0)
{
}

Producer::Producer (int num, std::string name, double biomass, double bodymass, double r, double K, double CN, double loss) : Group(num, name, biomass, bodymass, CN, loss), m_r(r), m_K(K)
{
}

Producer::~Producer()
{
}

double Producer :: getr()
{
    return m_r;
}

double Producer :: getK()
{
    return m_K;
}

