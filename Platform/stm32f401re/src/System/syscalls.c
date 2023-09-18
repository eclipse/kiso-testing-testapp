/**
 * @file Platform/${PROJECT_PLATFORM}/src/System/syscalls.c
 *
 * @brief Custom implementation for syscalls
 *
  * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * @author Aymen Bouldjadj
 */

#include <assert.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>
#undef errno
/// Error number
extern int errno;

/// Helper entry for environ
char* __env[1] = {0};  // NOLINT(readability-identifier-naming) Don't pollute global namespace

/**
 * @brief Stub for environ.
 *
 * This must point to a null terminated list of environment variable name-value pairs.
 */
char** environ = __env;

/**
 * @brief Stub for syscall _exit
 *
 * This will get called if a program should exit without any clean-up.
 * This stub will just loop indefinitely.
 *
 * @param[in] rc return code
 */
void _exit(int rc __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  while (1) {
  };
}

/**
 * @brief Stub for syscall _close
 *
 * This will get called if a file should be closed.
 * As no files are supported and stdout cannot be closed, this stub always fails.
 *
 * @param[in] fd file descriptor
 * @return -1
 */
int _close(int fd __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = EBADF;
  return -1;
}

/**
 * @brief Stub for syscall _execve
 *
 * This will be called to transfer control to a new process
 *
 * As no processes are supported, this stub always returns -1.
 *
 * @param[in] name name
 * @param[in] argv arguments
 * @param[in] env environment
 * @return -1
 */
int _execve(char* name __attribute__((unused)), char** argv __attribute__((unused)), char** env __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = ENOMEM;
  return -1;
}

/**
 * @brief Stub for syscall _fork
 *
 * This will be called to fork
 *
 * As no processes are supported, this stub always returns -1.
 *
 * @return -1
 */
int _fork(void) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = EAGAIN;
  return -1;
}

/**
 * @brief Stub for syscall _fstat
 *
 * This will be called to get a status of an open file.
 * As no files are supported, this stub reports all files as character devices.
 *
 * @param[in] fd file descriptor
 * @param[out] stat status
 * @return 0
 */
int _fstat(int fd __attribute__((unused)), struct stat* stat) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  stat->st_mode = S_IFCHR;
  return 0;
}

/**
 * @brief Stub for syscall _getpid
 *
 * This will be called to get the process id of the currently running process.
 * As no processes are supported, this stub always returns 0.
 *
 * @return 0
 */
int _getpid(void) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  return 0;
}

/**
 * @brief Stub for syscall _isatty
 *
 * This will be called to check if a file is a terminal.
 * As the only 'file' supported is stdout (the console), this stub always returns 1.
 *
 * @param[in] fd file descriptor
 * @return 1
 */
int _isatty(int fd __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  return 1;
}

/**
 * @brief Stub for syscall _kill
 *
 * This will be called to signal (or terminate) processes.
 * As no processes are supported, this stub always returns -1.
 *
 * @param[in] pid process id
 * @param[in] sig signal
 * @return -1
 */
int _kill(int pid __attribute__((unused)), int sig __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = EINVAL;

  return -1;
}

/**
 * @brief Stub for syscall _link
 *
 * This will be called to link files.
 *
 * As no files are supported, this stub always returns -1.
 *
 * @param[in] old old name
 * @param[in] new new name
 * @return -1
 */
int _link(char* old __attribute__((unused)), char* new __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = EMLINK;
  return -1;
}

/**
 * @brief Stub for syscall _lseek
 *
 * This will be called to set a position in a file.
 * As no files are supported, this stub will always return 0.
 *
 * @param[in] fd file descriptor
 * @param[in] offset offset
 * @param[in] whence whence
 * @return 0
 */
int _lseek(int fd __attribute__((unused)), int offset __attribute__((unused)), int whence __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  return 0;
}

/**
 * @brief Stub for syscall _open
 *
 * This will be called to open a file.
 * As no files are supported, this stub will always return -1.
 *
 * @param[in] name name
 * @param[in] flags flags
 * @param[in] mode mode
 * @return -1
 */
int _open(const char* name __attribute__((unused)), int flags __attribute__((unused)), int mode __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  errno = ENOSYS;
  return -1;
}

/**
 * @brief Stub for syscall _read
 *
 * This will be called to read from a file.
 * As no files are supported, this stub will always return 0.
 *
 * @param[in] fd file descriptor
 * @param[in] buf buffer
 * @param[in] count count
 * @return 0
 */
int _read(int fd __attribute__((unused)), void* buf __attribute__((unused)), int count __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  return 0;
}

/**
 * @brief Stub for syscall _sbrk
 *
 * This will be called to allocate memory from heap.
 * As no heap is supported, this stub will always return -1.
 *
 * @param[in] nbytes number of bytes
 * @return -1
 */
int _sbrk(int nbytes __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  assert(0);
  errno = ENOMEM;
  return -1;
}

/**
 * @brief Stub for syscall _stat
 *
 * This will be called to get a status of a file.
 * As no files are supported, this stub reports all files as character devices.
 *
 * @param[in] name file name
 * @param[out] st status
 * @return 0
 */
int _stat(const char* name __attribute__((unused)), struct stat* st) {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  st->st_mode = S_IFCHR;
  return 0;
}

/**
 * @brief Stub for syscall _times
 *
 * This will be called to get process timing information.
 * As no processes are supported, this stub always returns -1.
 *
 * @param[out] tms timing information
 * @return -1
 */
int _times(struct tms* tms __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  errno = EACCES;
  return -1;
}

/**
 * @brief Stub for syscall _unlink
 *
 * This will be called to unlink a file.
 * As no files are supported, this stub always returns -1.
 *
 * @param[in] name file name
 * @return -1
 */
int _unlink(const char* name __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  errno = ENOENT;
  return -1;
}

/**
 * @brief Stub for syscall _wait
 *
 * This will be called to wait for other processes.
 * As no processes are supported, this stub always returns -1.
 *
 * @param[out] status status
 * @return -1
 */
int _wait(int* status __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  errno = ECHILD;
  return -1;
}

/**
 * @brief Stub for syscall _write
 *
 * This will be called to write to a file.
 * As no files are supported, this stub always returns 0.
 *
 * @param[in] fd file descriptor
 * @param[in] buf buffer
 * @param[in] count count
 * @return 0
 */
int _write(int fd __attribute__((unused)), const void* buf __attribute__((unused)), int count __attribute__((unused))) {  // NOLINT(readability-identifier-naming) Name defined by 3rd party
  return 0;
}
