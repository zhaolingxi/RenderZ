#pragma once
#include <QVector>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <unordered_set>
#include <QGridLayout>
#include<QPushButton>
#include <QPainter>
#include <QStyleOption>
#include"zqtgui.h"
#include"logmonitor_page_sdk.h"

class RenderZMainPage : public QFrame
{
	Q_OBJECT
public:
	RenderZMainPage(QWidget* parent, const QString& viewName = "RenderZMainPage");
	virtual ~RenderZMainPage();

	bool createPage();
	bool createLayoutTop(QHBoxLayout* &pageViewLayoutTop);
	bool createLayoutMid(QHBoxLayout*& pageViewLayoutMid);
	bool createLayoutBtm(QHBoxLayout*& pageViewLayoutBtm);
	void initUI();
	void tempLoadTheme(QApplication* app=nullptr);


protected:
	std::shared_ptr<QString> readQssFiles(const QString& dirPath);

private:
	QGridLayout* mainLayout_{ nullptr }; /**< ҳ�������� */
	QWidget* mainSideBarLeft_{ nullptr }; /**< ����� */
	ZQtViewer* mianViewer_{ nullptr };   /*��viewer*/
	QWidget* mainSideBarRight_{ nullptr }; /**< ����� */
	QVBoxLayout* pageViewLayout_{ nullptr }; /**< ��ʾҳ����ͼ�Ĳ���(top|mid|btm) */


	QHBoxLayout* pageViewLayoutTop_{ nullptr };
	QHBoxLayout* pageViewLayoutMid_{ nullptr };
	QHBoxLayout* pageViewLayoutBtm_{ nullptr };

};