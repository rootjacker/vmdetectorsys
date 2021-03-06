#include <ntddk.h>
#include <wdm.h>
#include "VmDetectorUtils.h"

//////////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////////

//
//  MULTIVERSION NOTE: For this version of the driver, we need to know the
//  current OS version while we are running to make decisions regarding what
//  logic to use when the logic cannot be the same for all platforms.  We
//  will look up the OS version in DriverEntry and store the values
//  in these global variables.
//

// Unmangled C names
#ifdef __cplusplus
extern "C" {
#endif

	ULONG g_OsMajorVersion = 0;
	ULONG g_OsMinorVersion = 0;

#ifdef __cplusplus
}; // extern "C"
#endif

VOID
GetOSVersion ()
/*++

Reference: sfilter

Routine Description:

    This routine reads the current OS version using the correct routine based
    on what routine is available.

Arguments:

    None.
    
Return Value:

    None.

--*/
{
	UNICODE_STRING functionName;
	RTLGETVERSION RtlGetVersion;

	RtlInitUnicodeString( &functionName, L"RtlGetVersion" );

	RtlGetVersion = (RTLGETVERSION)MmGetSystemRoutineAddress( &functionName );

    if (NULL != RtlGetVersion) {

        RTL_OSVERSIONINFOW versionInfo;
        NTSTATUS status;

        //
        //  VERSION NOTE: RtlGetVersion does a bit more than we need, but
        //  we are using it if it is available to show how to use it.  It
        //  is available on Windows XP and later.  RtlGetVersion and
        //  RtlVerifyVersionInfo (both documented in the IFS Kit docs) allow
        //  you to make correct choices when you need to change logic based
        //  on the current OS executing your code.
        //

        versionInfo.dwOSVersionInfoSize = sizeof( RTL_OSVERSIONINFOW );

        status = RtlGetVersion( &versionInfo );

        ASSERT( NT_SUCCESS( status ) );

        g_OsMajorVersion = versionInfo.dwMajorVersion;
        g_OsMinorVersion = versionInfo.dwMinorVersion;
	}

	return;     
}