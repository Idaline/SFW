//
//  main.cpp
//  FW
//
//  Created by Idaline Laigle and Eric Giguere.
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <vector>
#include <fstream>
#include <boost/numeric/odeint.hpp>


#include "Group.h"
#include "Consumer.h"
#include "Producer.h"
#include "Detritus.h"
#include "Microbes.h"
#include "Communities.h"
#include "Param.h"


using namespace std;
using namespace boost::numeric::odeint;



Communities::Communities (vector<Consumer> &Consumer, vector<Detritus> &Detritus, vector<Producer> &Producer, vector<Microbes> &Microbes,
    int nbcons, int nbdet, int nbprod, int nbmic, state_type &B, state_type &Btemp, int timer, double Nin, double Nout, double DetIn, double CNDetIn, double Tf, double Tp) : m_Consumer(Consumer),
m_Detritus(Detritus), m_Producer(Producer), m_Microbes(Microbes), m_nbcons(nbcons), m_nbdet(nbdet), m_nbprod(nbprod), m_nbmic(nbmic), B(B), m_Btemp(Btemp), m_timer(timer), m_Nin(Nin), m_Nout(Nout), m_DetIn(DetIn), m_CNDetIn(CNDetIn), m_Tf(Tf), m_Tp(Tp)
{
    m_nbtot = m_nbcons+m_nbdet+m_nbprod+m_nbmic+1;
}

Communities::~Communities()
{
}


void testPrefName(par::Params parPref, par::Params parParam)
{
    std::map<string, std::map<string,double> > pref;
    for (int i=0; i< parPref.get_val<string>("Cons").size() ; i++)
    {
        std::string predname = parPref.get_val<std::string>("Cons")[i];
        std::string preyname = parPref.get_val<std::string>("Ress")[i];
        pref[predname][preyname] = parPref.get_val<double>("Preference")[i];

        int testPred=0;
        int testPrey=0;

        for (int j=0; j< parParam.get_val<string>("group").size() ; j++)
        {
            string key=parParam.get_val<string>("group")[j];
            if (predname==key)
            {
                testPred+=1;
            }
            if (preyname==key)
            {
                testPrey+=1;
            }
        }
            if (testPred==0)
            {
                cout << "Error pred" << "\t" << predname << "\t" << "is not present in param" << endl;
            }
            if (testPrey==0)
            {
                cout << "Error prey " << "\t" << preyname << "\t" << "is not present in param" << endl;
            }
    }
}

void Communities :: operator ()( state_type &x , state_type &dxdt , double t)
{
    map<string,double>PreyNameBM;
    //Initialisation

    int NiNum=0;
    dxdt[NiNum]=0;

    int num10;
    int numR;
    int num35;

    for (int itD=0; itD<m_nbdet; ++itD)
    {
        if (m_Detritus[itD].getName()=="Det10")
        {
            num10 = itD+1+m_nbcons+m_nbprod;
        }
        if (m_Detritus[itD].getName()=="DetR")
        {
            numR = itD+1+m_nbcons+m_nbprod;
        }
        if (m_Detritus[itD].getName()=="Det35")
        {
            num35 = itD+1+m_nbcons+m_nbprod;
        }
    }

    int i;

    for (int itC=0; itC<m_nbcons; ++itC)
    {
        i = itC+1;
        dxdt[i]=0.;

        if(m_Btemp[i]<0)
        {
            cout << "HALT" << "\t" << m_Consumer[itC].getName() << "\t" << m_Btemp[i] << std::endl;
            return;
        }
    }
    for (int itP=0; itP<m_nbprod; ++itP)
    {
        i = itP+1+m_nbcons;
        dxdt[i]=0.;
        if(m_Btemp[i]<0)
        {
            cout << "HALT" << "\t" << m_Producer[itP].getName() << "\t" << m_Btemp[i] << std::endl;
            return;
        }
    }
    for (int itD=0; itD<m_nbdet; ++itD)
    {
        i = itD+1+m_nbcons+m_nbprod;
        dxdt[i]=0.;
        dxdt[m_nbtot+itD]=0;
        dxdt[m_nbtot+itD+5]=0;

        if(m_Btemp[i]<0)
        {
            cout << "HALT" << "\t" << m_Detritus[itD].getName() << "\t" << m_Btemp[i] << std::endl;
            return;
        }
    }
    for (int itM=0; itM<m_nbmic; ++itM)
    {
        i = itM+1+m_nbcons+m_nbprod+m_nbdet;
        dxdt[i]=0.;
        if(m_Btemp[i]<0)
        {
            cout << "HALT" << "\t" << m_Microbes[itM].getName() << "\t" << m_Btemp[i] << std::endl;
            return;
        }
    }
    if (m_Btemp[NiNum]<0)
    {
        cout << "HALT" << "\t" << "Ni" << endl;
        return;
    }

//Detritus

    for (int itD=0; itD<m_nbdet; ++itD)
    {
        int p = m_nbcons+m_nbprod+1+itD;

        //leaching
        double leaching = m_Detritus[itD].getLoss() * m_Btemp[p];

        dxdt[p] -= leaching;
        dxdt[m_nbtot+itD]-=leaching;
        dxdt[m_nbtot+itD+5]-=leaching/(1/m_Detritus[itD].getNC());
    }

// Litter input from trees

    for (int itD=0; itD<m_nbdet; ++itD)
    {
        int Cond=m_Detritus[itD].SetClassDet(m_CNDetIn);
        if (Cond==1)
        {
            dxdt[m_nbcons+m_nbprod+1+itD] +=m_DetIn;
            dxdt[m_nbtot+itD]+=m_DetIn;
            dxdt[m_nbtot+itD+5]+=m_DetIn/m_CNDetIn;
        }
    }


//Producer
    for (int itP=0; itP<m_nbprod; ++itP)
    {
        int i = m_nbcons+1+itP;
        double loss = m_Producer[itP].getLoss();
        double litprod = 0;
    //Growth
        double r = m_Producer[itP].getr();
        double G = m_Btemp[NiNum]/(m_Btemp[NiNum]+m_Producer[itP].getK());

        if (m_Btemp[NiNum]>0)
        {
            dxdt[i] = m_Btemp[i] * r * G;
            dxdt[NiNum] -=  (r * G * m_Btemp[i])*m_Producer[itP].getNC();
            //cout << m_Producer[itP].getName() << "\t" << (r * G * m_Btemp[i])*m_Producer[itP].getNC() << endl;
        }

    //Litter production
        litprod = m_Btemp[i] * loss;
        dxdt[i] -= litprod;
//Calculate C/N ratio detritus

        double CNplant = 1/m_Producer[itP].getNC();
        if (litprod>0)
        {
            for (int itD=0; itD<m_nbdet; ++itD)
            {
                int Cond=m_Detritus[itD].SetClassDet(CNplant);
                if (Cond==1)
                {
                    dxdt[m_nbcons+m_nbprod+1+itD] +=litprod;
                    dxdt[m_nbtot+itD]+=litprod;
                    dxdt[m_nbtot+itD+5]+=litprod/CNplant;
                }
            }
        }
    }

//Microbes

    for (int itM=0; itM<m_nbmic; ++itM)
    {
        int i = m_nbcons+m_nbprod+m_nbdet+1+itM;
        double Ccons=0;
        double Ncons=0;
        double CNmic=1/m_Microbes[itM].getNC();
        for (int itD=0; itD<m_nbdet; ++itD)
        {
                int j = m_nbcons+m_nbprod+1+itD;

                if (m_Btemp[j]>0)
                {
                    double CconsT = m_Microbes[itM].consume(m_Btemp[i],m_Btemp[j],j);
                    double NconsT = CconsT*m_Detritus[itD].getNC();
                    if (m_Detritus[itD].getName()=="DetR"){ NconsT = 0;}
                    Ccons+=CconsT;
                    Ncons+=NconsT;
                    dxdt[j] -= CconsT;
                    dxdt[m_nbtot+itD] -= CconsT;
                    dxdt[m_nbtot+itD+5] -= NconsT;
                }
        }
        if ((Ccons/Ncons) < CNmic ) // Climitant
        {
            double Negest=Ncons-(Ccons/CNmic);
            if ((Ncons-(Ccons/CNmic))<0)
            {
                cout << "HALT" << "\t" << "Ni" << endl;
                return;
            }
            else
            {
                dxdt[NiNum]+=Negest;
            }
        }
        else if ((Ccons/Ncons) > CNmic) //Nlimitant
        {
            if (x[NiNum]>(Ccons/CNmic - Ncons))
            {
                dxdt[NiNum]-= Ccons/CNmic - Ncons;
                if ((Ccons/CNmic - Ncons)<0)
                {
                    cout << "HALT4" << endl;
                    return;
                }
            }
            else
            {
                double CEgest=Ccons-(Ncons*CNmic);
                if ((Ccons-(Ncons*CNmic))<0)
                {
                    cout << "HALT5" << endl;
                    return;
                }
                Ccons-=CEgest;
                dxdt[numR]+=CEgest;
            }
        }
        dxdt[i] += Ccons;

    //Respiration

        double d=m_Microbes[itM].getLoss();
        double resp=d*m_Btemp[i];
        dxdt[i]-=resp;
        dxdt[NiNum]+=resp*m_Microbes[itM].getNC();
    }

//Consumer
    // Load parameters
    double Bo = 0.5;
    double c = 1;
    double h = 2.0;
    //double SumNS=0;
    //Dynamic
    for (int itC=0; itC<m_nbcons; ++itC)
    {
        int i = itC+1;
        string name = m_Consumer[itC].getName();

        double y = m_Consumer[itC].getY();
        double z = m_Consumer[itC].getZ();
        double Ccons=0;
        double Ncons=0;
        state_type pref = m_Consumer[itC].getPref();
        double BMtotprey = m_Consumer[itC].getBMtotprey(m_Btemp);
        double TotCEgest = 0;
        double TotNEgest = 0;

    //Consume

        for (int itP=0; itP<m_nbprod; ++itP)
        {
            int j = m_nbcons+1+itP;
            if (pref[j] != 0)
            {
                double e =0.65;
                double conso = m_Btemp[i]*y*z* ((pref[j]*pow(m_Btemp[j],h)) / (pow(Bo,h)+c*m_Btemp[i]*pow(Bo,h)+BMtotprey));
                dxdt[j] -= conso;
                Ccons+=e*conso;
                Ncons+=e*conso*m_Producer[itP].getNC();
                TotCEgest += (1.0-e)*conso;
            //Nitrogen
                TotNEgest += (1.0-e)*conso * m_Producer[itP].getNC();
             }
        }
        for (int itCC=0; itCC<m_nbcons; ++itCC)
        {
            int j = itCC+1;
            if (pref[j]!=0)
            {
            //Carbon
                double e = 0.85;
                double conso = m_Btemp[i]*y*z* ((pref[j]*pow(m_Btemp[j],h)) / (pow(Bo,h)+c*m_Btemp[i]*pow(Bo,h)+BMtotprey));
                dxdt[j] -= conso;
                Ccons+=e*conso;
                Ncons+=e*conso*m_Consumer[itCC].getNC();
                TotCEgest += (1.0-e)*conso;
            //Nitrogen
                TotNEgest += (1-e) * conso * m_Consumer[itCC].getNC();
            }
        }
        for (int itD=0; itD<m_nbdet; ++itD )
        {
            int j = m_nbcons+m_nbprod+1+itD;
            if (pref[j]!=0)
            {
        //Carbon
                double e =0.65;
                double conso = m_Btemp[i]*y*z* ((pref[j]*pow(m_Btemp[j],h)) / (pow(Bo,h)+c*m_Btemp[i]*pow(Bo,h)+BMtotprey));
                dxdt[j] -= conso;
                Ccons+=e*conso;
                TotCEgest += (1.0-e)*conso;
                Ncons+=e*conso*m_Detritus[itD].getNC();
                TotNEgest += ((1.0-e)*conso) * m_Detritus[itD].getNC();
                dxdt[m_nbtot+itD] -= conso;
                dxdt[m_nbtot+itD+5] -= conso*m_Detritus[itD].getNC();
            }
        }
        for (int itM=0; itM<m_nbmic; ++itM)
        {
            int j = m_nbcons+m_nbprod+m_nbdet+1+itM;
            if (pref[j]!=0)
            {
        //Carbon
                double e =0.65;
                double conso = m_Btemp[i]*y*z* ((pref[j]*pow(m_Btemp[j],h)) / (pow(Bo,h)+c*m_Btemp[i]*pow(Bo,h)+BMtotprey));
                dxdt[j] -= conso;
                Ccons+=e*conso;
                TotCEgest += (1.0-e)*conso;
                Ncons+=e*conso*m_Microbes[itM].getNC();
                TotNEgest += (1.0-e)*conso * m_Microbes[itM].getNC();
            }
        }

    //Metabolism
       double resp = m_Btemp[i] * z;
       Ccons   -= resp;
    //Calculate C/N ratio conso

        double CNcons=1/m_Consumer[itC].getNC();
        if ((Ccons/Ncons)<CNcons) //Climitant
        {
            TotNEgest += Ncons-(Ccons/CNcons);
            if ((Ncons-(Ccons/CNcons))<0)
            {
                cout << "HALT1" << "\t" << Ccons << "\t" << m_Btemp[i] << endl;
                return;
            }
            Ncons-=Ncons-(Ccons/CNcons);
        }
        else
        {
            TotCEgest+=Ccons-(Ncons*CNcons);  // On ne fait pas de différence entre egestion et excretion de C
            Ccons-=Ccons-(Ncons*CNcons);
        }
        dxdt[i]+=Ccons;

//Calculate C/N ratio detritus
        if(TotCEgest/TotNEgest < 50 && TotCEgest/TotNEgest >= 1)
        {
            double CNdet = TotCEgest/TotNEgest ;
            for (int itD=0; itD<m_nbdet; ++itD)
            {
                int Cond=m_Detritus[itD].SetClassDet(CNdet);
                if (Cond==1)
                {
                    dxdt[m_nbcons+m_nbprod+1+itD]+=TotCEgest;
                    dxdt[m_nbtot+itD]+=TotCEgest;
                    dxdt[m_nbtot+itD+5]+=TotNEgest;
                }
            }
        }
        else if (TotCEgest/TotNEgest >= 50) // Trop de C, va dans Det R, le peu qui reste va en minéral
        {
            dxdt[NiNum]+=TotNEgest;
            dxdt[numR]+=TotCEgest;

        }
        else if (TotCEgest/TotNEgest < 1) // Y'a trop d'azote pour aller dans Det10, un max y va le reste c'est juste du minéral
        {
            double Ns=TotNEgest-TotCEgest;
            dxdt[num10]+=TotCEgest;
            dxdt[m_nbmic+m_nbdet+num10]+=TotCEgest;
            dxdt[m_nbmic+m_nbdet+num10+5]+=TotNEgest-Ns;
            dxdt[NiNum]+=Ns; //Ca c'est du pipi pur
        }
    }

//Inorganic nitrogen


    if (m_Btemp[NiNum]>0)
    {
            dxdt[NiNum]-=m_Nout*m_Btemp[NiNum];
    }
        //Input ->
            dxdt[NiNum]+=m_Nin;

}

ofstream out;

void Communities :: operator()( state_type &x , const double t)//Ordre changee
{
    //Set CN Detritus
    for (int itD=0; itD<m_nbdet; ++itD)
    {
        if (m_Detritus[itD].getName()!="DetR")
        {
            m_Detritus[itD].calcCN(m_Btemp[m_nbcons+m_nbprod+1+itD],x[m_nbtot+itD],x[m_nbtot+itD+5]);
            x[m_nbtot+itD]=0;
            x[m_nbtot+itD+5]=0;
        }
/*
        if (t>149.99)
        {
          cout << m_Detritus[itD].getName() << "\t" << 1/m_Detritus[itD].getNC() << endl;
        }
*/
    }

    for (int i=0; i<m_nbtot; i++)
    {
        m_Btemp[i]=x[i];
        if (x[i]<0.0001)
        {
            m_Btemp[i]=0;
        }
        if (x[i]<0)
        {
            cout <<  "HALT<0 " << i << endl;
            return;
        }
    }

    if(m_timer==0)
    {
        out << t ;
        for (int i=0; i<m_nbtot; i++)
        {
            out << '\t' << x[i];
        }
        out << endl;
    }
    if (m_timer==(m_Tf/m_Tp))
    {
        m_timer=0;
    }
    else
    {
        ++m_timer;
    }
}

void Communities :: integration(par::Params parParam, const char* result)//Ordre changee
{
        out.open(result);
            out << "t" << '\t';
            out << parParam.get_val<string>("group")[0] << '\t';
            for (int itC=0; itC<m_nbcons; ++itC){
                out << m_Consumer[itC].getName()<< '\t';
            }
            for (int itP=0; itP<m_nbprod; ++itP){
                out << m_Producer[itP].getName()<< '\t';
            }
            for (int itD=0; itD<m_nbdet; ++itD){
                out << m_Detritus[itD].getName()<< '\t';
            }
            for (int itM=0; itM<m_nbmic; ++itM){
                out << m_Microbes[itM].getName()<< '\t';
            }
        out << endl;

        //integrate_adaptive(make_controlled( 1.0e-6 , 1.0e-6 , runge_kutta_fehlberg78< state_type >()) , boost::ref(*this),B , 0.0 , 1.0 , 0.1 , boost::ref(*this));
        integrate_const(runge_kutta_fehlberg78< state_type >() , boost::ref(*this), B , 0.0, m_Tf, m_Tp, boost::ref(*this));
        out.close();
}

int main(int argc, const char *argv[])
{

    //Objects building

    par::Params ParamText(argv[1]," ");

    vector<Consumer> vecConsumer;
    vector<Detritus> vecDetritus;
    vector<Producer> vecProducer;
    vector<Microbes> vecMicrobes;
    map<string,int> Name2Num;

    int nbcons =0;
    int nbdet=0;
    int nbprod=0;
    int nbmic=0;

    state_type initial_biomass;
    /*
        L'ordre de la biomass est [Azote, [Consumer],[Producer],[Detritus]]
    */

    for(int i=1; i < ParamText.get_val<string>("type").size(); i++)
    {
        string type = ParamText.get_val<string>("type")[i];
        if     (type == "Detritus") { ++nbdet ; }
        else if(type == "Consumer") { ++nbcons; }
        else if(type == "Producer") { ++nbprod; }
        else if(type == "Microbes") { ++nbmic; }
    }
    vecConsumer.reserve(nbcons);
    vecDetritus.reserve(nbdet);
    vecProducer.reserve(nbprod);
    vecMicrobes.reserve(nbmic);

    int icons =1;
    int idet  =1;
    int iprod =1;
    int imic =1;

    state_type Btemp;

    //Azote
    initial_biomass[0] = ParamText.get_val<double>("biomass")[0];
    Btemp[0]=initial_biomass[0];
    Name2Num[ParamText.get_val<string>("group")[0]]=0;

    map<string,double>prefMic;

    for(int i=1; i < ParamText.get_val<string>("type").size(); i++)
    {
        string type = ParamText.get_val<string>("type" )[i];
        string key  = ParamText.get_val<string>("group")[i];

         if(type == "Consumer") {
            initial_biomass[icons] = ParamText.get_val<double>("biomass")[i];
            Btemp[icons]=initial_biomass[icons];
            Name2Num[key]=icons;
            vecConsumer.push_back( Consumer(icons,
                                            ParamText.get_val<string>("group")[i],
                                            ParamText.get_val<double>("biomass")[i],
                                            ParamText.get_val<double>("bodymass")[i],
                                            ParamText.get_val<double>("CN")[i],
                                            ParamText.get_val<double>("ax")[i],
                                            ParamText.get_val<double>("y")[i],
                                            0, 0, 0,
                                            ParamText.get_val<double>("loss")[i]));
            //cout << "cons" <<  "\t" << icons << "\t" << key << endl;
            ++icons;
            vecConsumer.back().calcZ();
        } else if(type == "Producer") {
            initial_biomass[iprod+nbcons] = ParamText.get_val<double>("biomass")[i];
            Btemp[iprod+nbcons]=initial_biomass[iprod+nbcons];
            Name2Num[key]=iprod+nbcons;
            vecProducer.push_back( Producer(iprod+nbcons,
                                            ParamText.get_val<string>("group")[i],
                                            ParamText.get_val<double>("biomass")[i],
                                            ParamText.get_val<double>("bodymass")[i],
                                            ParamText.get_val<double>("r")[i],
                                            ParamText.get_val<double>("K")[i],
                                            ParamText.get_val<double>("CN")[i],
                                            ParamText.get_val<double>("loss")[i]));
            //cout << "prod" <<  "\t" << iprod+nbcons << "\t" << key << endl;
            ++iprod;
        } else if(type == "Detritus") {
            initial_biomass[idet+nbcons+nbprod] = ParamText.get_val<double>("biomass")[i];
            Btemp[idet+nbcons+nbprod]=initial_biomass[idet+nbcons+nbprod];
            Name2Num[key]=idet+nbcons+nbprod;
            vecDetritus.push_back( Detritus(idet+nbcons+nbprod,
                                            ParamText.get_val<string>("group")[i],
                                            ParamText.get_val<double>("biomass")[i],
                                            ParamText.get_val<double>("bodymass")[i],
                                            ParamText.get_val<double>("CN")[i],
                                            ParamText.get_val<double>("loss")[i],
                                            ParamText.get_val<double>("CNmin")[i],
                                            ParamText.get_val<double>("CNmax")[i]));
            //cout << "det" <<  "\t" << idet+nbcons+nbprod << "\t" << key << endl;
            ++idet;
        } else if(type == "Microbes") {
            initial_biomass[imic+nbcons+nbprod+nbdet] = ParamText.get_val<double>("biomass")[i];
            Btemp[nbdet+nbcons+nbprod+imic]=initial_biomass[nbdet+nbcons+nbprod+imic];
            Name2Num[key]=nbdet+nbcons+nbprod+imic;
            vecMicrobes.push_back( Microbes(imic+nbcons+nbprod+nbdet,
                                            ParamText.get_val<string>("group")[i],
                                            ParamText.get_val<double>("biomass")[i],
                                            ParamText.get_val<double>("bodymass")[i],
                                            ParamText.get_val<double>("CN")[i],
                                            ParamText.get_val<double>("r")[i],
                                            ParamText.get_val<double>("K")[i],
                                            ParamText.get_val<double>("loss")[i]));
            //cout << "mic" <<  "\t" << imic+nbcons+nbprod+nbdet << "\t" << key << endl;
            ++imic;
        }
    }

    par::Params PrefText(argv[2]," ");
    // Test correspondance between name in param and pref
    testPrefName(PrefText, ParamText);
    //Les predateur on la liste, la construise une seule fois.
    {
        double pref[SPECIES_MAX_NUM][SPECIES_MAX_NUM];
        for (int i=1; i<nbcons + nbdet + nbprod + nbmic +1; i++)
        {
            for (int j=1; j<nbcons + nbdet + nbprod + nbmic; j++)
            {
                pref[i][j]=0;
            }
        }
        for (int i=1; i< PrefText.get_val<string>("Cons").size() ; i++)
        {
            std::string predname = PrefText.get_val<std::string>("Cons")[i];
            std::string preyname = PrefText.get_val<std::string>("Ress")[i];
            if (Name2Num[predname]>=SPECIES_MAX_NUM) cout<<"Too  many species, increase SPECIES_MAX_NUM\n";
            if (Name2Num[preyname]>=SPECIES_MAX_NUM) cout<<"Too  many species, increase SPECIES_MAX_NUM\n";
            pref[Name2Num[predname]][Name2Num[preyname]] = PrefText.get_val<double>("Preference")[i];
        }

        for (int itC=0; itC<nbcons; ++itC)
        {
            state_type hisPref;
            int h=1;
            for (int i=1; i<nbcons + nbdet + nbprod + nbmic + 1; ++i)
            {
                hisPref[h] = pref[itC+1][i];
                ++h;
            }
            vecConsumer[itC].SetPref(hisPref, nbcons + nbdet + nbprod + nbmic + 1);
        }
        for (int itM=0; itM<nbmic; ++itM)
        {
            state_type hisPref;
            int h=1;
            for (int i=1+nbcons+nbprod; i<nbcons + nbprod + nbdet + 1; ++i)
            {
                hisPref[h] = pref[nbcons + nbdet + nbprod + itM+1][i];
                ++h;
            }
            vecMicrobes[itM].getPref(hisPref, nbcons+ nbprod, nbdet + 1);
        }
    }

    // Groups that take received detritus

    int nbgp = nbcons + nbdet + nbprod + nbmic + 1;

    for (int v=nbgp+1;v<nbgp+12;++v)
    {
        initial_biomass[v] = 0;
        Btemp[v]=0;
    }

    int timer=0;

    par::Params ArgText(argv[4]," ");
    double Tp=ArgText.get_val<double>("Tf")[1];
    double Tf=ArgText.get_val<double>("Tp")[1];
    double Nin=ArgText.get_val<double>("Nin")[1];
    double Nout=ArgText.get_val<double>("Nout")[1];
    double DetIn=ArgText.get_val<double>("DetIn")[1];
    double CNDetIn=ArgText.get_val<double>("CNDetIn")[1];


    Communities Com1 (vecConsumer, vecDetritus, vecProducer, vecMicrobes, nbcons, nbdet, nbprod, nbmic, initial_biomass, Btemp, timer, Nin, Nout, DetIn, CNDetIn, Tf, Tp);

//Run the dynamic

    Com1.integration(ParamText, argv[3]);
    return 0;
}
