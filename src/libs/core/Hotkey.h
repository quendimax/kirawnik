#ifndef __CORE_HOTKEY_H__
#define __CORE_HOTKEY_H__


#include <QKeySequence>


class Hotkey
{
public:
	Hotkey(const QKeySequence &key1 = QKeySequence::UnknownKey, const QKeySequence &key2 = QKeySequence::UnknownKey);

	inline QKeySequence keySequence() const { return m_keySequence1; }
	inline QKeySequence keySequenceSecond() const { return m_keySequence2; }

	inline void setKeySequence(const QKeySequence &key) { m_keySequence1 = key; }
	inline void setKeySequenceSecond(const QKeySequence &key) { m_keySequence2 = key; }

	bool matches(const QKeySequence &keySequence) const;
	bool operator == (const Hotkey &) const;
	inline bool operator != (const Hotkey &hotkey) const { return ! operator == (hotkey); }

private:
	QKeySequence m_keySequence1;
	QKeySequence m_keySequence2;
};

#endif //__CORE_HOTKEY_H__
