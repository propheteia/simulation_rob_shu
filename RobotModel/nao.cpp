/*
   Copyright (C) 2007, 2008 by Team Apollo
   Nanjing University of Posts and Telecommunications, China

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "nao.h"

using namespace std;
using namespace salt;
using namespace boost;

Nao::Nao()
{
    //SetName("nao");

    /** these two values must be in constructor */
    JID_MIN             = JID_ROOT;
    JID_MAX             = JID_RLEG_6;
    PART_MIN            = JID_ROOT;
    PART_MAX            = JID_RLEG_6;

    FootLength          = 0.08;
    FootWidth           = 0.16;
    FootHeight          = 0.02;

    ShankLength         = 0.08;
    ShankWidth          = 0.07;
    ShankHeight         = 0.11;

    ThighLength         = 0.07;
    ThighWidth          = 0.07;
    ThighHeight         = 0.14;


    TorsoLength         = 0.1;
    TorsoWidth          = 0.1;
    TorsoHeight         = 0.18;
    /**mass*/
    FootMass            = 0.2;
    AnkleMass           = 0.125;
    ShankMass           = 0.225;
    ThighMass           = 0.275;
    Hip2Mass            = 0.125;
    Hip1Mass            = 0.09;

    TorsoMass           = 1.2171;

    HeadMass            = 0.35;
    NeckMass            = 0.05;

    ShoulderMass        = 0.07;
    UpperarmMass        = 0.150;
    ElbowMass           = 0.035;
    LowerarmMass        = 0.2;
    /**translation*/
    TorsoInitX          = 0.0;
    TorsoInitY          = 0.0;
    TorsoInitZ          = 0.385;

    NeckRelTorso_X       =0.0;
    NeckRelTorso_Y       =0.0;
    NeckRelTorso_Z       =0.09;

    HeadRelNeck_X        =0.0;
    HeadRelNeck_Y        =0.0;
    HeadRelNeck_Z        =0.065;

    ShoulderRelTorso_X = 0.098;
    ShoulderRelTorso_Y = 0.0;
    ShoulderRelTorso_Z = 0.075;

    UpperarmRelShoulder_X = 0.01;
    UpperarmRelShoulder_Y = 0.02;
    UpperarmRelShoulder_Z = 0.0;

    ElbowRelUpperarm_X = 0.01;
    ElbowRelUpperarm_Y = 0.07;
    ElbowRelUpperarm_Z = 0.009;

    LowerarmElbow_X = 0.0;
    LowerarmElbow_Y = 0.05;
    LowerarmElbow_Z = 0.0;

    Hip1RelTorso_X      = 0.055;
    Hip1RelTorso_Y      = -0.01;
    Hip1RelTorso_Z      = -0.115;

    Hip2RelHip1_X       = 0.0;
    Hip2RelHip1_Y       = 0.0;
    Hip2RelHip1_Z       = 0.0;

    ThighRelHip2_X      = 0.0;
    ThighRelHip2_Y      = 0.01;  //set 0.005 to check the tri-angle algorithm
    ThighRelHip2_Z      = -0.04;

    ShankRelThigh_X     = 0.0;
    ShankRelThigh_Y     = 0.005;
    ShankRelThigh_Z     = -0.125;

    AnkleRelShank_X     = 0.0;
    AnkleRelShank_Y     = -0.01;
    AnkleRelShank_Z     = -0.055;

    FootRelAnkle_X      = 0.0;
    FootRelAnkle_Y      = 0.03;
    FootRelAnkle_Z      = -0.03;

    Hip1Axis_X          = -0.7071;
    Hip1Axis_Y          = 0.0;
    Hip1Axis_Z          = 0.7071;
}

Nao::~Nao()
{
}

void Nao::SetupLinks()
{
    uLINK[JID_ROOT].name         = "j0";
    uLINK[JID_ROOT].eff_name     = "e0";
    uLINK[JID_ROOT].sister       = 0;
    uLINK[JID_ROOT].child        = JID_LLEG_1;
    uLINK[JID_ROOT].mother       = 0;
    uLINK[JID_ROOT].a            = Vector3f(0, 0, 0);
    uLINK[JID_ROOT].c            = Vector3f(0, 0, 0);
    uLINK[JID_ROOT].b            = Vector3f(0, 0, 0);
    uLINK[JID_ROOT].q            = 0;
    uLINK[JID_ROOT].p            = Vector3f(TorsoInitX, TorsoInitY, TorsoInitZ);
    uLINK[JID_ROOT].R.Identity();
	uLINK[JID_ROOT].m        = TorsoMass;

    /** ----------------------------- legs begin ---------------------- */
    uLINK[JID_LLEG_1].name       = "llj1";
    uLINK[JID_LLEG_1].eff_name   = "lle1";
    uLINK[JID_LLEG_1].sister     = JID_RLEG_1;
    uLINK[JID_LLEG_1].child      = JID_LLEG_2;
    uLINK[JID_LLEG_1].mother     = JID_ROOT;
    uLINK[JID_LLEG_1].a          = Vector3f(Hip1Axis_X, Hip1Axis_Y, -Hip1Axis_Z);
    uLINK[JID_LLEG_1].c          = Vector3f(0, 0, 0);
    uLINK[JID_LLEG_1].b          = Vector3f(-Hip1RelTorso_X, Hip1RelTorso_Y, Hip1RelTorso_Z)
                                   - uLINK[JID_LLEG_1].c + uLINK[JID_ROOT].c;
    uLINK[JID_LLEG_1].q          = 0;
	uLINK[JID_LLEG_1].m        = Hip1Mass;

    uLINK[JID_LLEG_2].name       = "llj2";
    uLINK[JID_LLEG_2].eff_name   = "lle2";
    uLINK[JID_LLEG_2].sister     = 0;
    uLINK[JID_LLEG_2].child      = JID_LLEG_3;
    uLINK[JID_LLEG_2].mother     = JID_LLEG_1;
    uLINK[JID_LLEG_2].a          = Vector3f(0, 1, 0); // Vector3f(1, 0, 0)
    uLINK[JID_LLEG_2].c          = Vector3f(0, 0, 0);
    uLINK[JID_LLEG_2].b          = Vector3f(Hip2RelHip1_X, Hip2RelHip1_Y, Hip2RelHip1_Z)
                                   - uLINK[JID_LLEG_2].c + uLINK[JID_LLEG_1].c;
    uLINK[JID_LLEG_2].q          = 0;
	uLINK[JID_LLEG_2].m        = Hip2Mass;

    uLINK[JID_LLEG_3].name       = "llj3";
    uLINK[JID_LLEG_3].eff_name   = "lle3";
    uLINK[JID_LLEG_3].sister     = 0;
    uLINK[JID_LLEG_3].child      = JID_LLEG_4;
    uLINK[JID_LLEG_3].mother     = JID_LLEG_2;
    uLINK[JID_LLEG_3].a          = Vector3f(1, 0, 0); // Vector3f(0, 1, 0)
    uLINK[JID_LLEG_3].c          = Vector3f(ThighRelHip2_X, ThighRelHip2_Y, ThighRelHip2_Z);
    uLINK[JID_LLEG_3].b          = Vector3f(ThighRelHip2_X, ThighRelHip2_Y, ThighRelHip2_Z)
                                   - uLINK[JID_LLEG_3].c + uLINK[JID_LLEG_2].c;
    uLINK[JID_LLEG_3].q          = 0;
	uLINK[JID_LLEG_3].m        = ThighMass;

    uLINK[JID_LLEG_4].name       = "llj4";
    uLINK[JID_LLEG_4].eff_name   = "lle4";
    uLINK[JID_LLEG_4].sister     = 0;
    uLINK[JID_LLEG_4].child      = JID_LLEG_5;
    uLINK[JID_LLEG_4].mother     = JID_LLEG_3;
    uLINK[JID_LLEG_4].a          = Vector3f(1, 0, 0);
    uLINK[JID_LLEG_4].c          = Vector3f(0, 0.01, -0.045);
    uLINK[JID_LLEG_4].b          = Vector3f(ShankRelThigh_X, ShankRelThigh_Y, ShankRelThigh_Z)
                                   - uLINK[JID_LLEG_4].c + uLINK[JID_LLEG_3].c;
    uLINK[JID_LLEG_4].q          = 0;
	uLINK[JID_LLEG_4].m        = ShankMass;

    uLINK[JID_LLEG_5].name       = "llj5";
    uLINK[JID_LLEG_5].eff_name   = "lle5";
    uLINK[JID_LLEG_5].sister     = 0;
    uLINK[JID_LLEG_5].child      = JID_LLEG_6;
    uLINK[JID_LLEG_5].mother     = JID_LLEG_4;
    uLINK[JID_LLEG_5].a          = Vector3f(1, 0, 0);
    uLINK[JID_LLEG_5].c          = Vector3f(0, 0, 0);
    uLINK[JID_LLEG_5].b          = Vector3f(AnkleRelShank_X, AnkleRelShank_Y, AnkleRelShank_Z)
                                   - uLINK[JID_LLEG_5].c + uLINK[JID_LLEG_4].c;
    uLINK[JID_LLEG_5].q          = 0;
	uLINK[JID_LLEG_5].m         = AnkleMass;

    uLINK[JID_LLEG_6].name       = "llj6";
    uLINK[JID_LLEG_6].eff_name   = "lle6";
    uLINK[JID_LLEG_6].sister     = 0;
    uLINK[JID_LLEG_6].child      = 0;
    uLINK[JID_LLEG_6].mother     = JID_LLEG_5;
    uLINK[JID_LLEG_6].a          = Vector3f(0, 1, 0);
    uLINK[JID_LLEG_6].c          = Vector3f(FootRelAnkle_X, FootRelAnkle_Y, FootRelAnkle_Z);
    uLINK[JID_LLEG_6].b          = Vector3f(FootRelAnkle_X, FootRelAnkle_Y, FootRelAnkle_Z)
                                   - uLINK[JID_LLEG_6].c + uLINK[JID_LLEG_5].c;
    uLINK[JID_LLEG_6].q          = 0;
	uLINK[JID_LLEG_6].m         = FootMass;

    uLINK[JID_RLEG_1].name       = "rlj1";
    uLINK[JID_RLEG_1].eff_name   = "rle1";
    uLINK[JID_RLEG_1].sister     = JID_HEAD_2;//change by Neil
    uLINK[JID_RLEG_1].child      = JID_RLEG_2;
    uLINK[JID_RLEG_1].mother     = JID_ROOT;
    uLINK[JID_RLEG_1].a          = Vector3f(Hip1Axis_X, Hip1Axis_Y, Hip1Axis_Z);
    uLINK[JID_RLEG_1].c          = Vector3f(0, 0, 0);
    uLINK[JID_RLEG_1].b          = Vector3f(Hip1RelTorso_X, Hip1RelTorso_Y, Hip1RelTorso_Z)
                                   - uLINK[JID_RLEG_1].c + uLINK[JID_ROOT].c;
    uLINK[JID_RLEG_1].q          = 0;
	uLINK[JID_RLEG_1].m        = Hip1Mass;

    uLINK[JID_RLEG_2].name       = "rlj2";
    uLINK[JID_RLEG_2].eff_name   = "rle2";
    uLINK[JID_RLEG_2].sister     = 0;
    uLINK[JID_RLEG_2].child      = JID_RLEG_3;
    uLINK[JID_RLEG_2].mother     = JID_RLEG_1;
    uLINK[JID_RLEG_2].a          = Vector3f(0, 1, 0); // Vector3f(1, 0, 0)
    uLINK[JID_RLEG_2].c          = Vector3f(0, 0, 0);
    uLINK[JID_RLEG_2].b          = Vector3f(Hip2RelHip1_X, Hip2RelHip1_Y, Hip2RelHip1_Z)
                                   - uLINK[JID_RLEG_2].c + uLINK[JID_RLEG_1].c;
    uLINK[JID_RLEG_2].q          = 0;
	uLINK[JID_RLEG_2].m        = Hip2Mass;

    uLINK[JID_RLEG_3].name      = "rlj3";
    uLINK[JID_RLEG_3].eff_name  = "rle3";
    uLINK[JID_RLEG_3].sister    = 0;
    uLINK[JID_RLEG_3].child     = JID_RLEG_4;
    uLINK[JID_RLEG_3].mother    = JID_RLEG_2;
    uLINK[JID_RLEG_3].a         = Vector3f(1, 0, 0); // Vector3f(0, 1, 0)
    uLINK[JID_RLEG_3].c         = Vector3f(ThighRelHip2_X, ThighRelHip2_Y, ThighRelHip2_Z);
    uLINK[JID_RLEG_3].b         = Vector3f(ThighRelHip2_X, ThighRelHip2_Y, ThighRelHip2_Z)
                                  - uLINK[JID_RLEG_3].c + uLINK[JID_RLEG_2].c;
    uLINK[JID_RLEG_3].q         = 0;
	uLINK[JID_RLEG_3].m        = ThighMass;

    uLINK[JID_RLEG_4].name      = "rlj4";
    uLINK[JID_RLEG_4].eff_name  = "rle4";
    uLINK[JID_RLEG_4].sister    = 0;
    uLINK[JID_RLEG_4].child     = JID_RLEG_5;
    uLINK[JID_RLEG_4].mother    = JID_RLEG_3;
    uLINK[JID_RLEG_4].a         = Vector3f(1, 0, 0);
    uLINK[JID_RLEG_4].c         = Vector3f(0, 0.01, -0.045);
    uLINK[JID_RLEG_4].b         = Vector3f(ShankRelThigh_X, ShankRelThigh_Y, ShankRelThigh_Z)
                                  - uLINK[JID_RLEG_4].c + uLINK[JID_RLEG_3].c;
    uLINK[JID_RLEG_4].q         = 0;
	uLINK[JID_RLEG_4].m        = ShankMass;

    uLINK[JID_RLEG_5].name      = "rlj5";
    uLINK[JID_RLEG_5].eff_name  = "rle5";
    uLINK[JID_RLEG_5].sister    = 0;
    uLINK[JID_RLEG_5].child     = JID_RLEG_6;
    uLINK[JID_RLEG_5].mother    = JID_RLEG_4;
    uLINK[JID_RLEG_5].a         = Vector3f(1, 0, 0);
    uLINK[JID_RLEG_5].c         = Vector3f(0, 0, 0);
    uLINK[JID_RLEG_5].b         = Vector3f(AnkleRelShank_X, AnkleRelShank_Y, AnkleRelShank_Z)
                                  - uLINK[JID_RLEG_5].c + uLINK[JID_RLEG_4].c;
    uLINK[JID_RLEG_5].q         = 0;
	uLINK[JID_RLEG_5].m         = AnkleMass;

    uLINK[JID_RLEG_6].name      = "rlj6";
    uLINK[JID_RLEG_6].eff_name  = "rle6";
    uLINK[JID_RLEG_6].sister    = 0;
    uLINK[JID_RLEG_6].child     = 0;
    uLINK[JID_RLEG_6].mother    = JID_RLEG_5;
    uLINK[JID_RLEG_6].a         = Vector3f(0, 1, 0);
    uLINK[JID_RLEG_6].c         = Vector3f(FootRelAnkle_X, FootRelAnkle_Y, FootRelAnkle_Z);
    uLINK[JID_RLEG_6].b         = Vector3f(FootRelAnkle_X, FootRelAnkle_Y, FootRelAnkle_Z)
                                  - uLINK[JID_RLEG_6].c + uLINK[JID_RLEG_5].c;
    uLINK[JID_RLEG_6].q         = 0;
	uLINK[JID_RLEG_6].m         = FootMass;

    /** ------------------------------ legs end ----------------------- */

    /** head */
    uLINK[JID_HEAD_2].name      = "hj1";
    uLINK[JID_HEAD_2].eff_name  = "he1";
    uLINK[JID_HEAD_2].sister    = JID_LARM_1;
    uLINK[JID_HEAD_2].child     = JID_HEAD_1;
    uLINK[JID_HEAD_2].mother    = JID_ROOT;
    uLINK[JID_HEAD_2].a         = Vector3f(0, 0, 1);
    uLINK[JID_HEAD_2].c         = Vector3f(0, 0, 0);
    uLINK[JID_HEAD_2].b         = Vector3f(NeckRelTorso_X, NeckRelTorso_Y, NeckRelTorso_Z)
                                  - uLINK[JID_HEAD_2].c + uLINK[JID_ROOT].c;
    uLINK[JID_HEAD_2].q         = 0;
	uLINK[JID_HEAD_2].m         = NeckMass;

    uLINK[JID_HEAD_1].name      = "hj2";
    uLINK[JID_HEAD_1].eff_name  = "he2";
    uLINK[JID_HEAD_1].sister    = 0;
    uLINK[JID_HEAD_1].child     = 0;
    uLINK[JID_HEAD_1].mother    = JID_HEAD_2;
    uLINK[JID_HEAD_1].a         = Vector3f(1, 0, 0);
    uLINK[JID_HEAD_1].c         = Vector3f(0, 0, 0.005);
    uLINK[JID_HEAD_1].b         = Vector3f(HeadRelNeck_X, HeadRelNeck_Y, HeadRelNeck_Z)
                                  - uLINK[JID_HEAD_1].c + uLINK[JID_HEAD_2].c;
    uLINK[JID_HEAD_1].q         = 0;
	uLINK[JID_HEAD_1].m       = HeadMass;

    /** ----------------------------- arms begin ---------------------- */
    uLINK[JID_LARM_1].name      = "laj1";
    uLINK[JID_LARM_1].eff_name  = "lae1";
    uLINK[JID_LARM_1].sister     = JID_RARM_1;
    uLINK[JID_LARM_1].child      = JID_LARM_2;
    uLINK[JID_LARM_1].mother     = JID_ROOT;
    uLINK[JID_LARM_1].a          = Vector3f(1, 0, 0);
    uLINK[JID_LARM_1].c          = Vector3f(0, 0, 0);
    uLINK[JID_LARM_1].b          = Vector3f(-ShoulderRelTorso_X,ShoulderRelTorso_Y ,ShoulderRelTorso_Z )
                                   - uLINK[JID_LARM_1].c + uLINK[JID_ROOT].c;
    uLINK[JID_LARM_1].q          = 0;
	uLINK[JID_LARM_1].m        = ShoulderMass;

    uLINK[JID_LARM_2].name      = "laj2";
    uLINK[JID_LARM_2].eff_name  = "lae2";
    uLINK[JID_LARM_2].sister     = 0;
    uLINK[JID_LARM_2].child      = JID_LARM_3;
    uLINK[JID_LARM_2].mother     = JID_LARM_1;
    uLINK[JID_LARM_2].a          = Vector3f(0, 0, 1);
    uLINK[JID_LARM_2].c          = Vector3f(-UpperarmRelShoulder_X, UpperarmRelShoulder_Y, UpperarmRelShoulder_Z);
    uLINK[JID_LARM_2].b          = Vector3f(-UpperarmRelShoulder_X, UpperarmRelShoulder_Y, UpperarmRelShoulder_Z)
                                   - uLINK[JID_LARM_2].c + uLINK[JID_LARM_1].c;
    uLINK[JID_LARM_2].q          = 0;
	uLINK[JID_LARM_2].m        = UpperarmMass;

    uLINK[JID_LARM_3].name      = "laj3";
    uLINK[JID_LARM_3].eff_name  = "lae3";
    uLINK[JID_LARM_3].sister     = 0;
    uLINK[JID_LARM_3].child      = JID_LARM_4;
    uLINK[JID_LARM_3].mother     = JID_LARM_2;
    uLINK[JID_LARM_3].a          = Vector3f(0, 1, 0);
    uLINK[JID_LARM_3].c          = Vector3f(0, 0, 0);
    uLINK[JID_LARM_3].b          = Vector3f(ElbowRelUpperarm_X, ElbowRelUpperarm_Y, ElbowRelUpperarm_Z)
                                   - uLINK[JID_LARM_3].c + uLINK[JID_LARM_2].c;
    uLINK[JID_LARM_3].q          = 0;
	uLINK[JID_LARM_3].m        = ElbowMass;

    uLINK[JID_LARM_4].name      = "laj4";
    uLINK[JID_LARM_4].eff_name  = "lae4";
    uLINK[JID_LARM_4].sister     = 0;
    uLINK[JID_LARM_4].child      = 0;
    uLINK[JID_LARM_4].mother     = JID_LARM_3;
    uLINK[JID_LARM_4].a          = Vector3f(0, 0, 1);
    uLINK[JID_LARM_4].c          = Vector3f(LowerarmElbow_X, LowerarmElbow_Y, LowerarmElbow_Z);
    uLINK[JID_LARM_4].b          = Vector3f(LowerarmElbow_X, LowerarmElbow_Y, LowerarmElbow_Z)
                                   - uLINK[JID_LARM_4].c + uLINK[JID_LARM_3].c;
    uLINK[JID_LARM_4].q          = 0;
	uLINK[JID_LARM_4].m        = LowerarmMass;

    uLINK[JID_RARM_1].name      = "raj1";
    uLINK[JID_RARM_1].eff_name  = "rae1";
    uLINK[JID_RARM_1].sister     = 0;
    uLINK[JID_RARM_1].child      = JID_RARM_2;
    uLINK[JID_RARM_1].mother     = JID_ROOT;
    uLINK[JID_RARM_1].a          = Vector3f(1, 0, 0);
    uLINK[JID_RARM_1].c          = Vector3f(0, 0, 0);
    uLINK[JID_RARM_1].b          = Vector3f(ShoulderRelTorso_X,ShoulderRelTorso_Y ,ShoulderRelTorso_Z )
                                   - uLINK[JID_RARM_1].c + uLINK[JID_ROOT].c;
    uLINK[JID_RARM_1].q          = 0;
	uLINK[JID_RARM_1].m        = ShoulderMass;

    uLINK[JID_RARM_2].name      = "raj2";
    uLINK[JID_RARM_2].eff_name  = "rae2";
    uLINK[JID_RARM_2].child      = JID_RARM_3;
    uLINK[JID_RARM_2].mother     = JID_RARM_1;
    uLINK[JID_RARM_2].a          = Vector3f(0, 0, 1);
    uLINK[JID_RARM_2].c          = Vector3f(UpperarmRelShoulder_X, UpperarmRelShoulder_Y, UpperarmRelShoulder_Z);
    uLINK[JID_RARM_2].b          = Vector3f(UpperarmRelShoulder_X, UpperarmRelShoulder_Y, UpperarmRelShoulder_Z)
                                   - uLINK[JID_RARM_2].c + uLINK[JID_RARM_1].c;
    uLINK[JID_RARM_2].q          = 0;
	uLINK[JID_RARM_2].m        = UpperarmMass;

    uLINK[JID_RARM_3].name      = "raj3";
    uLINK[JID_RARM_3].eff_name  = "rae3";
    uLINK[JID_RARM_3].sister     = 0;
    uLINK[JID_RARM_3].child      = JID_RARM_4;
    uLINK[JID_RARM_3].mother     = JID_RARM_2;
    uLINK[JID_RARM_3].a          = Vector3f(0, 1, 0);
    uLINK[JID_RARM_3].c          = Vector3f(0, 0, 0);
    uLINK[JID_RARM_3].b          = Vector3f(-ElbowRelUpperarm_X, ElbowRelUpperarm_Y, ElbowRelUpperarm_Z)
                                   - uLINK[JID_RARM_3].c + uLINK[JID_RARM_2].c;
    uLINK[JID_RARM_3].q          = 0;
	uLINK[JID_RARM_3].m        = ElbowMass;

    uLINK[JID_RARM_4].name      = "raj4";
    uLINK[JID_RARM_4].eff_name  = "rae4";
    uLINK[JID_RARM_4].sister     = 0;
    uLINK[JID_RARM_4].child      = 0;
    uLINK[JID_RARM_4].mother     = JID_RARM_3;
    uLINK[JID_RARM_4].a          = Vector3f(0, 0, 1);
    uLINK[JID_RARM_4].c          = Vector3f(LowerarmElbow_X, LowerarmElbow_Y, LowerarmElbow_Z);
    uLINK[JID_RARM_4].b          = Vector3f(LowerarmElbow_X, LowerarmElbow_Y, LowerarmElbow_Z)
                                   - uLINK[JID_RARM_4].c + uLINK[JID_RARM_3].c;
    uLINK[JID_RARM_4].q          = 0;
	uLINK[JID_RARM_4].m        = LowerarmMass;

    /** ------------------------------ arms end ----------------------- */

    ForwardKinematics(JID_ROOT);
//     cout <<"****************************PrintLink*************************"<< endl;
//     PrintLink(JID_ROOT);//test the link,by Neil.
	TMass = TotalMass(JID_ROOT);
	aLOG << "TotalMass : " << TMass << endl;
	TMC = CalcMC(JID_ROOT);
	aLOG << "TMC : " << TMC << endl;
	CalcCoM();
	aLOG << "CoM : " << CoM << endl;

    NAO_A               = uLINK[JID_RLEG_4].b.Length();
    //or (uLINK[JID_LLEG_4].p - uLINK[JID_LLEG_3].p).Length();
    NAO_B               = uLINK[JID_LLEG_5].b.Length();
    //or (uLINK[JID_LLEG_5].p - uLINK[JID_LLEG_4].p).Length();
    NAO_Dl              = uLINK[JID_LLEG_1].b; //or uLINK[JID_LLEG_3].p - uLINK[JID_ROOT].p;
    NAO_Dr              = uLINK[JID_RLEG_1].b; //or uLINK[JID_RLEG_3].p - uLINK[JID_ROOT].p;
    NAO_E               = - uLINK[JID_LLEG_6].c;

    //cout    <<  "NAO_A: " << uLINK[JID_RLEG_4].b.Length() << endl;
    aLOG    <<  "NAO_A: " << NAO_A << endl
    <<  "NAO_B: " << NAO_B << endl
    <<  "NAO_Dl: " << NAO_Dl<< endl
    <<  "NAO_Dr: " << NAO_Dr<< endl
    <<  "NAO_E: " << NAO_E <<endl;

    /**add for the limited vision by Neil*/
    uLinkForCamera[JID_ROOT].name         = "torso";
    uLinkForCamera[JID_ROOT].sister       = 0;
    uLinkForCamera[JID_ROOT].child        = JID_HEAD_2;
    uLinkForCamera[JID_ROOT].mother       = 0;
    uLinkForCamera[JID_ROOT].a            = Vector3f(0, 0, 0);
    uLinkForCamera[JID_ROOT].c            = Vector3f(0, 0, 0);
    uLinkForCamera[JID_ROOT].b            = Vector3f(0, 0, 0);
    uLinkForCamera[JID_ROOT].q            = 0;
    uLinkForCamera[JID_ROOT].p            = Vector3f(0, 0, 0);
    uLinkForCamera[JID_ROOT].R.Identity();

    /** head */
    uLinkForCamera[JID_HEAD_2].name      = "neck";
    uLinkForCamera[JID_HEAD_2].sister    = 0;
    uLinkForCamera[JID_HEAD_2].child     = JID_HEAD_1;
    uLinkForCamera[JID_HEAD_2].mother    = JID_ROOT;
    uLinkForCamera[JID_HEAD_2].a         = Vector3f(0, 0, 1);
    uLinkForCamera[JID_HEAD_2].c         = Vector3f(0, 0, 0);
    uLinkForCamera[JID_HEAD_2].b         = Vector3f(NeckRelTorso_X, NeckRelTorso_Y, NeckRelTorso_Z)
                                           - uLinkForCamera[JID_HEAD_2].c + uLinkForCamera[JID_ROOT].c;
    uLinkForCamera[JID_HEAD_2].q         = 0;

    uLinkForCamera[JID_HEAD_1].name      = "head";
    uLinkForCamera[JID_HEAD_1].sister    = 0;
    uLinkForCamera[JID_HEAD_1].child     = 0;
    uLinkForCamera[JID_HEAD_1].mother    = JID_HEAD_2;
    uLinkForCamera[JID_HEAD_1].a         = Vector3f(0, 1, 0);
    uLinkForCamera[JID_HEAD_1].c         = Vector3f(0, 0, 0.005);
    uLinkForCamera[JID_HEAD_1].b         = Vector3f(HeadRelNeck_X, HeadRelNeck_Y, HeadRelNeck_Z)
                                           - uLinkForCamera[JID_HEAD_1].c + uLinkForCamera[JID_HEAD_2].c;
    uLinkForCamera[JID_HEAD_1].q         = 0;

    ForwardKinematicsForCamera(JID_ROOT);



    /** cut a branch(sister) can make IK faster */
    // uLINK[JID_LLEG_1].sister = 0;
}

void Nao::SetupRobotPartInfo()
{
    // NOTE to be completed
    mRobotPartInfo[PART_BODY].name  = "torso";
    mRobotCameraInfo[PART_BODY].name = "torso";

    for (RobotPart part = GetRobotPartMin(); part <= GetRobotPartMax(); ++part)
    {
        mRobotPartInfo[part].p = uLINK[part].p + uLINK[part].R.Rotate(uLINK[part].c);
        mRobotPartInfo[part].R = uLINK[part].R;

        mRobotPartInfo[part].R.Pos() = mRobotPartInfo[part].p;
    }
    aLOG<<"head Matrix: " << endl
    << mRobotPartInfo[PART_HEAD].R.Right() << endl
    << mRobotPartInfo[PART_HEAD].R.Up() << endl
    << mRobotPartInfo[PART_HEAD].R.Forward() << endl
    << mRobotPartInfo[PART_HEAD].R.Pos() << endl;
    aLOG<<"body Matrix: " << endl
    << mRobotPartInfo[PART_BODY].R.Right() << endl
    << mRobotPartInfo[PART_BODY].R.Up() << endl
    << mRobotPartInfo[PART_BODY].R.Forward() << endl
    << mRobotPartInfo[PART_BODY].R.Pos() << endl;

    for (RobotPart part = PART_BODY; part <= PART_NECK; ++part)
    {
        mRobotCameraInfo[part].p = uLinkForCamera[part].p + uLinkForCamera[part].R.Rotate(uLinkForCamera[part].c);
        mRobotCameraInfo[part].R = uLinkForCamera[part].R;

        mRobotCameraInfo[part].R.Pos() = mRobotCameraInfo[part].p;
    }
    aLOG<<"Camera Matrix: " << endl
    << mRobotCameraInfo[PART_HEAD].R.Right() << endl
    << mRobotCameraInfo[PART_HEAD].R.Up() << endl
    << mRobotCameraInfo[PART_HEAD].R.Forward() << endl
    << mRobotCameraInfo[PART_HEAD].R.Pos() << endl;
    //cout << mRobotCameraInfo[PART_HEAD].R.Pos() << endl;
    aLOG<<"Body Matrix for camera: " << endl
    << mRobotCameraInfo[PART_BODY].R.Right() << endl
    << mRobotCameraInfo[PART_BODY].R.Up() << endl
    << mRobotCameraInfo[PART_BODY].R.Forward() << endl
    << mRobotCameraInfo[PART_BODY].R.Pos() << endl;
}

bool Nao::IK_leg(const Matrix& hip, float A, float B,
                 const Matrix& ankle, bool left)
{
	aLOG << "*****************************IK_leg_Test*************************" << endl;
	aLOG << "hip: " << '\n' << hip << '\n'
		 << "ankle: " << '\n' << ankle << '\n'
		 << "A: " << A << " B: " << B << " left: " << left << endl;  
    Matrix invHip = hip;
    invHip.InvertRotationMatrix();
    Matrix invAnkle = ankle;
    invAnkle.InvertRotationMatrix();
	aLOG << "invHip: " << '\n' << invHip << '\n' << "invAnkle: " << '\n' << invAnkle << endl;

    Vector3f r = invAnkle.Rotate(hip.Pos() - ankle.Pos());
    float C = r.Length();
	aLOG << "r: " << r << " C: " << C << endl;
    if (C > A + B )
    {
	C = A + B - 0.000001 ;
	//cerr << "C>A+B" << endl;
    } 
	aLOG << "C: " << C << endl;
    float cosKneePitch = (C * C - A * A - B * B) / (2.0f * A * B);
    float kneePitch = 0.0f;
    float default4 = atan(0.005/0.12);
	aLOG << "cosKneePitch: " << cosKneePitch << " kneePitch: " << kneePitch << " default4: " << default4 << endl;

    if (cosKneePitch >= 1.0f)
    {
        kneePitch = 0.0f;
        cerr << "cosKneePitch >= 1.0" << endl;
        return false;
    }
    else if (cosKneePitch <= -1.0f)
    {
        kneePitch = gPI;
        cerr << "cosKneePitch <= -1.0" << endl;
        return false;
    }
    else
        kneePitch = gArcCos(cosKneePitch);
	aLOG << "kneePitch: " << kneePitch << endl;

    float q6a = gArcSin((A / C) * gSin(gPI - kneePitch));
    float ankleRoll = -gArcTan2(r.x(), r.z());

    float anklePitch = gArcTan2(
                           r.y(),
                           gSign(r.z()) * gSqrt(r.x() * r.x() + r.z() * r.z())
                       ) + q6a;
	kneePitch = -(kneePitch - default4);
	aLOG << "q6a: " << q6a << " ankleRoll: " << ankleRoll << " anklePitch: " << anklePitch << " kneePitch: " << kneePitch << endl;

    Matrix R = invHip * ankle;

    R.RotateY(-ankleRoll);
    R.RotateX(-anklePitch);
    R.RotateX(-kneePitch);
	aLOG << "R: " << '\n' << R << endl;

    float hipPitch      = 0.0f;
    float hipRoll       = 0.0f;
    float hipYaw        = 0.0f;
    float cosHipYaw     = 0.0f;
    float sinHipYaw     = 0.0f;
    float cosHipRoll    = 0.0f;
    float sinHipRoll    = 0.0f;

    if (left)
    {
        hipPitch = gArcTan2(R(2,1) + R(0,1), R(2,2) + R(0,2));
		aLOG << "hipPitch: " << hipPitch << endl;
        hipPitch = hipPitch >  gHalfPI ? hipPitch - gPI :
                   hipPitch < -gHalfPI ? hipPitch + gPI : hipPitch;
		aLOG << "hipPitch: " << hipPitch << endl;

        float cosHipPitch   = gCos(hipPitch);
        float addCSHipRoll  = (R(2,2) + R(0,2)) / cosHipPitch;
		aLOG << "cosHipPitch: " << cosHipPitch << " addCSHipRoll: " << addCSHipRoll << endl;

        cosHipYaw = (R(0,0) - R(2,0)) / addCSHipRoll;
        sinHipYaw = R(1,0) / (-0.7071 * addCSHipRoll);
        hipYaw = gArcTan2(sinHipYaw, cosHipYaw);
		aLOG << "cosHipYaw: " << cosHipYaw << " sinHipYaw: " << sinHipYaw << endl;
        //hipYaw = -hipYaw; // special in left leg
        hipYaw = hipYaw >  gHalfPI ? hipYaw - gPI :
                 hipYaw < -gHalfPI ? hipYaw + gPI : hipYaw;
		aLOG << "hipYaw: " << hipYaw << endl;
        cosHipYaw = cos(hipYaw);

        float subCSHipRoll = (R(2,0) + R(0,0));
        cosHipRoll = addCSHipRoll + subCSHipRoll;
        sinHipRoll = addCSHipRoll - subCSHipRoll;
        hipRoll = gArcTan2(sinHipRoll, cosHipRoll);
		aLOG << "subCSHipRoll: " << subCSHipRoll << " cosHipRoll: " << cosHipRoll << " hipRoll: " << hipRoll << endl;
        hipRoll = hipRoll >  gHalfPI ? hipRoll - gPI :
                  hipRoll < -gHalfPI ? hipRoll + gPI : hipRoll;
		aLOG << "hipRoll: " << hipRoll << endl;
    }
    else
    {
        hipPitch = gArcTan2(R(2,1) - R(0,1), R(2,2) - R(0,2));
		aLOG << "hipPitch: " << hipPitch << endl;
        hipPitch = hipPitch >  gHalfPI ? hipPitch - gPI :
                   hipPitch < -gHalfPI ? hipPitch + gPI : hipPitch;
		aLOG << "hipPitch: " << hipPitch << endl;
		
        float cosHipPitch   = gCos(hipPitch);
        float subCSHipRoll  = (R(2,2) - R(0,2)) / cosHipPitch;

        cosHipYaw = (R(2,0) + R(0,0)) / subCSHipRoll;
        sinHipYaw = R(1,0) / (0.7071 * subCSHipRoll);
        hipYaw = gArcTan2(sinHipYaw, cosHipYaw);
		aLOG << "cosHipPitch: " << cosHipPitch << " subCSHipRoll: " << subCSHipRoll
			 << " cosHipYaw: " << cosHipYaw << " sinHipYaw: " << sinHipYaw << " hipYaw: " << hipYaw << endl;
        hipYaw = hipYaw >  gHalfPI ? hipYaw - gPI :
                 hipYaw < -gHalfPI ? hipYaw + gPI : hipYaw;
		aLOG << "hipYaw: " << hipYaw << endl;
        cosHipYaw = cos(hipYaw);

        float addCSHipRoll = (R(0,0) - R(2,0));
        cosHipRoll = addCSHipRoll + subCSHipRoll;
        sinHipRoll = addCSHipRoll - subCSHipRoll;
        hipRoll = gArcTan2(sinHipRoll, cosHipRoll);
		aLOG << "addCSHipRoll: " << addCSHipRoll << " cosHipRoll: " << cosHipRoll 
			 << " sinHipRoll: " << sinHipRoll << " hipRoll: " << hipRoll << endl;
        hipRoll = hipRoll >  gHalfPI ? hipRoll - gPI :
                  hipRoll < -gHalfPI ? hipRoll + gPI : hipRoll;
		aLOG <<"hipRoll: " << hipRoll << endl;
    }

    if (left)
    {
        mIKJointAngle[Nao::JID_LLEG_1]  = hipYaw;
        mIKJointAngle[Nao::JID_LLEG_2]  = hipRoll;
        mIKJointAngle[Nao::JID_LLEG_3]  = hipPitch;
        mIKJointAngle[Nao::JID_LLEG_4]  = kneePitch;
        mIKJointAngle[Nao::JID_LLEG_5]  = anklePitch;
        mIKJointAngle[Nao::JID_LLEG_6]  = ankleRoll;
		aLOG << "left: " << hipYaw << ' ' << hipRoll << ' ' << hipPitch << ' '
			 << kneePitch << ' ' << anklePitch << ' ' << anklePitch << endl;
    }
    else
    {
        mIKJointAngle[Nao::JID_RLEG_1] = hipYaw;
        mIKJointAngle[Nao::JID_RLEG_2] = hipRoll;
        mIKJointAngle[Nao::JID_RLEG_3] = hipPitch;
        mIKJointAngle[Nao::JID_RLEG_4] = kneePitch;
        mIKJointAngle[Nao::JID_RLEG_5] = anklePitch;
        mIKJointAngle[Nao::JID_RLEG_6] = ankleRoll;
		aLOG << "right: " << hipYaw << ' ' << hipRoll << ' ' << hipPitch << ' '
			 << kneePitch << ' ' << anklePitch << ' ' << anklePitch << endl;
    }

    return true;
}

bool Nao::IK_leg(const salt::Matrix& torso, const salt::Matrix& foot, bool left)
{
    uLINK[JID_ROOT].R = torso;
    uLINK[JID_ROOT].p = torso.Pos();

    Vector3f D      = left ? NAO_Dl : NAO_Dr;
    JointID to ;     //= left ? JID_LLEG_6 : JID_RLEG_6;
    if (left) to = JID_LLEG_6 ;
    else to = JID_RLEG_6;
    Matrix hip      = torso;
    hip.Pos()       = torso * D;
    Matrix ankle    = foot;
    ankle.Pos()     = foot * NAO_E;

    if (! IK_leg(hip, NAO_A, NAO_B, ankle, left))
    {
        cerr << "analytic method failed: leg -- "
        << left << endl;
        return false;
    }

    if (left)
    {
        uLINK[Nao::JID_LLEG_1].q = mIKJointAngle[Nao::JID_LLEG_1];
        uLINK[Nao::JID_LLEG_2].q = mIKJointAngle[Nao::JID_LLEG_2];
        uLINK[Nao::JID_LLEG_3].q = mIKJointAngle[Nao::JID_LLEG_3];
        uLINK[Nao::JID_LLEG_4].q = mIKJointAngle[Nao::JID_LLEG_4];
        uLINK[Nao::JID_LLEG_5].q = mIKJointAngle[Nao::JID_LLEG_5];
        uLINK[Nao::JID_LLEG_6].q = mIKJointAngle[Nao::JID_LLEG_6];
    }
    else
    {
        uLINK[Nao::JID_RLEG_1].q = mIKJointAngle[Nao::JID_RLEG_1];
        uLINK[Nao::JID_RLEG_2].q = mIKJointAngle[Nao::JID_RLEG_2];
        uLINK[Nao::JID_RLEG_3].q = mIKJointAngle[Nao::JID_RLEG_3];
        uLINK[Nao::JID_RLEG_4].q = mIKJointAngle[Nao::JID_RLEG_4];
        uLINK[Nao::JID_RLEG_5].q = mIKJointAngle[Nao::JID_RLEG_5];
        uLINK[Nao::JID_RLEG_6].q = mIKJointAngle[Nao::JID_RLEG_6];
    }

    ForwardKinematics(JID_ROOT); // apply the joint angles before InverseKinematics
    if (! InverseKinematics(to, ankle))
    {
        cerr << "numerical method failed: leg -- "
        << left << endl;
        return false;
    }

    if (left)
    {
        mIKJointAngle[Nao::JID_LLEG_1] = uLINK[Nao::JID_LLEG_1].q;
        mIKJointAngle[Nao::JID_LLEG_2] = uLINK[Nao::JID_LLEG_2].q;
        mIKJointAngle[Nao::JID_LLEG_3] = uLINK[Nao::JID_LLEG_3].q;
        mIKJointAngle[Nao::JID_LLEG_4] = uLINK[Nao::JID_LLEG_4].q;
        mIKJointAngle[Nao::JID_LLEG_5] = uLINK[Nao::JID_LLEG_5].q;
        mIKJointAngle[Nao::JID_LLEG_6] = uLINK[Nao::JID_LLEG_6].q;
    }
    else
    {
        mIKJointAngle[Nao::JID_RLEG_1] = uLINK[Nao::JID_RLEG_1].q;
        mIKJointAngle[Nao::JID_RLEG_2] = uLINK[Nao::JID_RLEG_2].q;
        mIKJointAngle[Nao::JID_RLEG_3] = uLINK[Nao::JID_RLEG_3].q;
        mIKJointAngle[Nao::JID_RLEG_4] = uLINK[Nao::JID_RLEG_4].q;
        mIKJointAngle[Nao::JID_RLEG_5] = uLINK[Nao::JID_RLEG_5].q;
        mIKJointAngle[Nao::JID_RLEG_6] = uLINK[Nao::JID_RLEG_6].q;
    }
    return true;
}

bool Nao::IK_leg(const Matrix& torso, const Matrix& leftfoot, const Matrix& rightfoot)
{
	uLINK[JID_ROOT].R = torso;
    uLINK[JID_ROOT].p = torso.Pos();

	Matrix hipLeft = torso;
	hipLeft.Pos() = torso * NAO_Dl;
	Matrix hipRight = torso;
	hipRight.Pos() = torso * NAO_Dr;
	Matrix ankleLeft = leftfoot;
	ankleLeft.Pos() = leftfoot * NAO_E;
	Matrix ankleRight = rightfoot;
	ankleRight.Pos() = rightfoot * NAO_E;

// 	aLOG << "****************************Test IK_Leg***************************" << endl;
// 	aLOG << torso.Right() << '\n' << torso.Up() << '\n' << torso.Forward() << '\n' << torso.Pos() << '\n' << endl;
// 	aLOG << hipLeft.Right() << '\n' << hipLeft.Up() << '\n' << hipLeft.Forward() << '\n' << hipLeft.Pos() << '\n' << endl;
// 	aLOG << hipRight.Right() << '\n' << hipRight.Up() << '\n' << hipRight.Forward() << '\n' << hipRight.Pos() << '\n' << endl;
// 	aLOG << ankleLeft.Right() << '\n' << ankleLeft.Up() << '\n' << ankleLeft.Forward() << '\n' << ankleLeft.Pos() << '\n' << endl;
// 	aLOG << ankleRight.Right() << '\n' << ankleRight.Up() << '\n' << ankleRight.Forward() << '\n' << ankleRight.Pos() << '\n' << endl;
// 	aLOG << "NAO_A: " << NAO_A << " NAO_B: " << NAO_B << endl;
// 	aLOG << "****************************Test IK_Leg End***********************" << endl; 
	IK_leg(hipLeft, NAO_A, NAO_B, ankleLeft, true);
	IK_leg(hipRight, NAO_A, NAO_B, ankleRight, false);
	
}

void Nao::CalcLegJointVel(const salt::Vector3f& vb, const salt::Vector3f& wb,
                          const salt::Vector3f& vt, const salt::Vector3f& wt,
                          bool left)
{
    int to     ;// = left ? JID_LLEG_6 : JID_RLEG_6;
    if (left) to = JID_LLEG_6 ;
    else to = JID_RLEG_6;
    float* vel  = mIKJointAngle.get();
    TIndex idx  = FindRoute(to);
    int jsize   = idx.size();

    Vector3f vd, vw;
    vd = vt - vb - wb.Cross(uLINK[to].p - uLINK[1].p);
    vw = wt - wb;

    float v[6];
    v[0] = vd.x();
    v[1] = vd.y();
    v[2] = vd.z();
    v[3] = vw.x();
    v[4] = vw.y();
    v[5] = vw.z();

    // allot memory for Jacobian matrix
    float** J = new float*[6];
    for (int i = 0; i < 6; ++i)
    {
        J[i] = new float[jsize];
    }

    CalcJacobian(J, idx);
    Solve(vel, (const float **)J, (const float *)v, 6, jsize);

    for (int i = 0; i < 6; ++i)
    {
        delete [] J[i];
        J[i] = NULL;
    }
    delete [] J;
    J = NULL;
}
