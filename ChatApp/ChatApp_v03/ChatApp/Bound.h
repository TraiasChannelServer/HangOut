#pragma once

/*
 * ���E��\�����N���X
 */

#include "DxLib.h"

class Bound
{
public:
	Bound() = default;
	virtual ~Bound() = default;

public:
	virtual RECT GetRect() const = 0;
	virtual bool IsIn(int x, int y) const = 0;
};
