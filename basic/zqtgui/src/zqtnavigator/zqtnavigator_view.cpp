#include "zqtnavigator_view.h"
#include"zqtnavigator_model.h"
#include"zqtnavigator_delegate.h"
ZQTGUI_NS_BEGIN

ZQtNavigatorView::ZQtNavigatorView(QWidget* parent, ZQtNavigatorModel* model)
{
	if (model) {
		setModel(model);
		//setColumnCount(model->columnCount());
	}

	ZQtNavigatorDelegate* delegate = new ZQtNavigatorDelegate(this);

	// ����ί��
	this->setItemDelegate(delegate);
}

ZQTGUI_NS_END


