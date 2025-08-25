#include"zqtnavigator/zqtnavigator_delegate.h"
#include<QPainter>
#include <QApplication>
#include <QMouseEvent>
#include "zqtnavigator/zqtnavigatordefs.h"
#include "zqtnavigator/zqtnavigator_view.h" // forward declaration won't work with signals

ZQTGUI_NS_BEGIN


ZQtNavigatorDelegate::ZQtNavigatorDelegate(QObject* parent)
    : QStyledItemDelegate(parent), m_hoveredButton(0)
{
    // 在这里加载您的自定义按钮图标
    std::string modelPath = ASSERT_PATH;
    std::string icon1_path = modelPath + "image/buttons/sharp_menu_black_20.png"; 
    std::string icon2_path = modelPath + "image/buttons/sharp_refresh_black_20.png";
    std::string icon3_path = modelPath + "image/buttons/sharp_more_vert_black_20.png";
    m_button1Icon.addFile(icon1_path.c_str());   // 示例路径
    m_button2Icon.addFile(icon2_path.c_str());  // 示例路径
    m_button3Icon.addFile(icon3_path.c_str()); // 示例路径
}

void ZQtNavigatorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    int totalButtonWidth = (BUTTON_WIDTH + BUTTON_MARGIN) * 3;
    QRect contentRect = opt.rect;
    contentRect.setWidth(contentRect.width() - totalButtonWidth);

    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    if (!icon.isNull()) {
        QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, opt.widget);
        iconRect.moveLeft(contentRect.left() + BUTTON_MARGIN);
        icon.paint(painter, iconRect, opt.decorationAlignment);
    }

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
    textRect.moveLeft(contentRect.left() + BUTTON_MARGIN * 2 + opt.decorationSize.width());
    textRect.setWidth(contentRect.width() - (textRect.left() - contentRect.left()));

    QString text = index.data(Qt::DisplayRole).toString();
    style->drawItemText(painter, textRect, opt.displayAlignment, opt.palette, true, text);

    if (option.state & QStyle::State_MouseOver)
    {
        // --- 修改部分 ---
        // 使用新的 getButtonRect 函数
        QRect btn1Rect = getButtonRect(option.rect, 1);
        QRect btn2Rect = getButtonRect(option.rect, 2);
        QRect btn3Rect = getButtonRect(option.rect, 3);

        QStyleOptionButton btnOpt;
        btnOpt.state = QStyle::State_Enabled;

        btnOpt.rect = btn1Rect;
        if (m_hoveredIndex == index && m_hoveredButton == 1) btnOpt.state |= QStyle::State_MouseOver;
        m_button1Icon.paint(painter, btn1Rect);

        btnOpt.rect = btn2Rect;
        if (m_hoveredIndex == index && m_hoveredButton == 2) btnOpt.state |= QStyle::State_MouseOver;
        m_button2Icon.paint(painter, btn2Rect);

        btnOpt.rect = btn3Rect;
        if (m_hoveredIndex == index && m_hoveredButton == 3) btnOpt.state |= QStyle::State_MouseOver;
        m_button3Icon.paint(painter, btn3Rect);
    }
}

QSize ZQtNavigatorDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    // 确保行高足够容纳按钮
    size.setHeight(qMax(size.height(), BUTTON_HEIGHT + 2 * BUTTON_MARGIN));
    return size;
}

bool ZQtNavigatorDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        // --- 修改部分 ---
        // 使用新的 getButtonRect 函数
        if (getButtonRect(option.rect, 1).contains(mouseEvent->pos())) {
            ZQtNavigatorView* view = qobject_cast<ZQtNavigatorView*>(this->parent());
            if (view) emit view->button1Clicked(index);
            return true;
        }
        if (getButtonRect(option.rect, 2).contains(mouseEvent->pos())) {
            ZQtNavigatorView* view = qobject_cast<ZQtNavigatorView*>(this->parent());
            if (view) emit view->button2Clicked(index);
            return true;
        }
        if (getButtonRect(option.rect, 3).contains(mouseEvent->pos())) {
            ZQtNavigatorView* view = qobject_cast<ZQtNavigatorView*>(this->parent());
            if (view) emit view->button3Clicked(index);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ZQtNavigatorDelegate::setHoveredInfo(const QModelIndex& index, int button)
{
    m_hoveredIndex = index;
    m_hoveredButton = button;
}

QRect ZQtNavigatorDelegate::getButtonRect(const QRect& itemRect, int buttonNumber) const
{
    int y = itemRect.top() + (itemRect.height() - BUTTON_HEIGHT) / 2;
    // 从右侧开始计算，更精确
    int x_btn3 = itemRect.right() - BUTTON_WIDTH - BUTTON_MARGIN;
    int x_btn2 = x_btn3 - BUTTON_WIDTH - BUTTON_MARGIN;
    int x_btn1 = x_btn2 - BUTTON_WIDTH - BUTTON_MARGIN;

    int x = 0;
    if (buttonNumber == 1) x = x_btn1;
    else if (buttonNumber == 2) x = x_btn2;
    else if (buttonNumber == 3) x = x_btn3;

    return QRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
}


ZQTGUI_NS_END