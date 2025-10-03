#pragma once
#include "zqtgui_sdk.h"
#include <QObject>
#include <memory>
#include <unordered_map>
#include<QString>
#include <vector>
#include <QMap>
class QWidget;
class QTranslator;

namespace ZQTGuiNS {

    class ZQTGUI_API QTPageManager : public QObject
    {
        Q_OBJECT

    public:
        // 单例模式获取实例
        static QTPageManager* instance();

        /**
         * @brief 注册一个视图，以便管理器可以管理其生命周期
         * @param[in] viewName 视图的唯一名称
         * @param[in] viewWidget 视图的QWidget指针
         */
        void registerView(const QString& viewName, QWidget* viewWidget);

        /**
         * @brief 退出并销毁View
         * @param[in] viewlTopoOrder View在依赖树上的Topo序列,被依赖的View在后
         */
        void exitViewManager(const std::vector<QString>& viewlTopoOrder);

    public:
        /**
         * @brief 加载界面样式
         * @param[in] themeDir 样式文件夹路径, 示例: "./resources/theme"
         * @param[in] themeName 应用程序样式主题名称, 示例:"default"
         * @param[in] isUpdateResPath 是否需要替换qss文件中的资源路径
         * @return true/false
         */
        bool loadTheme(const QString& themeDir,
            const QString& themeName,
            const bool isUpdateResPath = false);

        bool loadInternalTheme(const QString& themeName);
        /**
         * @brief 切换主题样式
         * @param[in] themeName 样式主题名称
         * @return true/false
         */
        bool changeTheme(const QString& themeName);

        /**
         * @brief 获取样式主题qss描述
         * @param[in] themeName 应用程序样式名称
         * @param[out] styleSheet 应用程序的样式qss描述
         * @return true/false
         */
        bool getStyleSheet(const QString& themeName, QString& styleSheet) const;

    public:
        /**
         * @brief 加载应用程序的语言包
         * @param[in] langDir 语言包文件夹路径, 示例: "./resources/lang"
         * @param[in] langName 语言名称, 示例: "zh_CN"
         * @return true/false
         */
        bool loadLanguage(const QString& langDir, const QString& langName);

        /**
         * @brief 切换语言
         * @param[in] languageName 语言名称, 示例: "zh_CN"
         * @return true/false
         */
        bool changeLanguage(const QString& languageName);

    protected:
        QTPageManager(QObject* parent = nullptr);
        virtual ~QTPageManager();

        // 删除拷贝构造和赋值操作，确保单例
        QTPageManager(const QTPageManager&) = delete;
        QTPageManager& operator=(const QTPageManager&) = delete;

    private:
        // 存储已加载的主题样式表 <themeName, styleSheetContent>
        QMap<QString, QString> m_themeStyleSheets;

        // 存储已加载的翻译器 <langName, translator>
        QMap<QString, QTranslator*> m_translators;
        QTranslator* m_currentTranslator = nullptr;

        // 存储注册的视图 <viewName, viewWidget>
        QMap<QString, QWidget*> m_views;
    };

} // namespace ZQTGuiNS