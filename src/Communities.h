//
//  Communities.h
//  FW
//
//  
//  Copyright (c) 2014 Idaline Laigle. All rights reserved.
//

#ifndef __BB__Communities__
#define __BB__Communities__

#include <iostream>
#include <map>
#include <fstream>
#include "Group.h"
#include "Consumer.h"
#include "Param.h"
#include "Detritus.h"
#include "Producer.h"
#include "Microbes.h"
#include <boost/numeric/odeint.hpp>


class Communities
{
	public :


	Communities(std::vector<Consumer> &Consumer,std::vector<Detritus> &Detritus,std::vector<Producer> &Producer,std::vector<Microbes> &Microbes, int nbcons, int nbdet, int nbprod, int nbmic, state_type &B, state_type &Btemp, int timer, double Nin, double Nout, double DetIn, double CNDetIn, double Tf, double Tp); 
	~Communities();  
	

	std::map<std::string, std::map<std::string,double> > loadpref(par::Params parPref);
	void operator()(state_type &x , state_type &dxdt , double t);
	void operator()(state_type &x , const double t);
	void integration(par::Params parParam, const char* result);
	void calcComZ();

	private :
	//pref_array m_pref;
	std::vector<Consumer> m_Consumer;
	std::vector<Detritus> m_Detritus;
	std::vector<Producer> m_Producer;
	std::vector<Microbes> m_Microbes;
	state_type B;
	int m_nbcons;
	int m_nbdet;
	int m_nbprod;
	int m_nbmic;
	int m_nbtot;
	int m_timer;
	state_type m_Btemp;
	double m_Nin;
	double m_Nout;
	double m_DetIn;
	double m_CNDetIn;
	double m_Tf;
	double m_Tp;

};

#endif /*defined(__BB__Communities__) */
