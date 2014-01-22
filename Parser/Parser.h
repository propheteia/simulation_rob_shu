/***************************************************************************
 *   Copyright (C) 2008 by Neil Zhu   *
 *   zhuming535984@gmail.com   *
 *   Just test the Parser                                                  *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _PARSER_H
#define _PARSER_H

//#include "Logger.h"
//#include "WorldModel.h"
#include "sexp.h"
#include <string>
#include <cstring>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Logger.h"
#include "worldmodel.h"
#include "self.h"
#include "robot.h"
#include "nao.h"
#include "fieldinfo.h"


#include "StaticModule.h"

#define PARSER StaticModule<Parser>::getInstance()

using namespace std;
/**example
(time (now 4.64))(GS (unum 1) (team left) (t 0.00) (pm BeforeKickOff))(GYR (n torso) (rt -0.00 -0.00 0.00))(See (G1L (pol 6.26 -97.35 3.70)) (G2L (pol 8.25 -95.57 2.81)) (G2R (pol 24.61 -19.47 0.94)) (G1R (pol 24.02 -14.96 0.96)) (F1L (pol 1.20 135.00 -19.29)) (F2L (pol 15.23 -93.01 -1.49)) (F1R (pol 23.22 1.97 -0.98)) (F2R (pol 27.74 -33.23 -0.82)) (B (pol 13.32 -32.74 -1.52)))(HJ (n hj1) (ax 0.00))(HJ (n hj2) (ax 0.00))(HJ (n raj1) (ax -0.00))(HJ (n raj2) (ax -0.00))(HJ (n raj3) (ax 0.00))(HJ (n raj4) (ax 0.00))(HJ (n laj1) (ax -0.00))(HJ (n laj2) (ax 0.00))(HJ (n laj3) (ax 0.00))(HJ (n laj4) (ax 0.00))(HJ (n rlj1) (ax 0.00))(HJ (n rlj2) (ax 0.00))(HJ (n rlj3) (ax 0.00))(HJ (n rlj4) (ax -0.00))(HJ (n rlj5) (ax -0.00))(HJ (n rlj6) (ax 0.00))(HJ (n llj1) (ax 0.00))(HJ (n llj2) (ax -0.00))(HJ (n llj3) (ax 0.00))(HJ (n llj4) (ax -0.00))(HJ (n llj5) (ax -0.00))(HJ (n llj6) (ax -0.00))

(time (now 19.34))(GS (t 3.06) (pm KickOff_Left))(GYR (n torso) (rt 0.44 0.48 -0.01))(See (G1L (pol 24.03 14.94 1.15)) (G2L (pol 24.62 19.44 1.12)) (G2R (pol 8.25 95.47 3.38)) (G1R (pol 6.27 97.22 4.45)) (F1L (pol 23.23 -1.99 -0.78)) (F2L (pol 27.75 33.20 -0.66)) (F1R (pol 1.17 -134.55 -15.60)) (F2R (pol 15.22 92.95 -1.18)) (B (pol 13.33 32.69 -1.18)) (P (team Strive3D) (id 6) (pol 17.72 39.17 0.20)) (P (team Strive3D) (id 7) (pol 15.61 25.48 0.24)))(HJ (n hj1) (ax -6.88))(HJ (n hj2) (ax -0.00))(HJ (n raj1) (ax -68.75))(HJ (n raj2) (ax -41.25))(HJ (n raj3) (ax -0.00))(HJ (n raj4) (ax 36.67))(HJ (n laj1) (ax -68.75))(HJ (n laj2) (ax 41.25))(HJ (n laj3) (ax 0.00))(HJ (n laj4) (ax -36.67))(HJ (n rlj1) (ax -0.00))(HJ (n rlj2) (ax 0.00))(HJ (n rlj3) (ax 45.84))(HJ (n rlj4) (ax -91.67))(HJ (n rlj5) (ax 45.84))(FRP (n rf) (c -0.01 -0.01 -0.01) (f 0.26 -0.24 26.88))(HJ (n rlj6) (ax 0.00))(HJ (n llj1) (ax -0.00))(HJ (n llj2) (ax -0.00))(HJ (n llj3) (ax 45.84))(HJ (n llj4) (ax -91.67))(HJ (n llj5) (ax 45.84))(FRP (n lf) (c -0.02 -0.03 -0.01) (f 0.27 -0.24 18.21))(HJ (n llj6) (ax -0.00))
*/

class Parser
{
public:
	Parser();
	~Parser(){}
	//parse all the messages from server 
	// msg-messages from server
	//if there is anything wrong FALSE, else TRUE
	bool parseAll( const string& msg );

	//parse the value of the sExp atom 
	//in-sExp atom pointer
	//out-the value
	//if there is anything wrong FALSE, else TRUE
	static bool parseValue( const sexp_t* sexp, string& res );

	static bool parseValue( const sexp_t* sexp, int &res );
	
	static bool parseValue( const sexp_t* sexp, float &res );
	
	//static bool parseValue( const sexp_t* sexp, Vector3f &res );
	
	//parse the given value of the sExp list 
	//insExp list pointer
	//in-the name of given value
	//out-the value
	//if the value name is the same with given name, and successfully get the value, return TRUE, otherwise return FALSE
	template <class T>
	static bool parseGivenValue( const sexp_t* sexp, const char* given, T &res )
	{
		if ( sexp->ty == SEXP_LIST )
		{
			sexp_t* tmp = sexp->list;
			if ( tmp->ty == SEXP_VALUE )
			{
				if ( 0==strcmp(given,tmp->val) )
				{
					return parseValue(tmp->next,res);
				}
			}
			return false;
		}
		return false;
	}
	
	bool parserWayPoints(std::string msg);

private:
	bool parseTime                  ( const sexp_t* sexp );
	//You can find all the GameState in (./plugin/soccer/soccertypes.h)
	bool parseGameState             ( const sexp_t* sexp );
	bool parseGyro                  ( const sexp_t* sexp );
	bool parseVision                ( const sexp_t* sexp );
	bool parseHingeJoint            ( const sexp_t* sexp );
    	bool parseUniversalJoint        ( const sexp_t* sexp );
	bool parseFRP                   ( const sexp_t* sexp );
	bool parseHear                  ( const sexp_t* sexp );
	bool parsePosition              ( const sexp_t* sexp );
	bool parsePlayer				( const sexp_t* sexp );
	bool parseLimbPosition			( const sexp_t* sexp, string name );
	bool parseAccelerometer			( const sexp_t* sexp);
	//indicate if know the opponents' team name 
	bool		iKnowOppName;
	//indicate how many opps and tmms
	int 		idCounterMate;
	int 		idCounterOpp;

	boost::shared_ptr<Self>    mSelf;
	
	//s-expression parser struct pointer 
	pcont_t*	_pcont;
	sexp_t*		_sexp;
};

#endif
