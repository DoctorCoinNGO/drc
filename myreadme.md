0. 修改 dnsseed

```java
// src/chainparams.cpp  line 170

// 修改dns起始种子  
vSeeds.push_back(CDNSSeedData("drcseed1", "seed.drccoin.world"));
vSeeds.push_back(CDNSSeedData("drcseed1", "seed2.drccoin.world"));
// 改成我们这边

vSeeds.push_back(CDNSSeedData("drcseed1", "seed1.powpool.cn"));
vSeeds.push_back(CDNSSeedData("drcseed1", "seed2.powpool.cn"));

// src/util.cpp  line 673

// 修改种子配置文件
////	fprintf(configFile,"addnode=159.65.152.125\r\n");
	// fprintf(configFile,"addnode=167.99.217.206\r\n");
	// fprintf(configFile,"addnode=167.99.190.68\r\n");
	// fprintf(configFile,"addnode=138.68.1.181\r\n");
	// fprintf(configFile,"addnode=138.68.156.199\r\n");
	// fprintf(configFile,"addnode=206.189.98.150\r\n");  


// TONDO .. 添加我们的自己的种子节点:

// 144.202.3.87
// 104.238.162.228

fprintf(configFile,"addnode=144.202.3.87\r\n");
fprintf(configFile,"addnode=199.247.19.37\r\n");
```


1. block time 60s

```c
// src/chainparams.cpp line  68.
// 计算减半块数
consensus.nSubsidyHalvingInterval = 1036800; // every130k blocks

// src/chainparams.cpp line 89. 90.
consensus.nPowTargetTimespan = 1 * 60; // DRC: every blocks. re-targeting every one hour
consensus.nPowTargetSpacing = 1 * 60; // DRC:  do new pow every 1 minutes.

// TODO 此处，我们仍然使用上面的，1分钟出块一次，但是对于每个块的奖励，可以设置少一些

// 计算减半块数
consensus.nSubsidyHalvingInterval = 60 * 24 * 365; // decrease every year


// test， regress

```

2. port


```c
// src/chainparams.cpp

// line 119   online
nDefaultPort = 35888;  // p2p port

// line test
nDefaultPort = 17717;  // p2p port

// regest
nDefaultPort = 17617;  // p2p port
// line


// TODO update ... each add +1


// line 119   online
nDefaultPort = 35890;  // p2p port

// line test
nDefaultPort = 17719;  // p2p port

// regest
nDefaultPort = 17619;  // p2p port
// line



// 修改RPC端口
// src/chainparamsbase.cpp

// nRPCPort = 9215;
nRPCPort = 9217;

```

3. block identity

```c

  // line 114  区块链唯一标识符  /src/chainparams.cpp

  pchMessageStart[0] = 0x6a;
  pchMessageStart[1] = 0xb1;
  pchMessageStart[2] = 0x9c;
  pchMessageStart[3] = 0xd5;

  // TODO 修改 add + 1

  pchMessageStart[0] = 0x6b;
  pchMessageStart[1] = 0xb2;
  pchMessageStart[2] = 0x9d;
  pchMessageStart[3] = 0xd6;


// 修改 地址前缀修饰符
// DRC addresses start with 'H' ABCDEFG HIJK
base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,40);
// DRC script addresses start with '7'
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
// DRC private keys start with 'E'
base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128+40);

修改为:

// DRC addresses start with 'H' ABCDEFG HIJK
base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
// DRC script addresses start with '7'
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
// DRC private keys start with 'E'
base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128+30);

```


3. block reward  2500( halving every 2 years)

```c
// src/main.cpp line 1755

CAmount nSubsidy = 2500 * COIN;

// TODO update
CAmount nSubsidy = 12 * COIN;
```

4. masternode collateral （MN的担保币）  2500 000

```c
// src/activemasternode.cpp line 232
collat_required = 2500000 * COIN;

// src/masternode.cpp line 645
collat_required = 2500000 * COIN;

// src/wallet.cpp search 2500000

// src/darksend.cpp


// TODO update 2500000 to  1000 * COIN;
collat_required = 1000 * COIN;


// src/wallet.cpp
if(fMasterNode && wtx.vout[i].nValue == 1000*COIN) continue;

// if(fMasterNode && (wtx.vout[i].nValue == 100 * COIN || wtx.vout[i].nValue == 2500000 * COIN )) continue;

// line 2683
// if(fMasterNode && (out.tx->vout[out.i].nValue == 2500000*COIN || out.tx->vout[out.i].nValue == 100*COIN)) continue; //masternode input
if(fMasterNode && out.tx->vout[out.i].nValue == 1000 * COIN ) continue; //masternode input


```



5. masternode reward ( 25% of block reward after charity & dev fees)

```c
//  src/main.cpp  line 1770
CAmount GetMasternodePayment(int nHeight, CAmount blockValue)
{
//if (chainActive.Height() <= 24804)
    if ((nHeight-1) % 100 == 0 && nHeight > 2){
	return blockValue * .9709;
	}
if (chainActive.Height() <= 24804){
    return 0.0000001 * COIN;
} else {
	return blockValue * .25;
}
return blockValue * .25;
}

// 修改币总量   src/amount.h
// static const CAmount MAX_MONEY = 21000000 * COIN;
static const CAmount MAX_MONEY = 15000000 * COIN;


// TODO ...
CAmount GetMasternodePayment(int nHeight, CAmount blockValue)
{
  return blockValue / 2;
}

```

6. PoW Reward (75% of block reward after charity & dev fees)

```java
// src/main.cpp line 1782
CAmount GetMasternodePayment(int nHeight, CAmount blockValue)
{
  return blockValue / 2;
}
```




7. Anti Instamine: first 500 blocks will reward 250 DRC


```java
//  src/main.cpp  line 1746
  CAmount GetBlockSubsidy(int nPrevBits, int nPrevHeight, const Consensus::Params& consensusParams, bool fSuperblockPartOnly)
  {

    if (nPrevHeight < 500) {
  	return 250 * COIN;
      }

      CAmount nSubsidy = 2500 * COIN;

      // yearly decline of production by 25% per 3 months.
      for (int i = consensusParams.nSubsidyHalvingInterval; i <= nPrevHeight; i += consensusParams.nSubsidyHalvingInterval) {
          nSubsidy -= nSubsidy * 0.5;
      }

      if(((nPrevHeight) % 100 == 0) && nPrevHeight >= 3) {
           nSubsidy = ((nSubsidy*.25) * 100) + (nSubsidy * .75);
      }
      else {
   	        nSubsidy * = .75;
      }
      return fSuperblockPartOnly ? 0 : nSubsidy;
  }


// TODO

CAmount GetBlockSubsidy(int nPrevBits, int nPrevHeight, const Consensus::Params& consensusParams, bool fSuperblockPartOnly)
{
    // 处理预挖
    if (nPrevHeight == 0) {
      //  return 3400000 * COIN;
        return 3750000 * COIN;  // 总量的  2.5%
    }

    //  comment by Tom
    // CAmount nSubsidy = 2500 * COIN;
    CAmount nSubsidy = 12 * COIN;  //

    for (int i = consensusParams.nSubsidyHalvingInterval; i <= nPrevHeight; i += consensusParams.nSubsidyHalvingInterval) {
        nSubsidy -= nSubsidy/2;
    }

    return nSubsidy;
}
```





8. Mature time 360 blocks


```c
// src/consensus/consensus.h  line 14
/** Coinbase transaction outputs can only be spent after this number of new blocks (network rule) */
static const int COINBASE_MATURITY = 60;

// TODO ....  update  30.o
static const int COINBASE_MATURITY = 30;
```


```c

// src/qt/transactionrecord.h line 92


/** Number of confirmation recommended for accepting a transaction */
static const int RecommendedNumConfirmations = 6;

```


9. 修改创始块


```


```


10. 处理版本号

```java

// TODO


// src.version.h

// 钱包版本 1.2.0 处理
// search 1.2.0 再处理



```


端口处理


'''bash



'''



#### 命题替换处理操作

1. 参考: https://blog.csdn.net/mjfh095215/article/details/80250487


#### 捐献地址 (drc)


```java
// src/masternode-payments.cpp line 288
    CBitcoinAddress VfundAddress("DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW");
    CBitcoinAddress VfundAddress_fork("HSmv2N2DEkyjrHzqjH459qWLfJ9uFahHHM");

// src/masternode-payments.cpp line 588

    CBitcoinAddress VfundAddress2("DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW");
    CBitcoinAddress VfundAddress_forkk("HSmv2N2DEkyjrHzqjH459qWLfJ9uFahHHM");
//}

// && /src/masternodeman.cpp line 572  

  CBitcoinAddress VfundAddress2("DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW");
  CScript VfundPayee2 = GetScriptForDestination(VfundAddress2.Get());

  if (VfundPayee2 == txout.scriptPubKey && nMasternodePayment == txout.nValue) {
      return true;
  }
  //  /src/masternodeman.cpp line 698
  if ( strPayee != "DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW"){
    LogPrintf("Sadness. :(\n");
    return -1;
  }else{
    LogPrintf("Selected MN collateral as rank\n");
    return 1;
  }

// TODO 修改捐款地址。。。。。。。

// author: DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW
// mytest addr:   DMYZMjSxCfHw4xHjETRps9A2b1wBcDEWqW

```




10. 修改所有名字，将 drc 修改为 drc


```bash

# 修改内容
find . -type f -print0 | xargs -0 sed -i 's/hth/drc/g'
find . -type f -print0 | xargs -0 sed -i 's/Drc/Drc/g'
find . -type f -print0 | xargs -0 sed -i 's/DRC/DRC/g'

# 修改文件名
find . -exec rename 's/hth/drc/' {} ";"
find . -exec rename 's/Hth/Drc/' {} ";"
find . -exec rename 's/HTH/DRC/' {} ";"


find . -type f -print0 | xargs -0 sed -i 's/8332/9443/' {} ";"
find . -type f -print0 | xargs -0 sed -i 's/8333/9444/' {} ";"
```


总量: 1500W
出块: 2min => 1min
每块出币： 12个
抽成： 1% + （50% MN + 50% POW)
减半: 每年减半
预挖: 2.5 %
MN最小币数： 1K
创始块：



### 安装程序


参考这个： https://blog.csdn.net/gammag/article/details/73702302


```shell
sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install libdb4.8-dev libdb4.8++-dev

sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev libevent-dev libboost-all-dev libminiupnpc-dev libqt4-dev libprotobuf-dev protobuf-compiler libqrencode-dev automake   libzmq3-dev

sudo apt-get install libdb5.1++-dev

sudo apt-get install libdb-dev libdb++-dev

sudo apt-get install libboost-all-dev

# install qt 5
# sudo apt-get install libqt4-core libqt4-gui libqt4-dev
sudo apt-get install cmake qt5-default qtcreator

sudo apt-get install libssl-dev

# start
sudo ./autogen.sh

# configure
./configure --with-incompatible-bdb

#  for linux server
# ./configure --with-incompatible-bdb  --disable-wallet --without-gui

# compile
make


# 如果编译过程中，遇到内存不足的情况下

sudo dd if=/dev/zero of=/swapfile bs=64M count=16
sudo mkswap /swapfile
sudo swapon /swapfile

# After compiling, you may wish to

# Code:
sudo swapoff /swapfile
sudo rm /swapfile
```




#### 附件说明


1. 关于地址前缀的操作

```java
// 修改 chainparams.cpp
base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,40);
// DRC script addresses start with '7'
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
// DRC private keys start with 'E'
base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128+40);

//  for more: https://en.bitcoin.it/wiki/List_of_address_prefixes
```



```js
var cs = require('coinstring')

var hash160L = "0000000000000000000000000000000000000000" //hash representing uncompressed
var hash160H = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" //hash representing uncompressed
function startsWith(version) {
        var hash160Buf = new Buffer(hash160L, 'hex')
        lowEnd = cs.encode(hash160Buf, version).charAt(0);
        hash160Buf = new Buffer(hash160H, 'hex')
        highEnd = cs.encode(hash160Buf, version).charAt(0);
        if(lowEnd == highEnd) {
                return lowEnd;
        } else {
                return [lowEnd, highEnd];
        }
}
var version = 0x00; //Bitcoin public address
for(var i = 0; i < 256; i++) {
        console.log(i + " will produce addresses starting with: " + startsWith(i));
}

// -----------------------------------------------------------------------------
// output
// 0 will produce addresses starting with: 1
// 1 will produce addresses starting with: Q,o
// 2 will produce addresses starting with: o,2
// 3 will produce addresses starting with: 2
// 4 will produce addresses starting with: 2,3
// 5 will produce addresses starting with: 3
// 6 will produce addresses starting with: 3
// 7 will produce addresses starting with: 3,4
// 8 will produce addresses starting with: 4
// 9 will produce addresses starting with: 4,5
// 10 will produce addresses starting with: 5
// 11 will produce addresses starting with: 5
// 12 will produce addresses starting with: 5,6
// 13 will produce addresses starting with: 6
// 14 will produce addresses starting with: 6,7
// 15 will produce addresses starting with: 7
// 16 will produce addresses starting with: 7
// 17 will produce addresses starting with: 7,8
// 18 will produce addresses starting with: 8
// 19 will produce addresses starting with: 8,9
// 20 will produce addresses starting with: 9
// 21 will produce addresses starting with: 9
// 22 will produce addresses starting with: 9,A
// 23 will produce addresses starting with: A
// 24 will produce addresses starting with: A,B
// 25 will produce addresses starting with: B
// 26 will produce addresses starting with: B
// 27 will produce addresses starting with: B,C
// 28 will produce addresses starting with: C
// 29 will produce addresses starting with: C,D
// 30 will produce addresses starting with: D
// 31 will produce addresses starting with: D
// 32 will produce addresses starting with: D,E
// 33 will produce addresses starting with: E
// 34 will produce addresses starting with: E,F
// 35 will produce addresses starting with: F
// 36 will produce addresses starting with: F
// 37 will produce addresses starting with: F,G
// 38 will produce addresses starting with: G
// 39 will produce addresses starting with: G,H
// 40 will produce addresses starting with: H
// 41 will produce addresses starting with: H
// 42 will produce addresses starting with: H,J
// 43 will produce addresses starting with: J
// 44 will produce addresses starting with: J,K
// 45 will produce addresses starting with: K
// 46 will produce addresses starting with: K
// 47 will produce addresses starting with: K,L
// 48 will produce addresses starting with: L
// 49 will produce addresses starting with: L,M
// 50 will produce addresses starting with: M
// 51 will produce addresses starting with: M
// 52 will produce addresses starting with: M,N
// 53 will produce addresses starting with: N
// 54 will produce addresses starting with: N,P
// 55 will produce addresses starting with: P
// 56 will produce addresses starting with: P
// 57 will produce addresses starting with: P,Q
// 58 will produce addresses starting with: Q
// 59 will produce addresses starting with: Q,R
// 60 will produce addresses starting with: R
// 61 will produce addresses starting with: R
// 62 will produce addresses starting with: R,S
// 63 will produce addresses starting with: S
// 64 will produce addresses starting with: S,T
// 65 will produce addresses starting with: T
// 66 will produce addresses starting with: T
// 67 will produce addresses starting with: T,U
// 68 will produce addresses starting with: U
// 69 will produce addresses starting with: U,V
// 70 will produce addresses starting with: V
// 71 will produce addresses starting with: V
// 72 will produce addresses starting with: V,W
// 73 will produce addresses starting with: W
// 74 will produce addresses starting with: W,X
// 75 will produce addresses starting with: X
// 76 will produce addresses starting with: X
// 77 will produce addresses starting with: X,Y
// 78 will produce addresses starting with: Y
// 79 will produce addresses starting with: Y,Z
// 80 will produce addresses starting with: Z
// 81 will produce addresses starting with: Z
// 82 will produce addresses starting with: Z,a
// 83 will produce addresses starting with: a
// 84 will produce addresses starting with: a,b
// 85 will produce addresses starting with: b
// 86 will produce addresses starting with: b,c
// 87 will produce addresses starting with: c
// 88 will produce addresses starting with: c
// 89 will produce addresses starting with: c,d
// 90 will produce addresses starting with: d
// 91 will produce addresses starting with: d,e
// 92 will produce addresses starting with: e
// 93 will produce addresses starting with: e
// 94 will produce addresses starting with: e,f
// 95 will produce addresses starting with: f
// 96 will produce addresses starting with: f,g
// 97 will produce addresses starting with: g
// 98 will produce addresses starting with: g
// 99 will produce addresses starting with: g,h
// 100 will produce addresses starting with: h
// 101 will produce addresses starting with: h,i
// 102 will produce addresses starting with: i
// 103 will produce addresses starting with: i
// 104 will produce addresses starting with: i,j
// 105 will produce addresses starting with: j
// 106 will produce addresses starting with: j,k
// 107 will produce addresses starting with: k
// 108 will produce addresses starting with: k
// 109 will produce addresses starting with: k,m
// 110 will produce addresses starting with: m
// 111 will produce addresses starting with: m,n
// 112 will produce addresses starting with: n
// 113 will produce addresses starting with: n
// 114 will produce addresses starting with: n,o
// 115 will produce addresses starting with: o
// 116 will produce addresses starting with: o,p
// 117 will produce addresses starting with: p
// 118 will produce addresses starting with: p
// 119 will produce addresses starting with: p,q
// 120 will produce addresses starting with: q
// 121 will produce addresses starting with: q,r
// 122 will produce addresses starting with: r
// 123 will produce addresses starting with: r
// 124 will produce addresses starting with: r,s
// 125 will produce addresses starting with: s
// 126 will produce addresses starting with: s,t
// 127 will produce addresses starting with: t
// 128 will produce addresses starting with: t
// 129 will produce addresses starting with: t,u
// 130 will produce addresses starting with: u
// 131 will produce addresses starting with: u,v
// 132 will produce addresses starting with: v
// 133 will produce addresses starting with: v
// 134 will produce addresses starting with: v,w
// 135 will produce addresses starting with: w
// 136 will produce addresses starting with: w,x
// 137 will produce addresses starting with: x
// 138 will produce addresses starting with: x
// 139 will produce addresses starting with: x,y
// 140 will produce addresses starting with: y
// 141 will produce addresses starting with: y,z
// 142 will produce addresses starting with: z
// 143 will produce addresses starting with: z
// 144 will produce addresses starting with: z,2
// 145 will produce addresses starting with: 2
// 146 will produce addresses starting with: 2
// 147 will produce addresses starting with: 2
// 148 will produce addresses starting with: 2
// ....  will produce addresses starting with: 2
```
