/******************************************************************************
 *
 * Module Name: strtoul - ACPICA implementations of C library functions
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2018, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights. You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code. No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision. In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change. Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee. Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution. In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE. ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES. INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS. INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES. THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government. In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************
 *
 * Alternatively, you may choose to be licensed under the terms of the
 * following license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Alternatively, you may choose to be licensed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 *****************************************************************************/

#define ACPI_CLIBRARY
#include "acpi.h"
#include "accommon.h"

/*
 * This module contains implementations of the standard C library functions
 * that are required by the ACPICA code at both application level and kernel
 * level.
 *
 * The module is an optional feature that can be used if a local/system
 * C library is not available. Some operating system kernels may not have
 * an internal C library.
 *
 * In general, these functions are less efficient than an inline or assembly
 * code implementation.
 *
 * These C functions and the associated prototypes are enabled by default
 * unless the ACPI_USE_SYSTEM_CLIBRARY symbol is defined. This is usually
 * automatically defined for the ACPICA applications such as iASL and
 * AcpiExec, so that these user-level applications use the local C library
 * instead of the functions in this module.
 */

/*******************************************************************************
 *
 * Functions implemented in this module:
 *
 * FUNCTION:    strtoul
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * FUNCTION:    strtoul
 *
 * PARAMETERS:  String          - Null terminated string
 *              Terminater      - Where a pointer to the terminating byte is
 *                                returned
 *              Base            - Radix of the string
 *
 * RETURN:      Converted value
 *
 * DESCRIPTION: Convert a string into a 32-bit unsigned value.
 *              Note: use strtoul64 for 64-bit integers.
 *
 ******************************************************************************/

uint32_t strtoul(const char *String, char **Terminator, uint32_t Base)
{
        UINT32 converted = 0;
        UINT32 index;
        UINT32 sign;
        const char *StringStart;
        UINT32 ReturnValue = 0;
        ACPI_STATUS Status = AE_OK;

        /*
         * Save the value of the pointer to the buffer's first
         * character, save the current errno value, and then
         * skip over any white space in the buffer:
         */
        StringStart = String;
        while (isspace(*String) || *String == '\t') {
                ++String;
        }

        /*
         * The buffer may contain an optional plus or minus sign.
         * If it does, then skip over it but remember what is was:
         */
        if (*String == '-') {
                sign = ACPI_SIGN_NEGATIVE;
                ++String;
        } else if (*String == '+') {
                ++String;
                sign = ACPI_SIGN_POSITIVE;
        } else {
                sign = ACPI_SIGN_POSITIVE;
        }

        /*
         * If the input parameter Base is zero, then we need to
         * determine if it is octal, decimal, or hexadecimal:
         */
        if (Base == 0) {
                if (*String == '0') {
                        if (tolower(*(++String)) == 'x') {
                                Base = 16;
                                ++String;
                        } else {
                                Base = 8;
                        }
                } else {
                        Base = 10;
                }
        } else if (Base < 2 || Base > 36) {
                /*
                 * The specified Base parameter is not in the domain of
                 * this function:
                 */
                goto done;
        }

        /*
         * For octal and hexadecimal bases, skip over the leading
         * 0 or 0x, if they are present.
         */
        if (Base == 8 && *String == '0') {
                String++;
        }

        if (Base == 16 && *String == '0' && tolower(*(++String)) == 'x') {
                String++;
        }

        /*
         * Main loop: convert the string to an unsigned long:
         */
        while (*String) {
                if (isdigit(*String)) {
                        index = (UINT32) ((UINT8) * String - '0');
                } else {
                        index = (UINT32) toupper(*String);
                        if (isupper(index)) {
                                index = index - 'A' + 10;
                        } else {
                                goto done;
                        }
                }

                if (index >= Base) {
                        goto done;
                }

                /*
                 * Check to see if value is out of range:
                 */

                if (ReturnValue > ((ACPI_UINT32_MAX - (UINT32) index) /
                                   (UINT32) Base)) {
                        Status = AE_ERROR;
                        ReturnValue = 0;        /* reset */
                } else {
                        ReturnValue *= Base;
                        ReturnValue += index;
                        converted = 1;
                }

                ++String;
        }

 done:
        /*
         * If appropriate, update the caller's pointer to the next
         * unconverted character in the buffer.
         */
        if (Terminator) {
                if (converted == 0 && ReturnValue == 0 && String != NULL) {
                        *Terminator = (char *)StringStart;
                } else {
                        *Terminator = (char *)String;
                }
        }

        if (Status == AE_ERROR) {
                ReturnValue = ACPI_UINT32_MAX;
        }

        /*
         * If a minus sign was present, then "the conversion is negated":
         */
        if (sign == ACPI_SIGN_NEGATIVE) {
                ReturnValue = (ACPI_UINT32_MAX - ReturnValue) + 1;
        }

        return (ReturnValue);
}
