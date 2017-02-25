/**
	Filter registration information
*/

#pragma once

#include <fltKernel.h>

#include "MiniFilter.h"

// Callbacks : list of FLT_OPERATION_REGISTRATION structures, one for each type of I/O for which the minifilter
//				registers preoperation and postoperation callback routines.
//				The last element must be IRP_MJ_OPERATION_END

CONST FLT_OPERATION_REGISTRATION Callbacks[] = 
{
	{ 
		IRP_MJ_CREATE,
		0,
		MfPreOperationCallback,
		MfPostOperationCallback 
	},
	{ IRP_MJ_OPERATION_END }
};


// Defines the minifilter context structure

typedef struct _MINIFILTER_TRANSACTION_CONTEXT {
	ULONG Flags;
	ULONG Count;
} MINIFILTER_TRANSACTION_CONTEXT, *PMINIFILTER_CONTEXT;

// Contexts : a context structure is used to register context types
const FLT_CONTEXT_REGISTRATION Contexts[] = 
{
	{
		FLT_TRANSACTION_CONTEXT,					// Context type
		0,											// Flags
		MfDeleteTxfContext,							// ContextCleanupCallback
		sizeof(MINIFILTER_TRANSACTION_CONTEXT),		// Size
		'ypsM'										// PoolTag
	},

	{
		FLT_CONTEXT_END
	}
};

// This defines what we want to filter with FltMgr

CONST FLT_REGISTRATION FilterRegistration = 
{
	sizeof(FLT_REGISTRATION),				// Size
	FLT_REGISTRATION_VERSION,				// Version
	0,   // Flags
	Contexts,								// Contexts
	Callbacks,								// Callbacks
	MfUnload,								// FilterUnload
	MfInstanceSetup,									// InstanceSetup
	MfInstanceQueryTeardown,						// InstanceQueryTeardown
	MfInstanceTeardownStart,									// InstanceTeardownStart
	MfInstanceTeardownComplete,									// InstanceTeardownComplete
	NULL,					  				// GenerateFileName
	NULL,					  				// GenerateDestinationFileName
	NULL,					  				// NormalizeNameComponent
};