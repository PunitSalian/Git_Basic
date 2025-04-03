#pragma once

enum class ErrorCode
{
    OK = 0,           // Success
    NotFound,         // Resource not found
    InvalidArgument,  // Invalid input or argument
    PermissionDenied, // Access denied
    OutOfRange,       // Value out of range
    Unavailable,      // Resource unavailable
    AlreadyExists,    // Resource already exists
    Timeout,          // Operation timed out
    InternalError,    // Generic internal error
    Unimplemented,    // Feature not implemented
    Cancelled,        // Operation cancelled
    FailToOpen,       // Failed to Open
    InvalidFile,      // Invalid File
    Unknown           // Unknown error
};
