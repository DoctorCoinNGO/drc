// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2017-2018 The Proton Core developers
// Copyright (c) 2018 The DRC Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "clientversion.h"
#include "rpcserver.h"
#include "init.h"
#include "noui.h"
#include "scheduler.h"
#include "util.h"
#include "masternodeconfig.h"
#include "httpserver.h"
#include "httprpc.h"
#include "rpcserver.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <stdio.h>



// ==============================
// #include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"
#include <boost/assign/list_of.hpp>



static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "help the homeless";
    // const char* pszTimestamp = "Love is all around";
    const CScript genesisOutputScript = CScript() << ParseHex("040a3ada5ba6280b99f49a92ba47221e6a72af844ec49d0c8bbdae1ec09a4c79b22e42eefe670ae04490556f91780eb57de76493d020c91d0c421c2fa052b28a2b") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

// ====================================

/* Introduction text for doxygen: */

/*! \mainpage Developer documentation
 *
 * \section intro_sec Introduction
 *
 * This is the developer documentation of the reference client for an experimental new digital currency called DRC (http://www.doctorcoin.cc//),
 * which enables instant payments to anyone, anywhere in the world. DRC uses peer-to-peer technology to operate
 * with no central authority: managing transactions and issuing money are carried out collectively by the network.
 *
 * The software is a community-driven open source project, released under the MIT license.
 *
 * \section Navigation
 * Use the buttons <code>Namespaces</code>, <code>Classes</code> or <code>Files</code> at the top of the page to start navigating the code.
 */

static bool fDaemon;

// void WaitForShutdown(boost::thread_group* threadGroup)
// {
//     bool fShutdown = ShutdownRequested();
//     // Tell the main threads to shutdown.
//     while (!fShutdown)
//     {
//         MilliSleep(200);
//         fShutdown = ShutdownRequested();
//     }
//     if (threadGroup)
//     {
//         Interrupt(*threadGroup);
//         threadGroup->join_all();
//     }
// }

//////////////////////////////////////////////////////////////////////////////
//
// Start
//
// bool AppInit(int argc, char* argv[])
// {
//     boost::thread_group threadGroup;
//     CScheduler scheduler;
//
//     bool fRet = false;
//
//     //
//     // Parameters
//     //
//     // If Qt is used, parameters/drc.conf are parsed in qt/drc.cpp's main()
//     ParseParameters(argc, argv);
//
//     // Process help and version before taking care about datadir
//     if (mapArgs.count("-?") || mapArgs.count("-h") ||  mapArgs.count("-help") || mapArgs.count("-version"))
//     {
//         std::string strUsage = _("DRC Core Daemon") + " " + _("version") + " " + FormatFullVersion() + "\n";
//
//         if (mapArgs.count("-version"))
//         {
//             strUsage += LicenseInfo();
//         }
//         else
//         {
//             strUsage += "\n" + _("Usage:") + "\n" +
//                   "  drcd [options]                     " + _("Start DRC Core Daemon") + "\n";
//
//             strUsage += "\n" + HelpMessage(HMM_BITCOIND);
//         }
//
//         fprintf(stdout, "%s", strUsage.c_str());
//         return true;
//     }
//
//     try
//     {
//         if (!boost::filesystem::is_directory(GetDataDir(false)))
//         {
//             fprintf(stderr, "Error: Specified data directory \"%s\" does not exist.\n", mapArgs["-datadir"].c_str());
//             return false;
//         }
//         try
//         {
//             ReadConfigFile(mapArgs, mapMultiArgs);
//         } catch (const std::exception& e) {
//             fprintf(stderr,"Error reading configuration file: %s\n", e.what());
//             return false;
//         }
//         // Check for -testnet or -regtest parameter (Params() calls are only valid after this clause)
//         try {
//             SelectParams(ChainNameFromCommandLine());
//         } catch (const std::exception& e) {
//             fprintf(stderr, "Error: %s\n", e.what());
//             return false;
//         }
//
//         // parse masternode.conf
//         std::string strErr;
//         if(!masternodeConfig.read(strErr)) {
//             fprintf(stderr,"Error reading masternode configuration file: %s\n", strErr.c_str());
//             return false;
//         }
//
//         // Command-line RPC
//         bool fCommandLine = false;
//         for (int i = 1; i < argc; i++)
//             if (!IsSwitchChar(argv[i][0]) && !boost::algorithm::istarts_with(argv[i], "drc:"))
//                 fCommandLine = true;
//
//         if (fCommandLine)
//         {
//             fprintf(stderr, "Error: There is no RPC client functionality in drcd anymore. Use the drc-cli utility instead.\n");
//             exit(EXIT_FAILURE);
//         }
// #ifndef WIN32
//         fDaemon = GetBoolArg("-daemon", false);
//         if (fDaemon)
//         {
//             fprintf(stdout, "DRC Core server starting\n");
//
//             // Daemonize
//             pid_t pid = fork();
//             if (pid < 0)
//             {
//                 fprintf(stderr, "Error: fork() returned %d errno %d\n", pid, errno);
//                 return false;
//             }
//             if (pid > 0) // Parent process, pid is child process id
//             {
//                 return true;
//             }
//             // Child process falls through to rest of initialization
//
//             pid_t sid = setsid();
//             if (sid < 0)
//                 fprintf(stderr, "Error: setsid() returned %d errno %d\n", sid, errno);
//         }
// #endif
//         SoftSetBoolArg("-server", true);
//
//         // Set this early so that parameter interactions go to console
//         InitLogging();
//         InitParameterInteraction();
//         fRet = AppInit2(threadGroup, scheduler);
//     }
//     catch (const std::exception& e) {
//         PrintExceptionContinue(&e, "AppInit()");
//     } catch (...) {
//         PrintExceptionContinue(NULL, "AppInit()");
//     }
//
//     if (!fRet)
//     {
//         Interrupt(threadGroup);
//         // threadGroup.join_all(); was left out intentionally here, because we didn't re-test all of
//         // the startup-failure cases to make sure they don't result in a hang due to some
//         // thread-blocking-waiting-for-another-thread-during-startup case
//     } else {
//         WaitForShutdown(&threadGroup);
//     }
//     Shutdown();
//
//     return fRet;
// }

int main(int argc, char* argv[])
{
    printf("hello ....\n");
    CBlock genesis = CreateGenesisBlock(1529878570, 749203, 0x1e0ffff0, 1, 50 * COIN);
    printf("genesis.GetHash = %064x\n ", genesis.GetHash());
    printf("genesis.hashMerkleRoot = %064x\n ", genesis.hashMerkleRoot);
    return 0;

    // SetupEnvironment();
    //
    // // Connect drcd signal handlers
    // noui_connect();
    //
    // return (AppInit(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE);
}
