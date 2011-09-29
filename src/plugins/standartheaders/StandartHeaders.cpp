#include <QPainter>
#include <QDateTime>
#include <QFileIconProvider>

#include "StandartHeaders.h"


static QFileIconProvider iconProvider;


NameHeaderItem::NameHeaderItem()
{
	m_name = QObject::tr("Name");
	m_description = QObject::tr("Name of file without a suffix");
	m_lessThan = 0;
}


void NameHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	QRect rect = op.rect;
	QIcon icon = iconProvider.icon(fi);
	icon.paint(&painter, rect.left(), rect.top(), rect.height(), rect.height());

	rect.setLeft(rect.left() + 2*op.margin + rect.height());
	QFontMetrics metrics(painter.font());
	QString name;
	QString addName;
	if (fi.isDir()) {
		name = fi.fileName();
		addName += "[]";	// addName.length == 2
	}
	else {
		name = fi.baseName();
		addName = "";
	}

	if (metrics.width(name) + metrics.width(addName) > rect.width())
		addName += "..";	// addName.length == 4

	while (metrics.width(name) + metrics.width(addName) > rect.width() && name.length() > 0)
		name.remove(name.length() - 1, 1);

	if (fi.isDir()) {
		if (addName.length() == 2)
			name = "[" + name + "]";
		else
			name = "[" + name + "..]";
	}
	else if (addName.length() == 2)
		name += "..";

	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, name);
}


SuffixHeaderItem::SuffixHeaderItem()
{
	m_name = QObject::tr("Ext");
	m_description = QObject::tr("Suffix of a file");
	m_lessThan = 0;
}


void SuffixHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	if (!fi.isDir()) {
		painter.drawText(op.rect, Qt::AlignLeft | Qt::AlignVCenter, fi.suffix());
	}
}


SizeHeaderItem::SizeHeaderItem()
{
	m_name = QObject::tr("Size");
	m_description = QObject::tr("Size of a file");
	m_lessThan = 0;
}


void SizeHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	QString strSize("");
	int size = fi.size();

	do {
		if (size >= 1000)
			strSize = QString(" %1").arg(size % 1000, 3, 10, QLatin1Char('0')) + strSize;
		else
			strSize = QString("%1").arg(size % 1000) + strSize;
		size /= 1000;
	}
	while (size);

	QRect r = op.rect;
	r.setLeft(op.margin);
	painter.drawText(r, Qt::AlignRight | Qt::AlignVCenter, strSize);
}


TextPermsHeaderItem::TextPermsHeaderItem()
{
	m_name = QObject::tr("Perms");
	m_description = QObject::tr("Permissions as text (rwxr-xr-x)");
	m_lessThan = 0;
}


void TextPermsHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	QFile::Permissions p = fi.permissions();
	QString perms("");

	perms += p.testFlag(QFile::ReadUser)   ? "r" : "-";
	perms += p.testFlag(QFile::WriteUser)  ? "w" : "-";
	perms += p.testFlag(QFile::ExeUser)    ? "x" : "-";
	perms += p.testFlag(QFile::ReadGroup)  ? "r" : "-";
	perms += p.testFlag(QFile::WriteGroup) ? "w" : "-";
	perms += p.testFlag(QFile::ExeGroup)   ? "x" : "-";
	perms += p.testFlag(QFile::ReadOther)  ? "r" : "-";
	perms += p.testFlag(QFile::WriteOther) ? "w" : "-";
	perms += p.testFlag(QFile::ExeOther)   ? "x" : "-";

	painter.drawText(op.rect, Qt::AlignRight | Qt::AlignVCenter, perms);
}


DigitPermsHeaderItem::DigitPermsHeaderItem()
{
	m_name = QObject::tr("Perms");
	m_description = QObject::tr("Permissions as '0755'");
	m_lessThan = 0;
}


void DigitPermsHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	uint perms = fi.permissions();

	perms = (perms & 0x00f)  |  ((perms & 0x0f0) >> 1)  |  ((perms & 0xf00) >> 2);

	painter.drawText(op.rect, Qt::AlignRight | Qt::AlignVCenter, QString("0%1").arg(perms, 3, 8, QLatin1Char('0')));
}


OwnerHeaderItem::OwnerHeaderItem()
{
	m_name = QObject::tr("Owner");
	m_description = QObject::tr("File owner");
	m_lessThan = 0;
}


void OwnerHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	painter.drawText(op.rect, Qt::AlignLeft | Qt::AlignVCenter, fi.owner());
}


GroupHeaderItem::GroupHeaderItem()
{
	m_name = QObject::tr("Group");
	m_description = QObject::tr("File group");
	m_lessThan = 0;
}


void GroupHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	painter.drawText(op.rect, Qt::AlignLeft | Qt::AlignVCenter, fi.group());
}


ModifiedHeaderItem::ModifiedHeaderItem()
{
	m_name = QObject::tr("Modified");
	m_description = QObject::tr("Last time of file modified");
	m_lessThan = 0;
}


void ModifiedHeaderItem::drawFileItem(const QFileInfo &fi, const PaintOption &op, QPainter &painter)
{
	QDateTime dt = fi.lastModified();
	painter.drawText(op.rect, Qt::AlignLeft | Qt::AlignVCenter, dt.toString("yyyy/MM/dd  hh:mm:ss"));
}
