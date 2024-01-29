#ifndef THRESHOLDSETPOPUPWIDGET_H
#define THRESHOLDSETPOPUPWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <any>

class ThresholdsetPopUpWidget : public QWidget
{
    Q_OBJECT

public:
    ThresholdsetPopUpWidget(QWidget* parent, const QString& viewName = "ThresholdsetPopUpWidget");
    virtual ~ThresholdsetPopUpWidget();

public:
    bool createView();

    void destroyView();

protected:
    void initUI();
    void connectSlots();

private slots:                               
    void gitPathImprotButtonClicked();
    void gitPathExportButtonClicked();

private:
    QWidget* improtPopUpWidget{ nullptr };
    QWidget* exportPopUpWidget{ nullptr };
    QGridLayout* improtPopUpLayout_{ nullptr };
    QGridLayout* exportPopUpLayout_{ nullptr };
    QGridLayout* mainlayout_{ nullptr };

private:
    QLabel* improtfile_;
    QLabel* exportfile_;

private:
    QPushButton* gitPathImprotButton_;
    QPushButton* gitPathExportButton_;
    QPushButton* improtOKButton_;
    QPushButton* exportOKButton_;

private:
    QLineEdit* gitPathImprotLineEdit_;
    QLineEdit* gitPathExportLineEdit_;

public:
    QString compName_;                                       // 组件名字
};

#endif // THRESHOLDSETPOPUPWIDGET_H