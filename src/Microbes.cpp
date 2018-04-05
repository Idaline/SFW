//
//  Microbes.cpp
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include "Microbes.h"


using namespace std;


Microbes::Microbes () : Group(), m_r(1)
{	
}

Microbes::Microbes (int num, std::string name, double biomass, double bodymass, double CN, double r, double K, double loss) : Group(num, name, biomass, bodymass, CN, loss), m_r(r), m_K(K)
{
}

Microbes::~Microbes()
{
}


double Microbes :: consume(double Micmass, double Detmass, int DetNum)
{
    double cons=(m_r*Micmass*(m_pref[DetNum-m_nbCP]*Detmass))/(m_K+(m_pref[DetNum-m_nbCP]*Detmass));
    return cons;
}
void Microbes :: getPref(state_type pref, int nbCP, int nbD)
{
	m_nbCP=nbCP;
    for (int i=0; i<nbD; ++i) {
        m_pref[i] = pref[i];
        if(m_pref[i]<0) cout << m_name << "\t" << i << endl;
    }
}