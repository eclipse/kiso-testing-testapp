/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack __attribute__((unused)), TaskFunction_t pxCode __attribute__((unused)), void *pvParameters __attribute__((unused))) {
    return ( StackType_t * ) NULL;
}

BaseType_t xPortStartScheduler( void ) {
    return 0u;
}

void vPortDeleteThread( void *pvTaskToDelete __attribute__((unused))) {}

void vPortCloseRunningThread( void *pvTaskToDelete __attribute__((unused)), volatile BaseType_t *pxPendYield __attribute__((unused))) {}

void vPortEndScheduler( void ) {}

BaseType_t xPortIsInsideInterrupt( void ) {	return pdFALSE; }

void vPortGenerateSimulatedInterrupt( uint32_t ulInterruptNumber __attribute__((unused)) ) {}

void vPortSetInterruptHandler( uint32_t ulInterruptNumber __attribute__((unused)), uint32_t (*pvHandler)( void ) __attribute__((unused))) {}

void vPortEnterCritical( void ) {}

void vPortExitCritical( void ) {}

void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions __attribute__((unused))) {}

void vPortGetHeapStats( HeapStats_t *pxHeapStats __attribute__((unused))) {}

void *pvPortMalloc( size_t xSize __attribute__((unused))) {
    return NULL;
}

void vPortFree( void *pv __attribute__((unused))) {}

void vPortInitialiseBlocks( void ) {}

size_t xPortGetFreeHeapSize( void ) {
    return 0u;
}

size_t xPortGetMinimumEverFreeHeapSize( void ) {
    return 0u;
}