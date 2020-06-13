#pragma once
//im lazy
#define KK_1  0x31
#define KK_2  0x32
#define KK_3  0x33
#define KK_4  0x34
#define KK_5  0x35
#define KK_6  0x36
#define KK_7  0x37
#define KK_8  0x38
#define KK_9  0x38
#define KK_C  0x43
#define KK_X  0x58

//voices
//page 1
std::vector<WORD> vYes{ KK_C, KK_1 };
std::vector<WORD> vNo{ KK_C, KK_2 };
std::vector<WORD> vHelp{ KK_C, KK_3 };
std::vector<WORD> vInsult{ KK_C, KK_4 };
std::vector<WORD> vIntimidate{ KK_C, KK_5 };

//page 2
std::vector<WORD> vSorry{ KK_C, KK_C, KK_1 };
std::vector<WORD> vLaugh{ KK_C, KK_C, KK_2 };
std::vector<WORD> vThank{ KK_C, KK_C, KK_3 };
std::vector<WORD> vFriendlies{ KK_C, KK_C, KK_4 };
std::vector<WORD> vRetreat{ KK_C, KK_C, KK_5 };

//page 3
std::vector<WORD> vHold{ KK_C, KK_C, KK_C, KK_1 };
std::vector<WORD> vHello{ KK_C, KK_C, KK_C, KK_2 };
std::vector<WORD> vFollow{ KK_C, KK_C, KK_C, KK_3 };
std::vector<WORD> vRespect{ KK_C, KK_C, KK_C, KK_4 };
std::vector<WORD> vCharge{ KK_C, KK_C, KK_C, KK_5 };

//emotes
//page 1
std::vector<WORD> vFlourish{ KK_X, KK_1 };
std::vector<WORD> vRaiseWeapon{ KK_X, KK_2 };
std::vector<WORD> vSwordSalute{ KK_X, KK_3 };
std::vector<WORD> vComeAtMe{ KK_X, KK_4 };
std::vector<WORD> vThroatCut{ KK_X, KK_5 };
std::vector<WORD> vSalute{ KK_X, KK_6 };
std::vector<WORD> vFacepalm{ KK_X, KK_7 };
std::vector<WORD> vSquat{ KK_X, KK_8 };
std::vector<WORD> vYelling{ KK_X, KK_9 };

//page 2
std::vector<WORD> vCheer{ KK_X, KK_X, KK_1 };
std::vector<WORD> vDance{ KK_X, KK_X, KK_2 };
std::vector<WORD> vDisapprove{ KK_X, KK_X, KK_3 };
std::vector<WORD> vShakeFist{ KK_X, KK_X, KK_4 };
std::vector<WORD> vShrug{ KK_X, KK_X, KK_5 };
std::vector<WORD> vWhatever{ KK_X, KK_X, KK_6 };
std::vector<WORD> vRoar{ KK_X, KK_X, KK_7 };
std::vector<WORD> vYield{ KK_X, KK_X, KK_8 };
std::vector<WORD> vCower{ KK_X, KK_X, KK_9 };

//page 3
std::vector<WORD> vLaughing{ KK_X, KK_X, KK_X, KK_1 };
std::vector<WORD> vPointBack{ KK_X, KK_X, KK_X, KK_2 };
std::vector<WORD> vItsAllOver{ KK_X, KK_X, KK_X, KK_3 };
std::vector<WORD> vBow{ KK_X, KK_X, KK_X, KK_4 };
std::vector<WORD> vCharge2{ KK_X, KK_X, KK_X, KK_5 };
