// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	int k = 8 * sizeof(TELEM);

	if (len < 0)
		throw "negative length";

	BitLen = len;
	MemLen = (BitLen + k - 1) / k;

	pMem = new TELEM [MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	pMem = new TELEM [MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0)
		throw "negative index";
	if (n > BitLen)
		throw "large index";

	return ( n / (8 * sizeof(TELEM)) );
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0)
		throw "negative index";
	if (n > BitLen)
		throw "large index";

	return (1 << n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw "negative index";
	if (n > BitLen)
		throw "large index";

	int k = GetMemIndex(n);

	pMem[k] = pMem[k] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw "negative index";
	if (n > BitLen)
		throw "large index";

	int k = GetMemIndex(n);

	pMem[k] = pMem[k] & ( ~GetMemMask(n) );
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)
		throw "negative index";
	if (n > BitLen)
		throw "large index";

	int k = GetMemIndex(n); 
	TELEM p1 = pMem[k];

	p1 = p1 & ( GetMemMask(n) );

	if (p1 > 0) // или if (p1 == GetMemMask(n))
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		delete[] pMem;

		BitLen = bf.BitLen;
		MemLen = bf.MemLen;

		pMem = new TELEM [MemLen];

		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int ii = 0;

	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] == bf.pMem[i]) 
			ii++;
	if (ii == MemLen) return 1;
	else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int n;

	if (bf.BitLen <= this->BitLen)
		n = this->BitLen;
	else 
		n = bf.BitLen;

	TBitField tmp(n);

	for (int i = 0; i < this->MemLen; i++)
		tmp.pMem[i] = this->pMem[i] | bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int n;

	if (bf.BitLen <= this->BitLen)
		n = this->BitLen;
	else 
		n = bf.BitLen;

	TBitField tmp(n);

	for (int i = 0; i < this->MemLen; i++)
		tmp.pMem[i] = this->pMem[i] & bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	int n = 0;

	for (int i = 0; i < MemLen; i++)
	{
		if (i == MemLen - 1) 
			n = 8 * sizeof(TELEM) - (BitLen - (MemLen - 1) * 8 * sizeof(TELEM) );

		pMem[i] = pMem[i] << abs(n);
		pMem[i] = ~pMem[i];
		pMem[i] = pMem[i] >> abs(n);
	}

	return (*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int x;

	for (int i = 0; i < bf.BitLen; i++)
	{
		cin >> x;
		switch (x)
		{
		case 1:
			bf.SetBit(i);
			break;
		case 0:
			bf.ClrBit(i);
			break;
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = bf.BitLen - 1; i >= 0; i--)
		cout << bf.GetBit(i);

	return ostr;
}