#pragma once
#include "zcore_sdk.h"
ZCORE_NS_BEGIN
class ZCORE_API BaseNotifyTree
{
public:
	BaseNotifyTree();
	virtual ~BaseNotifyTree();

	BaseNotifyTree* createChild();
	void addChild(const BaseNotifyTree*& node);
	void removeChild(const BaseNotifyTree*& node);
	void removeAllChild();
	void deleteChild(const BaseNotifyTree*& node);
	void deleteAllChild();

	virtual void notifyFather();

};
ZCORE_NS_BEGIN