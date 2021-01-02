// Copyright (c) 2017-2021 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "dtpdb.h"

#include "util.h"

CDTPDB::CDTPDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "dtp", nCacheSize, fMemory, fWipe) {}

std::string CDTPDB::GetIPFSofDTP(const std::string &dtpAddress) {
    std::pair<std::string, int> storedValue;
    
    if (Read(dtpAddress, storedValue))
        return storedValue.first;

    return "Not Found";
}

bool CDTPDB::ReadDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight) {
    std::pair<std::string, int> storedValue;

    if (Read(dtpAddress, storedValue)) {
        ipfsHash = storedValue.first;
        blockHeight = storedValue.second;
        
        return true;
    }

    return false;
}

bool CDTPDB::WriteDTPAssociation(const std::string &dtpAddress, std::string &ipfsHash, int &blockHeight) {
    CDBBatch batch(&GetObfuscateKey());
    batch.Write(dtpAddress, std::make_pair(ipfsHash, blockHeight));
    return WriteBatch(batch);
}

bool CDTPDB::UpdateDTPAssociation(const std::string &dtpAddress, std::string &newIpfsHash) {
    std::pair<std::string, int> storedValue;
    
    if (Read(dtpAddress, storedValue)) {
        CDBBatch batch(&GetObfuscateKey());
        batch.Write(dtpAddress, std::make_pair(newIpfsHash, storedValue.second));
        return WriteBatch(batch);
    }

    return false;
}

bool CDTPDB::EraseDTPAssociation(const std::string &dtpAddress) {
    CDBBatch batch(&GetObfuscateKey());
    batch.Erase(dtpAddress);
    return WriteBatch(batch);
}