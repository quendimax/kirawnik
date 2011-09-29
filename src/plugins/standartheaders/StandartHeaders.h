#ifndef __PLUGINS_STANDARTHEADERS_H__
#define __PLUGINS_STANDARTHEADERS_H__

#include <app/AbstractHeaderItem.h>


class NameHeaderItem : public AbstractHeaderItem
{
public:
	NameHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class SuffixHeaderItem : public AbstractHeaderItem
{
public:
	SuffixHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class SizeHeaderItem : public AbstractHeaderItem
{
public:
	SizeHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class TextPermsHeaderItem : public AbstractHeaderItem
{
public:
	TextPermsHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class DigitPermsHeaderItem : public AbstractHeaderItem
{
public:
	DigitPermsHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class OwnerHeaderItem : public AbstractHeaderItem
{
public:
	OwnerHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class GroupHeaderItem : public AbstractHeaderItem
{
public:
	GroupHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


class ModifiedHeaderItem : public AbstractHeaderItem
{
public:
	ModifiedHeaderItem();
	void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &);
};


#endif //__PLUGINS_STANDARTHEADERS_H__
