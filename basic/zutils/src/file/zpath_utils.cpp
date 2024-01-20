#include "zpath_utils.h"
#include "zsystem_api.h"

ZUTILS_NS_BEGIN
void ZPathUtils::parsePath(const ZString& tagPath, ZString& realPath)
{
	static const char* docDirTag = "$DOCDIR$";
	static const char* usrDirTag = "$USERDIR$";
	static const char* procDirTag = "$PROCDIR$";

	ZString tagDir;
	int tagLen = 0;
	if (tagPath.beginWith(docDirTag)) {
		ZSystemApi::getDocumentDir(tagDir);
		tagLen = strlen(docDirTag);
	}
	else if (tagPath.beginWith(usrDirTag)) {
		ZSystemApi::getUserHomeDir(tagDir);
		tagLen = strlen(usrDirTag);
	}
	else if (tagPath.beginWith(procDirTag)) {
		ZSystemApi::getCurrentProcName(tagDir);
		tagLen = strlen(procDirTag);
	}
	if (tagLen > 0) {
		realPath = tagDir;
	}
	realPath.append(tagPath.getData() + tagLen);
}

std::shared_ptr<std::string> ZPathUtils::parsePath(const ZString& tagPath)
{
	auto realPath = std::make_shared<std::string>();
	ZString tmpPath;
	parsePath(tagPath, tmpPath);
	*realPath = tmpPath.getData();
	return realPath;
}

void ZPathUtils::joinPath(const ZString& dirPath, const ZString& childPath, ZString& combPath)
{
	if (dirPath.isEmpty() || childPath.isEmpty()) {
		//AALOGFMTE("joinPath() dirPath or childPath is empty");
		return;
	}
	ZString tmpPath = dirPath;
	if (!tmpPath.endWith('/') && !tmpPath.endWith('\\')) {
#ifdef _MSVC
		tmpPath.append('\\');
#else
		tmpPath.append('/');
#endif
	}
	tmpPath.append(childPath.getData(), childPath.getLength());
	combPath = tmpPath;
}

void ZPathUtils::baseName(const ZString& filePath, ZString& strName)
{
	if (filePath.isEmpty()) {
		//AALOGFMTE("baseName() filePath is empty");
		return;
	}
	ZString strFilePath(filePath);
	// 去除末尾的 '/'  '\\' 字符
	if (strFilePath.endWith('/') || strFilePath.endWith('\\')) {
		strFilePath.setAt(strFilePath.getLength() - 1, 0);
	}
	int pos = strFilePath.reverseFind('/');
	if (pos < 0) {
		pos = strFilePath.reverseFind('\\');
	}
	strName = strFilePath.getData() + pos + 1;
}

void ZPathUtils::dirPath(const ZString& filePath, ZString& strPath)
{
	if (filePath.isEmpty()) {
		//AALOGFMTE("dirPath() filePath is empty");
		return;
	}
	strPath = filePath;
	// 去除末尾的 '/'  '\\' 字符
	if (strPath.endWith('/') || strPath.endWith('\\')) {
		strPath.setAt(strPath.getLength() - 1, 0);
	}
	// 去除最里层的文件或文件夹名称
	int pos = strPath.reverseFind('/');
	if (pos < 0) {
		pos = strPath.reverseFind('\\');
	}
	if (pos > 1) {
		strPath.setAt(pos, 0);
	}
}

std::shared_ptr<std::string> ZPathUtils::joinPath(const ZString& dirPath, const ZString& childPath)
{
	ZString combPath;
	joinPath(dirPath, childPath, combPath);
	std::shared_ptr<std::string> strPath = std::make_shared<std::string>(combPath.getData());
	return strPath;
}

std::shared_ptr<std::string> ZPathUtils::baseName(const ZString& filePath)
{
	ZString strName;
	baseName(filePath, strName);
	std::shared_ptr<std::string> spName = std::make_shared<std::string>(strName.getData());
	return spName;
}

std::shared_ptr<std::string> ZPathUtils::dirPath(const ZString& filePath)
{
	ZString strPath;
	dirPath(filePath, strPath);
	std::shared_ptr<std::string> spPath = std::make_shared<std::string>(strPath.getData());
	return spPath;
}
ZUTILS_NS_END
