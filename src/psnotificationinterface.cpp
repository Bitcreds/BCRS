// Copyright (c) 2014-2019 The Dash Core Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2021 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "psnotificationinterface.h"

#include "masternodeman.h"
#include "masternode-payments.h"
#include "masternode-sync.h"
#include "governance.h"
#include "instantsend.h"
#include "privatesend.h"

CPSNotificationInterface::CPSNotificationInterface()
{
}

CPSNotificationInterface::~CPSNotificationInterface()
{
}

void CPSNotificationInterface::InitializeCurrentBlockTip()
{
    LOCK(cs_main);
    UpdatedBlockTip(chainActive.Tip(), NULL, IsInitialBlockDownload());
}

void CPSNotificationInterface::AcceptedBlockHeader(const CBlockIndex *pindexNew)
{
    masternodeSync.AcceptedBlockHeader(pindexNew);
}

void CPSNotificationInterface::NotifyHeaderTip(const CBlockIndex *pindexNew, bool fInitialDownload)
{
    masternodeSync.NotifyHeaderTip(pindexNew, fInitialDownload);
}

void CPSNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload)
{
    if (pindexNew == pindexFork) // blocks were disconnected without any new ones
        return;

    masternodeSync.UpdatedBlockTip(pindexNew, fInitialDownload);

    if (fInitialDownload) // In IBD
        return;

    mnodeman.UpdatedBlockTip(pindexNew);
    privateSendPool.UpdatedBlockTip(pindexNew);
    instantsend.UpdatedBlockTip(pindexNew);
    mnpayments.UpdatedBlockTip(pindexNew);
    governance.UpdatedBlockTip(pindexNew);
}

void CPSNotificationInterface::SyncTransaction(const CTransaction &tx, const CBlock *pblock)
{
    instantsend.SyncTransaction(tx, pblock);
}
