/****************************************************************************
*                                                                           *
* Project64 - A Nintendo 64 emulator.                                      *
* http://www.pj64-emu.com/                                                  *
* Copyright (C) 2012 Project64. All rights reserved.                        *
*                                                                           *
* License:                                                                  *
* GNU/GPLv2 http://www.gnu.org/licenses/gpl-2.0.html                        *
*                                                                           *
****************************************************************************/
#include "stdafx.h"

#if defined(__arm__) || defined(_M_ARM)
#include <Project64-core/N64System/SystemGlobals.h>
#include <Project64-core/N64System/N64Class.h>
#include <Project64-core/N64System/Recompiler/RecompilerCodeLog.h>
#include <Project64-core/N64System/Recompiler/Arm/ArmRegInfo.h>

CArmRegInfo::CArmRegInfo() :
m_InCallDirect(false)
{
    for (int32_t i = 0, n = sizeof(m_ArmReg_MappedTo) / sizeof(m_ArmReg_MappedTo[0]); i < n; i++)
    {
        m_ArmReg_Protected[i] = false;
        m_ArmReg_MappedTo[i] = NotMapped;
        m_Variable_MappedTo[i] = VARIABLE_UNKNOWN;
    }
}

CArmRegInfo::CArmRegInfo(const CArmRegInfo& rhs)
{
    *this = rhs;
}

CArmRegInfo::~CArmRegInfo()
{
}

CArmRegInfo& CArmRegInfo::operator=(const CArmRegInfo& right)
{
    CRegBase::operator=(right);

    m_InCallDirect = right.m_InCallDirect;
    memcpy(&m_ArmReg_Protected, &right.m_ArmReg_Protected, sizeof(m_ArmReg_Protected));
    memcpy(&m_ArmReg_MappedTo, &right.m_ArmReg_MappedTo, sizeof(m_ArmReg_MappedTo));
    memcpy(&m_Variable_MappedTo, &right.m_Variable_MappedTo, sizeof(m_Variable_MappedTo));
#ifdef _DEBUG
    if (*this != right)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
#endif
    return *this;
}

void CArmRegInfo::BeforeCallDirect(void)
{
    if (m_InCallDirect)
    {
        CPU_Message("%s: in CallDirect", __FUNCTION__);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_InCallDirect = true;
}

void CArmRegInfo::AfterCallDirect(void)
{
    if (!m_InCallDirect)
    {
        CPU_Message("%s: Not in CallDirect", __FUNCTION__);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_InCallDirect = false;
}

CArmOps::ArmReg CArmRegInfo::FreeArmReg()
{
    if (m_InCallDirect)
    {
        CPU_Message("%s: in CallDirect",__FUNCTION__);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return Arm_Unknown;
    }
    if ((GetArmRegMapped(Arm_R7) == NotMapped || GetArmRegMapped(Arm_R7) == Temp_Mapped) && !GetArmRegProtected(Arm_R7)) { return Arm_R7; }
    if ((GetArmRegMapped(Arm_R6) == NotMapped || GetArmRegMapped(Arm_R6) == Temp_Mapped) && !GetArmRegProtected(Arm_R6)) { return Arm_R6; }
    if ((GetArmRegMapped(Arm_R5) == NotMapped || GetArmRegMapped(Arm_R5) == Temp_Mapped) && !GetArmRegProtected(Arm_R5)) { return Arm_R5; }
    if ((GetArmRegMapped(Arm_R4) == NotMapped || GetArmRegMapped(Arm_R4) == Temp_Mapped) && !GetArmRegProtected(Arm_R4)) { return Arm_R4; }
    if ((GetArmRegMapped(Arm_R3) == NotMapped || GetArmRegMapped(Arm_R3) == Temp_Mapped) && !GetArmRegProtected(Arm_R3)) { return Arm_R3; }
    if ((GetArmRegMapped(Arm_R2) == NotMapped || GetArmRegMapped(Arm_R2) == Temp_Mapped) && !GetArmRegProtected(Arm_R2)) { return Arm_R2; }
    if ((GetArmRegMapped(Arm_R1) == NotMapped || GetArmRegMapped(Arm_R1) == Temp_Mapped) && !GetArmRegProtected(Arm_R1)) { return Arm_R1; }
    if ((GetArmRegMapped(Arm_R0) == NotMapped || GetArmRegMapped(Arm_R0) == Temp_Mapped) && !GetArmRegProtected(Arm_R0)) { return Arm_R0; }
    if ((GetArmRegMapped(Arm_R12) == NotMapped || GetArmRegMapped(Arm_R12) == Temp_Mapped) && !GetArmRegProtected(Arm_R12)) { return Arm_R12; }
    if ((GetArmRegMapped(Arm_R11) == NotMapped || GetArmRegMapped(Arm_R11) == Temp_Mapped) && !GetArmRegProtected(Arm_R11)) { return Arm_R11; }
    if ((GetArmRegMapped(Arm_R10) == NotMapped || GetArmRegMapped(Arm_R10) == Temp_Mapped) && !GetArmRegProtected(Arm_R10)) { return Arm_R10; }
    if ((GetArmRegMapped(Arm_R9) == NotMapped || GetArmRegMapped(Arm_R9) == Temp_Mapped) && !GetArmRegProtected(Arm_R9)) { return Arm_R9; }
    if ((GetArmRegMapped(Arm_R8) == NotMapped || GetArmRegMapped(Arm_R8) == Temp_Mapped) && !GetArmRegProtected(Arm_R8)) { return Arm_R8; }

    g_Notify->BreakPoint(__FILE__, __LINE__);
    return Arm_Unknown;
}

void CArmRegInfo::WriteBackRegisters()
{
}

CArmOps::ArmReg CArmRegInfo::Map_Variable(VARIABLE_MAPPED variable)
{
    if (m_InCallDirect)
    {
        CPU_Message("%s: in CallDirect", __FUNCTION__);
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return Arm_Unknown;
    }
    for (int32_t i = 0, n = sizeof(m_ArmReg_MappedTo) / sizeof(m_ArmReg_MappedTo[0]); i < n; i++)
    {
        if (m_ArmReg_MappedTo[i] == Variable_Mapped && m_Variable_MappedTo[i] == variable)
        {
            SetArmRegProtected((ArmReg)i, true);
            return (ArmReg)i;
        }
    }

    ArmReg Reg = FreeArmReg();
    if (Reg == Arm_Unknown)
    {
        WriteTrace(TraceRegisterCache, TraceError, "Failed to find a free register");
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return Arm_Unknown;
    }
    SetArmRegMapped(Reg, Variable_Mapped);
    SetArmRegProtected(Reg, true);

    switch (variable)
    {
    case VARIABLE_GPR:
        CPU_Message("    regcache: allocate %s as pointer to GPR", ArmRegName(Reg));
        m_Variable_MappedTo[Reg] = variable;
        MoveConstToArmReg((uint32_t)_GPR, Reg, "_GPR");
        break;
    case VARIABLE_FPR:
        CPU_Message("    regcache: allocate %s as pointer to _FPR_S", ArmRegName(Reg));
        m_Variable_MappedTo[Reg] = variable;
        MoveConstToArmReg((uint32_t)_FPR_S, Reg, "_FPR_S");
        break;
    case VARIABLE_TLB_READMAP:
        CPU_Message("    regcache: allocate %s as pointer to TLB_READMAP", ArmRegName(Reg));
        m_Variable_MappedTo[Reg] = variable;
        MoveConstToArmReg((uint32_t)(g_MMU->m_TLB_ReadMap), Reg, "MMU->TLB_ReadMap");
        break;
    case VARIABLE_NEXT_TIMER:
        CPU_Message("    regcache: allocate %s as pointer to g_NextTimer", ArmRegName(Reg));
        m_Variable_MappedTo[Reg] = variable;
        MoveConstToArmReg((uint32_t)(g_NextTimer), Reg, "g_NextTimer");
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return Arm_Unknown;
    }
    return Reg;
}

#endif