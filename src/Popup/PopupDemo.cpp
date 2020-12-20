#include "PopupDemo.h"
#include "ui_PopupDemo.h"

#include "RBasicComboContainer.h"
#include "RMessageDialog.h"
#include <QDebug>

PopupDemo::PopupDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupDemo)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    initBasicComboBox();
    initMessageDialog();
}

PopupDemo::~PopupDemo()
{
    delete ui;
}

void PopupDemo::initBasicComboBox()
{
    ui->basicComboBoxA->getPopup()->setContainer(new RBasicComboContainer(ui->basicComboBoxA->getPopup()));
    ui->basicComboBoxB->getPopup()->setContainer(new RBasicComboContainer(ui->basicComboBoxB->getPopup()));

    ui->basicComboBoxA->setItems({"aaa","bbb","ccc","ddd","eee","fff","ggg",
                                  "fff","iii","jjj","kkk","lll","mmm","nnn"});
    connect(ui->basicComboBoxA,&RBasicComboBox::currentIndexChanged,[](int index){
        qDebug() << "RBasicComboBox::currentIndexChanged" << index;
    });

    connect(ui->basicComboBoxA,&RBasicComboBox::currentTextChanged,[](const QString &text){
        qDebug() << "RBasicComboBox::currentTextChanged" <<text;
    });

    ui->basicComboBoxB->setItems({"aaa","bbb","ccc"});
}

void PopupDemo::initMessageDialog()
{
    connect(ui->btnMessage, &QPushButton::clicked, []{
        // label 可以自定义 html 文本
        const int result = RMessageDialog::showMessage("测试提示框", "内容可以自定义\n目前只放了个Label");
        qDebug() << "RMessageDialog resuLt" << result << (result == RBasicDialog::Ok);
    });
}
