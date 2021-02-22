// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2014-2019 The Dash Core Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2021 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCREDS_QT_BITCREDSADDRESSVALIDATOR_H
#define BITCREDS_QT_BITCREDSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitcredsAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcredsAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Bitcreds address widget validator, checks for a valid Bitcreds address.
 */
class BitcredsAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcredsAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BITCREDS_QT_BITCREDSADDRESSVALIDATOR_H
