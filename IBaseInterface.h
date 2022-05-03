/*
* Пока не используется, нужно или переосмыслить класс, или удалить
*/

#ifndef IBASEINTERFACE_H
#define IBASEINTERFACE_H


// Базовый интерфейс
class IBaseInterface {
	IBaseInterface(const IBaseInterface&) = delete;            // запрет копирования
	IBaseInterface& operator=(const IBaseInterface&) = delete; // запрет присваивания
protected:
	IBaseInterface() = default;
	virtual ~IBaseInterface();
public:
	virtual void Delete();  // удаляет объект
};



void IBaseInterface::Delete() {  }
IBaseInterface::~IBaseInterface() { Delete(); }


#endif // !IBASEINTERFACE_H		    

