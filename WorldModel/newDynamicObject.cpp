#include "newDynamicObject.h"
#include "worldmodel.h"

NDynamicObject::NDynamicObject()
{

}

NDynamicObject::~NDynamicObject()
{

}

  void NDynamicObject::updateFromVision(VisionSense vs)
  {
    setDistance(vs.distanceToSelf);
    setLocalPos(vs.localPos);
    if (WM.CanLocalize())
      setGlobalPos(WM.getVisionPerceptorMatrix()*pos_local);
  }
  
    void NDynamicObject::printVisionModel()
  {
     cout<<"************** NEW BALL ***********************"<<endl;
     cout<<"G_POS:"<<pos<<endl;
     cout<<"L_POS:"<<pos_local<<endl;
     cout<<"DIST: "<<distance<<endl;
  }