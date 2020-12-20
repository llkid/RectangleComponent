#include "RBasicComboContainer.h"

#include <QVBoxLayout>

RBasicComboContainer::RBasicComboContainer(bool init, QWidget *parent)
    : QWidget(parent)
{
    init ? initDefault() : VOID_OBJ;
}

int RBasicComboContainer::getCurrentIndex() const
{
    return defaultView ? defaultView->getCurrentRow() : -1;
}

void RBasicComboContainer::setCurrentIndex(int index)
{
    return defaultView ? defaultView->setCurrentRow(index) : VOID_OBJ;
}

QList<QString> RBasicComboContainer::getItems() const
{
    return (defaultView && defaultView->getBasicModel())
            ? defaultView->getBasicModel()->getModelData()
            : QList<QString>();
}

void RBasicComboContainer::setItems(const QList<QString> &items)
{
    return (defaultView && defaultView->getBasicModel())
            ? defaultView->getBasicModel()->setModelData(items)
            : VOID_OBJ;
}

int RBasicComboContainer::checkTextRow(const QString &text)
{
    return defaultView ? defaultView->checkTextRow(text) : -1;
}

QString RBasicComboContainer::getCurrentText() const
{
    return defaultView ? defaultView->getCurrentText() : QString();
}

QString RBasicComboContainer::getPrevText()
{
    return defaultView ? defaultView->getPrevText() : QString();
}

QString RBasicComboContainer::getNextText()
{
    return defaultView ? defaultView->getNextText() : QString();
}

int RBasicComboContainer::getContentsHeight() const
{
    return defaultView ? defaultView->getContentsHeight() : 0;
}

void RBasicComboContainer::initDefault()
{
    defaultView = new RBasicComboView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(1);
    layout->addWidget(defaultView);
    //
    connect(defaultView, &RBasicComboView::currentRowChanged,
            this, &RBasicComboContainer::currentIndexChanged);
    connect(defaultView, &RBasicComboView::rowClicked,
            this, &RBasicComboContainer::updateData);
    connect(defaultView, &RBasicComboView::modelReseted,
            this, &RBasicComboContainer::updateData);
}

