#include "Hotkey.h"


Hotkey::Hotkey(const QKeySequence &key1, const QKeySequence &key2)
	: m_keySequence1(key1),
	  m_keySequence2(key2)
{
}


bool Hotkey::matches(const QKeySequence &keySequence) const
{
	if (keySequence == m_keySequence1)
		return true;
	if (keySequence == m_keySequence2)
		return true;
	return false;
}


bool Hotkey::operator == (const Hotkey &key) const
{
	if (m_keySequence1 == key.m_keySequence1 && m_keySequence2 == key.m_keySequence2)
		return true;
	if (m_keySequence1 == key.m_keySequence2 && m_keySequence2 == key.m_keySequence1)
		return true;
	return false;
}
