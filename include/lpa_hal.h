/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

/**
* @file lpa_hal.h
* @brief The lpa hal provides function call prototypes and structure definitions used for the  LPA hardware abstraction layer.
*
*/

#ifndef _LPA_HAL_H_
#define _LPA_HAL_H_

/**********************************************************************
                ENUMERATION DEFINITIONS
**********************************************************************/
/**
 * @brief Represents the preferred UICC (Universal Integrated Circuit Card) type.
 * This enumeration defines the possible value for preferred UICC.
 */
/*
 * TODO: This module must re-structre it's naming convention for all types
 *       CellularPreferredUICC_t -> LPA_HAL_CellularPreferredUICC_t
 *       CELLULAR_UICC_USIM -> LPA_HAL_CELLULAR_UICC_USIM
 *       etc. everywhere.
 */
typedef enum _Cellular_PreferredUICC_t
{
    CELLULAR_UICC_USIM     = 0,  /**<Universal Integrated Circuit Card with Universal Subscriber Identity Module */
    CELLULAR_UICC_ESIM,          /**< Universal Integrated Circuit Card with embedded SIM*/
} CellularPreferredUICC_t;

/**
 * @brief Represents the status of the cellular device sim.
 *
 * This enumeration provides the values for different status of sim in the cellular devices.  
 */
typedef enum _CellularDeviceSimStatus_t
{
   SIM_PREPARATION_PENDING = 1,    /**< Represents the status as pending */
   SIM_PREPARATION_INPROGRESS,     /**< Represents the status as inprogress*/  
   SIM_PREPARATION_DOWNLOADED,     /**< Represents the status as downloaded*/
   SIM_PREPARATION_DONE,           /**< Represents the status as completed*/
} CellularDeviceSimStatus_t;

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
/**
* @defgroup lpa_hal LPA HAL
*
* @defgroup LPA_HAL_TYPES  LPA HAL Data Types
* @ingroup  lpa_hal
*
* @defgroup LPA_HAL_APIS   LPA HAL  APIs
* @ingroup  lpa_hal
*
**/

/**
* @addtogroup LPA_HAL_TYPES
* @{
*/

#define RETURN_ERROR        (-1)
#define RETURN_OK           (0)
#ifndef TRUE
#define TRUE                (1)
#endif
#ifndef FALSE
#define FALSE               (0)
#endif

/
/** @brief Represents an eSIM profile. 
 *  Stores profile data, including ICCID, state (enabled/disabled), and name.
 */
typedef  struct
{
    char iccid[32];          /**<! ICCID (Integrated Circuit Card Identification), See ITU-T Recommendation E.118 for format details. */
    int  profileState;       /**<! Profile state: 00 (disabled), 01 (enabled). */
    char profileName[16];    /**<! Profile name (e.g., "Xfinity Mobile", "Comcast", "CRTC"). */
} eSIMProfileStruct;

/**
* @}
*/

/**
* @addtogroup LPA_HAL_APIS
* @{
*/

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/


/*
 * TODO: Extend the return codes by listing out the possible reasons of failure, to improve the interface in the future.
 *       This was reported during the review for header file migration to opensource github.
 *
 */

/** @brief Provides progress updates for eSIM profile download. 
 *
 * This callback function is called with status messages indicating each stage of the eSIM profile download process, from initiating authentication to the final download.
 *
 * @param[in] progress - String containing progress status messages.
 *                       \n (The string values are:- "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done","Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".)
 * @returns status of the operation
 * @retval RETURN_OK  - on successful progress reporting, 
 * @retval RETURN_ERR - on failure (e.g., invalid message format, communication issues). 
 */
typedef int (*cellular_sim_download_progress_callback)( char *progress );

/**
* @brief Downloads the eSIM profile using a provided activation code.
*
* Initiates the download and installation of an eSIM profile. Uses a callback to communicate download progress and status back to the caller.
*
* @param[in] ActivationCodeStr - Activation code needed to download the profile (e.g., "1$sm-v4-059-ppa-gtm.pr.go-esim.com$50949C4E5F62378896B6BE0EBEF554E6").
* @param[in] download_progress - Callback function (cellular_sim_download_progress_callback) to receive progress information. 
*                                \n (The string values are:- "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done","Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".)
* @return Status of the download operation:
* @retval TRUE - Download initiated successfully, modem present.
* @retval FALSE - Download initiation error or modem not present.
* 
* @note For detailed errors on FALSE, use lpaGetErrorCodeDescription(lpaGetErrorCode()).
*/
int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress);

/** @brief Downloads an eSIM profile from an SMDS address. 
*
* Initiates profile download by connecting to the specified SMDS server.
*
* @param[in] smds - SMDS address (valid URL format): 
*    @code "oem-smds-json.demo.gemalto.com" @endcode
* @return Status of the download operation.
* @retval RETURN_OK - Profile downloaded, ready for installation. 
* @retval RETURN_ERR - Download error (check specific error codes for details). 
*/
int cellular_esim_download_profile_from_smds(char* smds);

/** @brief Downloads an eSIM profile from a default SMDP+ address. 
*
* Initiates profile download from a predefined SMDP+ address. SMDP+ is used for secure eSIM profile delivery.
*
* @param[in] smdp - SMDP address (fully qualified URL): 
*    @code "smdp-plus.test.gsma.com" @endcode 
* @return Status of the download operation.
* @retval RETURN_OK - Profile downloaded successfully.
* @retval RETURN_ERR - Download error (e.g., invalid address, network issues, SMDP configuration errors). 
*/
int cellular_esim_download_profile_from_defaultsmdp(char* smdp);

/** @brief Retrieves a list of MNO profiles from the eSIM.
*
* Fetches information about available MNO profiles, providing a list and profile count. The function allocates memory for profile data.
* 
* @param[out] profile_list - Array of eSIMProfileStruct pointers (memory dynamically allocated by the function). 
* @param[out] nb_profiles -  Count of found profiles will be stored here.
* @return Status of the operation.
* @retval RETURN_OK - Profiles retrieved successfully.
* @retval RETURN_ERR - Error during retrieval (e.g., eSIM access issues, memory allocation failure, no profiles found).  
* @note The caller must free the dynamically allocated memory for profile data. 
*/
int cellular_esim_get_profile_info(eSIMProfileStruct **profile_list, int *nb_profiles);

/** @brief Enables a Mobile Network Operator (MNO) profile matching the provided ICCID.
*
* Activates the eSIM profile corresponding to the given ICCID.  Verifies the ICCID against stored profiles.
*
* @param[in] iccid - ICCID of the profile to enable (20-digit string).
* @param[in] iccid_size - Length of the ICCID string (should be 20).
* @return Status of the operation.
* @retval RETURN_OK - Profile enabled successfully.
* @retval RETURN_ERR - Error during enabling (e.g., no matching ICCID, invalid format, internal error). 
*/
int cellular_esim_enable_profile(char* iccid, int iccid_size);

/** @brief Disables an eSIM profile matching the provided ICCID.
*
* Deactivates the eSIM profile with the specified Integrated Circuit Card Identification (ICCID). The ICCID must match exactly for successful disabling.
*
* @param[in] iccid - ICCID of the profile to disable (20-digit string).
* @param[in] iccid_size - Length of the ICCID string (should be 20).
* @return Status of the operation.
* @retval RETURN_OK - Profile disabled successfully.
* @retval RETURN_ERR - Error during disabling (e.g., no matching ICCID, invalid format, internal error). 
*/
int cellular_esim_disable_profile(char* iccid, int iccid_size);

/** @brief Deletes an eSIM profile matching the specified ICCID.
*
* Permanently removes the eSIM profile with the provided Integrated Circuit Card Identification (ICCID). The ICCID must match exactly for successful deletion.
*
* @param[in] iccid - ICCID of the profile to delete (20-digit string).
* @param[in] iccid_size - Length of the ICCID string (should be 20).
* @return Status of the operation.
* @retval RETURN_OK - Profile deleted successfully.
* @retval RETURN_ERR - Error during deletion (e.g., no matching ICCID, invalid format, internal error). 
*/
int cellular_esim_delete_profile(char* iccid, int iccid_size);

/** @brief Initializes the Local Profile Assistant (LPA) for eSIM management.
*
* Configures the LPA module, establishes modem readiness, and sets up dependencies and parameters for eSIM operations.
*
* @return Status of the operation.
* @retval RETURN_OK - LPA initialized successfully.
* @retval RETURN_ERR - Initialization error (e.g., modem issues, configuration problems, LPA SDK errors). 
*/
int cellular_esim_lpa_init(void);

/** @brief Cleans up the Local Profile Assistant (LPA) for eSIM management.
*
* Deactivates the LPA module, releasing resources.  Call this before shutdown or when LPA functionality is no longer needed. 
*
* @return Status of the operation.
* @retval RETURN_OK - LPA uninitialized successfully.
* @retval RETURN_ERR - Error during cleanup (e.g., resource release issues, ongoing operations, LPA module errors). 
*/
int cellular_esim_lpa_exit(void);

/** @brief Retrieves the EID (eUICC Identifier) from the device's eSIM.
*
* Fetches the unique identifier of the embedded SIM, essential for managing eSIM profiles and subscriptions.
*
* @return Status of the operation.
* @retval RETURN_OK - EID retrieved successfully.
* @retval RETURN_ERR - Error retrieving the EID (e.g., eSIM communication issues, eSIM not present, configuration errors).  
*/
int cellular_esim_get_eid();

/** @brief Fetches information about the embedded Universal Integrated Circuit Card (eUICC).
*
* Retrieves details from the eUICC (or eSIM), providing data necessary for managing mobile subscriptions.
*
* @return Status of the operation.
* @retval RETURN_OK - eUICC information retrieved successfully.
* @retval RETURN_ERR - Error during retrieval (e.g., eUICC access issues, communication errors, configuration problems).
*/
int cellular_esim_get_euicc();
#endif //_LPA_HAL_H_

/**
* @}
*/
