#include "RToolTip.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QTimerEvent>
#include <QResizeEvent>
//#include <QStyle>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>

RToolTip::RToolTip(const QString &objectName, const QString text)
    : QWidget(nullptr)
    , contentLabel(new QLabel)
{
    //把内层label添加到透明widget
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(contentLabel);

    contentLabel->setAlignment(Qt::AlignCenter);
    contentLabel->setText(text);

    setObjectName(objectName);

    //把widget设置为透明，样式表设置给label
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose);
    //默认hide
    setVisible(false);

    qDebug() << "new tooltip" << objectName;
}

RToolTip::~RToolTip()
{
    qDebug() << "delete tooltip" << objectName();
}

int RToolTip::getRightOffset()
{
    return rightOffset;
}

void RToolTip::setRightOffset(int offset)
{
    if (rightOffset != offset) {
        rightOffset = offset;
        // 没有动态样式的处理，可自行添加
//        style()->unpolish(this);
//        style()->polish(this);
    }
}

int RToolTip::getBottomOffset()
{
    return bottomOffset;
}

void RToolTip::setBottomOffset(int offset)
{
    bottomOffset = (bottomOffset != offset)
            ? offset
            : bottomOffset;
}

QString RToolTip::getText() const
{
    return contentLabel->text();
}

void RToolTip::setText(const QString &text)
{
    contentLabel->setText(text);
}

Qt::Alignment RToolTip::getAlignment() const
{
    return contentLabel->alignment();
}

void RToolTip::setAlignment(Qt::Alignment alignment)
{
    contentLabel->setAlignment(alignment);
}

void RToolTip::anchorTarget(QWidget *target)
{
    if (target && target != targetWidget) {
        if (targetWidget) {
            targetWidget->removeEventFilter(this);
        }
        targetWidget = target;
        targetWidget->installEventFilter(this);
        targetWidget->setMouseTracking(true);
        // 如果是随窗口关闭的，看不到析构的打印，难道此时事件循环已停止？
//        connect(targetWidget, &QObject::destroyed, this, &QObject::deleteLater);
        connect(targetWidget, &QObject::destroyed, [this]{
//            qDebug() << "delete later...";
            this->deleteLater();
        });
    }
}

const QLabel *RToolTip::label() const
{
    return contentLabel;
}

void RToolTip::showTip(const QWidget *obj, int second)
{
    return obj ? showTip(obj->mapToGlobal(QPoint(0, 0)), second) : void();
}

void RToolTip::showTip(const QPoint &rightBottom, int second)
{
    targetPoint = rightBottom;
    //move(rightBottom.x() - width() + rightOffset,
    //     rightBottom.y() - height() + bottomOffset);
    // 直接用 size + point 得到的位置可能显示不全，这里计算下

    int rect_left = rightBottom.x() - width() + rightOffset;
    int rect_top = rightBottom.y() - height() + bottomOffset;

    rect_left = rect_left < 0 ? 0 : rect_left;
    rect_top = rect_top < 0 ? 0 : rect_top;
//    qDebug() << "1-> rect_left: " << rect_left << ";rect_top: " << rect_top;
    // 要考虑多个显示器情况，【待测试】
    // 根据当前所在屏幕尺寸计算
    QDesktopWidget * desktop = QApplication::desktop();
    if (desktop) {
        QRect desk_rect = desktop->screenGeometry(targetWidget ? targetWidget : this);

        rect_left = (rect_left + width() > desk_rect.width())
                ? desk_rect.width() - width()
                : rect_left;

        rect_top = (rect_top + height() > desk_rect.height())
                ? desk_rect.height() - height()
                : rect_top;
    }
//    qDebug() << "2-> rect_left: " << rect_left << ";rect_top: " << rect_top;

    move(rect_left, rect_top);
//    !showTimer.isActive() ? showTimer.start(200, this) : void();
    count = second;
    timer = !timer ? new QTimer(this) : timer;
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &RToolTip::showSeconds);
    timer->start();
    show();

}

void RToolTip::hideTip()
{
    !hideTimer.isActive() ? hideTimer.start(300,this) : void();
}

void RToolTip::hideTipLater()
{
    if (timer && timer->isActive()) {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
        !isHidden() ? hide() : void();
    }
}

bool RToolTip::eventFilter(QObject *target, QEvent *event)
{
    if (target == targetWidget) {
        switch (event->type()) {
        case QEvent::Enter:
            //showTip(QCursor::pos());
//            qDebug() << "鼠标进入...";
            showTip(targetWidget, 5);
            break;
        case QEvent::Leave:
//            qDebug() << "鼠标离开...";
//            hideTip();
            hideTipLater();
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(target, event);
}

void RToolTip::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == showTimer.timerId()) {
        showTimer.stop();
        //hideTimer.stop();
        (!hideTimer.isActive() && isHidden()) ? show() : void();
    } else if (event->timerId() == hideTimer.timerId()) {
        showTimer.stop();
        hideTimer.stop();
        !isHidden() ? hide() : void();
    } else {
        QWidget::timerEvent(event);
    }
}

void RToolTip::resizeEvent(QResizeEvent *event)
{
    // 初次 show 的时候可能 size 可能还没计算好
    showTip(targetPoint, 5);
    QWidget::resizeEvent(event);
}

void RToolTip::showSeconds()
{
    count-- == 0
            ? hideTipLater()
            : (isHidden() && timer->isActive()) ? show() : void();
}
