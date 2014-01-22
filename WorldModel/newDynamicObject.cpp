#include "newDynamicObject.h"
#include "worldmodel.h"

NDynamicObject::NDynamicObject()
{
cycles_out_of_sight =0;
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
     cout<<"************** over ***********************"<<endl;
  }
  
  
void NDynamicObject::changeState()
{
  if(cycles_out_of_sight > 10)
    missed = true;
}

void NDynamicObject::add_cycle()
{
  cycles_out_of_sight ++;
}
