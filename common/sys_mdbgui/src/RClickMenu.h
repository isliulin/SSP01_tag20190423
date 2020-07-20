#ifndef _RCLICKMENU_H 
#define _RCLICKMENU_H
#include <QtGui/QMenu>
//#define FUN()
class Mmenu:public QMenu
{
public:
	enum menuType
	{
		NOITEM,//�����ڿհ״�
		FATHERITEM,//�����ڸ��ڵ�
		CHILDITEM,//�������ӽڵ�
	};
	Mmenu(QWidget * parent = 0){}
	~Mmenu(void){}
	virtual void exec(QPoint pos){}
	virtual void regist(QObject * tar){}
	virtual	void setType(menuType mt){}
	QMenu * set(QMenu*qm){pMenu = qm;return pMenu;}
	QMenu * get(){return pMenu;};
public:
	QAction *aConnect;//����
	QAction *aDelDb;//ɾ������
	QAction *aDelTable;//ɾ�����
	QAction *aRegist;//ע���¼�
	QAction *aDisRegist;//ɾ��ע��
	QAction *aRegistAll;//ȫ��ע��
	QAction *aDisRegistAll;//ɾ��ȫ��ע��
private:
	QMenu * pMenu;
};
class connectMenu:public Mmenu
{
	public:
	connectMenu(QWidget * parent,menuType mt);
	~connectMenu();
	void exec(QPoint pos);
	void regist(QObject * tar);
	void setType(menuType mt);
private:
};
class topMenu:public Mmenu
{
public:
	topMenu(QWidget * parent = 0);
	~topMenu();
	void exec(QPoint pos);
	void regist(QObject * tar);
private:

	QAction *aConnect;
	QAction *aDel;
};
#endif
