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

//#include "Geometry.h"
#include "Parser.h"
#include <iostream>
#include "types.h"
using namespace std;
/*Parser instance */
//Parser seuparser;
/**example
(time (now 4.64))(GS (unum 1) (team left) (t 0.00) (pm BeforeKickOff))(GYR (n torso) (rt -0.00 -0.00 0.00))(See (G1L (pol 6.26 -97.35 3.70)) (G2L (pol 8.25 -95.57 2.81)) (G2R (pol 24.61 -19.47 0.94)) (G1R (pol 24.02 -14.96 0.96)) (F1L (pol 1.20 135.00 -19.29)) (F2L (pol 15.23 -93.01 -1.49)) (F1R (pol 23.22 1.97 -0.98)) (F2R (pol 27.74 -33.23 -0.82)) (B (pol 13.32 -32.74 -1.52)))(HJ (n hj1) (ax 0.00))(HJ (n hj2) (ax 0.00))(HJ (n raj1) (ax -0.00))(HJ (n raj2) (ax -0.00))(HJ (n raj3) (ax 0.00))(HJ (n raj4) (ax 0.00))(HJ (n laj1) (ax -0.00))(HJ (n laj2) (ax 0.00))(HJ (n laj3) (ax 0.00))(HJ (n laj4) (ax 0.00))(HJ (n rlj1) (ax 0.00))(HJ (n rlj2) (ax 0.00))(HJ (n rlj3) (ax 0.00))(HJ (n rlj4) (ax -0.00))(HJ (n rlj5) (ax -0.00))(HJ (n rlj6) (ax 0.00))(HJ (n llj1) (ax 0.00))(HJ (n llj2) (ax -0.00))(HJ (n llj3) (ax 0.00))(HJ (n llj4) (ax -0.00))(HJ (n llj5) (ax -0.00))(HJ (n llj6) (ax -0.00))

(time (now 19.34))(GS (t 3.06) (pm KickOff_Left))(GYR (n torso) (rt 0.44 0.48 -0.01))(See (G1L (pol 24.03 14.94 1.15)) (G2L (pol 24.62 19.44 1.12)) (G2R (pol 8.25 95.47 3.38)) (G1R (pol 6.27 97.22 4.45)) (F1L (pol 23.23 -1.99 -0.78)) (F2L (pol 27.75 33.20 -0.66)) (F1R (pol 1.17 -134.55 -15.60)) (F2R (pol 15.22 92.95 -1.18)) (B (pol 13.33 32.69 -1.18)) (P (team Strive3D) (id 6) (pol 17.72 39.17 0.20)) (P (team Strive3D) (id 7) (pol 15.61 25.48 0.24)))(HJ (n hj1) (ax -6.88))(HJ (n hj2) (ax -0.00))(HJ (n raj1) (ax -68.75))(HJ (n raj2) (ax -41.25))(HJ (n raj3) (ax -0.00))(HJ (n raj4) (ax 36.67))(HJ (n laj1) (ax -68.75))(HJ (n laj2) (ax 41.25))(HJ (n laj3) (ax 0.00))(HJ (n laj4) (ax -36.67))(HJ (n rlj1) (ax -0.00))(HJ (n rlj2) (ax 0.00))(HJ (n rlj3) (ax 45.84))(HJ (n rlj4) (ax -91.67))(HJ (n rlj5) (ax 45.84))(FRP (n rf) (c -0.01 -0.01 -0.01) (f 0.26 -0.24 26.88))(HJ (n rlj6) (ax 0.00))(HJ (n llj1) (ax -0.00))(HJ (n llj2) (ax -0.00))(HJ (n llj3) (ax 45.84))(HJ (n llj4) (ax -91.67))(HJ (n llj5) (ax 45.84))(FRP (n 
lf) (c -0.02 -0.03 -0.01) (f 0.27 -0.24 18.21))(HJ (n llj6) (ax -0.00))
*/

/**new
 (time (now 9.68))(GS (t 0.00) (pm BeforeKickOff))(GYR (n torso) (rt 0.00 0.00 0.00))(HJ (n hj1) (ax -0.00))(HJ (n hj2) (ax -0.00))(See (G2R (pol 11.03 -3.52 1.39)) (G1R (pol 11.04 3.78 1.41)) (F1R (pol 11.73 20.11 -2.59)) (F2R (pol 11.72 -19.91 -2.65)) (B (pol 5.03 -0.05 -5.86)) (P (team SEU-RedSun) (id 10) (head (pol 5.71 -20.47 -0.12)) (rlowerarm (pol 5.53 -20.28 -0.87)) (llowerarm (pol 5.60 -22.12 -0.82)) (rfoot (pol 5.65 -20.19 -5.44)) (lfoot (pol 5.69 -21.33 -5.48))) (P (team Strive3D) (id 1) (rlowerarm (pol 0.18 -35.28 -21.94)) (llowerarm (pol 0.19 33.15 -22.33))) (P (team SEU-RedSun) (id 9) (head (pol 5.30 1.97 -0.25)) (rlowerarm (pol 5.21 2.97 -2.08)) (llowerarm (pol 5.19 1.00 -2.23)) (rfoot (pol 5.30 2.89 -5.59)) (lfoot (pol 5.28 1.43 -5.81))) (P (team SEU-RedSun) (id 1) (head (pol 10.50 -0.04 -0.08)) (rlowerarm (pol 10.39 0.30 -1.11)) (llowerarm (pol 10.41 -0.27 -0.85)) (rfoot (pol 10.47 0.30 -2.87)) (lfoot (pol 10.48 -0.23 -2.81))))(HJ (n raj1) (ax 0.00))(HJ (n raj2) (ax -0.00))(HJ (n raj3) (ax -
0.00))(HJ (n raj4) (ax -0.00))(HJ (n laj1) (ax 0.00))(HJ (n laj2) (ax -0.00))(HJ (n laj3) (ax -0.00))(HJ (n laj4) (ax 0.00))(HJ (n rlj1) (ax 0.00))(HJ (n rlj2) (ax -0.00))(HJ (n rlj3) (ax 0.00))(HJ (n rlj4) (ax -0.00))(HJ (n rlj5) (ax 0.00))(HJ (n rlj6) (ax -0.00))(HJ (n llj1) (ax 0.00))(HJ (n llj2) (ax 0.00))(HJ (n llj3) (ax -0.00))(HJ (n llj4) (ax -0.00))(HJ (n llj5) (ax -0.00))(HJ (n llj6) (ax -0.00))*/

Parser::Parser():
iKnowOppName(false)//The agent doesn't know opponents' team name at the beginning.
{
	//set SEXP parser parmas
	set_parser_buffer_params(10,512);
}

bool Parser::parseAll( const string& msg )
{
	int msgLen = msg.size();	
	char* c = const_cast<char*>(msg.c_str());
	_pcont = init_continuation(c);
	_sexp = iparse_sexp(c,msgLen,_pcont);
	pLOG << "***************************************the beginning of parser***************************************" << endl;
	while( _sexp != 0)
	{	
		if ( _sexp->ty == SEXP_LIST )
		{
			sexp_t* tmp = _sexp->list;
			if ( tmp->ty == SEXP_VALUE )
			{
				switch ( *(tmp->val) )
				{
					case 't':
						pLOG << "parse time" << endl;
						parseTime( tmp->next );
					break;
					case 'A':
						pLOG << "parse Accelerometer" << endl;
						parseAccelerometer(tmp->next);
            		case 'G':	
						switch ( *((tmp->val)+1) )
						{
							case 'S' :
							pLOG << "parse GS" << endl;
							parseGameState( tmp->next );
							break;
							case 'Y':
							pLOG << "parse GYR" << endl;
							parseGyro( tmp->next );
							break;
							default:
							break;
						}
					break;
					case 'S':
						pLOG << "parse Vision" << endl;	
						parseVision( tmp->next );
					break;
					case 'H':
						pLOG << "parse HJ" << endl;	
						parseHingeJoint( tmp->next );
					break;
					case 'U':
						pLOG << "parse UJ" << endl;	
						parseUniversalJoint( tmp->next );
					break;
					case 'F':
						pLOG << "parse FRP" << endl;	
						parseFRP( tmp->next );
					break;
					case 'h': // hear
						pLOG << "parse Hear Information: " <<  endl;	
						parseHear( tmp->next );
					break;
					default:
						pLOG << "[Parser Warning]received unknown data: " << tmp->val << endl;
						return false;
					break;
				}	
			}
		}
	destroy_sexp(_sexp);
	_sexp = iparse_sexp(c,msg.size(),_pcont);
	}
	destroy_continuation(_pcont);
	pLOG << "***************************************[Parser]successfully parse one message form server***************************************" << endl;
	return true;
}

bool Parser::parseTime( const sexp_t* sexp )
{	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{	case 'n':
				float f_curCycle;
				long l_curCycle;
				f_curCycle = atof(tmp->next->val);
				l_curCycle = long((f_curCycle * 1000.0 + 1)/10.0);
				LOG.newCycle(l_curCycle);
				pLOG.newCycle(l_curCycle);
				pLOG << tmp->next->val << endl;
				WM.setMySimTime( atof(tmp->next->val) );
			break;
		}
		sexp = sexp->next;
	}
	return true;
}

bool Parser::parseGameState( const sexp_t* sexp )
{
	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{
			case 'u'://unum
				pLOG << tmp->next->val << endl;
				if (WM.getSelf().GetUnum() == 0)
    				{
        				WM.getSelf().setUnum( atoi(tmp->next->val) );
    				}
			break;
			case 't'://time/team
				if( *(tmp->val+1) == 'e')
				{	pLOG << tmp->next->val << endl;
					string TIName;
    					if ( WM.getFieldInfo().team_index == TI_NONE )
    					{	TIName = tmp->next->val;
        					if (TIName == "left")
            				{
								WM.getFieldInfo().team_index = TI_LEFT;
							}		
        					else if (TIName == "right")
            				{
								WM.getFieldInfo().team_index = TI_RIGHT;
							}
        					else
        					{
            						WM.getFieldInfo().team_index = TI_NONE;
            						pLOG << "(WorldModel) received TeamIndex TI_NONE\n";
        					}
							WM.SetupVisionSenseMap();
							pLOG << "Our GoalCenter: " << WM.getFieldInfo().ourgoalcenter << endl;
						}
				}
				else
				{	pLOG << tmp->next->val << endl;
					WM.getFieldInfo().match_time = atof(tmp->next->val);
				}
			break;
			case 'p'://playmode
				pLOG << tmp->next->val << endl;
				FieldInfo::TPlayModeMap::iterator idIter = WM.getFieldInfo().mPlayModeMap.find ( tmp->next->val );
    				TPlayMode pm = (*idIter).second;
    				WM.getFieldInfo().play_mode = pm;
			break;
		}
		sexp = sexp->next;
	}
	return true;
}

bool Parser::parseGyro( const sexp_t* sexp )
{	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{	case 'n':
				pLOG << tmp->next->val << endl;
			break;
			case 'r':
				for(int i = 0 ; i < 3 ; i++)
				{	pLOG << tmp->next->val << endl;
					WM.getSelf().setGyroAngularVel(i, atof( tmp->next->val) );
					tmp = tmp->next;
				}
			break;
		}
		sexp = sexp->next;
 	}
	return true;
}

bool Parser::parseVision( const sexp_t* sexp )
{
	WM.VisionOn();
	pLOG << "mVisionOn!" << endl;
	while ( sexp != 0 )
	{
		if ( sexp->ty == SEXP_LIST )
		{
			sexp_t* tmp = sexp->list;
			if ( tmp->ty == SEXP_VALUE && parsePosition(tmp) )
			{
				pLOG << "［parseVision］updated one object" << endl;
			}
			else
			{
				pLOG << "［parseVision］error " << endl;
			}
		}
		else
		{
			pLOG << "［parseVision］updated one object :" << sexp->val << endl;
		}
		sexp = sexp->next;
	}
	return true;
}

bool Parser::parseHingeJoint            ( const sexp_t* sexp )
{	//! read the object name
    	string name;
	JointID jid;
	HingeJointSense sense;
    	sexp_t* tmp = sexp->list;
	if ( 'n' == *(tmp->val) )
	{	pLOG << tmp->next->val << endl;
		name = tmp->next->val;
	}

    //! try to lookup the joint id
	jid = NAO->checkJointIDMap( name );
    	 
	sexp = sexp->next;
	tmp = sexp->list;

    	//! read the angle value
    	if ( 'a' == *(tmp->val) )
    	{	//pLOG << tmp->next->val << endl;
		sense.angle = atof ( tmp->next->val );
	}
	else 
	{	return false;
	}

    	//! update the map
    	NAO->setHingeJointSenseMap ( jid , sense );
   	//pLOG << "angle: " << sense.angle << "rate: " << sense.rate << endl;
	return true;
}

bool Parser::parseUniversalJoint        ( const sexp_t* sexp )
{	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{	case 'n':
				pLOG << tmp->next->val << endl;
			break;
			case 'a':
				switch ( *((tmp->val)+2) )
                		{
                			case '1' :
                    				pLOG << tmp->next->val << endl;
                    			break;
                			case '2':
						pLOG << tmp->next->val << endl;
                    			break;
                			default:
                    			break;
                		}
			break;
		}
		sexp = sexp->next;
	}
	return true;
}

bool Parser::parseFRP                   ( const sexp_t* sexp )
{	string name;
	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{	case 'n':
				pLOG << tmp->next->val << endl;
				name = tmp->next->val;
			break;
			case 'c':
				if ( name == "lf" )
				{	for(int i = 0 ; i < 3 ; i++)
					{	pLOG << tmp->next->val << endl;
						WM.getSelf().setLFCenterVel( i, atof( tmp->next->val) );
						tmp = tmp->next;
					}
				}
				else
				{	for(int i = 0 ; i < 3 ; i++)
					{	pLOG << tmp->next->val << endl;
						WM.getSelf().setRFCenterVel( i, atof( tmp->next->val) );
						tmp = tmp->next;
					}
				}
			break;
			case 'f':
				if (  name == "lf" )
				{	for(int i = 0 ; i < 3 ; i++)
					{	pLOG << tmp->next->val << endl;
						WM.getSelf().setLFForce( i, atof( tmp->next->val) );
						tmp = tmp->next;
					}
				}
				else
				{	for(int i = 0 ; i < 3 ; i++)
					{	pLOG << tmp->next->val << endl;
						WM.getSelf().setRFForce( i, atof( tmp->next->val) );
						tmp = tmp->next;
					}
				}
			break;
		}
		sexp = sexp->next;
	}
	return true;
}

bool Parser::parseHear( const sexp_t* sexp )
{
	const sexp_t* tmp = sexp;
	HearInfo HI;
	
	HI.time = atof(tmp->val);	//!time
	tmp = tmp->next;
	
	if('s' == *(tmp->val))	//!sender & dirction
	{
		HI.sender = tmp->val;
		HI.direction = -1000;
	}
	else 
	{
		HI.sender = "others";
		HI.direction = atof(tmp->val);
	}
	tmp = tmp->next;
	
	HI.message = tmp->val;	//!message
	
	pLOG << "Hear: " << HI.time << ' ' << HI.sender << ' ' << HI.direction << ' ' << HI.message << endl;
	WM.HearingOn();
	WM.setHearInfo(HI);
	return true;
}

bool Parser::parsePosition( const sexp_t* sexp )
{	
	idCounterMate = 0;
	idCounterOpp = 0;
	stringstream playerName;
	string objName = sexp->val;//player or flag
	//Polar pol;
	sexp = sexp->next;
	sexp_t* tmp = sexp->list;

	switch(objName[0])
	{
		case 'P':		//!Player
			parsePlayer(sexp);
			break;
		case 'F':		//!Flag
		case 'G':		//!Goal
			if(WM.getFieldInfo().team_index == TI_RIGHT)	//!The Right Team has the reverse position
			{
				if('L' == objName[2])						//!Change them so as to get the same view for both 
					objName[2] = 'R';						//!Right Team and Left Team
				else if('R' == objName[2])
					objName[2] = 'L';
				else
					pLOG << "Unknow Flag or Goal(L or R)" << endl;

				if('1' == objName[1])
					objName[1] = '2';
				else if('2' == objName[1])
					objName[1] = '1';
				else
					pLOG << "Unknow Flag or Goal(1 or 2)" << endl;
			}
		case 'B':		//!Ball
			if( *(tmp->val) == 'p')//pol
			{	float distance, theta, phi;
				VisionObject VO;
				VO = WM.setVisionObjectMap ( objName );
				pLOG << objName << endl;
				distance = atof(tmp->next->val);
				tmp = tmp->next;
				theta = atof(tmp->next->val);
				tmp = tmp->next;
				phi = atof (tmp->next->val);
				pLOG << distance << ' ' << theta << ' ' << phi << endl; 
				WM.setVisionSenseMap (  VO,distance,theta,phi );
			}
			break;
		default:
			pLOG << "Unknow VisionObject" << endl;
	}


}

bool Parser::parsePlayer( const sexp_t* sexp )
{
	sexp_t* tmp = sexp->list;
	string teamName, playerName, partName;
	if ('t' == *(tmp->val) )//teamname
	{	
		teamName = tmp->next->val;
		if ( !iKnowOppName )
		{	
			if ( !( teamName == WM.getFieldInfo().our_teamname) )
			{	// set opp team name
				pLOG << teamName << endl;
				WM.getFieldInfo().setOppTeamName ( teamName ) ;
				iKnowOppName=true;
			}
		}
		sexp = sexp->next;
		tmp = sexp->list;
		int id =0;
		if ('i' == *(tmp->val) )	//id
		{	
			id = atoi ( tmp->next->val );
			pLOG << "id: " << id << endl;
		}
		if (teamName == WM.getFieldInfo().our_teamname)
		{
			stringstream ss;
			ss << "TEAMMATE";
			idCounterMate += id;
			ss << idCounterMate;
			playerName = ss.str();
		}
		else if (teamName == WM.getFieldInfo().opp_teamname)
		{
			stringstream ss;
			ss << "OPPONENT";
			idCounterOpp += id;
			ss << idCounterOpp;
			playerName = ss.str();
		}
		sexp = sexp->next;
		if('p' == *(sexp->list->val))
		{
			float distance, theta, phi;
			VisionObject VO;
			tmp = sexp->list->next;
			playerName += "HEAD";
			VO = WM.setVisionObjectMap ( playerName );
			pLOG << playerName << endl;
			distance = atof(tmp->val);
			tmp = tmp->next;
			theta = atof(tmp->val);
			tmp = tmp->next;
			phi = atof (tmp->val);
			pLOG << distance << ' ' << theta << ' ' << phi << endl;
			WM.setVisionSenseMap (  VO,distance,theta,phi );
		}
		else
		{
			while ( 0 != sexp )
			{
				parseLimbPosition(sexp, playerName);
				sexp = sexp->next;
			}
		}
		return true;
	}
}

bool Parser::parseLimbPosition( const sexp_t* sexp, string name )
{
	sexp_t* tmp = sexp->list;
	switch ( *(tmp->val) )
	{
		case 'h':
			name += "HEAD";
			pLOG << name << endl;
			break;
		case 'l':
			if ( 'l' == *(tmp->val + 1) )
			{
				name += "LEFTARM";
				pLOG << name << endl;
			}
			else if ( 'f' == *(tmp->val + 1) )
			{
				name += "LEFTFOOT";
				pLOG << name << endl;
			}
			break;
		case 'r':
			if ( 'l' == *(tmp->val + 1) )
			{
				name += "RIGHTARM";
				pLOG << name << endl;
			}
			else if ( 'f' == *(tmp->val + 1) )
			{
				name += "RIGHTFOOT";
				pLOG << name << endl;
			}
			break;
		default:
			pLOG << "[Parser Warning]received unknown Limb: " << *(tmp->val) << endl;
			return false;
			break;
	}
	tmp = tmp->next->list;
	if ( 'p' == *(tmp->val) )
	{	
		float distance, theta, phi;
		tmp = tmp->next;
		distance = atof(tmp->val);
		tmp = tmp->next;
		theta = atof(tmp->val);
		tmp = tmp->next;
		phi = atof(tmp->val);
		pLOG << distance << ' ' << theta << ' ' << phi << endl;
		VisionObject VO = WM.setVisionObjectMap ( name );
		WM.setVisionSenseMap (  VO,distance,theta,phi );
	}
	return true;
}

bool Parser::parseValue( const sexp_t* sexp, string& res )
{
	if ( sexp->ty == SEXP_VALUE )
	{
		res = sexp->val;
		return true;
	}
	return false;
}

bool Parser::parseValue( const sexp_t* sexp, int &res )
{
	if ( sexp->ty == SEXP_VALUE )
	{
		res = atoi(sexp->val);
		return true;
	}
	return false;
}

bool Parser::parseValue( const sexp_t* sexp, float &res )
{
	if ( sexp->ty == SEXP_VALUE )
	{
		res = atof(sexp->val);
		return true;
	}
	return false;
}

/*bool Parser::parseValue( const sexp_t* sexp, Vector3f &res )
{
	for ( int i=0;i<3;i++)
	{
		if ( sexp->ty != SEXP_VALUE )
		{
			return false;
		}
		res[i] = atof(sexp->val);
		sexp=sexp->next;
	}
	return true;
}*/

//Zheng Yonglei
bool Parser::parserWayPoints(std::string msg)
{
	int msgLen = msg.size();	
	char* c = const_cast<char*>(msg.c_str());
	_pcont = init_continuation(c);
	_sexp = iparse_sexp(c,msgLen,_pcont);
	LOG << "Parser::parserWayPoints-->Begin parsing..."<< endl;
	while( _sexp != 0)
	{	
		if ( _sexp->ty == SEXP_LIST )
		{
			sexp_t* tmp = _sexp->list;
			if ( tmp->ty == SEXP_VALUE )
			{
				if ( *(tmp->val) == 'W')
				{
					const sexp_t* sexp = tmp->next;
					while ( sexp !=0 )
					{
						sexp_t* tmp = sexp->list;
						switch ( *(tmp->val) )
						{	
							case 'w':
								//px-py-vx-vy
								for(int i = 0 ; i < 4 ; i++)
								{	LOG << tmp->next->val << " ";
									tmp = tmp->next;
								}
								LOG<<endl;
								//pLOG << tmp->next->val << endl;
								//WM.setMySimTime( atof(tmp->next->val) );
								break;
						}
						sexp = sexp->next;
					}
					//return true;
				}
			}
		}
		destroy_sexp(_sexp);
		_sexp = iparse_sexp(c,msg.size(),_pcont);
	}
	destroy_continuation(_pcont);
	LOG << "Parser::parserWayPoints-->Parser finished" << endl;
	return true;
}
//!By Qu Junjun 2010.1.23
bool Parser::parseAccelerometer( const sexp_t* sexp)
{
	while ( sexp !=0 )
	{
		sexp_t* tmp = sexp->list;
		switch ( *(tmp->val) )
		{	case 'n':
				pLOG << tmp->next->val << endl;
			break;
			case 'a':
				for(int i = 0 ; i < 3 ; i++)
				{	pLOG << tmp->next->val << endl;
					WM.getSelf().setAcceleration(i, atof( tmp->next->val) );
					tmp = tmp->next;
				}
			break;
		}
		sexp = sexp->next;
 	}
	return true;
}
