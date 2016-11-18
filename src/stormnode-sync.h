// Copyright (c) 2014-2017 The Dash Core Developers
// Copyright (c) 2015-2017 Silk Network Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef STORMNODE_SYNC_H
#define STORMNODE_SYNC_H

#include "chain.h"
#include "net.h"

#include <univalue.h>

class CStormnodeSync;

static const int STORMNODE_SYNC_FAILED          = -1;
static const int STORMNODE_SYNC_INITIAL         = 0;
static const int STORMNODE_SYNC_SPORKS          = 1;
static const int STORMNODE_SYNC_LIST            = 2;
static const int STORMNODE_SYNC_SNW             = 3;
static const int STORMNODE_SYNC_GOVERNANCE      = 4;
static const int STORMNODE_SYNC_GOVOBJ          = 10;
static const int STORMNODE_SYNC_GOVERNANCE_FIN  = 11;
static const int STORMNODE_SYNC_FINISHED        = 999;

static const int STORMNODE_SYNC_TIMEOUT_SECONDS = 30; // our blocks are 2.5 minutes so 30 seconds should be fine

extern CStormnodeSync stormnodeSync;

//
// CStormnodeSync : Sync stormnode assets in stages
//

class CStormnodeSync
{
private:
    // Keep track of current asset
    int nRequestedStormnodeAssets;
    // Count peers we've requested the asset from
    int nRequestedStormnodeAttempt;

    // Time when current stormnode asset sync started
    int64_t nTimeAssetSyncStarted;

    // Last time when we received some stormnode asset ...
    int64_t nTimeLastStormnodeList;
    int64_t nTimeLastPaymentVote;
    int64_t nTimeLastBudgetItem;
    // ... or failed
    int64_t nTimeLastFailure;

    // How many times we failed
    int nCountFailures;

    // Keep track of current block index
    const CBlockIndex *pCurrentBlockIndex;

    void Fail();
    void ClearFulfilledRequests();

public:
    CMasternodeSync() { Reset(); }

    void AddedStormnodeList() { nTimeLastStormnodeList = GetTime(); }
    void AddedPaymentVote() { nTimeLastPaymentVote = GetTime(); }
    void AddedBudgetItem(uint256 hash);

    bool IsFailed() { return nRequestedStormnodeAssets == STORMNODE_SYNC_FAILED; }
    bool IsBlockchainSynced();
    bool IsMasternodeListSynced() { return nRequestedStormnodeAssets > STORMNODE_SYNC_LIST; }
    bool IsWinnersListSynced() { return nRequestedStormnodeAssets > STORMNODE_SYNC_SNW; }
    bool IsSynced() { return nRequestedStormnodeAssets == STORMNODE_SYNC_FINISHED; }

    int GetAssetID() { return nRequestedStormnodeAssets; }
    int GetAttempt() { return nRequestedStormnodeAttempt; }
    std::string GetAssetName();
    std::string GetSyncStatus();

    void Reset();
    void SwitchToNextAsset();

    void ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
    void ProcessTick();

    void UpdatedBlockTip(const CBlockIndex *pindex);
};

#endif // STORMNODE_SYNC_H
