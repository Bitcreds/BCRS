// Copyright (c) 2017-2021 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCREDS_DTPDB_H
#define BITCREDS_DTPDB_H

#include "dbwrapper.h"

#include <string>
#include <utility>

/** Access to the DTP database (dtp/) */
class CDTPDB : public CDBWrapper
{
public:
    CDTPDB(size_t nCacheSize, bool fMemory = false, bool fWipe = false);

    std::string GetIPFSofDTP(const std::string &dtpAddress);
    bool ReadDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight);
    bool WriteDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight);
    bool UpdateDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash);
    bool EraseDTPAssociation(const std::string &dtpAddress);
};

#endif // BITCREDS_DTPDB_H