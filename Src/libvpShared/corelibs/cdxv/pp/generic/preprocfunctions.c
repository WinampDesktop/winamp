	/****************************************************************************
	*
	*   Module Title :     PreProcFunctions.c
	*
	*   Description  :     
	*
	*    AUTHOR      :     Paul Wilkins
	*
	*****************************************************************************
	*   Revision History
	*
	*   1.00 JBB 22 Aug 00  Configuration baseline
	*
	*****************************************************************************
	*/
	
	/****************************************************************************
	*  Header Files
	*****************************************************************************
	*/
	
	#define STRICT              /* Strict type checking. */
	
	#include "preproc.h"
	#ifdef _MSC_VER 
	#pragma warning( disable : 4799 )  // Disable no emms instruction warning!
	#endif
	/****************************************************************************
	*  Module constants.
	*****************************************************************************
	*/        
	
	/****************************************************************************
	*  Imports.
	*****************************************************************************
	*/   

	/****************************************************************************
	*  Exported Global Variables
	*****************************************************************************
	*/
	
	/****************************************************************************
	*  Exported Functions 
	*****************************************************************************
	*/              
	
	/****************************************************************************
	*  Module Statics
	*****************************************************************************
	*/  
	
	
	/****************************************************************************
	*  Forward References
	*****************************************************************************
	*/  
	
	/****************************************************************************
    * 
    *  ROUTINE       :     MachineSpecificConfig
    *
    *  INPUTS        :     None
    *
    *  OUTPUTS       :     None
    *
    *  RETURNS       :     None
    *
    *  FUNCTION      :     Checks for machine specifc features such as MMX support 
    *                      sets approipriate flags and function pointers.
    *
    *  SPECIAL NOTES :     None. 
    *
    *
    *  ERRORS        :     None.
    *
    ****************************************************************************/
	#define MMX_ENABLED 1
	void MachineSpecificConfig(PP_INSTANCE *ppi)
	{
UINT32 FeatureFlags = 0;
ppi->RowSAD = ScalarRowSAD;
ppi->ColSAD = ScalarColSAD;
	}
	
	
	/****************************************************************************
    * 
    *  ROUTINE       :     ClearMmxState()
    *
    *
    *  INPUTS        :     None
    *
    *  OUTPUTS       :     
    *
    *  RETURNS       :    
    * 
    *
    *  FUNCTION      :     Clears down the MMX state
    *
    *  SPECIAL NOTES :     None. 
    *
    *
    *  ERRORS        :     None.
    *
    ****************************************************************************/
	void ClearMmxState(PP_INSTANCE *ppi)
	{
return;
	}
	