#include "toolbox_main_view.h"

ToolBoxMainView::ToolBoxMainView(UIViewBase* parent, const QString& viewName)
	: UIViewBase(parent, viewName)
{

}

ToolBoxMainView::~ToolBoxMainView()
{

}

bool ToolBoxMainView::createView()
{
	setModel(nullptr);
	setDelegate(nullptr, nullptr);
	// 特别提醒:因为setWindowFlags会附带修改父窗体的属性,因此需要将该窗体的子窗体都设置完毕后才会生效
	setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	setDragEnabled(true);
	mainLayout_ = new GridLayout(this);
	// 侧边栏
    mainSideBar_ = new Widget(this, "MainSideBar");
    mainSideBar_->setParentRouter(this);
	mainSideBar_->createWidget();
	auto sideBarLayout = new VBoxLayout(mainSideBar_);
	// 域控配置
	PushButton* dcuManageBtn = new PushButton(mainSideBar_, QStringLiteral("域控配置"), "DcuManageViewBtn");
	dcuManageBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(dcuManageBtn);
	mainSideBar_->addObjToContainer(dcuManageBtn, UIStandardCmd::kPopView, QString("DcuManageView"));
	// Bag包录制
	PushButton* bagRecordBtn = new PushButton(mainSideBar_, QStringLiteral("Bag包录制"), "BagRecordViewBtn");
	bagRecordBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(bagRecordBtn);
	mainSideBar_->addObjToContainer(bagRecordBtn, UIStandardCmd::kOpenView, QString("BagRecordView"));
	// Bag包回放
	PushButton* bagReplayBtn = new PushButton(mainSideBar_, QStringLiteral("Bag包回放"), "BagReplayViewBtn");
	bagReplayBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(bagReplayBtn);
	mainSideBar_->addObjToContainer(bagReplayBtn, UIStandardCmd::kOpenView, QString("BagReplayView"));
	// 日志监控
	PushButton* logMonitorBtn = new PushButton(mainSideBar_, QStringLiteral("日志监控"), "LogMonitorViewBtn");
	logMonitorBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(logMonitorBtn);
	mainSideBar_->addObjToContainer(logMonitorBtn, UIStandardCmd::kOpenView, QString("LogMonitorView"));

	// 资源监控
	PushButton* dcuMonitorBtn = new PushButton(mainSideBar_,
		QStringLiteral("资源监控"),
		"DcuMonitorViewBtn");
	dcuMonitorBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(dcuMonitorBtn);
	mainSideBar_->addObjToContainer(dcuMonitorBtn,
		UIStandardCmd::kOpenView,
		QString("DcuMonitorView"));
	// 消息监控
	PushButton* msgMonitorBtn = new PushButton(mainSideBar_,
		QStringLiteral("消息监控"),
		"MsgMonitorViewBtn");
	msgMonitorBtn->setContentsMargins(2, 15, 2, 0);
	sideBarLayout->addWidget(msgMonitorBtn);
	mainSideBar_->addObjToContainer(msgMonitorBtn,
		UIStandardCmd::kOpenView,
		QString("MsgMonitorView"));

    // 拓扑展示
    PushButton* topoViewBtn = new PushButton(mainSideBar_,
                                             QStringLiteral("拓扑展示"),
                                             "TopoViewViewBtn");
    topoViewBtn->setContentsMargins(2, 15, 2, 0);
    sideBarLayout->addWidget(topoViewBtn);
	mainSideBar_->addObjToContainer(topoViewBtn,
		UIStandardCmd::kOpenView,
		QString("TopoViewView"));

    // 设置选中效果
    QVector<PushButton*> btns{bagRecordBtn,
                              bagRecordBtn,
                              bagReplayBtn,
                              logMonitorBtn,
                              dcuMonitorBtn,
                              msgMonitorBtn,
                              topoViewBtn};
    dcuManageBtn->setProperty("class", "normal");
    for (PushButton* btn : btns) {
        btn->setProperty("class", "normal");
        connect(btn, &QPushButton::released, this, [btn, btns]() {
            for (PushButton* b : btns) {
                if (b != btn) {
                    b->setProperty("class", "normal");
                    b->setStyle(b->style());
                }
            }
            btn->setProperty("class", "selected");
            btn->setStyle(btn->style());
        });
    }
    logMonitorBtn->setProperty("class", "selected");

    // 嵌入显示子页面的layout
    QVector<QString> childTabViewNames = {"BagRecordView",
                                          "BagReplayView",
                                          "LogMonitorView",
                                          "DcuMonitorView",
                                          "MsgMonitorView",
                                          "TopoViewView"};

    pageViewLayout_ = new VBoxLayout();
    for (auto& viewName : childTabViewNames) {
        auto childView = getViewByName(viewName);
        if (childView == nullptr) {
            LOGFMTW("getViewByName() childView == nullptr, viewName: %s", viewName.toUtf8().data());
            continue;
        }
        pageViewLayout_->addWidget(childView.get());
        childView->hide();
    }
    sideBarLayout->addStretch();
    mainLayout_->addWidget(mainSideBar_, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    mainLayout_->addLayout(pageViewLayout_, 0, 1);
    setMinimumSize(1510, 700);
    mainSideBar_->setContentsMargins(0, 20, 0, 0);
    mainSideBar_->setFixedWidth(100);
    mainSideBar_->setMinimumHeight(2080);

    // 设置命令的前置处理回调
    setCmdsPreHandleCB(VIEWCMD_HANDLECB_BINDER3(&ToolBoxMainView::preHandleViewCmd, this));

    // 弹出/打开子功能页面
    std::vector<int64_t> cmdIds = {UIStandardCmd::kPopView, UIStandardCmd::kOpenView};
    bindViewCmdsHandleCB(cmdIds, VIEWCMD_HANDLECB_BINDER3(&ToolBoxMainView::handleSwitchViewCmd, this));

	return UIViewBase::createView();
}

bool ToolBoxMainView::handleSwitchViewCmd(const int64_t cmdId, const std::any& param, QObject* sender)
{
	QString viewName = std::any_cast<QString>(param);
	auto nextView = getViewByName(viewName);
	if (nextView == nullptr) {
		LOGFMTE("handlePopViewCmd() viewName: %s, not found", viewName.toUtf8().data());
		return true;
	}
	if (cmdId == UIStandardCmd::kPopView) {
		nextView->sendViewCmd(UIStandardCmd::kPrepareView);
		nextView->show();
		nextView->adjustSize();
	}
	else if (cmdId == UIStandardCmd::kOpenView) {
		if (currentOpenedView_.get() != nextView.get()) {
			if (currentOpenedView_ != nullptr) {
				currentOpenedView_->sendViewCmd(UIStandardCmd::kResetView);
				currentOpenedView_->hide();
			}
			nextView->sendViewCmd(UIStandardCmd::kPrepareView);
			nextView->show();
			currentOpenedView_ = nextView;
        }
    }
    return true;
}

bool ToolBoxMainView::preHandleViewCmd(const int64_t cmdId, const std::any& param, QObject* sender)
{
	if (getCmdDelegate() == nullptr || getViewManager() == nullptr) {
		LOGFMTE("ToolBoxMainView::preHandleViewCmd() not set manager or cmdDelegate");
		return true;
	}
	bool ret = getCmdDelegate()->handleViewCmd(cmdId, param, sender);
	if (ret) {
		return true;
	}
	// 若命令是全局命令,则转发给对应的View,仅限主框架页面使用
	if (getViewManager()->isGlobalViewCmd(cmdId)) {
		LOGFMTD("ToolBoxMainView::preHandleViewCmd() transferGlobalCmdToView cmdId = %lld", cmdId);
		getViewManager()->transferGlobalCmdToView(cmdId, param, sender);
	}
	return true;
}

void ToolBoxMainView::showEvent(QShowEvent* event)
{
	if (currentOpenedView_ == nullptr) {
		sendViewCmd(UIStandardCmd::kOpenView, QString("LogMonitorView"));
	}
	UIViewBase::showEvent(event);
}
