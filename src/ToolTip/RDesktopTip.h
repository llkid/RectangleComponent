#ifndef RDESKTOPTIP_H
#define RDESKTOPTIP_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>

namespace Ui {
class RDesktopTip;
}

/**
 * @brief 桌面右下角弹框
 * @author 龚建波
 * @date 2020-6-21
 * @note 可以增加 show 的 parent 参数，使模态有父子关系
 * @details 只有单个实例对象显示弹框，
 * 后面设置的信息会覆盖之前的。
 * 也可以修改为每次show单独创建一个实例对象
 */
class RDesktopTip : public QWidget
{
    Q_OBJECT
private:
    explicit RDesktopTip();

public:
    // 动画模式枚举
    enum AnimationMode {
        // 无动画
        NoAnimation = 0x00,
        // 仅透明度动画
        OpacityAnimation = 0x01,
        // 仅位置动画
        PosAnimation = 0x02,
        // 全部动画
        // OpacityAnimation | PosAnimation
        AllAnimation = 0xFF
    };
public:
    ~RDesktopTip();

    // 显示弹框-已显示动画重新开始, timeout <= 0 不会定时消失
    static void showTip(const QStringList &texts,int timeout=0);
    // 显示弹框-已显示不重复动画
    static void keepTip(const QStringList &texts);
    // 隐藏弹框
    static void hideTip();
    // 设置动画模式
    static RDesktopTip::AnimationMode getMode();
    static void setMode(RDesktopTip::AnimationMode newMode);

private:
    // 初始化动画设置
    void initAnimation();
    // 初始化定时器设置
    void initTimer();
    // 准备定时器
    void readyTimer(int timeout);
    // 启动显示动画-已显示动画重新开始
    void showAnimation();
    // 启动显示动画-已显示不重复动画
    void keepAnimation();
    // 启动隐藏动画
    void hideAnimation();
    // 显示的文本
    void setTextList(const QStringList &texts);

private:
    Ui::RDesktopTip *ui;
    // 唯一实例
    static RDesktopTip *instance;

    // 动画设置
    static AnimationMode mode;
    // 动画组
    QParallelAnimationGroup *showGroup;
    // 保存动画结束状态
    bool showAnimEnd = false;
    // 透明度属性动画
    QPropertyAnimation *showOpacity = nullptr;
    // 位置属性动画
    QPropertyAnimation *showPos = nullptr;

    // 定时关闭
    QTimer *hideTimer = nullptr;
    // 定时计数
    int hideCount = 0;
};

#endif // RDESKTOPTIP_H
