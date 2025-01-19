#pragma once

#include"zqtgui.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>

class ZQTGUI_API SideSettingView : public QWidget {
    Q_OBJECT

public:
    explicit SideSettingView(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        // »¬¿é
        QLabel* sliderLabel = new QLabel("Slider:", this);
        QSlider* slider = new QSlider(Qt::Horizontal, this);
        layout->addWidget(sliderLabel);
        layout->addWidget(slider);

        // ±à¼­Æ÷
        QLabel* editorLabel = new QLabel("Editor:", this);
        QLineEdit* editor = new QLineEdit(this);
        layout->addWidget(editorLabel);
        layout->addWidget(editor);

        // µ¥Ñ¡¿ò
        QLabel* radioLabel = new QLabel("Radio Button:", this);
        QRadioButton* radioButton = new QRadioButton("Option 1", this);
        layout->addWidget(radioLabel);
        layout->addWidget(radioButton);

        // ¶àÑ¡¿ò
        QLabel* checkBoxLabel = new QLabel("Check Box:", this);
        QCheckBox* checkBox1 = new QCheckBox("Option 1", this);
        QCheckBox* checkBox2 = new QCheckBox("Option 2", this);
        layout->addWidget(checkBoxLabel);
        layout->addWidget(checkBox1);
        layout->addWidget(checkBox2);

        setLayout(layout);
        setMaximumWidth(400);
    }
};
