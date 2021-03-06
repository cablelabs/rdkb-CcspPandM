/*

 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/**************************************************************************

    module: cosa_userinterface_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------
 
    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlRaInit
        *  CosaDmlRaSetCfg
        *  CosaDmlRaGetCfg
        *  CosaDmlRaGetSupportedProtocols
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_userinterface_apis.h"
#include "dmsb_tr181_psm_definitions.h"
#include <arpa/inet.h>

//PSM-ACCESS
extern ANSC_HANDLE bus_handle;
extern char g_Subsystem[32];

#define _PSM_WRITE_PARAM(_PARAM_NAME) { \
        _ansc_sprintf(param_name, _PARAM_NAME); \
        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, param_name, ccsp_string, param_value); \
        if (retPsmSet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d writing %s %s\n", __FUNCTION__, retPsmSet, param_name, param_value));\
        } \
        else \
        { \
            AnscTraceFlow(("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value)); \
           printf("%s: retPsmSet == CCSP_SUCCESS writing %s = %s \n", __FUNCTION__,param_name,param_value); \
        } \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_memset(param_value, 0, sizeof(param_value)); \
    }

#define _PSM_READ_PARAM(_PARAM_NAME) { \
        _ansc_memset(param_name, 0, sizeof(param_name)); \
        _ansc_sprintf(param_name, _PARAM_NAME); \
        retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, param_name, NULL, &param_value); \
        if (retPsmGet != CCSP_SUCCESS) { \
            AnscTraceFlow(("%s Error %d reading %s %s\n", __FUNCTION__, retPsmGet, param_name, param_value));\
        } \
        else { \
            AnscTraceFlow(("%s: retPsmGet == CCSP_SUCCESS reading %s = \n%s\n", __FUNCTION__,param_name, param_value)); \
                printf("param_name (%s) and param_value (%s) \n",param_name, param_value); \
        } \
    }

static COSA_DML_RA_CFG  g_RaCfg = {
    .bEnabled           = TRUE,
    .bFromAnyIp         = FALSE,
    .SupportedProtocols = "HTTP,HTTPS,SSH,Telnet",
    .StartIp.Dot        = {0, 0, 0, 0},
    .EndIp.Dot          = {0, 0, 0, 0},
    .HttpEnable         = FALSE,
    .HttpPort           = 80,
    .HttpsEnable        = FALSE,
    .HttpsPort          = 443,
    .SSHEnable          = FALSE,
    .SSHPort            = 22,
    .TelnetEnable       = FALSE,
    .TelnetPort         = 23,
    .UpgradePermission  = FALSE,
};

void PSM_Set_RemoteManagement_RecordValues(PCOSA_DML_RA_CFG pCfg)
{
	int retPsmSet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char param_value[256] = {0};
	char start_buf[100];
        char dest_buf[100];
        struct in_addr start_addr;
        struct in_addr end_addr;
	_ansc_sprintf(param_value,"%ld",pCfg->HttpPort);	
	_PSM_WRITE_PARAM(PSM_RM_HTTPPORT);
	if (pCfg->HttpEnable == TRUE)
	AnscCopyString(param_value,"TRUE");
	else
	AnscCopyString(param_value,"FALSE");
	_PSM_WRITE_PARAM(PSM_RM_HTTPENABLE);
	_ansc_sprintf(param_value,"%ld",pCfg->HttpsPort);	
	_PSM_WRITE_PARAM(PSM_RM_HTTPSPORT);
	if (pCfg->HttpsEnable == TRUE)
	AnscCopyString(param_value,"TRUE");
	else
	AnscCopyString(param_value,"FALSE");
	_PSM_WRITE_PARAM(PSM_RM_HTTPSENABLE);

        start_addr.s_addr = pCfg->StartIp.Value;
        inet_ntop(AF_INET, &start_addr, start_buf, sizeof start_buf);
	 AnscCopyString(param_value,start_buf);
	_PSM_WRITE_PARAM(PSM_RM_STARTIP);
        end_addr.s_addr = pCfg->EndIp.Value;
        inet_ntop(AF_INET, &end_addr, dest_buf, sizeof dest_buf);
	 AnscCopyString(param_value,dest_buf);
	_PSM_WRITE_PARAM(PSM_RM_ENDIP);
	if(pCfg->bFromAnyIp == TRUE)
	AnscCopyString(param_value,"TRUE");	
	else
	AnscCopyString(param_value,"FALSE");
	_PSM_WRITE_PARAM(PSM_RM_FROMANYIP);
	
}

void PSM_Get_RemoteManagement_RecordValues(PCOSA_DML_RA_CFG pCfg)
{
	int retPsmGet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char *param_value = NULL;
	struct sockaddr_in antelope;
	_PSM_READ_PARAM(PSM_RM_HTTPENABLE);
	if(strcmp(param_value,"TRUE") == 0)
	pCfg->HttpEnable = TRUE;
	else
	pCfg->HttpEnable = FALSE;
	_PSM_READ_PARAM(PSM_RM_HTTPSENABLE);
	if(strcmp(param_value,"TRUE") == 0)
        pCfg->HttpsEnable = TRUE;
        else
        pCfg->HttpsEnable = FALSE;
	_PSM_READ_PARAM(PSM_RM_STARTIP);
	antelope.sin_addr.s_addr = inet_addr(param_value);
	pCfg->StartIp.Value = antelope.sin_addr.s_addr;
	_PSM_READ_PARAM(PSM_RM_ENDIP);
	antelope.sin_addr.s_addr = inet_addr(param_value);
        pCfg->EndIp.Value = antelope.sin_addr.s_addr;
	_PSM_READ_PARAM(PSM_RM_FROMANYIP);
	if(strcmp(param_value,"TRUE") == 0)
        pCfg->bFromAnyIp = TRUE;
        else
        pCfg->bFromAnyIp = FALSE;
}
ANSC_STATUS
CosaDmlRaInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS
CosaDmlRaSetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
    if (pCfg)
        AnscCopyMemory(&g_RaCfg, pCfg, sizeof(COSA_DML_RA_CFG));
	RemoteManagementiptableRulessetoperation(pCfg);//RDKB_EMULATOR
	PSM_Set_RemoteManagement_RecordValues(pCfg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRaGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_RA_CFG            pCfg
    )
{
	ULONG value;
	int retPsmGet = CCSP_SUCCESS;
        char param_name[256] = {0};
        char *param_value = NULL;
    if (pCfg)
        AnscCopyMemory(pCfg, &g_RaCfg, sizeof(COSA_DML_RA_CFG));
	pCfg->HttpPort  = GetHttpPortValue(value);//RDKB_EMULATOR
	if (pCfg->HttpPort == 0) 
	{	
	_PSM_READ_PARAM(PSM_RM_HTTPPORT);
	pCfg->HttpPort = atol(param_value);
	}
	pCfg->HttpsPort = GetHttpsPortValue(value);//RDKB_EMULATOR
	if(pCfg->HttpsPort == 0)
	{
	_PSM_READ_PARAM(PSM_RM_HTTPSPORT);
	pCfg->HttpsPort = atol(param_value);
	}
	PSM_Get_RemoteManagement_RecordValues(pCfg);
    return ANSC_STATUS_SUCCESS;
}


ULONG
CosaDmlUIIPRangeGetNumberOfEntries
    (
        ANSC_HANDLE                 hContext
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return 0;
}

ANSC_STATUS
CosaDmlUIIPRangeGetEntry
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetValues
    (
        ANSC_HANDLE                 hContext,
        ULONG                       ulIndex,
        ULONG                       ulInstanceNumber,
        char*                       pAlias
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeAddEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeDelEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeSetEntry
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlUIIPRangeGetCfg
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_UI_IPRANGE_ENTRY  pEntry
    )
{
    fprintf(stderr, "%s is not implemented!\n", __FUNCTION__);

    return ANSC_STATUS_SUCCESS;
}

