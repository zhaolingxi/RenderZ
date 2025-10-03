#include "qtpage_manager.h"
#include <QFile>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QDebug>
#include <QObject>
#include <QWidget>  
#include <QResource> 
#include<QDirIterator>
ZQTGUI_NS_BEGIN

// 初始化静态实例指针
static QTPageManager* g_instance = nullptr;

QTPageManager* QTPageManager::instance()
{
    if (g_instance == nullptr) {
        g_instance = new QTPageManager(qApp);
    }
    return g_instance;
}

QTPageManager::QTPageManager(QObject* parent)
    : QObject(parent)
    , m_currentTranslator(nullptr)
{
}

QTPageManager::~QTPageManager()
{
    // 清理翻译器
    qDeleteAll(m_translators);
    m_translators.clear();

    // 清理视图
    // 注意：这里的清理可能在 exitViewManager 之后是多余的，但作为最后的保障是好的
    qDeleteAll(m_views);
    m_views.clear();

    g_instance = nullptr;
}

void QTPageManager::registerView(const QString& viewName, QWidget* viewWidget)
{
    if (m_views.contains(viewName)) {
        qWarning() << "QTPageManager::registerView - view with name" << viewName << "already exists. Replacing it.";
        QWidget* oldView = m_views.value(viewName);
        if (oldView) {
            oldView->deleteLater();
        }
    }
    m_views.insert(viewName, viewWidget);
}

void QTPageManager::exitViewManager(const std::vector<QString>& viewlTopoOrder)
{
    for (const auto& viewName : viewlTopoOrder) {
        if (m_views.contains(viewName)) {
            QWidget* view = m_views.take(viewName); // 从Map中移除并获取
            if (view) {
                qInfo() << "Destroying view:" << viewName;
                view->deleteLater();
            }
        }
    }
}

bool QTPageManager::loadTheme(const QString& themeDir, const QString& themeName, const bool isUpdateResPath)
{
    // 推荐的样式文件路径结构： themeDir/themeName/style.qss
    QString qssPath = QDir(themeDir).filePath(themeName + "/style.qss");

    QFile file(qssPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "QTPageManager::loadTheme - Could not open theme file:" << qssPath;
        return false;
    }

    QString styleSheet = file.readAll();
    file.close();

    if (isUpdateResPath) {
        // 将QSS中的相对资源路径(如 url(:/...) )替换为绝对路径
        // 我们假设QSS中的资源路径相对于主题目录
        // 例如 url(:/icons/home.png) -> url(D:/app/resources/theme/default/icons/home.png)
        QString themePath = QFileInfo(qssPath).absolutePath();
        styleSheet.replace("url(:/", "url(" + themePath + "/");
    }

    m_themeStyleSheets.insert(themeName, styleSheet);
    qInfo() << "Theme" << themeName << "loaded successfully from" << qssPath;
    return true;
}

/**
 * @brief 从Qt资源系统加载一个内嵌的主题。
 * @param themeName 主题名称，例如 "default"
 * @return true 如果加载成功，否则 false
 */
bool QTPageManager::loadInternalTheme(const QString& themeName)
{
    // 根据.qrc文件中定义的alias构建资源路径
    // e.g., ":/theme/default/style.qss"
    QString resourcePath = QString(":/theme/default/%1/style.qss").arg(themeName);
    qDebug() << "============ Dumping All Known Resources ============";
    QDirIterator it(":", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }
    qDebug() << "=====================================================";
    qDebug() << "Attempting to load:" << resourcePath;
    QFile file(resourcePath);
    // 使用资源路径打开文件，这几乎不可能失败（除非内存问题）
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "QTPageManager::loadInternalTheme - Could not open internal resource:" << resourcePath;
        return false;
    }

    QString styleSheet = file.readAll();
    file.close();

    // === 关键点: 不需要任何路径替换！ ===
    // QSS中的 "url(:/icons/home.png)" 已经是正确的、可直接使用的资源路径。
    // Qt会自动处理它们。任何替换都是有害的。

    m_themeStyleSheets.insert(themeName, styleSheet);
    qInfo() << "Internal theme" << themeName << "loaded successfully from" << resourcePath;
    return true;
}

bool QTPageManager::changeTheme(const QString& themeName)
{
    if (!m_themeStyleSheets.contains(themeName)) {
        qWarning() << "QTPageManager::changeTheme - Theme" << themeName << "not loaded.";
        return false;
    }

    QString styleSheet = m_themeStyleSheets.value(themeName);
    qApp->setStyleSheet(styleSheet);
    qInfo() << "Application theme changed to" << themeName;
    return true;
}

bool QTPageManager::getStyleSheet(const QString& themeName, QString& styleSheet) const
{
    if (m_themeStyleSheets.contains(themeName)) {
        styleSheet = m_themeStyleSheets.value(themeName);
        return true;
    }
    qWarning() << "QTPageManager::getStyleSheet - Theme" << themeName << "not found.";
    return false;
}

bool QTPageManager::loadLanguage(const QString& langDir, const QString& langName)
{
    // 推荐的语言文件路径结构： langDir/app_langName.qm
    QString langFilePath = QDir(langDir).filePath(QString("app_%1.qm").arg(langName));

    if (!QFile::exists(langFilePath)) {
        qWarning() << "QTPageManager::loadLanguage - Language file not found:" << langFilePath;
        return false;
    }

    if (m_translators.contains(langName)) {
        qInfo() << "Language" << langName << "is already loaded.";
        return true;
    }

    QTranslator* translator = new QTranslator(this); // 设置parent，由manager管理内存
    if (translator->load(langFilePath)) {
        m_translators.insert(langName, translator);
        qInfo() << "Language" << langName << "loaded successfully from" << langFilePath;
        return true;
    }
    else {
        qWarning() << "QTPageManager::loadLanguage - Failed to load language file:" << langFilePath;
        delete translator; // 加载失败，清理内存
        return false;
    }
}

bool QTPageManager::changeLanguage(const QString& languageName)
{
    if (!m_translators.contains(languageName)) {
        qWarning() << "QTPageManager::changeLanguage - Language" << languageName << "not loaded.";
        return false;
    }

    // 1. 移除当前的翻译器
    if (m_currentTranslator) {
        qApp->removeTranslator(m_currentTranslator);
    }

    // 2. 安装新的翻译器
    QTranslator* newTranslator = m_translators.value(languageName);
    qApp->installTranslator(newTranslator);

    // 3. 更新当前翻译器指针
    m_currentTranslator = newTranslator;

    qInfo() << "Application language changed to" << languageName;
    qInfo() << "Note: UI elements must handle QEvent::LanguageChange to update their text.";

    return true;
}


ZQTGUI_NS_END
