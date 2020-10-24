// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCREDS_CONSENSUS_PARAMS_H
#define BITCREDS_CONSENSUS_PARAMS_H

#include "uint256.h"

#include <map>
#include <string>

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_CSV, // Deployment of BIP68, BIP112, and BIP113.
    MAX_VERSION_BITS_DEPLOYMENTS // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nHardForkOne; // block
    int nHardForkTwo; // block
    int nHardForkThree; //block
	int nHardForkFour; //block
	int nHardForkFive; //block
    int nHardForkSix; // block
    int nTempDevFundIncreaseEnd; //block height for temporal Dev fund increase ending
    int nMasternodePaymentsStartBlock;
    int nInstantSendKeepLock; // in blocks
    int nBudgetPaymentsStartBlock;
    int nBudgetPaymentsCycleBlocks;
    int nBudgetPaymentsWindowBlocks;
    int nBudgetProposalEstablishingTime; // in seconds
    int nSuperblockStartBlock;
    int nSuperblockCycle; // in blocks
    int nGovernanceMinQuorum; // Min absolute vote count to trigger an action
    int nGovernanceFilterElements;
    int nMasternodeMinimumConfirmations;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;

    int nIntPhaseTotalBlocks;
    int nBlocksPerYear; // expected blocks per year
	
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargetting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nOldPowTargetSpacing;
    int64_t nNewPowTargetSpacing;
    int64_t GetCurrentPowTargetSpacing(const int& nHeight) const {
        if (nHeight > nHardForkSix)
            return nNewPowTargetSpacing;
        else
            return nOldPowTargetSpacing;
    }
    int64_t nDifficultyAdjustmentInterval;
};
} // namespace Consensus

#endif // BITCREDS_CONSENSUS_PARAMS_H
