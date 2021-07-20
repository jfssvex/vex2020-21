/**
 * @file include/macros.h
 * 
 * Declarations of macro functions
 */

#pragma once

#include "main.h"

// Blocking = The thread will only continue when the action in the brief is complete
// Asynchronous = The thread will continue immediately. Most likely just changes a state machine's state

/**
 * @brief Run the flipout routine. Blocking.
 */
void flipout();

/**
 * @brief Set state machines to start intaking. Asynchronous.
 */
void in();

/**
 * @brief Eject given number of balls. Blocking.
 * 
 * @param balls Number of balls to eject
 */
void outtake(uint8_t balls);

/**
 * @brief Stop both intake and uptake/shooting rollers. Asynchronous.
 */
void stopRollers();

/**
 * @brief Shoot given number of balls without second ball-delay. Blocking.
 * 
 * @param balls Number of balls to shoot
 */
void shootClean(uint8_t balls);

/**
 * @brief Shoot given number of balls with second ball-delay. Blocking.
 * 
 * @param balls Number of balls to shoot
 */
void shootStaggered(uint8_t balls);

/**
 * @brief shootClean while also running intakes to pull balls
 * out of the bottom of the goal. Blocking.
 * 
 * @param balls Number of balls to shoot
 */
void shootCleanIntake(uint8_t balls);

/**
 * @brief shootStaggered while also running intakes to pull balls
 * out of the bottom of hte goal. Blocking.
 * 
 * @param balls Number of balls to shoot
 */
void shootStaggeredIntake(uint8_t balls);

extern bool suspendDrive; // Used to stop driver from controlling drive train during routine
