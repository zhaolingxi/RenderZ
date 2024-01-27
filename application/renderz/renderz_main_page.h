#pragma once
#include <QVector>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <unordered_set>
#include <QGridLayout>
#include<QPushButton>

class RenderZMainPage : public QWidget
{
	Q_OBJECT
public:
	RenderZMainPage(QWidget* parent, const QString& viewName = "RenderZMainPage");
	virtual ~RenderZMainPage();

	bool createPage();
	void initUI();


protected:
	void addPushBtm(QHBoxLayout*& ioLayout);

private:
	QGridLayout* mainLayout_{ nullptr }; /**< ҳ�������� */
	QWidget* mainSideBarLeft_{ nullptr }; /**< ����� */
	QWidget* mainSideBarRight_{ nullptr }; /**< ����� */
	QVBoxLayout* pageViewLayout_{ nullptr }; /**< ��ʾҳ����ͼ�Ĳ���(top|mid|btm) */
	QHBoxLayout* pageViewLayoutTop_{ nullptr };
	QHBoxLayout* pageViewLayoutMid_{ nullptr };
	QHBoxLayout* pageViewLayoutBtm_{ nullptr };

};
