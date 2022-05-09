#ifndef ICOLLISIONENGINE_H
#define ICOLLISIONENGINE_H


// Можно создать интерфейс ICanCollide и реализации объектов наследовать от него
  // в классе будут методы вроде .getboundingbox 
class IShape;

class ICollisionEngine	
{
public:
	virtual bool isPossibleToCollide(IShape& a, IShape& b) = 0;
	//virtual void DrawCollisionBox();

protected:
private:

};


#endif // ICOLLISION_H