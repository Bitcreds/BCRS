// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2014-2019 The Dash Core Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2019 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CREDITS_CORE_MEMUSAGE_H
#define CREDITS_CORE_MEMUSAGE_H

#include "memusage.h"

#include "primitives/block.h"
#include "primitives/transaction.h"

static inline size_t RecursiveBitcredsUsage(const CScript& script) {
    return memusage::BitcredsUsage(*static_cast<const CScriptBase*>(&script));
}

static inline size_t RecursiveBitcredsUsage(const COutPoint& out) {
    return 0;
}

static inline size_t RecursiveBitcredsUsage(const CTxIn& in) {
    return RecursiveBitcredsUsage(in.scriptSig) + RecursiveBitcredsUsage(in.prevout);
}

static inline size_t RecursiveBitcredsUsage(const CTxOut& out) {
    return RecursiveBitcredsUsage(out.scriptPubKey);
}

static inline size_t RecursiveBitcredsUsage(const CTransaction& tx) {
    size_t mem = memusage::BitcredsUsage(tx.vin) + memusage::BitcredsUsage(tx.vout);
    for (std::vector<CTxIn>::const_iterator it = tx.vin.begin(); it != tx.vin.end(); it++) {
        mem += RecursiveBitcredsUsage(*it);
    }
    for (std::vector<CTxOut>::const_iterator it = tx.vout.begin(); it != tx.vout.end(); it++) {
        mem += RecursiveBitcredsUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveBitcredsUsage(const CMutableTransaction& tx) {
    size_t mem = memusage::BitcredsUsage(tx.vin) + memusage::BitcredsUsage(tx.vout);
    for (std::vector<CTxIn>::const_iterator it = tx.vin.begin(); it != tx.vin.end(); it++) {
        mem += RecursiveBitcredsUsage(*it);
    }
    for (std::vector<CTxOut>::const_iterator it = tx.vout.begin(); it != tx.vout.end(); it++) {
        mem += RecursiveBitcredsUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveBitcredsUsage(const CBlock& block) {
    size_t mem = memusage::BitcredsUsage(block.vtx);
    for (std::vector<CTransaction>::const_iterator it = block.vtx.begin(); it != block.vtx.end(); it++) {
        mem += RecursiveBitcredsUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveBitcredsUsage(const CBlockLocator& locator) {
    return memusage::BitcredsUsage(locator.vHave);
}

#endif // CREDITS_CORE_MEMUSAGE_H
