//
//  Consumer.cpp
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include "Consumer.h"


using namespace std;


Consumer::Consumer () : Group(), m_ax(0.314), m_y(8)
{	
}

Consumer::Consumer (int num, std::string name, double biomass, double bodymass, double CN, double ax, double y, double z, double BMtotprey, int totprey, double loss) : Group(num, name, biomass, bodymass, CN, loss), m_ax(ax), m_y(y), m_z(z), m_BMtotprey(BMtotprey), m_totprey(totprey)
{
}

Consumer::~Consumer()
{
}

void Consumer :: calcZ()
{
    if (m_bodymass>=0.00001)
    {
        m_z=m_ax * pow(m_bodymass,-0.25); 
    }
    else if (m_bodymass<0.00001 && m_bodymass>=0.0000001)
    {
        m_z=m_ax * pow(m_bodymass,0.06); 
    }
    else
    {
        m_z=m_ax * pow(m_bodymass,0.96); 
    }
}

double Consumer :: getBM()
{
    return m_bodymass;
}

double Consumer :: getY()
{
    return m_y;
}

double Consumer :: getZ()
{
    return m_z;
}

double Consumer :: getBMtotprey(state_type Btemp)
{
    m_BMtotprey =0.;
    for (int i=0; i<m_nbtot; ++i) {
        m_BMtotprey += Btemp[i]*m_pref[i];
    }
    return m_BMtotprey;
}

void Consumer :: SetPref(state_type pref, int nbtot){
    m_totprey=0;
    m_nbtot=nbtot;
    for (int i=0; i<nbtot; ++i) {
        if (pref[i] != 0)
        {
            m_totprey+=1;
        }
    }
    for (int i=0; i<nbtot; ++i) {
        m_pref[i] = pref[i];//m_totprey; A remettre selon si besoin de calculer les pref 
        if(m_pref[i]<0) cout << m_name << "\t" << i << endl;
    }
}

state_type Consumer :: getPref(){
    return m_pref;
}
