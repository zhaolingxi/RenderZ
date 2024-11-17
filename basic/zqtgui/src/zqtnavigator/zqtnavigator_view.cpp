#include "zqtnavigator/zqtnavigator_view.h"
#include"zqtnavigator/zqtnavigator_model.h"
#include"zqtnavigator/zqtnavigator_delegate.h"
ZQTGUI_NS_BEGIN

ZQtNavigatorView::ZQtNavigatorView(QWidget* parent, ZQtNavigatorModel* model)
{
	if (model) {
		setModel(model);
		//setColumnCount(model->columnCount());
	}

	ZQtNavigatorDelegate* delegate = new ZQtNavigatorDelegate(this);

	// ÉèÖÃÎ¯ÍÐ
	this->setItemDelegate(delegate);

	setMaximumWidth(400);
}

ZQTGUI_NS_END


