// Copyright (c) 2009-2019 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Developers
// Copyright (c) 2014-2019 The Dash Core Developers
// Copyright (c) 2016-2019 Duality Blockchain Solutions Developers
// Copyright (c) 2017-2019 Bitcreds Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Functionality for communicating with Tor.
 */
#ifndef BITCREDS_TORCONTROL_H
#define BITCREDS_TORCONTROL_H

#include "scheduler.h"

extern const std::string DEFAULT_TOR_CONTROL;
static const bool DEFAULT_LISTEN_ONION = true;

void StartTorControl(boost::thread_group& threadGroup, CScheduler& scheduler);
void InterruptTorControl();
void StopTorControl();

#endif // BITCREDS_TORCONTROL_H
