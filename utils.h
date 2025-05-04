#ifndef UTILS_H
#define UTILS_H

#include <winerror.h>
#include <stdbool.h>



/**
 * @brief Prints the HRESULT value and its corresponding system error message.
 *
 * This function takes an HRESULT value and prints it to the console. If the HRESULT
 * indicates success, it outputs a success message. Otherwise, it attempts to retrieve
 * the system error message associated with the HRESULT using FormatMessageW and prints
 * it along with the HRESULT value. If the error message cannot be retrieved, it indicates
 * an unknown error.
 *
 * @param hr The HRESULT value to be printed.
 */
void print_h_result(HRESULT hr);

/**
 * @brief Waits for the user to press the Enter key in the console.
 *
 * This function blocks execution until the user presses the Enter key ('\r') in the console.
 * It monitors console input events using ReadConsoleInput and checks for a key-down event
 * corresponding to the Enter key. Additionally, it processes Windows messages to prevent
 * the application from becoming unresponsive during the wait.
 */
void wait_for_console_enter(void);

#endif // !UTILS_H
