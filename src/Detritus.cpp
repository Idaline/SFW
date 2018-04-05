//
//  Nutrients.cpp
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include "Detritus.h"


using namespace std;

Detritus::Detritus () : Group()
{
}

Detritus::Detritus (int num, std::string name, double biomass, double bodymass, double CN, double loss, double CNmin, double CNmax) : Group (num, name, biomass, bodymass, CN, loss), m_CNmin (CNmin), m_CNmax(CNmax)
{
}

Detritus::~Detritus()
{
}

void Detritus :: calcCN(double C1,double C2, double N2)
{
    if (((C1/m_CN)+N2)>0)
    {
        m_CN = (C1+C2)/((C1/m_CN)+N2);
    }  
}

int Detritus :: SetClassDet (double CNdet)
{
    if (CNdet>=m_CNmin && CNdet<m_CNmax)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

