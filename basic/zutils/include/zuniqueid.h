#pragma once
#include "zutils_sdk.h"
#include <set>
#include <queue>
#include <cstdint>
/**********************************************************************
 *@file   ZUniqueID.h
 *@date   2022/10/26 20 : 56
 *@brief  唯一ID管理器
 ***********************************************************************/
ZUTILS_NS_BEGIN
template<class Type>
class ZUniqueID
{
public:
	/**
	 @brief 构造唯一ID生成器,唯一种子值在[lowerLimit, upperLimit](包括lowerLimit和upperLimit)
	 @param[in] lowerLimit 产生唯一ID的种子下限
	 @param[in] upperLimit 产生唯一ID的种子上限
	 */
	ZUniqueID(const Type& lowerLimit, const Type& upperLimit)
	{
		static_assert(std::is_same<int, Type>::value || std::is_same<int64_t, Type>::value,
			"Type must be int or int64_t");
		init(lowerLimit, upperLimit);
	}

	ZUniqueID() 
	{
		seedLowerLimt_ = 0;
		seedUpperLimt_ = 0;
		currentSeed_ = 0;
	}

	virtual ~ZUniqueID()
	{
		
	}
	/**
	 @brief 初始化生成器,唯一种子值在[lowerLimit, upperLimit](包括lowerLimit和upperLimit)
	 @param[in] lowerLimit 产生唯一ID的种子下限
	 @param[in] upperLimit 产生唯一ID的种子上限
	 */
	void init(const Type& lowerLimit, const Type& upperLimit)
	{
		if (lowerLimit > upperLimit) {
			throw std::runtime_error("lowerLimit <= upperLimit");
		}
		seedLowerLimt_ = lowerLimit; /**< 产生唯一ID的种子下限 */
		seedUpperLimt_ = upperLimit; /**< 产生唯一ID的种子上限 */
		currentSeed_ = seedLowerLimt_; /**< 当前正在使用的种子 */
	}

	// 退出唯一ID生成器
	void exit() 
	{
		seedLowerLimt_ = 0;
		seedUpperLimt_ = 0;
		currentSeed_ = 0;
	}

	/**
	 @brief 获取一个空闲的ID，优先使用空闲列表中的ID减少内存占用
	 @return 空闲ID值
	 */
	const Type getFreeId()
	{
		if (currentSeed_ > seedUpperLimt_ && freeIdQueue_.empty()) {
			throw std::runtime_error("no free id");
		}
		if (currentSeed_ <= seedUpperLimt_) {
			Type id(currentSeed_);
			usedIdSet_.insert(id);
			currentSeed_++;
			return id;
		}
		Type id = freeIdQueue_.front();
		usedIdSet_.insert(id);
		freeIdQueue_.pop();
		return id;
	}

	const bool hasFreeId() 
	{
		if (currentSeed_ <= seedUpperLimt_ || freeIdQueue_.size() > 0 ) {
			return true;
		}
		return false;
	}

	void freeUsedId(const Type &id)
	{
		auto it = usedIdSet_.find(id);
		if ( it != usedIdSet_.end()) {
			freeIdQueue_.push(std::move(*it));
			usedIdSet_.erase(it);
		}
	}
	// id 是否已被使用
	bool isUsed(const Type &id) 
	{
		return (usedIdSet_.find(id) != usedIdSet_.end());
	}

private:
	Type seedLowerLimt_; /**< ID种子的上限 */
	Type seedUpperLimt_; /**< ID种子的下限 */
	Type currentSeed_;  /**< 当前正在使用的ID种子 */
	std::queue<Type> freeIdQueue_; /**< 未使用的ID */
	std::set<Type> usedIdSet_; /**< 已使用的ID */
};
ZUTILS_NS_END