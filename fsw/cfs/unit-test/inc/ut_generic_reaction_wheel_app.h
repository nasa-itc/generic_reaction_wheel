/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/**
 * @file
 *
 *
 * Purpose:
 * Extra scaffolding functions for the generic_reaction_wheel_app unit test
 *
 * Notes:
 * This is an extra UT-specific extern declaration
 * to obtain access to an internal data structure
 *
 * UT often needs to modify internal data structures in ways that
 * actual applications never would (bypassing the normal API) in
 * order to exercise or set up for off-nominal cases.
 */

#ifndef UT_GENERIC_REACTION_WHEEL_APP_H
#define UT_GENERIC_REACTION_WHEEL_APP_H

/*
 * Necessary to include these here to get the definition of the
 * "GENERIC_REACTION_WHEEL_APP_Data_t" typedef.
 */
#include "generic_reaction_wheel_app.h"

/*
 * Allow UT access to the global "GENERIC_REACTION_WHEEL_APP_Data" object.
 */
// extern GENERIC_REACTION_WHEEL_AppData_t GENERIC_REACTION_WHEEL_APP_Data;

#endif /* UT_GENERIC_REACTION_WHEEL_APP_H */
