/*
* ���� �� ������������, ����� ��� ������������� �����, ��� �������
*/

#ifndef IBASEINTERFACE_H
#define IBASEINTERFACE_H


// ������� ���������
class IBaseInterface {
	IBaseInterface(const IBaseInterface&) = delete;            // ������ �����������
	IBaseInterface& operator=(const IBaseInterface&) = delete; // ������ ������������
protected:
	IBaseInterface() = default;
	virtual ~IBaseInterface();
public:
	virtual void Delete();  // ������� ������
};



void IBaseInterface::Delete() {  }
IBaseInterface::~IBaseInterface() { Delete(); }


#endif // !IBASEINTERFACE_H		    

