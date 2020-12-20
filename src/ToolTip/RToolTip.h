#ifndef RTOOLTIP_H
#define RTOOLTIP_H

#include <QLabel>
#include <QBasicTimer>

/**
 * @brief 最简易的 ToolTip 提示框
 * @author 龚建波
 * @date 2020-6-7
 * @note 这是顶层窗口不要设置 parent
 * @details 顶层设置透明后，样式表失效了，所以我在里面套了一层 label
 * 本来想外层也用 QLabel，show 时内层 label 把属性设置为外层的，感觉没必要
 */
class RToolTip : public QWidget
{
    Q_OBJECT
    // READ WRITE 可以替换成 MEMBER
    // 默认显示为 point 的左上角，通过属性设置偏移，以右下角为起点，左减右加，上减下加
    // qss 右移 1px: qproperty-rightOffset:1;
    Q_PROPERTY(int rightOffset READ getRightOffset WRITE setRightOffset)
    // qss 上移 1px: qproperty-bottomOffset:"-1";
    Q_PROPERTY(int bottomOffset READ getBottomOffset WRITE setBottomOffset)
    Q_PROPERTY(QString text READ getText WRITE setText)
    Q_PROPERTY(Qt::Alignment alignment READ getAlignment WRITE setAlignment)
public:
    // 独立的窗口不设置 parent，样式表可用 qApp 设置
    explicit RToolTip(const QString &objectName = QString(), const QString text = QString());
    ~RToolTip();

    // 右侧偏移
    int getRightOffset();
    void setRightOffset(int offset);
    // 底部偏移
    int getBottomOffset();
    void setBottomOffset(int offset);

    // 文本
    QString getText() const;
    void setText(const QString &text);
    // 对齐方式
    Qt::Alignment getAlignment() const;
    void setAlignment(Qt::Alignment alignment);

    // 设置锚定窗口，鼠标放上去时显示 tooltip
    void anchorTarget(QWidget *target);
    // 获取内层 label 对象
    const QLabel *label() const;
    // 显示 tip 在 widget 的左上角
    void showTip(const QWidget *obj, int second);
    // 显示 tip 在点的左上角
    void showTip(const QPoint &rightBottom, int second);
    // 隐藏tip
    void hideTip();
    void hideTipLater();

protected:
    // 过滤锚定窗口的 enter 和 leave 事件
    bool eventFilter(QObject *target, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showSeconds();

private:
    // 外层设置背景透明后样式不生效，所以嵌套了一层
    QLabel *contentLabel;
    // 默认显示为 point 的左上角，通过属性设置偏移
    // 以右下角为起点，左减右加，上减下加
    int rightOffset = 0;
    int bottomOffset = 0;
    int count = 0;

    // 锚定的窗口
    QWidget *targetWidget = nullptr;
    QPoint targetPoint;
    // show 和 hide延迟
    QBasicTimer showTimer;
    QBasicTimer hideTimer;
    QTimer* timer;
};

#endif // RTOOLTIP_H
