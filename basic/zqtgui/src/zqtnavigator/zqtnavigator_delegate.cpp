#include"zqtnavigator/zqtnavigator_delegate.h"
#include<QPainter>
ZQTGUI_NS_BEGIN
ZQtNavigatorDelegate::ZQtNavigatorDelegate(QObject* parent) :QStyledItemDelegate(parent) {

}

void ZQtNavigatorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(painter, option, index);

    //QStyleOptionViewItem drawOption(option);

    //// ʹ�� myOption ���� initStyleOption ����
    //initStyleOption(&drawOption, index);

    //// ��ȡ�ڵ�����
    //QString text = index.data().toString();
    //QIcon icon = /* ��ȡ�ڵ�ͼ�꣬������ݽڵ��Ƿ�չ�� */;

    //// ���Ʊ���
    //painter->fillRect(option.rect, option.state & QStyle::State_Selected ? Qt::blue : Qt::white);

    //// ����ͼ��
    //QRect iconRect = /* ����ͼ��λ�� */;
    //icon.paint(painter, iconRect);

    //// �����ı�
    //QRect textRect = /* �����ı�λ�� */;
    //painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

    //// ����������ʽ
    //QFont font = /* ������Ҫ����������ʽ */;
    //painter->setFont(font);

}

ZQTGUI_NS_END