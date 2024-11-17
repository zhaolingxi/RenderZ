#include"zqtnavigator/zqtnavigator_delegate.h"
#include<QPainter>
ZQTGUI_NS_BEGIN
ZQtNavigatorDelegate::ZQtNavigatorDelegate(QObject* parent) :QStyledItemDelegate(parent) {

}

void ZQtNavigatorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(painter, option, index);

    //QStyleOptionViewItem drawOption(option);

    //// 使用 myOption 调用 initStyleOption 方法
    //initStyleOption(&drawOption, index);

    //// 获取节点数据
    //QString text = index.data().toString();
    //QIcon icon = /* 获取节点图标，例如根据节点是否展开 */;

    //// 绘制背景
    //painter->fillRect(option.rect, option.state & QStyle::State_Selected ? Qt::blue : Qt::white);

    //// 绘制图标
    //QRect iconRect = /* 计算图标位置 */;
    //icon.paint(painter, iconRect);

    //// 绘制文本
    //QRect textRect = /* 计算文本位置 */;
    //painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

    //// 设置字体样式
    //QFont font = /* 根据需要设置字体样式 */;
    //painter->setFont(font);

}

ZQTGUI_NS_END