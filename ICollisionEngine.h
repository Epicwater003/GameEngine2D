#ifndef ICOLLISIONENGINE_H
#define ICOLLISIONENGINE_H


// ����� ������� ��������� ICanCollide � ���������� �������� ����������� �� ����
  // � ������ ����� ������ ����� .getboundingbox 
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