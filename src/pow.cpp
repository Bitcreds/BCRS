// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2014-2019 The Dash Core Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2021 Bitcreds Developers
// Copyright (c) 2015-2016 The Gulden developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "primitives/block.h"
#include "chain.h"
#include "chainparams.h"
#include "main.h"
#include "uint256.h"
#include "util.h"

#include <algorithm>

const CBlockIndex* GetLastBlockIndex(const CBlockIndex* pindex)
{
    while (pindex && pindex->pprev)
        pindex = pindex->pprev;
    return pindex;
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // Most recent algo first
    if (pindexLast->nHeight + 1 >= params.nHardForkSeven) {
        return DeriveNextWorkRequiredLWMA(pindexLast, pblock, params);
    } else {
        return DeriveNextWorkRequiredDELTA(pindexLast, pblock, params);
    }
}

unsigned int DeriveNextWorkRequiredLWMA(const INDEX_TYPE pindexLast, const BLOCK_TYPE block, const Consensus::Params& params)
{
    const int64_t T = params.GetCurrentPowTargetSpacing(pindexLast->nHeight + 1);

    // For T=600, 300, 150 use approximately N=60, 90, 120
    const int64_t N = 80; // TODO_BCRS_LOW maybe move to params

    // Define a k that will be used to get a proper average after weighting the solvetimes.
    const int64_t k = N * (N + 1) * T / 2;

    const int64_t height = pindexLast->nHeight;
    const arith_uint256 powLimit = UintToArith256(params.powLimit);

    // New coins just "give away" first N blocks. It's better to guess
    // this value instead of using powLimit, but err on high side to not get stuck.
    if (height < N) { return powLimit.GetCompact(); }

    arith_uint256 avgTarget, nextTarget;
    int64_t thisTimestamp, previousTimestamp;
    int64_t sumWeightedSolvetimes = 0, j = 0;

    const CBlockIndex* blockPreviousTimestamp = pindexLast->GetAncestor(height - N);
    previousTimestamp = blockPreviousTimestamp->GetBlockTime();

    // Loop through N most recent blocks.
    for (int64_t i = height - N + 1; i <= height; i++) {
        const CBlockIndex* block = pindexLast->GetAncestor(i);

        // Prevent solvetimes from being negative in a safe way. It must be done like this.
        // Do not attempt anything like  if (solvetime < 1) {solvetime=1;}
        // The +1 ensures new coins do not calculate nextTarget = 0.
        thisTimestamp = (block->GetBlockTime() > previousTimestamp) ? block->GetBlockTime() : previousTimestamp + 1;

        // 6*T limit prevents large drops in diff from long solvetimes which would cause oscillations.
        int64_t solvetime = std::min(6 * T, thisTimestamp - previousTimestamp);

        // The following is part of "preventing negative solvetimes".
        previousTimestamp = thisTimestamp;

        // Give linearly higher weight to more recent solvetimes.
        j++;
        sumWeightedSolvetimes += solvetime * j;

        arith_uint256 target;
        target.SetCompact(block->nBits);
        avgTarget += target / N / k; // Dividing by k here prevents an overflow below.
    }

    // Desired equation in next line was nextTarget = avgTarget * sumWeightSolvetimes / k
    // but 1/k was moved to line above to prevent overflow in new coins
    nextTarget = avgTarget * sumWeightedSolvetimes;

    if (nextTarget > powLimit) { nextTarget = powLimit; }

    return nextTarget.GetCompact();
}

unsigned int DeriveNextWorkRequiredDELTA(const INDEX_TYPE pindexLast, const BLOCK_TYPE block,
                                    const Consensus::Params& params)
{
    int64_t nRetargetTimespan = params.GetCurrentPowTargetSpacing(pindexLast->nHeight + 1);
    const unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    unsigned int initalBlock = 0;
    const unsigned int nLastBlock = 1;
    const unsigned int nShortFrame = 3;
    const unsigned int nMiddleFrame = 24;
    const unsigned int nLongFrame = 576;

    const int64_t nLBWeight = 64;
    const int64_t nShortWeight = 8;
    int64_t nMiddleWeight = 2;
    int64_t nLongWeight = 1;

    const int64_t nLBMinGap = nRetargetTimespan / 6;
    const int64_t nLBMaxGap = nRetargetTimespan * 6;

    const int64_t nQBFrame = nShortFrame + 1;
    const int64_t nQBMinGap = (nRetargetTimespan * PERCENT_FACTOR / 120) * nQBFrame;

    const int64_t nBadTimeLimit = 0;
    const int64_t nBadTimeReplace = nRetargetTimespan / 10;

    const int64_t nLowTimeLimit = nRetargetTimespan * 90 / PERCENT_FACTOR;
    const int64_t nFloorTimeLimit = nRetargetTimespan * 65 / PERCENT_FACTOR;

    const int64_t nDrift = 1;
    int64_t nLongTimeLimit = ((6 * nDrift)) * 60;
    int64_t nLongTimeStep = nDrift * 60;

    unsigned int nMinimumAdjustLimit = (unsigned int)nRetargetTimespan * 75 / PERCENT_FACTOR;

    unsigned int nMaximumAdjustLimit = (unsigned int)nRetargetTimespan * 150 / PERCENT_FACTOR;

    int64_t nDeltaTimespan = 0;
    int64_t nLBTimespan = 0;
    int64_t nShortTimespan = 0;
    int64_t nMiddleTimespan = 0;
    int64_t nLongTimespan = 0;
    int64_t nQBTimespan = 0;

    int64_t nWeightedSum = 0;
    int64_t nWeightedDiv = 0;
    int64_t nWeightedTimespan = 0;

    const INDEX_TYPE pindexFirst = pindexLast; // multi algo - last block is selected on a per algo basis.

    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    if (INDEX_HEIGHT(pindexLast) <= nQBFrame)
        return nProofOfWorkLimit;

    pindexFirst = INDEX_PREV(pindexLast);
    nLBTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(pindexFirst);

    if (nLBTimespan > nBadTimeLimit && nLBTimespan < nLBMinGap)
        nLBTimespan = nLBTimespan * 50 / PERCENT_FACTOR;

    if (nLBTimespan <= nBadTimeLimit)
        nLBTimespan = nBadTimeReplace;

    if (nLBTimespan > nLBMaxGap)
        nLBTimespan = nLBTimespan * 150 / PERCENT_FACTOR;

    pindexFirst = pindexLast;
    for (unsigned int i = 1; pindexFirst != NULL && i <= nQBFrame; i++) {
        nDeltaTimespan = INDEX_TIME(pindexFirst) - INDEX_TIME(INDEX_PREV(pindexFirst));

        if (nDeltaTimespan <= nBadTimeLimit)
            nDeltaTimespan = nBadTimeReplace;

        if (i <= nShortFrame)
            nShortTimespan += nDeltaTimespan;
        nQBTimespan += nDeltaTimespan;
        pindexFirst = INDEX_PREV(pindexFirst);
    }

    if (INDEX_HEIGHT(pindexLast) - initalBlock <= nMiddleFrame) {
        nMiddleWeight = nMiddleTimespan = 0;
    }
    else {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst != NULL && i <= nMiddleFrame; i++) {
            nDeltaTimespan = INDEX_TIME(pindexFirst) - INDEX_TIME(INDEX_PREV(pindexFirst));

            if (nDeltaTimespan <= nBadTimeLimit)
                nDeltaTimespan = nBadTimeReplace;

            nMiddleTimespan += nDeltaTimespan;
            pindexFirst = INDEX_PREV(pindexFirst);
        }
    }

    if (INDEX_HEIGHT(pindexLast) - initalBlock <= nLongFrame) {
        nLongWeight = nLongTimespan = 0;
    }
    else {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst != NULL && i <= nLongFrame; i++)
            pindexFirst = INDEX_PREV(pindexFirst);

        nLongTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(pindexFirst);
    }

    if ((nQBTimespan > nBadTimeLimit) && (nQBTimespan < nQBMinGap) && (nLBTimespan < nRetargetTimespan * 40 / PERCENT_FACTOR)) {
        nMiddleWeight = nMiddleTimespan = nLongWeight = nLongTimespan = 0;
    }

    nWeightedSum = (nLBTimespan * nLBWeight) + (nShortTimespan * nShortWeight);
    nWeightedSum += (nMiddleTimespan * nMiddleWeight) + (nLongTimespan * nLongWeight);
    nWeightedDiv = (nLastBlock * nLBWeight) + (nShortFrame * nShortWeight);
    nWeightedDiv += (nMiddleFrame * nMiddleWeight) + (nLongFrame * nLongWeight);
    nWeightedTimespan = nWeightedSum / nWeightedDiv;

    if (DIFF_ABS(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 20 / PERCENT_FACTOR) {
        nMinimumAdjustLimit = (unsigned int)nRetargetTimespan * 90 / PERCENT_FACTOR;
        nMaximumAdjustLimit = (unsigned int)nRetargetTimespan * 110 / PERCENT_FACTOR;
    }
    else if (DIFF_ABS(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 30 / PERCENT_FACTOR) {
        nMinimumAdjustLimit = (unsigned int)nRetargetTimespan * 80 / PERCENT_FACTOR;
        nMaximumAdjustLimit = (unsigned int)nRetargetTimespan * 120 / PERCENT_FACTOR;
    }

    if (nWeightedTimespan < nMinimumAdjustLimit)
        nWeightedTimespan = nMinimumAdjustLimit;

    if (nWeightedTimespan > nMaximumAdjustLimit)
        nWeightedTimespan = nMaximumAdjustLimit;

    arith_uint256 bnNew;
    SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
    bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(nWeightedTimespan));
    bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(nRetargetTimespan));

    nLBTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(INDEX_PREV(pindexLast));
    arith_uint256 bnComp;
    SET_COMPACT(bnComp, INDEX_TARGET(pindexLast));
    if (nLBTimespan > 0 && nLBTimespan < nLowTimeLimit && BIGINT_GREATER_THAN(bnNew, bnComp)) {
        if (nLBTimespan < nFloorTimeLimit) {
            SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
            bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(95));
            bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(PERCENT_FACTOR));
        }
        else {
            SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
        }
    }

    if ((BLOCK_TIME(block) - INDEX_TIME(pindexLast)) > nLongTimeLimit) {
        int64_t nNumMissedSteps = ((BLOCK_TIME(block) - INDEX_TIME(pindexLast) - nLongTimeLimit) / nLongTimeStep) + 1;
        for (int i = 0; i < nNumMissedSteps; ++i) {
            bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(110));
            bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(PERCENT_FACTOR));
        }
    }

    SET_COMPACT(bnComp, nProofOfWorkLimit);
    if (BIGINT_GREATER_THAN(bnNew, bnComp))
        SET_COMPACT(bnNew, nProofOfWorkLimit);

    return GET_COMPACT(bnNew);
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return error("CheckProofOfWork(): nBits below minimum work");

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return error("CheckProofOfWork(): hash doesn't match nBits");

    return true;
}

arith_uint256 GetBlockProof(const CBlockIndex& block)
{
    arith_uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a arith_uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}

int64_t GetBlockProofEquivalentTime(const CBlockIndex& to, const CBlockIndex& from, const CBlockIndex& tip, const Consensus::Params& params)
{
    arith_uint256 r;
    int sign = 1;
    if (to.nChainWork > from.nChainWork) {
        r = to.nChainWork - from.nChainWork;
    } else {
        r = from.nChainWork - to.nChainWork;
        sign = -1;
    }

    r = r * arith_uint256(params.GetCurrentPowTargetSpacing(tip.nHeight)) / GetBlockProof(tip);
    if (r.bits() > 63) {
        return sign * std::numeric_limits<int64_t>::max();
    }
    return sign * r.GetLow64();
}
